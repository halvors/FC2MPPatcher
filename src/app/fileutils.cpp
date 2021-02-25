#include <QCryptographicHash>
#include <QStringList>
#include <QDebug>

#include "fileutils.h"
#include "defs.h"

#if defined(Q_OS_WIN)
#include <qt_windows.h>
#endif

const char *FileUtils::checkSum(QFile file)
{
    if (!file.open(QFile::ReadOnly))
        return nullptr;

    QCryptographicHash hash(QCryptographicHash::Sha256);
    hash.addData(&file);
    file.close();

    return hash.result().toHex().constData();
}

bool FileUtils::isValid(const QDir &dir, const FileEntry &file, const TargetEntry &target, bool patched)
{
    const char *fileCheckSum = checkSum(dir.filePath(file.getName()));

    for (const HashEntry &entry : target.getHashEntries()) {
        const char *targetCheckSum = patched ? entry.result : entry.original;

        if (strcmp(fileCheckSum, targetCheckSum) == 0)
            return true;
    }

    return false;
}

QString FileUtils::prependToName(const QDir &dir, const FileEntry &file, const QString &head)
{
    QStringList split = QString(file.getName()).split('.');
    QString suffix = '.' + split.takeLast();

    return dir.filePath(head + split.join(QString()) + suffix);
}

QString FileUtils::appendToName(const QDir &dir, const FileEntry &file, const QString &tail)
{
    QStringList split = QString(file.getName()).split('.');
    QString suffix = '.' + split.takeLast();

    return dir.filePath(split.join(QString()) + tail + suffix);
}

bool FileUtils::setHidden(const QString &fileName, bool hidden)
{
#if defined(Q_OS_LINUX)
    return fileName.startsWith('.') == hidden;
#elif defined(Q_OS_WIN)
    std::wstring fileNameW = fileName.toStdWString();
    DWORD attributes = GetFileAttributesW(fileNameW.c_str());

    if (hidden)
        attributes |= FILE_ATTRIBUTE_HIDDEN;
    else
        attributes &= ~FILE_ATTRIBUTE_HIDDEN;

    return SetFileAttributesW(fileNameW.c_str(), attributes);
#endif

    return false;
}

bool FileUtils::copy(const QDir &dir, const FileEntry &fileEntry, bool backup)
{
    QString fileName = dir.filePath(fileEntry.getName());
    QString fileCopyName = prependToName(dir, fileEntry, game_backup_prefix);
    QFile file = fileName;
    QFile fileCopy = fileCopyName;

    if (backup) {
        return !fileCopy.exists() &&
               (file.rename(fileCopyName) &
               file.copy(fileName) &
               setHidden(fileCopyName, backup));
    } else {
        // Temp, leave for some iterations, was changed in 0.1.12.
        QFile fileCopyLegacy = appendToName(dir, fileEntry, game_backup_suffix);

        if (fileCopyLegacy.exists() && (file.remove() & fileCopyLegacy.rename(fileName)))
            return true;

        return fileCopy.exists() &&
               (file.remove() &
               fileCopy.rename(fileName) &
               setHidden(fileName, backup));
    }
}

bool FileUtils::backup(const QDir &dir, const FileEntry &file)
{
    bool result = copy(dir, file, true);

    if (result)
        qDebug().noquote() << QT_TR_NOOP(QString("Backing up file: %1").arg(file.getName()));

    return result;
}

bool FileUtils::restore(const QDir &dir, const FileEntry &file)
{
    bool result = copy(dir, file, false);

    if (result)
        qDebug().noquote() << QT_TR_NOOP(QString("Restoring file: %1").arg(file.getName()));

    return result;
}
