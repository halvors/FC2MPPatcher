#ifndef NETUTILS_H
#define NETUTILS_H

#include <QNetworkInterface>
#include <QList>
#include <QString>

class NetUtils {
public:
    static bool isValid(const QNetworkInterface &networkInterface);
    static QList<QNetworkInterface> getValidInterfaces();
    static QNetworkInterface findValidInterface(const QString &networkInterfaceName);
};

#endif // NETUTILS_H
