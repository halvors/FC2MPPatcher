#ifndef MPPATCH_H
#define MPPATCH_H

#include <cstdint>
#include <string>

#include <winsock2.h>
#include <iphlpapi.h>

#include "mppatch_global.h"

class MPPatch
{
public:
    static MPPATCHSHARED_EXPORT int WSAAPI __stdcall bind_patch(SOCKET s, const sockaddr *name, int namelen);
    static MPPATCHSHARED_EXPORT int WSAAPI __stdcall sendTo_patch(SOCKET s, const char* buf, int len, int flags, const sockaddr* to, int tolen);
    static MPPATCHSHARED_EXPORT uint64_t __stdcall getAdaptersInfo_patch(IP_ADAPTER_INFO* adapterInfo, unsigned long* sizePointer);
    static MPPATCHSHARED_EXPORT hostent* WSAAPI __stdcall getHostByName_patch(const char* name);

    static MPPATCHSHARED_EXPORT int __cdecl generateCdKeyIdHex(uint8_t* out, uint32_t* outLen, char* serialName, char* cdKey);
    static MPPATCHSHARED_EXPORT int __cdecl generateOneTimeKey(uint8_t* out, uint32_t* outLen, char* challenge, char* username, char* password);

private:
    static std::string address;
    static std::string broadcast;

    static void readSettings();
};

std::string MPPatch::address;
std::string MPPatch::broadcast;

#endif // MPPATCH_H
