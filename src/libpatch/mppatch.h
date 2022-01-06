#ifndef MPPATCH_H
#define MPPATCH_H

#include <QString>

#include <winsock2.h>
#include <iphlpapi.h>

#include "mppatch_global.h"

class MPPatch
{
public:
    static MPPATCHSHARED_EXPORT int WSAAPI __stdcall bind_patch(SOCKET s, const sockaddr *name, int namelen);
    static MPPATCHSHARED_EXPORT int WSAAPI __stdcall sendTo_patch(SOCKET s, const char *buf, int len, int flags, const sockaddr *to, int tolen);
    static MPPATCHSHARED_EXPORT uint64_t __stdcall getAdaptersInfo_patch(IP_ADAPTER_INFO *adapterInfo, unsigned long *sizePointer);
    static MPPATCHSHARED_EXPORT hostent *WSAAPI __stdcall getHostByName_patch(const char *name);

    static MPPATCHSHARED_EXPORT int __cdecl genOneTimeKey(uint8_t *out, uint32_t *outLen, char *challenge, char *username, char *password);
    static MPPATCHSHARED_EXPORT uint32_t __stdcall getPublicIPAddress();
    static MPPATCHSHARED_EXPORT int genCdKeyIdHex();

private:
    static QString address;
    static QString broadcast;
    static uint32_t publicAddress;

    static void readSettings();
};

QString MPPatch::address = QString();
QString MPPatch::broadcast = QString();
uint32_t MPPatch::publicAddress = 0;

#endif // MPPATCH_H
