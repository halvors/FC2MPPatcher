#include <QFile>
#include <QDebug>

#include <fstream>

#include "pe.h"

Pe::Pe(QObject *parent) : QObject(parent)
{

}

Pe::~Pe()
{

}

bool Pe::addLibraryFunction(const QString &libraryName, const QString &functionName)
{
    // Add a couple of import functions to it.
    imported_function function;
    function.set_name(functionName.toStdString());

    //func.set_iat_va(0x1);	 // Write a non-zero absolute address in the import address table

    // We have specified incorrect contents (0x1 and 0x2) for the cells where the addresses of the imported functions will be written
    // It doesn't matter in the general case, because these values ​​are always overwritten by the loader.
    // These addresses are important only if the exe file has import bound

    // Create a new library from which we will import functions.
    import_library *importLibrary = new import_library();

    if (!map.contains(libraryName)) {
        importLibrary->set_name(libraryName.toStdString());
        map.insert(libraryName, importLibrary);
    } else {
        importLibrary = map.value(libraryName);
    }

    // Add imported functions to library.
    importLibrary->add_import(function);

    return true;
}

bool Pe::apply(const QString &fileName)
{
    // Open the file
    ifstream pe_file(fileName.toStdString(), std::ios::in | std::ios::binary);

    if (!pe_file) {
        qDebug() << "Cannot open " << fileName;

        return false;
    }

    try {
        // Create an instance of a PE or PE + class using a factory
        pe_base image(pe_factory::create_pe(pe_file));

        // Get the list of imported libraries and functions.
        imported_functions_list imports = get_imported_functions(image);

        // Add Imports
        for (import_library *importLibrary : map.values()) {
            imports.push_back(*importLibrary); // Add imported library to imports
        }

        // But we'll just rebuild the import table
        // It will be larger than before our editing
        // so we write it in a new section so that everything fits
        // (we cannot expand existing sections, unless the section is right at the end of the file)
        section importSection;
        importSection.get_raw_data().resize(1);						// We cannot add empty sections, so let it be the initial data size 1
        importSection.set_name(Constants::patch_name.toStdString());							// Section Name
        importSection.readable(true).writeable(true);					// Available for read and write
        section &attached_section = image.add_section(importSection); // Add a section and get a link to the added section with calculated dimensions

        // Structure responsible for import reassembler settings
        import_rebuilder_settings settings(true, false);			 // Modify the PE header and do not clear the IMAGE_DIRECTORY_ENTRY_IAT field
        rebuild_imports(image, imports, attached_section, settings); // Rebuild Imports

        // Create a new PE file
        std::ofstream new_pe_file(fileName.toStdString().c_str(), std::ios::out | std::ios::binary | std::ios::trunc);

        if (!new_pe_file) {
            qDebug() << "Cannot create " << fileName;

            return false;
        }

        // Rebuild PE file
        rebuild_pe(image, new_pe_file);

        qDebug() << "PE was rebuilt and saved to " << fileName;
    } catch (const pe_exception &e) {
        // If an error occurred
        qDebug() << "Error: " << e.what();

        return false;
    }

    return true;
}
