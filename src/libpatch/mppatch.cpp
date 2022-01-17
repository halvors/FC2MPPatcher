#include "mppatch.h"

#include <cstring>

#include <QCryptographicHash>
#include <QHostAddress>
#include <QNetworkInterface>
#include <QSettings>

#include "defs.h"
#include "helper.h"
#include "patch_defs.h"
#include "netutils.h"
#include "cryptoutils.h"

void MPPatch::readSettings()
{
    if (!address.empty() && !broadcast.empty())
        return;

    QSettings settings(patch_configuration_file, QSettings::IniFormat);

    settings.beginGroup(patch_configuration_network);
        QNetworkInterface networkInterface = NetUtils::findValidInterface(settings.value(patch_configuration_network_interface).toString());

        // Scan thru addresses for this interface.
        for (const QNetworkAddressEntry &addressEntry : networkInterface.addressEntries()) {
            QHostAddress hostAddress = addressEntry.ip();

            // We're only looking for IPv4 addresses.
            if (hostAddress.protocol() == QAbstractSocket::IPv4Protocol) {
                address = hostAddress.toString().toStdString();
                broadcast = addressEntry.broadcast().toString().toStdString();
            }
        }
    settings.endGroup();
}

int WSAAPI __stdcall MPPatch::bind_patch(SOCKET s, const sockaddr* name, int namelen)
{
    sockaddr_in* name_in = reinterpret_cast<sockaddr_in*>(const_cast<sockaddr*>(name));

    // Change address to bind to any.
    name_in->sin_addr.s_addr = INADDR_ANY;

    return bind(s, name, namelen);
}

int WSAAPI __stdcall MPPatch::sendTo_patch(SOCKET s, const char* buf, int len, int flags, const sockaddr* to, int tolen)
{
    sockaddr_in* to_in = reinterpret_cast<sockaddr_in*>(const_cast<sockaddr*>(to));

    readSettings();

    // If destination address is 255.255.255.255, use subnet broadcast address instead.
    if (to_in->sin_addr.s_addr == INADDR_BROADCAST)
        to_in->sin_addr.s_addr = inet_addr(broadcast.c_str());

    return sendto(s, buf, len, flags, to, tolen);
}

uint64_t __stdcall MPPatch::getAdaptersInfo_patch(IP_ADAPTER_INFO* adapterInfo, unsigned long* sizePointer)
{
    unsigned long result = GetAdaptersInfo(adapterInfo, sizePointer);

    if (result == ERROR_BUFFER_OVERFLOW)
        return result;

    IP_ADAPTER_INFO* adapter = adapterInfo;

    readSettings();

    while (strcmp(adapter->IpAddressList.IpAddress.String, address.c_str()) != 0)
        adapter = adapter->Next;

    adapter->Next = nullptr;
    std::memcpy(adapterInfo, adapter, sizeof(IP_ADAPTER_INFO));

    return result;
}

hostent* WSAAPI __stdcall MPPatch::getHostByName_patch(const char* name)
{
    Q_UNUSED(name);

    readSettings();

    return gethostbyname(address.c_str());
}

int __cdecl MPPatch::generateCdKeyIdHex(uint8_t* out, uint32_t* outLen, char* serialName, char* cdKey)
{
    uint32_t version = Helper::toInt(APP_VERSION);

    // CD key hex
    QCryptographicHash cdKeyHash(QCryptographicHash::Algorithm::Sha1);
    cdKeyHash.addData(serialName);
    cdKeyHash.addData(cdKey);

    // Machine id
    QCryptographicHash identifierHash(QCryptographicHash::Algorithm::Sha1);
    identifierHash.addData(QSysInfo::machineUniqueId());

    // Sending ASN.1 structure inside IA5String, currently 12 bytes of space left in buffer of 128 bytes.
    QByteArray result = QByteArray("\x30\x32").append( // SEQUENCE
                                   "\x80\x04").append(reinterpret_cast<const char*>(&version), sizeof(version)).append( // INTEGER
                                   "\x81\x14").append(cdKeyHash.result()).append( // OCTET STRING
                                   "\x82\x14").append(identifierHash.result()).toHex(); // OCTET STRING
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
