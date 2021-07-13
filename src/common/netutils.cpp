#include "netutils.h"

#include <QNetworkInterface>
#include <QString>
#include <QList>
#include <QNetworkAddressEntry>

bool NetUtils::isValid(const QNetworkInterface &networkInterface)
{
    const QNetworkInterface::InterfaceFlags flags = networkInterface.flags();

    return networkInterface.isValid() &&
           !flags.testFlag(QNetworkInterface::IsLoopBack) &&
           flags.testFlag(QNetworkInterface::IsUp) &&
           flags.testFlag(QNetworkInterface::CanBroadcast);
}

QNetworkInterface NetUtils::findValidInterface(const QString &networkInterfaceName)
{
    QList<QNetworkInterface> list = QNetworkInterface::allInterfaces();

    // Insert configured network interface.
    list.prepend(QNetworkInterface::interfaceFromName(networkInterfaceName));

    // Loop thru all of the systems network interfaces, and return the first valid found.
    for (const QNetworkInterface &interface : list) {
        // Skip invalid interfaces and loopback interfaces.
        if (!isValid(interface))
            continue;

        // Scan thru addresses for this interface.
        for (const QNetworkAddressEntry &addressEntry : interface.addressEntries()) {
            // We're only looking for IPv4 addresses.
            if (addressEntry.ip().protocol() == QAbstractSocket::IPv4Protocol)
                return interface;
        }
    }

    return QNetworkInterface();
}
