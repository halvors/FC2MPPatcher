#ifndef MPPATCH_H
#define MPPATCH_H

#include <QString>
#include <QNetworkInterface>

#include <winsock2.h>
#include <iphlpapi.h>

#include "mppatch_global.h"

class MPPatch
{
public:
    static MPPATCHSHARED_EXPORT int WSAAPI __stdcall bind_patch(SOCKET s, const sockaddr *name, int namelen);
    static MPPATCHSHARED_EXPORT int WSAAPI __stdcall connect_patch(SOCKET s,const sockaddr *name, int namelen);
    static MPPATCHSHARED_EXPORT int WSAAPI __stdcall sendTo_patch(SOCKET s, const char *buf, int len, int flags, const sockaddr *to, int tolen);
    static MPPATCHSHARED_EXPORT unsigned long __stdcall getAdaptersInfo_patch(IP_ADAPTER_INFO *adapterInfo, unsigned long *sizePointer);
    static MPPATCHSHARED_EXPORT hostent *WSAAPI __stdcall getHostByName_patch(const char *name);

    static MPPATCHSHARED_EXPORT unsigned int __stdcall getPublicIPAddress();

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
