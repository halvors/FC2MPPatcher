#include <QCryptographicHash>
#include <QStringList>
#include <QDebug>

#include "fileutils.h"
#include "constants.h"

QString FileUtils::checkSum(QFile file)
{
    if (file.open(QFile::ReadOnly)) {
        QCryptographicHash hash(QCryptographicHash::Sha256);
        hash.addData(&file);
        file.close();

        return hash.result().toHex();
    }

    return QString();
}

bool FileUtils::isValid(const QDir &path, const FileEntry &fileEntry, const TargetEntry &target, bool isPatched)
{
    return (isPatched ? target.getCheckSumPatched() : target.getCheckSum()) == checkSum(path.filePath(fileEntry.getName()));
}

QString FileUtils::appendToName(const QDir &path, const FileEntry &fileEntry, const QString &toAppend)
{
    QStringList split = QString(fileEntry.getName()).split(".");
    QString suffix = '.' + split.takeLast();

    return path.filePath(split.join(QString()) + toAppend + suffix);
}

bool FileUtils::copy(const QDir &path, const FileEntry &fileEntry, bool isBackup)
{
    QFile file = path.filePath(fileEntry.getName());
    QFile fileCopy = appendToName(path, fileEntry, Constants::game_backup_file_suffix);

    if (isBackup) {
        if (!fileCopy.exists()) {
            return file.copy(fileCopy.fileName());
        }
    } else {
        if (fileCopy.exists() && (!file.exists() || file.remove())) {
            return fileCopy.copy(file.fileName());
        }
    }

    return false;
}

bool FileUtils::backup(const QDir &path, const FileEntry &fileEntry)
{
    bool result = copy(path, fileEntry, true);

    if (result) {
        qDebug() << QT_TR_NOOP(QString("Backing up: %1").arg(fileEntry.getName()));
    }

    return result;
}

bool FileUtils::restore(const QDir &path, const FileEntry &fileEntry)
{
    bool result = copy(path, fileEntry, false);

    if (result) {
        qDebug() << QT_TR_NOOP(QString("Restoring: %1").arg(fileEntry.getName()));
    }

    return result;
}
