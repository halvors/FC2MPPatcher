#ifndef MPPATCH_H
#define MPPATCH_H

#include <QSettings>
#include <QString>

#include <winsock2.h>

#include "mppatch_global.h"

class MPPatch
{
public:
    static MPPATCHSHARED_EXPORT int WSAAPI __stdcall bind_patch(SOCKET s, const sockaddr* name, int namelen);
    static MPPATCHSHARED_EXPORT int WSAAPI __stdcall connect_patch(SOCKET s,const sockaddr* name, int namelen);
    static MPPATCHSHARED_EXPORT int WSAAPI __stdcall sendTo_patch(SOCKET s, const char* buf, int len, int flags, const sockaddr* to, int tolen);

private:
    static QSettings* settings;
    static QString address;
    static QString broadcast;

    static void readSettings();
};

QSettings* MPPatch::settings = nullptr;
QString MPPatch::broadcast = QString();

#endif // MPPATCH_H
