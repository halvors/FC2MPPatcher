#ifndef PATCHER_H
#define PATCHER_H

#include <QDir>
#include <QWidget>
#include <QNetworkAddressEntry>

#include "entry.h"

class Patcher
{
public:
    static bool isPatched(const QDir &path);
    static bool patch(QWidget* parent, const QDir &path);
    static void undoPatch(const QDir &path);
    static void generateNetworkConfigFile(const QDir &path, const QNetworkAddressEntry &address);

private:
    static void copyFiles(const QDir &path);
    static bool patchFile(const QDir &path, const FileEntry &fileEntry, const TargetEntry &target);
};

#endif // PATCHER_H
