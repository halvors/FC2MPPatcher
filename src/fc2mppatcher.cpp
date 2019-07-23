#include <QFile>
#include <QDebug>

#include "fc2mppatcher.h"

FC2MPPatcher::FC2MPPatcher(QObject *parent) : QObject(parent)
{

}

FC2MPPatcher::~FC2MPPatcher()
{
    if (peFile) {
        delete peFile;
    }
}

bool FC2MPPatcher::open(const QString &filename)
{
    peFile = new PeFile32(filename.toStdString());

    if (!peFile) {
        qDebug("Invalid PE file.");

        return false;
    }

    // Read the headers from disk.
    peFile->readMzHeader();
    peFile->readPeHeader();

    // Reads the import directory from file.
    peFile->readImportDirectory();

    return true;
}

bool FC2MPPatcher::save()
{
    if (!peFile) {
        return false;
    }

    //unsigned uiImpDir = peFile->peHeader().getIddImportRva(); // TODO: What number is this???
    // TODO: Problem is here, does not write to file. Only applied in memory.
    //peFile->impDir().write(peFile->getFileName(), peFile->peHeader().rvaToOffset(uiImpDir), uiImpDir);

    unsigned uiImpDir = peFile->peHeader().getIddImportRva();
    //peFile->peHeader().setIddImportSize(peFile->impDir().size());
    //peFile->peHeader().makeValid(peFile->mzHeader().size());
    peFile->mzHeader().write(peFile->getFileName(), 0);
    peFile->peHeader().write(peFile->getFileName(), 0xE8);
    peFile->impDir().write(peFile->getFileName(), peFile->peHeader().rvaToOffset(uiImpDir), uiImpDir);

    return true;
}

bool FC2MPPatcher::addFunction(const QString &libraryName, const QString &functionName)
{
    if (!peFile) {
        return false;
    }

    return peFile->impDir().addFunction(libraryName.toStdString(), functionName.toStdString()) == ERROR_NONE;
}

void FC2MPPatcher::dumpImportDirectory()
{
    qDebug("Import Directory");

    if (peFile->readImportDirectory()) {
        qDebug("Not available.");

        return;
    }

    const ImportDirectory<32> &imp = peFile->impDir();

    for (unsigned int i = 0; i < imp.getNumberOfFiles(OLDDIR); i++) {
        qDebug() << "Library Name: " << QString::fromStdString(imp.getFileName(i, OLDDIR));
        //qDebug("Functions");

        // Available information
        //qDebug() << "DLL Name" << QString::fromStdString(imp.getFileName(i, OLDDIR));
        //qDebug() << "OriginalFirstThunk" << imp.getOriginalFirstThunk(i, OLDDIR);
        //qDebug() << "TimeDateStamp" << imp.getTimeDateStamp(i, OLDDIR);
        //qDebug() << "ForwarderChain" << imp.getForwarderChain(i, OLDDIR);
        //qDebug() << "Name" << imp.getRvaOfName(i, OLDDIR);
        //qDebug() << "FirstThunk" << imp.getFirstThunk(i, OLDDIR);

        for (unsigned int j = 0; j < imp.getNumberOfFunctions(i, OLDDIR); j++) {
            qDebug() << "Function Name" << QString::fromStdString(imp.getFunctionName(i, j, OLDDIR));
            qDebug() << "Hint" << imp.getFunctionHint(i, j, OLDDIR);
            qDebug() << "First Thunk" << imp.getFirstThunk(i, j, OLDDIR);
            qDebug() << "Original First Thunk" << imp.getOriginalFirstThunk(i, j, OLDDIR);
        }

    }
}

void FC2MPPatcher::patch(const QString &installDir)
{
    // Create path to binary folder.
    QString path = installDir + "/bin/";
    QString fileName = path + "FarCry2_patched.exe";

    // Copy original file to other workfile.
    if (QFile::exists(fileName)) {
        QFile::remove(fileName);
    }

    QFile::copy(path + "FarCry2.exe", fileName);

    // Load the file into this program.
    open(fileName);

    //addFunction(Constants::library_name, Constants::library_function_getAdaptersInfo);
    //addFunction(Constants::library_name, Constants::library_function_getHostbyname);
    //dumpImportDirectory();

    save();
}
