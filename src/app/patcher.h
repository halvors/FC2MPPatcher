#ifndef PATCHER_H
#define PATCHER_H

#include <QDir>
#include <QWidget>
#include <QNetworkInterface>

#include "entry.h"

class Patcher
{
public:
    static bool isPatched(QDir dir);
    static bool patch(QWidget* parent, const QDir &dir);
    static void undoPatch(const QDir &dir);
    static void generateNetworkConfigFile(const QDir &dir, const QNetworkInterface &interface);

private:
    static void copyFiles(const QDir &dir);
    static bool patchFile(const QDir &dir, const FileEntry &fileEntry, const TargetEntry &target);
};

#endif // PATCHER_H
