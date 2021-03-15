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
    enum State {
        INSTALLED,
        NOT_INSTALLED,
        UPGRADABLE,
        INVALID
    };

    static enum State isPatched(const QString &path);
    static bool patch(const QDir &dir, QWidget *widget = nullptr);
    static void undoPatch(const QDir &dir);
    static void generateConfigurationFile(const QDir &dir, const QNetworkInterface &interface);

private:
    static bool copyFiles(const QDir &dir);
    static bool patchFile(const QDir &dir, const FileEntry &fileEntry, const TargetEntry &target);
    static void log(const QString &msg, QWidget *widget = nullptr);
};

#endif // PATCHER_H
