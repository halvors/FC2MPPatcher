#ifndef FILEUTILS_H
#define FILEUTILS_H

#include <string>

#include <QFile>
#include <QDir>
#include <QString>

#include "entry.h"

class FileUtils
{
public:
    static const std::string calculateChecksum(QFile file);
    static bool isValid(const QDir &dir, const FileEntry &file, const TargetEntry &target, bool patched = false);
    static QString appendToName(const QDir &dir, const QString &fileName, const QString &tail);
    static bool replicate(const QDir &dir, const FileEntry &fileEntry, bool backup);

private:
    static bool setHidden(const QDir &dir, const QString &fileName, bool hidden);
};

#endif // FILEUTILS_H
