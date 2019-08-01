#include "mppatch.h"
#include "constants.h"

#include <QDebug>

void MPPatch::readSettings()
{
    if (!settings) {
        settings = new QSettings(Constants::patch_network_configuration_file, QSettings::IniFormat);
        settings->beginGroup(Constants::patch_library_name);
            address = settings->value(Constants::patch_network_configuration_address).toString();
            broadcast = settings->value(Constants::patch_network_configuration_broadcast).toString();
        settings->endGroup();
    }
}

// inet_addr
hostent* WSAAPI __stdcall MPPatch::getHostByName_patch(const char* name)
{
    Q_UNUSED(name);
    readSettings();

    return gethostbyname(address.toStdString().c_str());
}

unsigned long __stdcall MPPatch::getAdaptersInfo_patch(IP_ADAPTER_INFO* AdapterInfo, unsigned long* SizePointer)
{
    unsigned long result = GetAdaptersInfo(AdapterInfo, SizePointer);

    if (result == ERROR_BUFFER_OVERFLOW) {
        return result;
    }

    readSettings();

    IP_ADAPTER_INFO* adapter = AdapterInfo;

    while (strcmp(adapter->IpAddressList.IpAddress.String, address.toStdString().c_str()) != 0) {
        qDebug() << "Address3 is:" << adapter->IpAddressList.IpAddress.String;
        qDebug() << "Address4 is:" << address.toStdString().c_str();

        adapter = adapter->Next;
    }

    adapter->Next = nullptr;
    memcpy(AdapterInfo, adapter, sizeof(IP_ADAPTER_INFO));

    return result;
}

int WSAAPI __stdcall MPPatch::sendTo_patch(SOCKET socket, const char* buffer, int length, int flags, const struct sockaddr* to, int toLength)
{
    sockaddr_in* newto = (sockaddr_in*) to;

    if (newto->sin_addr.s_addr == 0xffffffff) {
        newto->sin_addr.s_addr = inet_addr(broadcast.toStdString().c_str());
    }

    return sendto(socket, buffer, length, flags, to, toLength);
}
