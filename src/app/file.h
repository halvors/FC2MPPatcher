#ifndef FILE_H
#define FILE_H

#include <QString>

#include "entry.h"

class File
{
public:
    static QString getCheckSum(const QString &fileName);
    static bool isValid(const QString &path, const FileEntry &file, const TargetEntry &target, bool patched);

    static bool backup(const QString &path, const FileEntry &file);
    static bool restore(const QString &path, const FileEntry &file);

private:
    static bool copyIfNotExists(const QString &fileName);
    static bool copy(const QString &path, const FileEntry &file, bool isBackup);
};

#endif // FILE_H
