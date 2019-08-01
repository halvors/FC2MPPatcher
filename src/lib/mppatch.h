#ifndef MPPATCH_H
#define MPPATCH_H

#include <QSettings>

#include <winsock2.h>
#include <iphlpapi.h>

#include "mppatch_global.h"

class MPPatch
{
public:
    static MPPATCHSHARED_EXPORT hostent* __stdcall WSAAPI getHostByName_patch(const char* name);
    static MPPATCHSHARED_EXPORT unsigned long __stdcall getAdaptersInfo_patch(IP_ADAPTER_INFO* AdapterInfo, unsigned long* SizePointer);

private:
    static QSettings* settings;
    static const char* address;

    static void readSettings();
};

QSettings* MPPatch::settings = nullptr;
const char* MPPatch::address = nullptr;

#endif // MPPATCH_H
