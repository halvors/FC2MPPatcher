#include "mppatch.h"

#include <cstring>

#include <QCryptographicHash>
#include <QHostAddress>
#include <QNetworkInterface>
#include <QSettings>

#include "defs.h"
#include "patch_defs.h"
#include "netutils.h"
#include "cryptoutils.h"
#include "HTTPRequest.h"

void MPPatch::readSettings()
{
    if (!address.isEmpty() && !broadcast.isEmpty())
        return;

    QSettings *settings = new QSettings(patch_configuration_file, QSettings::IniFormat);

    settings->beginGroup(patch_configuration_network);
        QNetworkInterface networkInterface = NetUtils::findValidInterface(settings->value(patch_configuration_network_interface).toString());

        // Scan thru addresses for this interface.
        for (const QNetworkAddressEntry &addressEntry : networkInterface.addressEntries()) {
            QHostAddress hostAddress = addressEntry.ip();

            // We're only looking for IPv4 addresses.
            if (hostAddress.protocol() == QAbstractSocket::IPv4Protocol) {
                address = hostAddress.toString();
                broadcast = addressEntry.broadcast().toString();
            }
        }
    settings->endGroup();

    delete settings;
}

int WSAAPI __stdcall MPPatch::bind_patch(SOCKET s, const sockaddr* name, int namelen)
{
    sockaddr_in *name_in = reinterpret_cast<sockaddr_in*>(const_cast<sockaddr*>(name));

    // Change address to bind to any.
    name_in->sin_addr.s_addr = INADDR_ANY;

    return bind(s, name, namelen);
}

int WSAAPI __stdcall MPPatch::sendTo_patch(SOCKET s, const char* buf, int len, int flags, const sockaddr* to, int tolen)
{
    sockaddr_in *to_in = reinterpret_cast<sockaddr_in*>(const_cast<sockaddr*>(to));

    readSettings();

    // If destination address is 255.255.255.255, use subnet broadcast address instead.
    if (to_in->sin_addr.s_addr == INADDR_BROADCAST)
        to_in->sin_addr.s_addr = inet_addr(broadcast.toStdString().c_str());

    return sendto(s, buf, len, flags, to, tolen);
}

uint64_t __stdcall MPPatch::getAdaptersInfo_patch(IP_ADAPTER_INFO* adapterInfo, unsigned long* sizePointer)
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

hostent *WSAAPI __stdcall MPPatch::getHostByName_patch(const char* name)
{
    Q_UNUSED(name);

    readSettings();

    return gethostbyname(address.toStdString().c_str());
}

int __cdecl MPPatch::generateCdKeyIdHex(uint8_t* out, uint32_t* outLen, char* serialName, char* cdKey)
{
    Q_UNUSED(cdKey);

    // TODO: Read out hardware unique id?
    // https://www.codeproject.com/Tips/5263343/How-to-Get-the-BIOS-UUID

    QCryptographicHash cdKeyHash(QCryptographicHash::Algorithm::Sha1);
    cdKeyHash.addData(serialName);
    cdKeyHash.addData(QSysInfo::machineUniqueId());

    QByteArray result = cdKeyHash.result().toHex();
    *outLen = result.size();
    std::memcpy(out, result.constData(), *outLen);

    return 0;
}

int __cdecl MPPatch::generateOneTimeKey(uint8_t* out, uint32_t* outLen, char* challenge, char* username, char* password)
{
    QByteArray passwordHash = CryptoUtils::hashPassword(password).toHex();

    QCryptographicHash oneTimeHash(QCryptographicHash::Algorithm::Sha1);
    oneTimeHash.addData(username);
    oneTimeHash.addData(challenge);
    oneTimeHash.addData(passwordHash);

    QByteArray result = oneTimeHash.result().toHex();
    *outLen = result.size();
    std::memcpy(out, result.constData(), *outLen);

    return 0;
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
    } catch (const std::exception& e) {

    }

    return publicAddress;
}
