#ifndef PATCHER_H
#define PATCHER_H

#include <QObject>
#include <QString>
#include <QNetworkAddressEntry>

#include "constants.h"

class Patcher
{
public:
    static bool isFileValid(const QString &path, const TargetEntry &target);
    static bool isFileTypeMismatch(const QString &path, const TargetEntry &target);
    static bool backupFile(const QString &path, const TargetEntry &target);
    static bool restoreFile(const QString &path, const TargetEntry &target);

    static void applyPatch(const QString &path, const TargetEntry &target);
    static void generateNetworkConfigFile(const QString &path, const QNetworkAddressEntry &address);

private:
    static QString checksumFile(const QString &fileName);
    static bool copyFile(const QString &path, const TargetEntry &target, bool isBackup);
};

#endif // PATCHER_H
