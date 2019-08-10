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

bool PeFile::apply(const QString &libraryName, const QString &libraryFile, const QStringList &libraryFunctions, const QList<unsigned int> &addresses, const QString &sectionName) const
{
    // Check that image is loaded.
    if (!image) {
        return false;
    }

    // Get the list of imported libraries and functions.
    imported_functions_list imports = get_imported_functions(*image);

    // Create a new library from which we will import functions.
    import_library importLibrary;
    importLibrary.set_name(libraryName.toStdString());

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
    importSection.set_name(libraryFile.toStdString()); // Section Name.
    importSection.readable(true).writeable(true); // Available for read and write.

    // Add a section and get a link to the added section with calculated dimensions.
    section &attachedSection = image->add_section(importSection);

    // Structure responsible for import reassembler settings
    import_rebuilder_settings settings; // Modify the PE header and do not clear the IMAGE_DIRECTORY_ENTRY_IAT field.
    settings.fill_missing_original_iats(true); // Needed to preserve original IAT.

    // Rebuild imports.
    rebuild_imports(*image, imports, attachedSection, settings);

    // Patch code.
    patchFunctions(libraryName, libraryFunctions, addresses, sectionName);

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

QList<unsigned int> PeFile::getFunctionAddresses(const QString &libraryName) const
{
    QList<unsigned int> addresses;

    // Loop thru all imported libraries.
    for (const import_library &library : get_imported_functions(*image)) {
        // Only build map for the selected target library.
        if (library.get_name() == libraryName.toStdString()) {
            unsigned int address = image->get_image_base_32() + library.get_rva_to_iat();

            for (unsigned int i = 0; i < library.get_imported_functions().size(); i++) {
                addresses.append(address);
                address += 4; // Offset is 4 between entries.
            }

            break;
        }
    }

    return addresses;
}

bool PeFile::patchFunctions(const QString &libraryName, const QStringList &libraryFunctions, const QList<unsigned int> &addresses, const QString &sectionName) const
{
    for (section &section : image->get_image_sections()) {
        // Only patch our specified section specified.
        if (section.get_name() == sectionName.toStdString()) {
            // Getting the base image address for later use.
            unsigned int baseImageAddress = image->get_image_base_32() + section.get_virtual_address();

            // Read raw data of section as byte array.
            unsigned char* data = reinterpret_cast<unsigned char*>(&section.get_raw_data()[0]);

            // Get a compiled list of all functiona addreses.
            const QList<unsigned int> &functionAddresses = getFunctionAddresses(libraryName);

            // Patching all addresses specified for this target.
            for (int i = 0; i < addresses.length(); i++) {
                unsigned int address = addresses[i];
                unsigned int functionAddress = functionAddresses[i];

                // Verify to some degree addresses to be patched.
                if (address == 0 || functionAddress == 0) {
                    qDebug() << QT_TR_NOOP(QString("Error: An address is zero, something went wrong! Aborting."));

                    return false;
                }

                // Creating pointer to the data that is to be updated (aka. does pointer yoga).
                unsigned int* dataPtr = reinterpret_cast<unsigned int*>(data + address - baseImageAddress);

                // Change the old address to point to new function instead.
                *dataPtr = functionAddress;

                // TODO: add translation support for this string?
                qDebug() << QT_TR_NOOP(QString("Patched function call to %1, address changed from 0x%2 to 0x%3").arg(libraryFunctions[i]).arg(address, 0, 16).arg(functionAddress, 0, 16));
            }

            return true;
        }
    }

    return false;
}
