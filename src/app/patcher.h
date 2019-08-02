#ifndef PATCHER_H
#define PATCHER_H

#include <QString>
#include <QWidget>
#include <QNetworkAddressEntry>

#include "entry.h"

class Patcher
{
public:
    static bool isPatched(const QString &path);
    static bool patch(QWidget* parent, const QString &path);
    static void undoPatch(const QString &path);
    static void generateNetworkConfigFile(const QString &path, const QNetworkAddressEntry &address);

private:
    static void copyFiles(const QString &path);
    static bool patchFile(const QString &path, const FileEntry &file, const TargetEntry &target);
};

#endif // PATCHER_H
