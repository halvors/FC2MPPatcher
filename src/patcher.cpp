#include <QFile>
#include <QCryptographicHash>
#include <QSettings>
#include <QTextStream>

#include "patcher.h"

Patcher::Patcher(QObject* parent) : QObject(parent)
{

}

bool Patcher::isFileValid(const QString &path, const TargetEntry &target)
{
    return target.checkSum == checksumFile(path + target.fileName);
}

void Patcher::backupFile(const QString &path, const TargetEntry &target)
{
    QString fileName = path + target.fileName;
    QString fileNameBackup = fileName.split(".")[0] + Constants::target_backup_suffix;

    if (!QFile::exists(fileNameBackup)) {
        QFile::copy(fileName, fileNameBackup);
    }
}

QString Patcher::checksumFile(const QString &filePath)
{
    QFile file(filePath);

    if (file.open(QFile::ReadOnly)) {
        QCryptographicHash hash(QCryptographicHash::Sha1);

        if (hash.addData(&file)) {
            return hash.result().toHex();
        }
    }

    return QString();
}

void Patcher::applyPatch(const QString &path, const TargetEntry &target)
{
    // Backup original file.
    backupFile(path, target);

    // Create PeFile instance for this particular target.
    PeFile* peFile = new PeFile();

    // Load PE from file.
    peFile->load(path, target.fileName);

    // Apply PE and binary patches.
    peFile->apply(Constants::patch_library_name, target.functions);

    // Save PE from memory to file.
    peFile->save();

    delete peFile;
}

void Patcher::generateNetworkConfigFile(const QString &path, const QNetworkAddressEntry &address)
{
    QSettings settings(path + Constants::patch_network_configuration_file + ".ini", QSettings::IniFormat);

    settings.beginGroup(Constants::patch_library_name);
        settings.setValue("Address", address.ip().toString());
        settings.setValue("Netmask", address.netmask().toString());
        settings.setValue("Broadcast", address.broadcast().toString());
    settings.endGroup();

    QFile file(path + Constants::patch_network_configuration_file);

    if (file.open(QIODevice::ReadWrite | QIODevice::Truncate)) {
        QTextStream stream(&file);
        stream << address.ip().toString() << endl;
    }
}
