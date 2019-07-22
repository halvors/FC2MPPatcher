#include <QDebug>

#include <iomanip>

#include "fc2mppatcher.h"

FC2MPPatcher::FC2MPPatcher(QObject *parent) : QObject(parent)
{
    // Open the file and load PE header.
    openFile("/home/halvors/Dokumenter/Prosjekter/FC2MPPatcher/Dunia.dll");

    // Actually do stuff.
    dumpImportDirectory(peFile);
}

FC2MPPatcher::~FC2MPPatcher()
{
    delete peFile;
}

void FC2MPPatcher::openFile(const QString &filename)
{
    peFile = new PeLib::PeFile32(filename.toStdString());

    if (!peFile) {
        qDebug("Invalid PE File");

        return;
    }

    // These is needed in order to actually read the file from disk.
    peFile->readMzHeader();
    peFile->readPeHeader();
}

void FC2MPPatcher::dumpImportDirectory(PeLib::PeFile32 *pef)
{
    qDebug("Import Directory");

    if (pef->readImportDirectory()) {
        qDebug("Not available");

        return;
    }

    const PeLib::ImportDirectory32 &imp = pef->impDir();

    for (unsigned int i = 0; i < imp.getNumberOfFiles(PeLib::OLDDIR); i++) {
        qDebug() << "Library Name: " << QString::fromStdString(imp.getFileName(i, PeLib::OLDDIR));
        qDebug("Functions");

        // Available information
        //qDebug() << "DLL Name" << QString::fromStdString(imp.getFileName(i, PeLib::OLDDIR));
        //qDebug() << "OriginalFirstThunk" << imp.getOriginalFirstThunk(i, PeLib::OLDDIR);
        //qDebug() << "TimeDateStamp" << imp.getTimeDateStamp(i, PeLib::OLDDIR);
        //qDebug() << "ForwarderChain" << imp.getForwarderChain(i, PeLib::OLDDIR);
        //qDebug() << "Name" << imp.getRvaOfName(i, PeLib::OLDDIR);
        //qDebug() << "FirstThunk" << imp.getFirstThunk(i, PeLib::OLDDIR);

        for (unsigned int j = 0; j < imp.getNumberOfFunctions(i, PeLib::OLDDIR); j++) {
            qDebug() << "Function Name" << QString::fromStdString(imp.getFunctionName(i, j, PeLib::OLDDIR));

            // Available information
            //qDebug() << "Function Name" << QString::fromStdString(imp.getFunctionName(i, j, PeLib::OLDDIR));
            //qDebug() << "Hint" << imp.getFunctionHint(i, j, PeLib::OLDDIR);
            //qDebug() << "First Thunk" << imp.getFirstThunk(i, j, PeLib::OLDDIR);
            //qDebug() << "Original First Thunk" << imp.getOriginalFirstThunk(i, j, PeLib::OLDDIR);
        }

    }
}
