#include <QSettings>
#include <QNetworkInterface>
#include <QHostAddress>

#include <iostream>

#include "mppatch.h"
#include "global.h"
#include "HTTPRequest.h"

void MPPatch::readSettings()
{
    if (!address.isEmpty() && !broadcast.isEmpty())
        return;

    QSettings *settings = new QSettings(patch_configuration_file, QSettings::IniFormat);

    settings->beginGroup(patch_configuration_network);
        const QNetworkInterface &networkInterface = findValidInterface(settings->value(patch_configuration_network_interface).toString());

        // Scan thru addresses for this interface.
        for (const QNetworkAddressEntry &addressEntry : networkInterface.addressEntries()) {
            // We're only looking for IPv4 addresses.
            if (addressEntry.ip().protocol() == QAbstractSocket::IPv4Protocol) {
                address = addressEntry.ip().toString();
                broadcast = addressEntry.broadcast().toString();
            }
        }
    settings->endGroup();

    delete settings;
}

QNetworkInterface MPPatch::findValidInterface(const QString &interfaceName)
{
    QList<QNetworkInterface> list = QNetworkInterface::allInterfaces();

    // Insert configured network interface.
    const QNetworkInterface &configuredNetworkInterface = QNetworkInterface::interfaceFromName(interfaceName);

    if (configuredNetworkInterface.isValid())
        list.prepend(configuredNetworkInterface);

    // Loop thru all of the systems network interfaces, and return the first valid found.
    for (const QNetworkInterface &networkInterface : list) {
        const QNetworkInterface::InterfaceFlags &flags = networkInterface.flags();

        // Skip invalid interfaces and loopback interfaces.
        if (!networkInterface.isValid() && !flags.testFlag(QNetworkInterface::IsLoopBack))
            continue;

        // We only want active network interfaces.
        if (flags.testFlag(QNetworkInterface::IsUp)) {
            // Scan thru addresses for this interface.
            for (const QNetworkAddressEntry &addressEntry : networkInterface.addressEntries()) {
                // We're only looking for IPv4 addresses.
                if (addressEntry.ip().protocol() == QAbstractSocket::IPv4Protocol) {
                    return networkInterface;
                }
            }
        }
    }

    return QNetworkInterface();
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
    if (name_in->sin_addr.s_addr == inet_addr(patch_network_lobbyserver_address) && name_in->sin_port == htons(3100))
        name_in->sin_port = htons(patch_network_lobbyserver_port);

    return connect(s, name, namelen);
}

int WSAAPI __stdcall MPPatch::sendTo_patch(SOCKET s, const char *buf, int len, int flags, const sockaddr *to, int tolen)
{
    sockaddr_in *to_in = reinterpret_cast<sockaddr_in*>(const_cast<sockaddr*>(to));

    readSettings();

    // If destination address is 255.255.255.255, use subnet broadcast address instead.
    if (to_in->sin_addr.s_addr == INADDR_BROADCAST)
        to_in->sin_addr.s_addr = inet_addr(broadcast.toStdString().c_str());

    return sendto(s, buf, len, flags, to, tolen);
}

unsigned long __stdcall MPPatch::getAdaptersInfo_patch(IP_ADAPTER_INFO *adapterInfo, unsigned long *sizePointer)
{
    unsigned long result = GetAdaptersInfo(adapterInfo, sizePointer);

    if (result == ERROR_BUFFER_OVERFLOW)
        return result;

    IP_ADAPTER_INFO *adapter = adapterInfo;

    readSettings();

    while (strcmp(adapter->IpAddressList.IpAddress.String, address.toStdString().c_str()) != 0)
        adapter = adapter->Next;

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

uint32_t __stdcall MPPatch::getPublicIPAddress()
{
    // Return cached address if it exists.
    if (publicAddress != 0)
        return publicAddress;

    try {
        // you can pass http::InternetProtocol::V6 to Request to make an IPv6 request
        http::Request request("http://api.ipify.org");

        // send a get request
        const http::Response response = request.send("GET");

        publicAddress = htonl(QHostAddress(QString::fromStdString(std::string(response.body.begin(), response.body.end()))).toIPv4Address());
    } catch (const std::exception &e) {

    }

    /*
    // Query for public ip address.
    QNetworkAccessManager *con = new QNetworkAccessManager();
    QNetworkReply *reply = con->get(QNetworkRequest(QUrl("http://api.ipify.org")));

    // Trick to do this synchronously.
    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (reply->error() == QNetworkReply::NoError)
        publicAddress = QHostAddress(reply->readAll().constData()).toIPv4Address();

    delete reply;
    delete con;
    */

    return publicAddress;
}
