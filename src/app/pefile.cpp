#include <QDebug>

#include <fstream>
#include <cstring>

#include "pefile.h"

PeFile::PeFile(const QFile &file, QObject *parent) :
    QObject(parent),
    file(file)
{    
    // Read PE from file.
    read();
}

PeFile::~PeFile()
{
    if (image)
        delete image;
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

bool PeFile::apply(const QString &libraryName, const QString &libraryFile, const QStringList &libraryFunctions, const QList<AddressEntry> &addresses) const
{
    // Check that image is loaded.
    if (!image)
        return false;

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
    settings.fill_missing_original_iats(true); // Needed in order to preserve original IAT.

    // Rebuild imports.
    rebuild_imports(*image, imports, attachedSection, settings);

    // Add extra .text section.
    buildTextSection();

    // Patch code.
    patchAddresses(libraryFile, libraryFunctions, addresses);

    return true;
}

bool PeFile::write() const
{
    // Check that image is loaded.
    if (!image)
        return false;

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

void PeFile::buildTextSection() const
{
    section section;
    section.get_raw_data().resize(1); // We cannot add empty sections, so let it be the initial data size 1.
    section.set_name(".text_mp"); // Section Name.
    section.readable(true).writeable(true).executable(true);

    // Test demonstrating building a char array.
    char text[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
    section.set_raw_data(text);

    image->add_section(section);
}

QList<unsigned int> PeFile::getFunctionAddresses(const QString &libraryFile) const
{
    QList<unsigned int> addresses;

    // Loop thru all imported libraries.
    for (const import_library &library : get_imported_functions(*image)) {
        // Only build map for the selected target library.
        if (library.get_name() == libraryFile.toStdString()) {
            unsigned int address = image->get_image_base_32() + library.get_rva_to_iat();

            for (unsigned int i = 0; i < library.get_imported_functions().size(); i++) {
                addresses.append(address);
                address += 4; // Size of one address entry.
            }

            break;
        }
    }

    return addresses;
}

bool PeFile::patchAddresses(const QString &libraryFile, const QStringList &libraryFunctions, const QList<AddressEntry> &addresses) const
{
    // Get a compiled list of all functiona addreses.
    const QList<unsigned int> &functionAddresses = getFunctionAddresses(libraryFile);

    for (section &section : image->get_image_sections()) {
        qDebug().noquote() << QT_TR_NOOP(QString("Entering section: \"%1\"").arg(QString::fromStdString(section.get_name())));

        // Getting the base image address for later use.
        unsigned int sectionAddress = image->get_image_base_32() + section.get_virtual_address();

        // Read raw data of section as byte array.
        unsigned char *rawDataPtr = reinterpret_cast<unsigned char*>(&section.get_raw_data()[0]); // NOTE: Seems to be same as section.get_virtual_data(0)[0];

        // Patching all addresses specified for this target.
        for (const AddressEntry &addressEntry : addresses) {
            // Only patch addresses in their specified section.
            if (section.get_name() != addressEntry.getSection().toStdString())
                continue;

            unsigned int address = addressEntry.getAddress();
            QByteArray data = addressEntry.getValue();

            // If address is zero, that means this function is not use for this file.
            if (address == 0)
                continue;

            // Creating pointer to the data that is to be updated (aka. does pointer yoga).
            unsigned int *basePtr = reinterpret_cast<unsigned int*>(rawDataPtr + address - sectionAddress);

            // Handle symbols differently from data.
            if (addressEntry.isSymbol()) {
                int index = data.toInt();
                unsigned int functionAddress = functionAddresses[index];

                // Verify to some degree addresses to be patched.
                if (functionAddress == 0) {
                    qDebug() << QT_TR_NOOP(QString("Error: Address is zero, something went wrong! Aborting."));

                    return false;
                }

                qDebug().noquote() << QT_TR_NOOP(QString("Patched function call at address 0x%1, new function is \"%2\" with address of 0x%3.").arg(address, 0, 16).arg(libraryFunctions[index]).arg(functionAddress, 0, 16));

                // Change the old address to point to new function instead.
                unsigned int *dataPtr = reinterpret_cast<unsigned int*>(basePtr);
                *dataPtr = functionAddress;
            } else {
                // Change the value at the address to the gived code.
                char *dataPtr = reinterpret_cast<char*>(basePtr);

                if (data.length() <= 0) {
                    qDebug() << QT_TR_NOOP(QString("Error: Data length is zero, something went wrong! Aborting."));

                    return false;
                }

                qDebug().noquote() << QT_TR_NOOP(QString("Patched data at address 0x%1, changed from \"%2\" to \"%3\", offset from address is %4.").arg(address, 0, 16).arg(QByteArray(dataPtr, data.length()).constData()).arg(data.constData()).arg(data.length()));

                // Copy data
                std::memcpy(dataPtr, data.constData(), data.length());
            }
        }
    }

    return true;
}
