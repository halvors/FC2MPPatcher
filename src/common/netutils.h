#ifndef NETUTILS_H
#define NETUTILS_H

#include <QNetworkInterface>
#include <QString>

class NetUtils
{
public:
    static bool isValid(const QNetworkInterface &interface);
    static QNetworkInterface findValidInterface(const QString &interfaceName);
};

#endif // NETUTILS_H
