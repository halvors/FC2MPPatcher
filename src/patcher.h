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

    static void applyPatch(const QString &path, const QString &fileName);
    static bool generateNetworkConfigFile(const QString &path, const QNetworkAddressEntry &address);

};

#endif // PATCHER_H
