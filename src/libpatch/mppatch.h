#ifndef MPPATCH_H
#define MPPATCH_H

#include <QSettings>
#include <QString>

#include <winsock2.h>
#include <iphlpapi.h>

#include "mppatch_global.h"

class MPPatch
{
public:
    static MPPATCHSHARED_EXPORT unsigned long __stdcall getAdaptersInfo_patch(IP_ADAPTER_INFO* AdapterInfo, unsigned long* SizePointer);
    static MPPATCHSHARED_EXPORT hostent* __stdcall WSAAPI getHostByName_patch(const char* name);
    static MPPATCHSHARED_EXPORT int WSAAPI __stdcall sendTo_patch(SOCKET socket, const char* buffer, int length, int flags, const struct sockaddr* to, int toLength);

private:
    static QSettings* settings;
    static QString address;
    static QString broadcast;

    static void readSettings();
};

QSettings* MPPatch::settings = nullptr;
QString MPPatch::address = QString();
QString MPPatch::broadcast = QString();

#endif // MPPATCH_H
