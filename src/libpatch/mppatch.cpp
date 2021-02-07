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
                    address = addressEntry.ip().toString();
                    broadcast = addressEntry.broadcast().toString();
                }
            }
        settings->endGroup();
    }

    // TODO: Delete settings? Memory leak.
}

int WSAAPI __stdcall MPPatch::bind_patch(SOCKET s, const sockaddr *name, int namelen)
{
    sockaddr_in *name_in = reinterpret_cast<sockaddr_in*>(const_cast<sockaddr*>(name));

    // Change address to bind to any.
    name_in->sin_addr.s_addr = INADDR_ANY;

    return bind(s, name, namelen);
}

int WSAAPI __stdcall MPPatch::connect_patch(SOCKET s, const sockaddr *name, int namelen)
{
    sockaddr_in *name_in = reinterpret_cast<sockaddr_in*>(const_cast<sockaddr*>(name));

    // If connecting to lobbyserver on port 3100, use default lobby server port instead.
    if (name_in->sin_addr.s_addr == inet_addr(patch_network_lobbyserver_address) && name_in->sin_port == htons(3100)) {
        name_in->sin_port = htons(patch_network_lobbyserver_port);
    }

    return connect(s, name, namelen);
}

int WSAAPI __stdcall MPPatch::sendTo_patch(SOCKET s, const char *buf, int len, int flags, const sockaddr *to, int tolen)
{
    sockaddr_in *to_in = reinterpret_cast<sockaddr_in*>(const_cast<sockaddr*>(to));

    readSettings();

    // If destination address is 255.255.255.255, use subnet broadcast address instead.
    if (to_in->sin_addr.s_addr == INADDR_BROADCAST) {
        // TODO: Test this experimental feature supporting multiple network interfaces even when doing broadcasts.
        // Loop thru all of the systems network interfaces.
        for (const QNetworkInterface &interface : QNetworkInterface::allInterfaces()) {
            const QNetworkInterface::InterfaceFlags &flags = interface.flags();

            // Only show active network interfaces and not loopback interfaces.
            if (flags.testFlag(QNetworkInterface::IsUp) && !flags.testFlag(QNetworkInterface::IsLoopBack) && !flags.testFlag(QNetworkInterface::CanBroadcast)) { // TODO: OR these together somehow?
                // Scan thru addresses for this interface.
                for (const QNetworkAddressEntry &addressEntry : interface.addressEntries()) {
                    // Only select first IPv4 address found.
                    if (addressEntry.ip().protocol() == QAbstractSocket::IPv4Protocol) {
                        to_in->sin_addr.s_addr = inet_addr(addressEntry.broadcast().toStdString().c_str());
                        sendto(s, buf, len, flags, to, tolen);

                        break;
                    }
                }

                ui->comboBox_network_interface->addItem(interface.humanReadableName() + " (" + selectedAddressEntry.ip().toString() + ")", QVariant::fromValue<QNetworkInterface>(interface));
            }
        }
    }

    return sendto(s, buf, len, flags, to, tolen);
}

unsigned long __stdcall MPPatch::getAdaptersInfo_patch(IP_ADAPTER_INFO *adapterInfo, unsigned long *sizePointer)
{
    unsigned long result = GetAdaptersInfo(adapterInfo, sizePointer);

    if (result == ERROR_BUFFER_OVERFLOW) {
        return result;
    }

    IP_ADAPTER_INFO *adapter = adapterInfo;

    readSettings();

    while (strcmp(adapter->IpAddressList.IpAddress.String, address.toStdString().c_str()) != 0) {
        adapter = adapter->Next;
    }

    adapter->Next = nullptr;
    memcpy(adapterInfo, adapter, sizeof(IP_ADAPTER_INFO));

    return result;
}

hostent *WSAAPI __stdcall MPPatch::getHostByName_patch(const char *name)
{
    Q_UNUSED(name);

    readSettings();

    return gethostbyname(address.toStdString().c_str());
}
