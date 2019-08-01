#include <QFile>
#include <QCryptographicHash>
#include <QDebug>

#include "file.h"
#include "constants.h"

QString File::checkSum(const QString &fileName)
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

bool File::isValid(const QString &path, const FileEntry &file, const TargetEntry &target, bool isPatched)
{
    QString fileCheckSum = isPatched ? target.getFileCheckSumPatched() : target.getFileCheckSum();

    return fileCheckSum == checkSum(path + file.getFileName());
}

bool File::copy(const QString &path, const FileEntry &file, bool isBackup)
{
    QStringList split = file.getFileName().split(".");
    QString suffix = "." + split.takeLast();
    QString fileName = path + split.join(QString()) + suffix;
    QString fileNameCopy = path + split.join(QString()) + Constants::game_backup_file_suffix + suffix;

    if (isBackup) {
        if (!QFile::exists(fileNameCopy)) {
            return QFile::copy(fileName, fileNameCopy);
        }
    } else {
        if (QFile::exists(fileNameCopy) && (!QFile::exists(fileName) || QFile::remove(fileName))) {
            return QFile::copy(fileNameCopy, fileName);
        }
    }

    return false;
}

bool File::backup(const QString &path, const FileEntry &file)
{
    bool result = copy(path, file, true);

    if (result) {
        qDebug() << "Backing up:" << file.getFileName();
    }

    return result;
}

bool File::restore(const QString &path, const FileEntry &file)
{
    bool result = copy(path, file, false);

    if (result) {
        qDebug() << "Restoring:" << file.getFileName();
    }

    return result;
}
