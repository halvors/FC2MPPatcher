#ifndef PATCHER_H
#define PATCHER_H

#include <QString>
#include <QWidget>
#include <QDir>
#include <QNetworkInterface>

#include "entry.h"

class Patcher
{
public:
    static bool isPatched(QString path);
    static bool patch(QWidget *parent, const QDir &dir);
    static void undoPatch(const QDir &dir);
    static void generateConfigurationFile(const QDir &dir, const QNetworkInterface &interface);

private:
    static bool copyFiles(const QDir &dir);
    static bool patchFile(const QDir &dir, const FileEntry &fileEntry, const TargetEntry &target);
};

#endif // PATCHER_H
