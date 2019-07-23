#include <QFile>
#include <QDebug>

#include "pe.h"

Pe::Pe(QObject *parent) : QObject(parent)
{

}

Pe::~Pe()
{
    if (file) {
        delete file;
    }
}

bool Pe::open(const QString &fileName)
{
    // Verify that the file we're trying to open actually exists.
    if (!QFile::exists(fileName)) {
        qDebug("Specified file does not exist.");

        return false;
    }

    file = new PeFile32(fileName.toStdString());

    if (!file) {
        qDebug("Invalid PE file.");

        return false;
    }

    // Read headers.
    file->readMzHeader();
    file->readPeHeader();

    // Read import directory .
    file->readImportDirectory();

    // Read import address table directory.
    file->readIatDirectory();

    // Create pointers.
    mzHeader = &file->mzHeader();
    peHeader = &file->peHeader();
    importDirectory = &file->impDir();
    iatDirectory = &file->iatDir();

    return true;
}

bool Pe::save()
{
    if (!file) {
        qDebug() << "No PE file loaded.";

        return false;
    }

    const string fileName = file->getFileName();

    unsigned int importDirectoryRva = peHeader->getIddImportRva();
    unsigned int iatDirectoryRva = peHeader->getIddIatRva();

    // Write to directories calls rebuild().
    iatDirectory->write(fileName, peHeader->rvaToOffset(iatDirectoryRva));
    importDirectory->write(fileName, peHeader->rvaToOffset(importDirectoryRva), importDirectoryRva);

    peHeader->setIddImportSize(importDirectory->size());
    peHeader->setIddIatSize(iatDirectory->size());
    peHeader->makeValid(mzHeader->getAddressOfPeHeader());

    mzHeader->write(fileName, 0);
    peHeader->write(fileName, mzHeader->getAddressOfPeHeader());

    return true;
}

bool Pe::addLibraryFunction(const QString &libraryName, const QString &functionName)
{
    if (!file) {
        qDebug() << "No PE file loaded.";

        return false;
    }

    return importDirectory->addFunction(libraryName.toStdString(), functionName.toStdString()) == ERROR_NONE;
}

void Pe::dumpImportDirectory()
{
    qDebug("Import Directory");

    /*
    if (file->readImportDirectory()) { // Should we read this again here or just use what's in memory?
        qDebug("Not available.");

        return;
    }
    */

    for (unsigned int i = 0; i < importDirectory->getNumberOfFiles(OLDDIR); i++) {
        qDebug() << "Library Name: " << QString::fromStdString(importDirectory->getFileName(i, OLDDIR));
        //qDebug("Functions");

        // Available information
        //qDebug() << "DLL Name" << QString::fromStdString(imp.getFileName(i, OLDDIR));
        //qDebug() << "OriginalFirstThunk" << imp.getOriginalFirstThunk(i, OLDDIR);
        //qDebug() << "TimeDateStamp" << imp.getTimeDateStamp(i, OLDDIR);
        //qDebug() << "ForwarderChain" << imp.getForwarderChain(i, OLDDIR);
        //qDebug() << "Name" << imp.getRvaOfName(i, OLDDIR);
        //qDebug() << "FirstThunk" << imp.getFirstThunk(i, OLDDIR);

        for (unsigned int j = 0; j < importDirectory->getNumberOfFunctions(i, OLDDIR); j++) {
            //qDebug() << "Function Name" << QString::fromStdString(imp.getFunctionName(i, j, OLDDIR));
            //qDebug() << "Hint" << imp.getFunctionHint(i, j, OLDDIR);
            //qDebug() << "First Thunk" << imp.getFirstThunk(i, j, OLDDIR);
            //qDebug() << "Original First Thunk" << imp.getOriginalFirstThunk(i, j, OLDDIR);
        }

    }
}
