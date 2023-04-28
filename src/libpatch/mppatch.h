#ifndef MPPATCH_H
#define MPPATCH_H

#include <cstdint>
#include <string>

#include <winsock2.h>
#include <iphlpapi.h>

#include "mppatch_global.h"

void read_settings();

extern "C" {
    int WSAAPI _stdcall patch_bind(SOCKET s, const sockaddr *name, int namelen);
    MPPATCHSHARED_EXPORT int WSAAPI __stdcall patch_sendto(SOCKET s, const char* buf, int len, int flags, const sockaddr* to, int tolen);
    MPPATCHSHARED_EXPORT uint64_t __stdcall patch_get_adapters_info(IP_ADAPTER_INFO* adapterInfo, unsigned long* sizePointer);
    MPPATCHSHARED_EXPORT hostent* WSAAPI __stdcall patch_get_host_by_name(const char* name);

    MPPATCHSHARED_EXPORT int __cdecl generate_cd_key_id_hex(uint8_t* out, uint32_t* outLen, const char* serialName, const char* cdKey);
    MPPATCHSHARED_EXPORT int __cdecl generate_one_time_key(uint8_t* out, uint32_t* outLen, const char* challenge, const char* username, const char* password);
};

static std::string address;
static std::string broadcast;

#endif // MPPATCH_H
