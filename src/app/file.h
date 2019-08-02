#ifndef FILE_H
#define FILE_H

#include <QDir>
#include <QFile>
#include <QString>

#include "entry.h"

class File
{
public:
    static bool isValid(const QDir &path, const FileEntry &file, const TargetEntry &target, bool isPatched);
    static bool backup(const QDir &path, const FileEntry &file);
    static bool restore(const QDir &path, const FileEntry &file);

private:
    static QString checkSum(QFile file);
    static bool copy(const QDir &path, const FileEntry &fileEntry, bool isBackup);
};

#endif // FILE_H
