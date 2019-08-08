#ifndef FILEUTILS_H
#define FILEUTILS_H

#include <QDir>
#include <QString>
#include <QFile>

#include "entry.h"

class FileUtils
{
public:
    static bool isValid(const QDir &path, const FileEntry &fileEntry, const TargetEntry &target, bool isPatched);
    static QString appendToName(const QDir &path, const FileEntry &fileEntry, const QString &toAppend);
    static bool backup(const QDir &path, const FileEntry &fileEntry);
    static bool restore(const QDir &path, const FileEntry &fileEntry);

private:
    static QString checkSum(QFile file);
    static bool copy(const QDir &path, const FileEntry &fileEntry, bool isBackup);
};

#endif // FILEUTILS_H
