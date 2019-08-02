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
        qDebug() << "Cannot open" << file.fileName();

        return false;
    }

    try {
        // Create an instance of a PE or PE + class using a factory
        image = new pe_base(pe_factory::create_pe(inputStream));
    } catch (const pe_exception &exception) {
        qDebug() << "Error:" << exception.what();

        return false;
    }

    return true;
}

bool PeFile::apply(const QString &libraryName, const QString &libraryFile, QStringList libraryFunctions, QList<FunctionEntry> targetFunctions, const QString &sectionName) const
{
    // Check that image is loaded.
    if (!image) {
        return false;
    }

    // Get the list of imported libraries and functions.
    imported_functions_list imports = imported_functions_list(get_imported_functions(*image));

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
    import_rebuilder_settings settings(true, false); // Modify the PE header and do not clear the IMAGE_DIRECTORY_ENTRY_IAT field.
    settings.fill_missing_original_iats(true); // Needed to preserve original IAT.

    // Rebuild imports.
    rebuild_imports(*image, imports, attachedSection, settings);

    // Patch code.
    patchFunctions(libraryName, targetFunctions, sectionName);

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
            qDebug() << "Cannot create" << file.fileName();

            return false;
        }

        // Rebuild PE file.
        rebuild_pe(*image, outputStream);

        qDebug() << "PE was rebuilt and saved to" << file.fileName();
    } catch (const pe_exception &exception) {
        qDebug() << "Error:" << exception.what();

        return false;
    }

    return true;
}

const QList<FunctionEntry> PeFile::buildAddressOfFunctions(const QString &libraryName) const
{
    QList<FunctionEntry> list;

    // Loop thru all imported libraries.
    for (const import_library &library : get_imported_functions(*image)) {
        // Only build map for the selected target library.
        if (library.get_name() == libraryName.toStdString()) {
            unsigned int address = image->get_image_base_32() + library.get_rva_to_iat();

            for (const imported_function &function : library.get_imported_functions()) {
                list.append({ address, QString::fromStdString(function.get_name()) });
                address += 4; // Offset is 4 between entries.
            }

            break;
        }
    }

    return list;
}

bool PeFile::patchFunctions(const QString &libraryName, const QList<FunctionEntry> &functions, const QString &sectionName) const
{
    for (section &section : image->get_image_sections()) {
        // Only patch our specified section specified.
        if (section.get_name() == sectionName.toStdString()) {
            // Getting the base image address for later use.
            unsigned int baseImageAddress = image->get_image_base_32() + section.get_virtual_address();

            // Read raw data of section as byte array.
            unsigned char* data = reinterpret_cast<unsigned char*>(&section.get_raw_data()[0]);

            // Patching all functions specified for this target.
            for (const FunctionEntry &function : functions) {
                unsigned int newAddress = 0;

                // Find address matching function.
                for (const FunctionEntry &importFunction : buildAddressOfFunctions(libraryName)) {
                    if (function.getName() == importFunction.getName()) {
                        newAddress = importFunction.getAddress();

                        break;
                    }
                }

                // Verify to some degree addresses to be patched.
                if (function.getAddress() == 0 || newAddress == 0) {
                    qDebug() << "Error: An address is zero, something went wrong! Aborting.";

                    return false;
                }

                // Creating pointer to the data that is to be updated (aka. does pointer yoga).
                unsigned int* dataPtr = reinterpret_cast<unsigned int*>(data + function.getAddress() - baseImageAddress);
                //qDebug() << showbase << hex << "dataPtr:" << reinterpret_cast<void*>(dataPtr) << "=" << reinterpret_cast<void*>(data) << "+" << reinterpret_cast<void*>(function.getAddress() - baseImageAddress);

                // Change the old address to point to new function instead.
                *dataPtr = newAddress;
                qDebug() << showbase << hex << "Patched function" << function.getName() << ", address changed from" << function.getAddress() << "to" << newAddress;
            }

            return true;
        }
    }

    return false;
}
