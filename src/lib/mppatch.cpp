#include "mppatch.h"

void MPPatch::readSettings()
{
    if (!settings) {
        settings = new QSettings("network.cfg.ini", QSettings::IniFormat);
    }

    if (!address) {
        settings->beginGroup("MPPatch");
            address = settings->value("Address").toString().toStdString().c_str();
        settings->endGroup();
    }
}

hostent* WSAAPI __stdcall MPPatch::patch_gethostbyname(char* name)
{
    Q_UNUSED(name);

    readSettings();

    return gethostbyname(address);
}

ULONG __stdcall MPPatch::patch_GetAdaptersInfoPX(PIP_ADAPTER_INFO AdapterInfo, PULONG SizePointer)
{
    ULONG result = GetAdaptersInfo(AdapterInfo, SizePointer);

    if (result == ERROR_BUFFER_OVERFLOW) {
        return result;
    }

    readSettings();

    PIP_ADAPTER_INFO adapter = AdapterInfo;

    while (strcmp(adapter->IpAddressList.IpAddress.String, address) != 0) {
        adapter = adapter->Next;
    }

    adapter->Next = nullptr;
    memcpy(AdapterInfo, adapter, sizeof(IP_ADAPTER_INFO));

    return result;
}
