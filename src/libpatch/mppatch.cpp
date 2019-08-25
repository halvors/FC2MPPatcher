#include <QNetworkInterface>

#include "mppatch.h"
#include "global.h"

void MPPatch::readSettings()
{
    if (!settings) {
        settings = new QSettings(patch_configuration_file, QSettings::IniFormat);
        settings->beginGroup(patch_configuration_network);
            QNetworkInterface networkInterface = QNetworkInterface::interfaceFromIndex(settings->value(patch_configuration_network_interface_index).toInt());

            // Scan thru addresses for this interface.
            for (const QNetworkAddressEntry &addressEntry : networkInterface.addressEntries()) {
                // We're onlt looking for IPv4 addresses.
                if (addressEntry.ip().protocol() == QAbstractSocket::IPv4Protocol) {
                    broadcast = addressEntry.broadcast().toString();
                }
            }
        settings->endGroup();
    }

    // TODO: Delete settings? Memory leak.
}

int WSAAPI __stdcall MPPatch::bind_patch(SOCKET s, const sockaddr* name, int namelen)
{
    sockaddr_in* name_in = reinterpret_cast<sockaddr_in*>(const_cast<sockaddr*>(name));

    // Change address to bind to any.
    name_in->sin_addr.s_addr = inet_addr("0.0.0.0");

    return bind(s, name, namelen);
}

int WSAAPI __stdcall MPPatch::connect_patch(SOCKET s, const sockaddr *name, int namelen)
{
    sockaddr_in* name_in = reinterpret_cast<sockaddr_in*>(const_cast<sockaddr*>(name));

    // If connecting to lobbyserver on port 3100, use default lobby server port instead.
    if (name_in->sin_addr.s_addr == inet_addr(patch_network_lobbyserver_address) && name_in->sin_port == htons(3100)) {
        name_in->sin_port = htons(patch_network_lobbyserver_port);
    }

    return connect(s, name, namelen);
}

int WSAAPI __stdcall MPPatch::sendTo_patch(SOCKET s, const char* buf, int len, int flags, const sockaddr* to, int tolen)
{
    sockaddr_in* to_in = reinterpret_cast<sockaddr_in*>(const_cast<sockaddr*>(to));

    readSettings();

    // If destination address is 255.255.255.255, use subnet broadcast address instead.
    if (to_in->sin_addr.s_addr == inet_addr("255.255.255.255")) {
        to_in->sin_addr.s_addr = inet_addr(broadcast.toStdString().c_str());
    }

    return sendto(s, buf, len, flags, to, tolen);
}
