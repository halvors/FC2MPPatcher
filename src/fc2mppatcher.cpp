#include <QDebug>

#include "fc2mppatcher.h"

FC2MPPatcher::FC2MPPatcher(QObject *parent) : QObject(parent)
{

}

FC2MPPatcher::~FC2MPPatcher()
{
    delete peFile;
}

bool FC2MPPatcher::open(const QString &filename)
{
    peFile = new PeLib::PeFile32(filename.toStdString());

    if (!peFile) {
        qDebug("Invalid PE file.");

        return false;
    }

    // These is needed in order to actually read the file from disk.
    peFile->readMzHeader();
    peFile->readPeHeader();

    return true;
}

bool FC2MPPatcher::save()
{
    if (!peFile) {
        return false;
    }

    return true;
}

void FC2MPPatcher::addImportFunction(const QString &libraryName, const QString &functionName)
{
    // Reads the import directory from file.
    peFile->readImportDirectory();
    unsigned uiImpDir = peFile->peHeader().getIddImportRva();
    //peFile->impDir().addFunction(libraryName.toStdString(), functionName.toStdString());
    //peFile->peHeader().setIddImportSize(peFile->impDir().size());
    //peFile->peHeader().makeValid(peFile->mzHeader().size());
    // TODO: Problem is here, does not write to file. Only applied in memory.
    peFile->impDir().write(peFile->getFileName(), peFile->peHeader().rvaToOffset(uiImpDir), uiImpDir);
    peFile->peHeader().write(peFile->getFileName(), peFile->mzHeader().size());
}

void FC2MPPatcher::dumpImportDirectory()
{
    qDebug("Import Directory");

    if (peFile->readImportDirectory()) {
        qDebug("Not available.");

        return;
    }

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
            qDebug() << "Function Name" << QString::fromStdString(imp.getFunctionName(i, j, PeLib::OLDDIR));
            qDebug() << "Hint" << imp.getFunctionHint(i, j, PeLib::OLDDIR);
            qDebug() << "First Thunk" << imp.getFirstThunk(i, j, PeLib::OLDDIR);
            qDebug() << "Original First Thunk" << imp.getOriginalFirstThunk(i, j, PeLib::OLDDIR);
        }

    }
}
