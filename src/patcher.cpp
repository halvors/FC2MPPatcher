#include <QFile>
#include <QTextStream>

#include "patcher.h"

Patcher::Patcher(QObject* parent) : QObject(parent)
{

}

void Patcher::applyPatch(const QString &path, const QString &target)
{
    QString fileName = path + target;

    // Backup original file.
    QString fileNameBackup = fileName + "_backup";

    if (!QFile::exists(fileNameBackup)) {
        QFile::copy(fileName, fileNameBackup);
    }

    // Temporarily workaround.
    QString fileNamePatched = fileName + "_patched.dll";

    if (QFile::exists(fileNamePatched)) {
        QFile::remove(fileNamePatched);
    }

    QFile::copy(fileNameBackup, fileNamePatched);

    PeFile* peFile = new PeFile();

    // Add all functions for target.
    for (QString &functionName : Constants::targets.value(target).keys()) {
        peFile->addFunction(Constants::patch_library_name, functionName);
    }

    // Load PE from file.
    peFile->load(path, target);

    // Apply PE and binary patches.
    peFile->apply();

    // Save PE from memory to file.
    peFile->save();
}

bool Patcher::generateNetworkConfigFile(const QString &path, const QString &address)
{
    QFile file(path + Constants::patch_network_configuration_file);

    if (file.open(QIODevice::ReadWrite | QIODevice::Truncate)) {
        QTextStream stream(&file);
        stream << address << endl;

        return true;
    }

    return false;
}
