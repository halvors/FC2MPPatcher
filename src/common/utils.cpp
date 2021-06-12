#include "utils.h"

#include <QNetworkInterface>
#include <QString>
#include <argon2.h>

#define HASH_LENGTH 32
#define SALT_LENGTH 16

bool Utils::isValid(const QNetworkInterface &networkInterface)
{
    QNetworkInterface::InterfaceFlags flags = networkInterface.flags();

    return networkInterface.isValid() &&
           !flags.testFlag(QNetworkInterface::IsLoopBack) &&
           flags.testFlag(QNetworkInterface::IsUp) &&
           flags.testFlag(QNetworkInterface::CanBroadcast);
}

QNetworkInterface Utils::findValidInterface(const QString &networkInterfaceName)
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

QByteArray Utils::hashPassword(const QByteArray &password)
{
    if (!password.isEmpty()) {
        QByteArray hash(HASH_LENGTH, 0);

        const uint32_t timeCost = 3;      // 3-pass computation
        const uint32_t memoryCost = 4096;   // 64 mebibytes memory usage
        const uint32_t parallelism = 1; // number of threads and lanes
        const uint8_t salt[SALT_LENGTH] = { 0x9f, 0xfa, 0x6a, 0x64, 0x23, 0x39, 0xf7, 0x95, 0xb0, 0x87, 0x2f, 0x07, 0x0a, 0x02, 0x78, 0xe8 };

        if (argon2i_hash_raw(timeCost, memoryCost, parallelism, password.constData(), password.size(), salt, SALT_LENGTH, hash.data(), hash.size()) == ARGON2_OK)
            return hash;
    }

    return QByteArray();
}
