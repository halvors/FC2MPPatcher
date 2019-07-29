#ifndef MPPATCH_H
#define MPPATCH_H

#include <QSettings>

#include <winsock2.h>
#include <iphlpapi.h>

#include "mppatch_global.h"

class MPPatch
{
public:
    static MPPATCHSHARED_EXPORT hostent* WSAAPI __stdcall patch_gethostbyname(char* name);
    static MPPATCHSHARED_EXPORT ULONG __stdcall patch_GetAdaptersInfoPX(PIP_ADAPTER_INFO AdapterInfo, PULONG SizePointer);

private:
    static QSettings* settings;
    static const char* address;

    static void readSettings();
};

QSettings* MPPatch::settings = nullptr;
const char* MPPatch::address = nullptr;

#endif // MPPATCH_H
