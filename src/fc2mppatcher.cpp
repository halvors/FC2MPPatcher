#include <QDebug>

#include "fc2mppatcher.h"

FC2MPPatcher::FC2MPPatcher(QObject *parent) : QObject(parent)
{
    // Open the file and load PE header.
    open(filename);

    addImportFunction("fix.dll", "_Z17GetAdaptersInfoPXP16_IP_ADAPTER_INFOPm@8");
    addImportFunction("fix.dll", "_Z14getHostbyname2Pc@4");

    // Actually do stuff.
    dumpImportDirectory();
}

FC2MPPatcher::~FC2MPPatcher()
{
    delete peFile;
}

bool FC2MPPatcher::open(const QString &filename)
{
    peFile = new PeLib::PeFile32(filename.toStdString());

    if (!peFile) {
        qDebug("Invalid PE File");

        return false;
    }

    // These is needed in order to actually read the file from disk.
    peFile->readMzHeader();
    mzHeaderSize = peFile->mzHeader().size();
    peFile->readPeHeader();

    return true;
}

bool FC2MPPatcher::save()
{
    if (!peFile) {
        return false;
    }

    peFile->peHeader().write(peFile->getFileName(), mzHeaderSize);

    return true;
}

void FC2MPPatcher::addImportFunction(const QString &libraryName, const QString &functionName)
{
    // Reads the import directory from file.
    peFile->readImportDirectory();

    unsigned uiImpDir = peFile->peHeader().getVirtualAddress(0) + 0x100; // TODO: What number is this???
    peFile->impDir().addFunction(libraryName.toStdString(), functionName.toStdString());

    // TODO: Problem is here, does not write to file.
    peFile->impDir().write(peFile->getFileName(), peFile->peHeader().rvaToOffset(uiImpDir), uiImpDir);
}

void

void FC2MPPatcher::dumpImportDirectory()
{
    qDebug("Import Directory");

    /*
    if (peFile->readImportDirectory()) {
        qDebug("Not available");

        return;
    }
    */

    const PeLib::ImportDirectory32 &imp = peFile->impDir();

    for (unsigned int i = 0; i < imp.getNumberOfFiles(PeLib::OLDDIR); i++) {
        qDebug() << "Library Name: " << QString::fromStdString(imp.getFileName(i, PeLib::OLDDIR));
        //qDebug("Functions");

        // Available information
        //qDebug() << "DLL Name" << QString::fromStdString(imp.getFileName(i, PeLib::OLDDIR));
        //qDebug() << "OriginalFirstThunk" << imp.getOriginalFirstThunk(i, PeLib::OLDDIR);
        //qDebug() << "TimeDateStamp" << imp.getTimeDateStamp(i, PeLib::OLDDIR);
        //qDebug() << "ForwarderChain" << imp.getForwarderChain(i, PeLib::OLDDIR);
        //qDebug() << "Name" << imp.getRvaOfName(i, PeLib::OLDDIR);
        //qDebug() << "FirstThunk" << imp.getFirstThunk(i, PeLib::OLDDIR);

        for (unsigned int j = 0; j < imp.getNumberOfFunctions(i, PeLib::OLDDIR); j++) {
            //qDebug() << "Function Name" << QString::fromStdString(imp.getFunctionName(i, j, PeLib::OLDDIR));

            // Available information
            //qDebug() << "Function Name" << QString::fromStdString(imp.getFunctionName(i, j, PeLib::OLDDIR));
            //qDebug() << "Hint" << imp.getFunctionHint(i, j, PeLib::OLDDIR);
            //qDebug() << "First Thunk" << imp.getFirstThunk(i, j, PeLib::OLDDIR);
            //qDebug() << "Original First Thunk" << imp.getOriginalFirstThunk(i, j, PeLib::OLDDIR);
        }

    }
}
