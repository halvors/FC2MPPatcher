#include <QDebug>

#include <fstream>

#include "pefile.h"

PeFile::PeFile(const QFile &file, QObject* parent) :
    QObject(parent),
    file(file)
{    
    // Read PE from file.
    read();
}

PeFile::~PeFile()
{
    if (image) {
        delete image;
    }
}

bool PeFile::read()
{ 
    // Open the file.
    std::ifstream inputStream(file.fileName().toStdString(), std::ios::in | std::ios::binary);

    if (!inputStream) {
        qDebug() << QT_TR_NOOP(QString("Cannot open: %1").arg(file.fileName()));

        return false;
    }

    try {
        // Create an instance of a PE or PE + class using a factory
        image = new pe_base(pe_factory::create_pe(inputStream));
    } catch (const pe_exception &exception) {
        qDebug() << QT_TR_NOOP(QString("Error: %1").arg(exception.what()));

        return false;
    }

    return true;
}

bool PeFile::apply(const QString &libraryName, const QString &libraryFile, const QStringList &libraryFunctions, const QList<AddressEntry> &addresses, const QString &sectionName) const
{
    // Check that image is loaded.
    if (!image) {
        return false;
    }

    // Get the list of imported libraries and functions.
    imported_functions_list imports = get_imported_functions(*image);

    // Create a new library from which we will import functions.
    import_library importLibrary;
    importLibrary.set_name(libraryFile.toStdString());

    // Add a new import functions to library.
    for (const QString &functionName : libraryFunctions) {
        imported_function importFunction;
        importFunction.set_name(functionName.toStdString());
        importLibrary.add_import(importFunction);
    }

    imports.push_back(importLibrary);

    // But we'll just rebuild the import table.
    // It will be larger than before our editing.
    // so we write it in a new section so that everything fits.
    // (we cannot expand existing sections, unless the section is right at the end of the file).
    section importSection;
    importSection.get_raw_data().resize(1);	// We cannot add empty sections, so let it be the initial data size 1.
    importSection.set_name(libraryName.toStdString()); // Section Name.
    importSection.readable(true).writeable(true); // Available for read and write.

    // Add a section and get a link to the added section with calculated dimensions.
    section &attachedSection = image->add_section(importSection);

    // Structure responsible for import reassembler settings
    import_rebuilder_settings settings; // Modify the PE header and do not clear the IMAGE_DIRECTORY_ENTRY_IAT field.
    settings.fill_missing_original_iats(true); // Needed to preserve original IAT.

    // Rebuild imports.
    rebuild_imports(*image, imports, attachedSection, settings);

    // Patch code.
    patchFunctions(libraryFile, libraryFunctions, addresses, sectionName);

    return true;
}

bool PeFile::write() const
{
    // Check that image is loaded.
    if (!image) {
        return false;
    }

    try {
        // Create a new PE file.
        std::ofstream outputStream(file.fileName().toStdString(), std::ios::out | std::ios::binary | std::ios::trunc);

        if (!outputStream) {
            qDebug() << QT_TR_NOOP(QString("Cannot create: %1").arg(file.fileName()));

            return false;
        }

        // Rebuild PE file.
        rebuild_pe(*image, outputStream);

        qDebug() << QT_TR_NOOP(QString("PE was rebuilt and saved to: %1").arg(file.fileName()));
    } catch (const pe_exception &exception) {
        qDebug() << QT_TR_NOOP(QString("Error: %1").arg(exception.what()));

        return false;
    }

    return true;
}

QList<unsigned int> PeFile::getFunctionAddresses(const QString &libraryFile) const
{
    QList<unsigned int> addresses;

    // Loop thru all imported libraries.
    for (const import_library &library : get_imported_functions(*image)) {
        // Only build map for the selected target library.
        if (library.get_name() == libraryFile.toStdString()) {
            unsigned int address = image->get_image_base_32() + library.get_rva_to_iat();

            int i = 0;
            unsigned int lastAddress = 0;

            for (const imported_function &function : library.get_imported_functions()) {
                if (i > 0) {
                    qDebug() << QString("Difference: 0x%1").arg(function.get_iat_va() - lastAddress, 0, 16);
                }

                //qDebug() << QString::fromStdString(function.get_name());
                //qDebug() << QString("0x%1 + 0x%2 + 0x%3").arg(image->get_image_base_32(), 0, 16).arg(library.get_rva_to_iat(), 0, 16).arg(function.get_iat_va(), 0, 16);
                //qDebug() << QString("Is it? 0x%1").arg(function.get_iat_va() - library.get_rva_to_iat(), 0, 16);
                //qDebug() << QString("Or? 0x%1").arg(function.get_iat_va() - library.get_rva_to_iat(), 0, 16);

                i++;
                lastAddress = function.get_iat_va();
            }

            for (unsigned int i = 0; i < library.get_imported_functions().size(); i++) {
                //addresses.append(address);
                //address += 4; // Offset is 4 between entries.
            }

            break;
        }
    }

    return addresses;
}

bool PeFile::patchFunctions(const QString &libraryFile, const QStringList &libraryFunctions, const QList<AddressEntry> &addresses, const QString &sectionName) const
{
    for (section &section : image->get_image_sections()) {
        // Only patch our specified section specified.
        if (section.get_name() == sectionName.toStdString()) {
            // Getting the base image address for later use.
            unsigned int baseImageAddress = image->get_image_base_32() + section.get_virtual_address();

            // Read raw data of section as byte array.
            unsigned char* data = reinterpret_cast<unsigned char*>(&section.get_raw_data()[0]);

            // Get a compiled list of all functiona addreses.
            const QList<unsigned int> &functionAddresses = getFunctionAddresses(libraryFile);

            // Patching all addresses specified for this target.
            for (const AddressEntry &addressEntry : addresses) {
                unsigned int address = addressEntry.getAddress();

                // If address is zero, that means this function is not use for this file.
                if (address == 0) {
                    continue;
                }

                // Creating pointer to the data that is to be updated (aka. does pointer yoga).
                unsigned int* dataPtr = reinterpret_cast<unsigned int*>(data + address - baseImageAddress);

                if (addressEntry.isFunction()) {
                    int index = static_cast<int>(addressEntry.getValue());
                    unsigned int functionAddress = functionAddresses[index];

                    // Verify to some degree addresses to be patched.
                    if (functionAddress == 0) {
                        qDebug() << QT_TR_NOOP(QString("Error: An address is zero, something went wrong! Aborting."));

                        return false;
                    }

                    // Change the old address to point to new function instead.
                    *dataPtr = functionAddress;

                    qDebug() << QT_TR_NOOP(QString("Patched function call to %1, address changed from 0x%2 to 0x%3").arg(libraryFunctions[index]).arg(address, 0, 16).arg(functionAddress, 0, 16));
                } else {
                    // Change the value at the address to the gived code.
                    *dataPtr = addressEntry.getValue();

                    qDebug() << QT_TR_NOOP(QString("Patched code at address %1, code changed to 0x%3").arg(address, 0, 16).arg(addressEntry.getValue(), 0, 16));
                }
            }

            return true;
        }
    }

    return false;
}
