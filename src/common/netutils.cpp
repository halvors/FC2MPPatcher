#include "netutils.h"

#include <QNetworkInterface>
#include <QString>

bool NetUtils::isValid(const QNetworkInterface &networkInterface)
{
    QNetworkInterface::InterfaceFlags flags = networkInterface.flags();

    return networkInterface.isValid() &&
           !flags.testFlag(QNetworkInterface::IsLoopBack) &&
           flags.testFlag(QNetworkInterface::IsUp) &&
           flags.testFlag(QNetworkInterface::CanBroadcast);
}

QNetworkInterface NetUtils::findValidInterface(const QString &networkInterfaceName)
{
    QList<QNetworkInterface> list = QNetworkInterface::allInterfaces();

    // Insert configured network interface.
    const QNetworkInterface &configuredInterface = QNetworkInterface::interfaceFromName(networkInterfaceName);

    if (configuredInterface.isValid())
        list.prepend(configuredInterface);

    // Loop thru all of the systems network interfaces, and return the first valid found.
    for (const QNetworkInterface &interface : list) {
        const QNetworkInterface::InterfaceFlags &flags = interface.flags();

        // Skip invalid interfaces and loopback interfaces.
        if (!isValid(interface))
            continue;

        // We only want active network interfaces.
        if (flags.testFlag(QNetworkInterface::IsUp)) {
            // Scan thru addresses for this interface.
            for (const QNetworkAddressEntry &addressEntry : interface.addressEntries()) {
                // We're only looking for IPv4 addresses.
                if (addressEntry.ip().protocol() == QAbstractSocket::IPv4Protocol) {
                    return interface;
                }
            }
        }
    }

    return QNetworkInterface();
}
