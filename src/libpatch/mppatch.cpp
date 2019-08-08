#include "mppatch.h"
#include "global.h"

void MPPatch::readSettings()
{
    if (!settings) {
        settings = new QSettings(patch_network_configuration_file, QSettings::IniFormat);
        settings->beginGroup(patch_library_name);
            address = settings->value(patch_network_configuration_address).toString();
            broadcast = settings->value(patch_network_configuration_broadcast).toString();
        settings->endGroup();
    }
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
        adapter = adapter->Next;
    }

    adapter->Next = nullptr;
    memcpy(AdapterInfo, adapter, sizeof(IP_ADAPTER_INFO));

    return result;
}

hostent* WSAAPI __stdcall MPPatch::getHostByName_patch(const char* name)
{
    Q_UNUSED(name);

    readSettings();

    return gethostbyname(address.toStdString().c_str());
}

int WSAAPI __stdcall MPPatch::sendTo_patch(SOCKET socket, const char* buffer, int length, int flags, const struct sockaddr* to, int toLength)
{
    sockaddr_in* toAddress = (sockaddr_in*) to;

    if (toAddress->sin_addr.s_addr == 0xFFFFFFFF) {
        toAddress->sin_addr.s_addr = inet_addr(broadcast.toStdString().c_str());
    }

    return sendto(socket, buffer, length, flags, to, toLength);
}
