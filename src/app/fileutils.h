#ifndef FILEUTILS_H
#define FILEUTILS_H

#include <QDir>
#include <QString>
#include <QFile>

#include "entry.h"

class FileUtils
{
public:
    static const char *checkSum(QFile file);
    static bool isValid(const QDir &dir, const FileEntry &file, const TargetEntry &target, bool patched);
    static QString appendToName(const QDir &dir, const FileEntry &fileEntry, const QString &append);
    static bool backup(const QDir &dir, const FileEntry &fileEntry);
    static bool restore(const QDir &dir, const FileEntry &fileEntry);

private:
    static bool copy(const QDir &dir, const FileEntry &fileEntry, bool backup);
};

#endif // FILEUTILS_H
