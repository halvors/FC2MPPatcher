#include <QCryptographicHash>
#include <QStringList>
#include <QDebug>

#include "fileutils.h"
#include "defs.h"

#if defined(Q_OS_WIN)
#include <qt_windows.h>
#endif

QByteArray FileUtils::calculateChecksum(QFile file)
{
    if (!file.open(QFile::ReadOnly))
        return nullptr;

    QCryptographicHash hash(QCryptographicHash::Sha256);
    hash.addData(&file);
    file.close();

    return hash.result().toHex();
}

bool FileUtils::isValid(const QDir &dir, const FileEntry &file, const TargetEntry &target, bool patched)
{
    QByteArray fileChecksum = calculateChecksum(dir.filePath(file.getName()));

    if (fileChecksum.isEmpty())
        return false;

    for (const HashEntry &entry : target.getHashEntries()) {
        QByteArray targetChecksum = patched ? entry.result : entry.original;

        if (fileChecksum == targetChecksum)
            return true;
    }

    return false;
}

QString FileUtils::appendToName(const QDir &dir, const QString &fileName, const QString &tail)
{
    QStringList split = QString(fileName).split('.');
    QString suffix = '.' + split.takeLast();

    return dir.filePath(split.join(QString()) + tail + suffix);
}

bool FileUtils::setHidden(const QDir &dir, const QString &fileName, bool hidden)
{
    bool prefix = fileName.startsWith(game_hidden_prefix);
    QString newFileName = QString(fileName);

    if (hidden && !prefix) {
        newFileName = newFileName.prepend(game_hidden_prefix);
    } else if (!hidden && prefix) {
        newFileName.remove(0, 1);
    }

    bool success = true;

#if defined(Q_OS_LINUX)
    QFile file = dir.filePath(fileName);
    QString newFilePath = dir.filePath(newFileName);
    file.rename(newFilePath);

    success &= newFileName.startsWith(game_hidden_prefix) == hidden;
#elif defined(Q_OS_WIN)
    std::wstring newFileNameW = dir.filePath(newFileName).toStdWString();
    DWORD attributes = GetFileAttributesW(newFileNameW.c_str());

    if (hidden)
        attributes |= FILE_ATTRIBUTE_HIDDEN;
    else
        attributes &= ~FILE_ATTRIBUTE_HIDDEN;

    success &= SetFileAttributesW(newFileNameW.c_str(), attributes);
#endif

    return success;
}

bool FileUtils::replicate(const QDir &dir, const FileEntry &fileEntry, bool backup)
{
    const QString fileName = fileEntry.getName();
    const QString fileCopyName = QString(fileName).prepend(game_hidden_prefix);
    const QString &filePath = dir.filePath(fileName);
    const QString &fileCopyPath = dir.filePath(fileCopyName);

    QFile file = dir.filePath(fileName);
    QFile fileCopy = dir.filePath(fileCopyName);

    if (backup) {
        qDebug().noquote() << QT_TR_NOOP(QString("Backing up file: %1").arg(fileName));

        return !fileCopy.exists() && (file.rename(fileCopyPath) &
                                      file.copy(filePath) &
                                      setHidden(dir, fileCopyName, backup));
    } else {
        qDebug().noquote() << QT_TR_NOOP(QString("Restoring file: %1").arg(fileName));

        // Temp, leave for some iterations, was changed in 0.1.12.
        QFile fileCopyLegacy = appendToName(dir, fileName, game_backup_suffix);

        if (fileCopyLegacy.exists() && (file.remove() & fileCopyLegacy.rename(filePath)))
            return true;

        return fileCopy.exists() && (file.remove() &
                                     fileCopy.rename(filePath) &
                                     setHidden(dir, fileName, backup));
    }

    return false;
}
