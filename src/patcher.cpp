#include <QFile>
#include <QCryptographicHash>
#include <QSettings>
#include <QTextStream>

#include "patcher.h"
#include "pefile.h"

QString Patcher::checksumFile(const QString &fileName)
{
    QFile file(fileName);

    if (file.open(QFile::ReadOnly)) {
        QCryptographicHash hash(QCryptographicHash::Sha1);

        if (hash.addData(&file)) {
            return hash.result().toHex();
        }
    }

    return QString();
}

bool Patcher::isFileValid(const QString &path, const TargetEntry &target)
{
    return target.getFileCheckSum() == checksumFile(path + target.getFileName());
}

bool Patcher::copyFile(const QString &path, const TargetEntry &target, bool isBackup)
{
    QStringList split = target.getFileName().split(".");
    QString suffix = "." + split.takeLast();
    QString fileName = path + split.join(QString()) + suffix;
    QString fileNameCopy = path + split.join(QString()) + Constants::target_backup_suffix + suffix;

    if (isBackup) {
        if (!QFile::exists(fileNameCopy)) {
            return QFile::copy(fileName, fileNameCopy);
        }
    } else {
        if (!QFile::exists(fileName) || QFile::remove(fileName)) {
            return QFile::copy(fileNameCopy, fileName);
        }
    }

    return false;
}

bool Patcher::backupFile(const QString &path, const TargetEntry &target)
{
    return copyFile(path, target);
}

bool Patcher::restoreFile(const QString &path, const TargetEntry &target)
{
    return copyFile(path, target, false);
}

void Patcher::applyPatch(const QString &path, const TargetEntry &target)
{
    // Create PeFile instance for this particular target.
    PeFile* peFile = new PeFile();

    // Load PE from file.
    peFile->load(path, target.getFileName());

    // Apply PE and binary patches.
    peFile->apply(Constants::patch_library_file, target.getFunctions());

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

    // Old way for compatibility with old library.
    QFile file(path + Constants::patch_network_configuration_file);

    if (file.open(QIODevice::ReadWrite | QIODevice::Truncate)) {
        QTextStream stream(&file);
        stream << address.ip().toString() << endl;
    }
}
