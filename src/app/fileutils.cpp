#include <QCryptographicHash>
#include <QStringList>
#include <QDebug>

#include "fileutils.h"
#include "constants.h"

QString FileUtils::checkSum(QFile file)
{
    if (file.open(QFile::ReadOnly)) {
        QCryptographicHash hash(QCryptographicHash::Sha1);

        if (hash.addData(&file)) {
            return hash.result().toHex();
        }
    }

    return QString();
}

bool FileUtils::isValid(const QDir &path, const FileEntry &file, const TargetEntry &target, bool isPatched)
{
    return (isPatched ? target.getCheckSumPatched() : target.getCheckSum()) == checkSum(path.filePath(file.getName()));
}

bool FileUtils::copy(const QDir &path, const FileEntry &fileEntry, bool isBackup)
{
    QStringList split = fileEntry.getName().split(".");
    QString suffix = "." + split.takeLast();
    QFile file = path.filePath(split.join(QString()) + suffix);
    QFile fileCopy = path.filePath(split.join(QString()) + Constants::game_backup_file_suffix + suffix);

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

bool FileUtils::backup(const QDir &path, const FileEntry &file)
{
    bool result = copy(path, file, true);

    if (result) {
        qDebug() << "Backing up:" << file.getName();
    }

    return result;
}

bool FileUtils::restore(const QDir &path, const FileEntry &file)
{
    bool result = copy(path, file, false);

    if (result) {
        qDebug() << "Restoring:" << file.getName();
    }

    return result;
}
