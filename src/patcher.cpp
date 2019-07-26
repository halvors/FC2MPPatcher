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

    // Load PE from file.
    peFile->load(path, target);

    // Apply PE and binary patches.
    // TODO: Verify this, correct to get these from hashmap and not patch specific thingy?
    peFile->apply(Constants::patch_library_name, Constants::targets.value(target).keys());

    // Save PE from memory to file.
    peFile->save();

    delete peFile;
}

bool Patcher::generateNetworkConfigFile(const QString &path, const QNetworkAddressEntry &address)
{
    QFile file(path + Constants::patch_network_configuration_file);

    if (file.open(QIODevice::ReadWrite | QIODevice::Truncate)) {
        QTextStream stream(&file);
        stream << address.ip().toString() << endl;

        return true;
    }

    return false;
}
