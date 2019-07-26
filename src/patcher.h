#ifndef PATCHER_H
#define PATCHER_H

#include <QObject>
#include <QString>
#include <QNetworkAddressEntry>

#include "constants.h"
#include "pefile.h"

class Patcher : public QObject
{
    Q_OBJECT

public:
    explicit Patcher(QObject *parent = nullptr);

    static bool isValid(const QString &path, const TargetEntry &target);
    static void applyPatch(const QString &path, const TargetEntry &target);
    static void generateNetworkConfigFile(const QString &path, const QNetworkAddressEntry &address);

private:
    static QString checksum(const QString &filePath);
};

#endif // PATCHER_H
