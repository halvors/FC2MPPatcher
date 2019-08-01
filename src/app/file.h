#ifndef FILE_H
#define FILE_H

#include <QString>

#include "entry.h"

class File
{
public:
    static bool isValid(const QString &path, const FileEntry &file, const TargetEntry &target, bool isPatched);

    static bool backup(const QString &path, const FileEntry &file);
    static bool restore(const QString &path, const FileEntry &file);

private:
    static QString checkSum(const QString &fileName);
    static bool copy(const QString &path, const FileEntry &file, bool isBackup);
};

#endif // FILE_H
