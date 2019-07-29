#include <QDebug>

#include <fstream>

#include "pefile.h"

PeFile::PeFile(QObject* parent) : QObject(parent)
{

}

PeFile::~PeFile()
{
    clear();
}

QList<FunctionEntry> PeFile::buildAddressOfFunctions() {
    QList<FunctionEntry> list;

    // Loop thru all imported libraries.
    for (import_library library : get_imported_functions(*image)) {
        // Only build map for the selected target library.
        if (library.get_name() == Constants::patch_library_file.toStdString()) {
            unsigned int address = image->get_image_base_32() + library.get_rva_to_iat();

            for (imported_function function : library.get_imported_functions()) {
                list.append({ address, QString::fromStdString(function.get_name()) });
                address += 4; // Offset is 4 between entries.
            }

            break;
        }
    }

    return list;
}

bool PeFile::patchCode(QList<FunctionEntry> &functions)
{
    for (section &section : image->get_image_sections()) {
        // Only patch our section specified.
        if (section.get_name() == Constants::patch_pe_section.toStdString()) {
            unsigned int baseImageAddress = image->get_image_base_32() + section.get_virtual_address();
            qDebug() << "Address of base image:" << showbase << hex << baseImageAddress;

            // Read raw data of section as byte array.
            unsigned char* data = reinterpret_cast<unsigned char*>(&section.get_raw_data()[0]);

            QList<FunctionEntry> importFunctions = buildAddressOfFunctions(); // Build address of function table on the fly.

            qDebug() << "Size:" << importFunctions.size();

            for (FunctionEntry &function : functions) {
                unsigned int newAddress = 0;

                // Find address matching function.
                for (FunctionEntry &importFunction : importFunctions) {
                    if (function.getName() == importFunction.getName()) {
                        newAddress = importFunction.getAddress();

                        break;
                    }
                }

                qDebug() << "#####################################################################";

                qDebug() << showbase << hex << "Old address: " << function.getAddress();
                qDebug() << showbase << hex << "New address: " << newAddress;

                // Verify to some degree addresses to be patched.
                if (function.getAddress() == 0 || newAddress == 0) {
                    qDebug() << "Error: Addresses in patch should not be zero! Patch was not applied.";

                    return false;
                }

                // Creating a pointer to data to be updated.
                unsigned int* dataPtr = reinterpret_cast<unsigned int*>(data + function.getAddress() - baseImageAddress);
                qDebug() << showbase << hex << "dataPtr =" << reinterpret_cast<void*>(dataPtr) << "=" << reinterpret_cast<void*>(data) << "+" << reinterpret_cast<void*>(function.getAddress() - baseImageAddress);

                // Change the old address to point to new function instead.
                *dataPtr = newAddress;
                qDebug() << showbase << hex << "Patched" << function.getName() << ", changed address" << function.getAddress() << "to" << newAddress;
            }

            return true;
        }
    }

    return false;
}

void PeFile::clear()
{
    path.clear();
    target.clear();

    // TODO: clear this?
    /*
    if (image) {
        delete image;
    }
    */
}

bool PeFile::load(const QString &path, const QString &target)
{
    // Store paramters for later use.
    this->path = path;
    this->target = target;

    QString fileName = path + target;

    // Open the file.
    std::ifstream inputStream(fileName.toStdString(), std::ios::in | std::ios::binary);

    if (!inputStream) {
        qDebug() << "Cannot open" << fileName;

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

void PeFile::apply(const QString &libraryName, QList<FunctionEntry> functions)
{
    // Check that image is loaded.
    if (!image) {
        return;
    }

    // Get the list of imported libraries and functions.
    imported_functions_list imports = imported_functions_list(get_imported_functions(*image));

    // Create a new library from which we will import functions.
    import_library importLibrary;
    importLibrary.set_name(libraryName.toStdString());

    // TODO: Avoid duplicate added functions here?...

    // Add a new import functions.
    for (const QString &functionName : Constants::patch_library_functions) {
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
    importSection.set_name(Constants::patch_library_name.toStdString()); // Section Name.
    importSection.readable(true).writeable(true); // Available for read and write.

    // Add a section and get a link to the added section with calculated dimensions.
    section &attachedSection = image->add_section(importSection);

    // Structure responsible for import reassembler settings
    import_rebuilder_settings settings(true, false); // Modify the PE header and do not clear the IMAGE_DIRECTORY_ENTRY_IAT field.
    settings.fill_missing_original_iats(true); // Needed to preserve original IAT.

    // Rebuild imports.
    rebuild_imports(*image, imports, attachedSection, settings);

    // Patch code.
    patchCode(functions);
}

bool PeFile::save()
{
    // Check that image is loaded.
    if (!image) {
        return false;
    }

    QString fileName = path + target;

    try {
        // Create a new PE file.
        std::ofstream outputStream(fileName.toStdString(), std::ios::out | std::ios::binary | std::ios::trunc);

        if (!outputStream) {
            qDebug() << "Cannot create" << fileName;

            return false;
        }

        // Rebuild PE file.
        rebuild_pe(*image, outputStream);

        qDebug() << "PE was rebuilt and saved to" << fileName;
    } catch (const pe_exception &exception) {
        qDebug() << "Error:" << exception.what();

        return false;
    }

    clear();

    return true;
}
