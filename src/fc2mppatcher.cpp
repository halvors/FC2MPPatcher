#include "fc2mppatcher.h"
#include "pelib/PeLib.h"

FC2MPPatcher::FC2MPPatcher(QObject *parent) : QObject(parent)
{
    PeLib::PeFile *peFile = PeLib::openPeFile(fc2_install_dir.toStdString() + "bin/FarCry2.exe");
}
