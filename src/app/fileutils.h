#ifndef FILEUTILS_H
#define FILEUTILS_H

#include <QFile>
#include <QDir>
#include <QString>

#include "entry.h"

class FileUtils
{
public:
    static const char *checkSum(QFile file);
    static bool isValid(const QDir &dir, const FileEntry &file, const TargetEntry &target, bool patched);
    static QString prependToName(const QDir &dir, const FileEntry &file, const QString &head);
    static QString appendToName(const QDir &dir, const FileEntry &file, const QString &tail);
    static bool backup(const QDir &dir, const FileEntry &file);
    static bool restore(const QDir &dir, const FileEntry &fileEntry);

private:
    static bool setHidden(const QString &fileName, bool hidden);
    static bool copy(const QDir &dir, const FileEntry &fileEntry, bool backup);
};

#endif // FILEUTILS_H
