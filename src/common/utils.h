#ifndef UTILS_H
#define UTILS_H

class QNetworkInterface;
class QString;
class QByteArray;

class Utils {
public:
    static bool isValid(const QNetworkInterface &networkInterface);
    static QNetworkInterface findValidInterface(const QString &networkInterfaceName);
    static QByteArray hashPassword(const QByteArray &password);
};

#endif // UTILS_H
