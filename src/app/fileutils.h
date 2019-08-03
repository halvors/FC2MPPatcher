#ifndef FILEUTILS_H
#define FILEUTILS_H

#include <QDir>
#include <QString>
#include <QFile>

#include "entry.h"

class FileUtils
{
public:
    static bool isValid(const QDir &path, const FileEntry &file, const TargetEntry &target, bool isPatched);
    static bool backup(const QDir &path, const FileEntry &file);
    static bool restore(const QDir &path, const FileEntry &file);

private:
    static QString checkSum(QFile file);
    static bool copy(const QDir &path, const FileEntry &fileEntry, bool isBackup);
};

#endif // FILEUTILS_H
