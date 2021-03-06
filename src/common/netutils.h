#ifndef NETUTILS_H
#define NETUTILS_H

class QNetworkInterface;
class QString;

class NetUtils {
public:
    static bool isValid(const QNetworkInterface &networkInterface);
    static QNetworkInterface findValidInterface(const QString &networkInterfaceName);
};

#endif // NETUTILS_H
