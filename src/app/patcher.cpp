#include <QFile>
#include <QCryptographicHash>
#include <QSettings>

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

TargetType Patcher::detectType(const QString &path)
{
    // Loop thru target and return if a match is found.
    for (const TargetEntry &target : Constants::targets) {
        if (isFileValid(path, target)) {
            return target.getType();
        }
    }

    return TargetType::UNKNOWN;
}

bool Patcher::isFileTypeMismatch(const QString &path, const TargetEntry &target)
{
    QString checkSum = checksumFile(path + target.getFileName());

    for (const TargetEntry &targetEntry : Constants::targets) {
        if (targetEntry.getType() != target.getType() && targetEntry.getFileCheckSum() == checkSum) {
            return true;
        }
    }

    return false;
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
    return copyFile(path, target, true);
}

bool Patcher::restoreFile(const QString &path, const TargetEntry &target)
{
    return copyFile(path, target, false);
}

void Patcher::applyPatch(const QString &path, const TargetEntry &target)
{
    qDebug() << "Patching:" << target.getFileName();

    // Create PeFile instance for this particular target.
    PeFile* peFile = new PeFile(path + target.getFileName());

    // Apply PE and binary patches.
    peFile->apply(Constants::patch_library_file, Constants::patch_library_functions, target.getFunctions(), Constants::patch_pe_section);

    // Write PE to file.
    peFile->write();

    delete peFile;
}

void Patcher::generateNetworkConfigFile(const QString &path, const QNetworkAddressEntry &address)
{
    QSettings settings(path + Constants::patch_network_configuration_file, QSettings::IniFormat);

    settings.beginGroup(Constants::patch_library_name);
        settings.setValue(Constants::patch_network_configuration_address, address.ip().toString());
        settings.setValue(Constants::patch_network_configuration_broadcast, address.broadcast().toString());
        settings.setValue(Constants::patch_network_configuration_netmask, address.netmask().toString());
    settings.endGroup();
}

QString Patcher::findPath()
{
    QString installDirectory = QString();

#ifdef Q_OS_WIN
    // Extract Far Cry 2 registry installdir here.
    QSettings registry("HKEY_LOCAL_MACHINE\\SOFTWARE", QSettings::NativeFormat, this);
    registry.beginGroup("7-Zip");

    qDebug() << "Registry:" << registry.value("Path").toString();

    registry.endGroup();
#endif

    return installDirectory;
}
