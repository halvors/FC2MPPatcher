#ifndef PATCH_DEFS_H
#define PATCH_DEFS_H

#include <QByteArray>
#include <QList>

#include "defs.h"
#include "entry.h"

const QStringList patch_library_functions = {
    "_ZN7MPPatch10bind_patchEjPK8sockaddri@12",                   // bind()
    "_ZN7MPPatch12sendTo_patchEjPKciiPK8sockaddri@24",            // sendTo()
    "_ZN7MPPatch21getAdaptersInfo_patchEP16_IP_ADAPTER_INFOPm@8", // getAdapersInfo()
    "_ZN7MPPatch19getHostByName_patchEPKc@4",                     // getHostByName()
    "_ZN7MPPatch13genOneTimeKeyEPhPjPcS2_S2_",                    // genOneTimeKey()
    "_ZN7MPPatch18getPublicIPAddressEv@0"                         // getPublicIpAddress()
};

/**
 * Patch values patch related to community backend.
 *
 * Original: gconnect.ubi.com0000
 * New:      conf.farcry2.online0
 *
 * Original: onlineconfigservice.ubi.com
 * New:      conf.farcry2.online:3074000
 *
 * Original: stun.apac.ubi.com00
 * New:      stun.farcry2.online
 *
 * Original: stun.emea.ubi.com00
 * New:      stun.farcry2.online
 *
 * Original: stun.ncsa.ubi.com00
 * New:      stun.farcry2.online
 */
static const QByteArray patch_endpoint_config_host = "conf.farcry2.online";
const constexpr uint16_t patch_endpoint_config_port = 3074;
const QByteArray patch_endpoint_onlineconfig = QByteArray(patch_endpoint_config_host).append(':').append(QByteArray::number(patch_endpoint_config_port)).append(3, '\0');
const QByteArray patch_endpoint_stun_host = "stun.farcry2.online";

// CryptoPP - 4096 key (OK)
const QByteArray patch_agora_root_public_key(
    "\x30\x82\x02\x0a\x02\x82\x02\x01\x00\xaf\xcb\xc2\xed\xf0\xdc\x7e"
    "\x11\x3f\xf1\xe7\x63\xc4\x2a\x37\xb7\xc2\x80\x7f\x29\x6f\xdf\x54"
    "\x54\x6d\xc0\x4b\x36\x4a\x88\xa8\x9b\xdd\xf3\x71\x6e\xe4\x3b\x8b"
    "\xbd\x5c\x67\x08\x9d\x54\xb9\x81\x7d\x6b\xe2\x8c\xfc\xfb\x19\xfa"
    "\x1a\x02\x8b\x2e\x23\x6a\x09\x1e\xe5\x57\xe7\x04\xbf\x16\x94\x49"
    "\xad\x56\x94\xd9\xb4\xd2\x68\x1a\xc2\x8e\x19\x78\x45\x24\x53\x5d"
    "\xb1\x08\x5b\x42\xa2\x93\x50\x11\x45\xc5\x46\xd4\xf8\xb5\x79\x4a"
    "\x75\xe5\x33\xb2\x7d\xe0\x94\xf9\xa9\xc0\x5d\xf1\xa8\xca\x35\x91"
    "\xe2\xe7\xa0\x52\x95\xfb\xf4\xba\xeb\x8e\x02\x8d\x1e\x7b\x32\xbb"
    "\x10\x0d\x53\x13\xd3\x6c\x0f\x1b\xe7\xb7\x7f\x28\xb2\x05\x9a\x55"
    "\xf4\xf6\xf8\xe0\xee\x86\xb6\x22\xc2\x94\xf7\x96\x78\xf8\x18\xb0"
    "\x07\x37\x5f\x38\x74\xee\x7e\x8a\xd1\xd7\xc4\xee\x51\x1b\xa2\x7e"
    "\x5a\x19\x63\xe6\x76\xc3\x92\x3d\xd4\x66\x1a\x39\xca\xaa\x64\xf2"
    "\x79\xb1\x33\x70\x2a\x55\xe6\xc2\xa1\x24\x38\x99\x87\x3a\x1a\x51"
    "\xa9\x50\x5b\x73\xa4\x43\xb3\x1a\xe1\xce\x92\x07\x96\xc4\xfd\x92"
    "\xd3\xe2\xba\x22\xab\x72\xf6\x0c\x8c\xe1\xe4\x9d\xff\x4e\xda\x51"
    "\x4b\xa1\x17\xb1\x9d\xa1\x22\xd4\x85\x66\x64\x5d\x3d\x8f\x34\x96"
    "\x61\xa8\x82\x1d\x66\x06\x90\xd1\x4e\x78\xd7\x4b\x1c\x35\xcc\x2e"
    "\x20\x15\xa5\xc3\x35\x22\x38\xaa\x66\xf3\xc9\x53\xa6\x2b\x6b\xe6"
    "\x8f\xe6\xe3\x8a\x87\x5e\x21\x1e\xf3\x8a\xeb\xe6\x4f\x14\x9e\xa5"
    "\xc0\xe1\x06\xd5\xf2\xb6\x77\x38\x5b\x9a\x41\x08\x54\x6e\xab\x62"
    "\x24\xcb\x85\x73\xb0\xab\x2e\xbd\xf4\xd3\xd0\x12\xe9\x1b\xe5\x87"
    "\xf1\x48\x07\x58\xba\xa5\xde\xd2\xc6\x24\x01\x45\x7c\x2a\x24\x6b"
    "\x76\x96\x99\x6c\x6b\x23\xae\xd5\xb4\x51\xaa\xbc\x36\x5b\x46\xe1"
    "\xe6\xf1\x4c\x78\x40\x1a\xb3\xbb\x1b\x6b\x5b\x8e\x2f\x74\xec\xd3"
    "\x30\x18\x17\x77\xb1\xcb\xd0\x39\x41\xef\xc2\x55\x2c\x5f\x87\x61"
    "\x92\x4f\xdf\x3a\x9e\xb9\xba\xf7\xe6\xc7\x06\x73\x49\x2c\x42\x60"
    "\xad\xe6\x12\x93\x9a\xe3\x86\x8f\x6b\x1d\xce\xe0\x05\xd7\x8a\xe7"
    "\x3d\x39\xfd\xd3\xb8\x19\x6e\xf9\xa9\x76\x83\x7c\x3d\x35\x16\xad"
    "\x8c\x9d\x98\xc9\x99\x97\xda\x61\xfd\x02\x05\x28\xb7\x34\xe5\xba"
    "\x33\xc1\xb9\xec\xf4\xf2\x07\x3a\x93\xb9\x02\xfe\x0a\xe3\xdb\xe6"
    "\x61\x40\x71\x3f\x71\x55\xde\x44\x7a\x15\x7d\xe7\x8d\xf3\x30\x40"
    "\x07\x3b\xd0\xef\xed\x3f\xa7\xbb\xe7\x02\x03\x01\x00\x01", 526);

/* CryptoPP - 3072 key (OK)
const QByteArray patch_agora_root_public_key(
    "\x30\x82\x01\x8a\x02\x82\x01\x81\x00\xbb\x60\x2f\xd1\x82\xf2\x16"
    "\x7d\xf8\x48\xb4\x08\x33\xa6\x9c\xac\x52\x2e\x9e\x35\xc8\xb9\x6a"
    "\x19\x60\x64\xcd\x1c\x7c\x41\x11\x4f\x60\x71\x4b\xe1\x39\x3c\x7f"
    "\x98\x66\x0e\x22\x5a\x63\x67\xa8\x3e\xf7\x67\x14\xbe\x20\x26\x18"
    "\x31\x9a\x30\xaa\x4a\xd5\xbe\x47\xe0\x21\xf6\xc9\x4a\x1d\x9b\x11"
    "\x09\x18\xe2\x35\x2e\x53\xaf\xad\xdf\x1a\x43\x71\x8f\xa4\x2e\x86"
    "\xcd\x9f\xd8\xbf\x68\xae\x30\xa4\xa6\x9d\xf4\xb0\xa2\x83\x89\x3f"
    "\xfe\xea\xa2\x02\xcb\x8c\x94\x53\x05\x6a\x21\x8d\xb4\x45\x80\x5f"
    "\x63\xc4\x54\x4b\x88\x4b\x66\xbf\xfd\xab\xc0\x2f\x4a\x88\x51\xbb"
    "\x3a\xfb\xa2\x71\xa2\xd3\x48\xf0\xb6\x25\xf6\xaa\xa9\x9b\x43\xe2"
    "\x85\x97\x4f\xd0\xb0\x8d\x50\xf1\x8d\xd0\xaf\x84\x5f\x37\xed\x4c"
    "\xc4\xfe\x2b\x0b\x09\xd2\x5d\xd0\xe8\x7e\xb7\x55\x1e\xbb\x7c\x91"
    "\xfc\x46\x41\x01\x38\x83\x12\xd5\xbe\xda\xd2\xcf\x42\x54\x66\x02"
    "\x5a\xe0\xce\x1b\x65\x12\xcf\x72\xea\xdb\xf0\x7e\x6f\x09\xb6\xc0"
    "\xc3\x6f\x44\x81\x68\x3e\x37\xd1\x4a\x1a\xc0\xba\x74\xaa\xe8\x01"
    "\x6c\xe9\xad\x86\xd1\xcf\xb3\x36\xad\x93\x39\x45\xb0\xc1\xec\x76"
    "\xbb\xe8\xba\x05\x08\xcc\xcb\xbd\x2b\xa2\x0f\x13\x1a\x3c\x1f\x3b"
    "\xe7\x41\x52\xe5\x8a\xd0\xd5\x29\x52\x7e\x8e\x90\xea\xb9\xe6\xe2"
    "\xdd\x45\xd4\x00\xdf\xae\x9a\x98\xdf\xf3\xf5\xe4\xb7\xa1\xbc\x3f"
    "\xdc\xf0\x79\x37\xaa\xc7\xda\x24\x6d\xa8\xfd\x6b\x4f\x25\x1c\x34"
    "\x4a\x46\x76\xf1\xba\xe0\x26\x0d\xd6\xca\x19\x3d\xf3\xd0\xd0\x7f"
    "\x36\xa5\xf9\x8e\x4c\x75\xc7\x84\x52\x7b\xbe\x82\x91\xeb\xb0\xa5"
    "\xf3\xa8\x11\x88\xbe\x0a\x16\x68\x07\xbd\xd2\x5a\x57\xf4\x21\xe8"
    "\xda\xf7\xbf\xf4\xe4\xa7\x3d\xd6\x05\xc6\xdd\xf8\xfa\xd0\xd6\xce"
    "\xf4\xab\x9a\x50\x6b\xd5\xb7\x2c\x13\x02\x03\x01\x00\x01", 526); // 398
*/

/*
//Zenoy key (Fails, probably due to signature length of 513 bytes but only 512 allocated)
const QByteArray patch_agora_root_public_key(
    "\x30\x82\x02\x0a\x02\x82\x02\x01\x07\x92\x18\x8f\x3a\x50\x03\x04"
    "\xc0\xfc\xa3\x6e\x39\xa4\x6a\x8f\x4b\x16\x40\x99\x73\xc6\xc0\x85"
    "\x04\xc5\x1c\x4a\xc4\x77\x3e\xbe\x73\x9f\x7a\x54\xc0\x7b\xb7\xdd"
    "\x39\x05\xd0\x35\x05\xf3\x99\xcc\x41\x40\x38\x18\x0c\xbf\xa5\xa5"
    "\x2b\xee\x7e\xee\xa3\xb1\xbb\x59\x3c\xc8\xe0\x10\x05\x32\x28\x4c"
    "\x11\x33\x93\xe4\x2a\x19\xe7\xb4\x71\x6e\x41\x45\xfa\xa0\x08\x23"
    "\x75\x58\x0f\xfe\x53\x7b\xd4\x97\x8d\x91\xda\x0e\x8d\x4c\x2a\x8e"
    "\x59\x4f\x8c\x33\x7f\x74\x79\x70\x0c\x30\x5d\x08\x1a\xcf\xbe\x8a"
    "\x71\xc6\xfd\x8f\x61\xb8\x64\x43\x43\x9e\xd3\xe9\xe6\xec\x77\x37"
    "\x35\xb0\xb9\xe1\xc9\xd8\x32\xdf\x3c\xcf\xc4\xbc\x12\x08\x8f\xaf"
    "\x2a\x39\xad\x50\xd7\xab\xe3\x24\xd4\x35\x15\x10\x69\xcc\xb5\x49"
    "\xd8\x5a\xef\xa6\x80\xb4\xde\x6c\x2c\xd6\x5f\xc8\x1a\xbe\x75\x11"
    "\x1d\x2e\xdc\x3e\x14\x71\x16\x06\x1c\xb0\x69\xed\x46\xe9\x37\xb2"
    "\xcf\x3d\x02\x18\x5f\x6d\x40\x4b\x62\xbf\x0d\x2f\x74\x35\x53\xa9"
    "\x6e\x70\xd5\x48\xf0\x33\xfe\x39\xc3\x02\xa5\x45\xcb\x88\x8f\xdf"
    "\xce\xfb\x71\xaf\x79\x07\xd6\x03\xad\x1b\xf1\x58\x2c\x7c\x45\x95"
    "\x91\x11\xba\x25\x58\xe2\xde\xce\x57\x95\x16\x8c\xb2\x17\x76\xb6"
    "\x3e\xa0\x31\x28\x6f\x7d\x92\x5a\xd0\x12\xb6\x76\xb6\x21\xc1\xd7"
    "\x20\xb5\x08\x70\xa3\x1d\xc2\x2c\xe1\x04\xfd\x68\x5d\xfc\x81\xf2"
    "\x03\xd1\x07\x2b\x4c\x73\x1c\x51\x83\xe0\x27\x1d\xad\xa0\xd9\x8c"
    "\x3f\x16\x84\x03\x11\x2e\xb6\xd1\x6f\xf3\x49\x41\x1d\x5b\xd5\x28"
    "\x40\x50\xf6\xdb\x42\x79\x06\x0e\xd2\x44\x18\x4d\x94\x39\xec\x2d"
    "\x39\xfe\xd3\xee\x7a\xef\x64\x37\x02\x65\xc3\x9c\x13\x4c\xf9\x04"
    "\xa2\x54\x01\x24\x18\xe8\x73\xb2\x92\xda\x17\x0e\x2f\xdb\x64\x26"
    "\x3a\xfd\x4c\x88\xac\x1d\xa6\xcc\x99\x49\xbb\xcd\x7a\xa0\xb7\x6a"
    "\xd0\xb5\x70\xbe\x49\x98\x65\x4e\x1c\xfa\x42\x02\x84\xa1\x2e\xab"
    "\x2b\x30\xae\xb5\xad\x91\x5e\x84\x94\x03\x6c\xaf\x77\x3c\xa3\x8d"
    "\x04\x7d\x8a\x8a\x0e\x5e\x3e\x20\x8d\x29\xd1\xb2\x0d\x81\xf3\x11"
    "\xbb\xbe\x76\xfa\x93\x68\x35\x20\x68\xe1\xd5\x37\x96\x61\x41\xc6"
    "\xf5\x5c\x99\x7d\xe1\xaf\xcf\x02\x92\x3b\xc4\x86\xe6\xb6\x75\xd1"
    "\x7e\x66\x44\xc3\x06\x7d\x8d\x13\xea\x98\xe7\xef\x59\xe0\x42\xa5"
    "\x83\x29\x25\xa3\xfa\xb7\x7a\x51\x0d\xdf\x11\xac\x34\x26\xac\xc9"
    "\xd2\x53\x38\xa4\x8c\xa2\x48\x95\x1d\x02\x03\x01\x00\x01", 526);
*/

const QList<QByteArray> agoraIdList = {
    "2c66b725e7fb0697c0595397a14b0bc8", // Ubi (Retail)
    "5865cbb3ffd54d7eb2ba667044b0cab1", // Ubi (Steam)
    "9cc10a3d6fb2cc872794b475104c204e", // Ubi (Dedicated server)

    "3338b93611000c12bea41bcd7e9ad8c1", // Retail
    "75eec5a819fd971e63a55c466a36211c", // Steam
    "d4284988e671c5febefe39a09aa893c0", // Dedicated server

    "9c111cf209674ba51a79523f848b17fa", // Staging (Retail)
    "6d748d974900863b0bc45fe3c9d99311", // Staging (Steam)
    "ace72bbedda97396a7807ea10e1b87cf", // Staging (Dedicated server)

    "787292792231d5d89f9015576217b5a4", // Dev (Retail)
    "68cf276e2f1b31dfade9df215744b6dd", // Dev (Steam)
    "d75461366f6cabb69e833dbe61fc904a"  // Dev (Dedicated server)
};

// Reusable assembly constants.
const QByteArray asm_jmp("\xEB", 1);

inline const QByteArray get_asm_nop(const uint16_t numBytes) {
    QByteArray nop;

    for (uint16_t i = 0; i < numBytes; i++)
        nop.append(asm_nop);

    return nop;
}

const QList<FileEntry> files = {
    {
        "Dunia.dll",
        {
            { // Retail (GOG is identical)
                {
                    { "7b82f20088e5c046a99fcaed65dc8bbb8202fd622a69737be83e00686b172d53",
                      "" }
                },
                {
                    { "ada22369c47a00b4279a70e9bf90355877e4e500410c23ff8c1e5852e4158ff6" }, // Hash for Retail version 0.1.17.
                    { "9bfb6cf3be83af54a3625df887bf6a3aeacb13dd32a683a282c4fc4f03a4d50f" }, // Hash for Retail version 0.1.16.
                    { "cd14c176f0e86cc5b362e91badf32d882e54be0ecec85057d5200206433c1cff" }, // Hash for Retail version 0.1.14.
                    { "7f0ff9ac6b07c17944df76beb563ad4d9a35cd7832f4f3f2d6aa5da1f482d9b8" }, // Hash for Retail version 0.1.13.
                    { "58b30f35da8afc9ac5de2f526d97846234d2cd332195b5a3567265702d5bb077" }, // Hash for Retail version 0.1.12.
                    { "a3ae457d942ae499d79c583f32e356cc101ffe1a221aae9220d1ac64695f3c79" }, // Hash for Retail version 0.1.11.
                    { "020ba8709ba7090fa9e29c77f26a66ea230aef92677fe93560d97e391be43c97" }, // Hash for Retail version 0.1.10.
                    { "dde6c2bcd9e333ca68a6c40ab6da8446a53b0721a03700b7302c3d40054cac8a" }, // Hash for Retail version 0.1.6.
                    { "db4ba60a1d2520aa89c4371ebb89df231a69b3d13793eec28dc0b4d1e79da0eb" }, // Hash for Retail version 0.1.5.
                    { "9d32381e05449c845e5fa28245f38ee40ea8154c050dcaee45014547a7703921" }, // Hash for Retail version 0.1.4.
                    { "1fc060b4c1abd28a91d8b9442aa46b72aa28433f6c02611c9ee9d520024bce0b" }  // Hash for Retail version 0.1.2.
                },
                {
                    /* Common */
                    { 0x1001088e, 0 }, // bind()
                    { 0x10213d18, 0 }, // bind()
                    { 0x10c4e97a, 0 }, // bind()
                    { 0x10cb9a8c, 0 }, // bind()
                    { 0x10cb9ad4, 0 }, // bind()
                    { 0x10014053, 1 }, // sendTo()
                    { 0x10c5bde2, 2 }, // getAdapersInfo()
                    { 0x1001431c, 3 }, // getHostByName()

                    // Fix: Patch Ubisoft endpoints with our own.
                    { 0x10e7cb98, patch_endpoint_config_host, ".rdata" },
                    { 0x10e7f400, patch_endpoint_stun_host, ".rdata" },
                    { 0x10e7f414, patch_endpoint_stun_host, ".rdata" },
                    { 0x10e7f428, patch_endpoint_stun_host, ".rdata" },

                    /* Client */
                    // Tweak: Replace game_id with new for community backend.
                    { 0x10dba4c4, agoraIdList[3], ".rdata" }, // game_id

                    // Fix: Hack to avoid verfiying agora certificate with public key from game files.
                    //{ 0x10c1354c, get_asm_nop(2) }, // Just importing key no matter if sig verification was success or not :-)

                    // Fix: Patch in our own agora root public key, and use that instead
                    { patch_agora_root_public_key },

                    // Fix: Call rsa_import() with our agora root public key
                    { QByteArray("\x8D\x86\x58\x33\x00\x00"                // lea  eax,dword ptr ds:[esi+3358] ; Get address of rsa_key *key
                                 "\x50"                                    // push eax                         ; rsa_key *key
                                 "\x68\x0E\x02\x00\x00"                    // push 526                         ; unsigned long inlen
                                 "\x68\x00\xF0\x92\x11"                    // push dunia.1192F000              ; const unsigned char *in
                                 "\xE8\x1A\x12\x33\xFF"                    // call dunia.10C60450              ; Calling rsa_import()
                                 "\xE9\x94\x39\x2E\xFF", 27) },            // jmp  <dunia.return>
                    { 0x10c12bca, QByteArray("\xE9\x51\xC6\xD1\x00", 5) }, // jmp  dunia.1192F220              ; Jump to codecave because of space constrains
                    { 0x10c12bcf, get_asm_nop(15) },

                    // Fix: Change function call genOneTimeKey() to instead call external.
                    { QByteArray("\xFF\x15\x08\xD7\x92\x11"                // call dword ptr ds:[<&_ZN7MPPatch13genOneTimeKeyEPcPyS0_S0_S0_>] ; MPPatch::genOneTimeKey()
                                 "\xE9\xE5\x44\x2E\xFF", 11) },            // jmp  <dunia.return>
                    { 0x10c1372b, QByteArray("\xE9\x10\xBB\xD1\x00", 5) }, // jmp  dunia.1192f240                                             ; Change function call to instead jump to the .text_p section.

                    // Tweak: Remove mouse clamp
                    { 0x105f2338, get_asm_nop(8) }, // Replace byte 0x105ffc78 to 0x105ffc7f with "nop" instruction.

                    /* Server */
                    // Fix: Custom map download
                    { 0x10cb29e2, asm_jmp }, // change JZ (74) to JMP (EB)
                    { QByteArray("\xE8\x6B\x19\xE4\xFE"                    // call dunia.10770BD0 ; GetNetFileServerAddress()
                                 "\x51"                                    // push ecx
                                 "\x50"                                    // push eax
                                 "\xE8\x64\x8C\xEB\xFE"                    // call dunia.107E7ED0 ; IsSessionTypeLAN()
                                 "\x84\xC0"                                // test al,al
                                 "\x75\x16"                                // jne  <dunia.lan>
                                 "\x90"                                    // nop
                                 "\x90"                                    // nop
                                 "\x90"                                    // nop
                                 "\x90"                                    // nop
                                 "\xFF\x15\x0C\xD7\x92\x11"                // call dword ptr ds:[<&_ZN7MPPatch18getPublicIPAddressEv@0>] ; MPPatch::getPublicIPAddress()
                                 "\x8B\xC8"                                // mov  ecx,eax
                                 "\x58"                                    // pop  eax
                                 "\x89\x48\x08"                            // mov  dword ptr ds:[eax+8],ecx
                                 "\x59"                                    // pop  ecx
                                 "\xE9\x96\x22\xE4\xFE"                    // jmp  <dunia.return>
                                 "\x58"                                    // pop  eax
                                 "\x59"                                    // pop  ecx
                                 "\xE9\x8F\x22\xE4\xFE", 45) },            // jmp  <dunia.return>
                    { 0x10771517, QByteArray("\xE9\x44\xDD\x1B\x01", 5) }, // jmp  dunia.1192F260 ; Change function call to instead jump to the .text_p section.
                    { 0x10cb2588, get_asm_nop(6) } // bypassing the rate limiting of map downloads by NOP out rate limit jump.
                }
            },
            { // Steam and Uplay
                {
                    // Steam
                    { "6353936a54aa841350bb30ff005727859cdef1aa10c209209b220b399e862765",
                      "" },

                    // Uplay
                    { "b7219dcd53317b958c8a31c9241f6855cab660a122ce69a0d88cf4c356944e92",
                      "" }
                },
                {
                    { "7af71319ef055fcc2193862ea43cf63dca92a957eb1c19c3956c72faaaf94804" }, // Hash for Steam version 0.1.17.
                    { "5391e8b949ecf8b6506b925814c1d8d77d134d83660fb593645c2ed466c02282" }, // Hash for Uplay version 0.1.17.
                    { "3cb9feaf36ac1c057a3fac56e57f3a60bd0f7749f26d3ad55e9366bdbdd7d696" }, // Hash for Steam version 0.1.16.
                    { "b32d68ad40ad1e7cd7fc22836125b9b6ea25c8c2e4d17801ad8efeda58876ea3" }, // Hash for Uplay version 0.1.16.
                    { "03175df34cde45fccc02fc0c23d4aca40eef49cf228dfdc5932069a941082fa7" }, // Hash for Steam version 0.1.14.
                    { "1d8da17ef910181520a03005710fe6a2a56c10277ddecf8cab5ea4371e228c3f" }, // Hash for Uplay version 0.1.14.
                    { "98f0a42ead437a2587a2695da2a7a54aceff691c7228b8878cce54e838fc271b" }, // Hash for Steam version 0.1.13.
                    { "1f3049ee8991b639b9370b01be17c3ab9b68da6024651596520fd5e2037c11ed" }, // Hash for Uplay version 0.1.13.
                    { "d5c17109e9cc7d4f8b73804ac5a905952a858bd2ec09d359f49a46719eb30b4e" }, // Hash for Steam version 0.1.12.
                    { "bd18c139db5751efcdb99ccdef8a19a6c2730177f0fc967abedce5526d0676cd" }, // Hash for Uplay version 0.1.12.
                    { "d4942259d5d61c2f743bed7a0b156ab8489e868aa8cfd2b20fc5e417dd52cefa" }, // Hash for Steam version 0.1.11.
                    { "83e2ea791c6227cc96f2515c4638b24809439944d183ad6b0117d58eb2c8869b" }, // Hash for Uplay version 0.1.11.
                    { "40f4d55fe0ac6b370798983de2ca1dd09ef0423a7c523b7c424cadddbd894a25" }, // Hash for Steam version 0.1.10.
                    { "c7674c14bad4214e547da3d60ccb14225665394f75b941b10c33362b206575c5" }, // Hash for Uplay version 0.1.10.
                    { "9eec1b6b2eee87da3a6ec97760359e9afb58fadad506fffbe0db8fea7849d511" }, // Hash for Steam version 0.1.9.
                    { "0dd4f7074debda053b655b6ecde3785bd4e228a2ff15e35836aa300e89258916" }, // Hash for Uplay version 0.1.9.
                    { "11e413d632ec52ae01fdc07accc8dca792c44c5cf6e285863ee74f8a82d23062" }, // Hash for Steam version 0.1.6.
                    { "04ca4e9e4e9e76563aabf8d5a3d81184aa64dcb63af6c95a2ab89d45cf2549d6" }, // Hash for Uplay version 0.1.8.
                    { "bc56e2e248118365c8bedc68f3998b57c63de606c91c29e9c911cedecbfeacdf" }, // Hash for Steam version 0.1.5.
                    { "f28878931b7d6c804647545377b064ccf560ee0c6f38cfaf9668373f851114c7" }, // Hash for Steam version 0.1.4.
                    { "64c82e039ab72b30563d94e2d8072a946ef9733302c99c8cf20e3b8503fbafec" }  // Hash for Steam version 0.1.2.
                },
                {
                    /* Common */
                    { 0x1001076e, 0 }, // bind()
                    { 0x102161a8, 0 }, // bind()
                    { 0x10c5d10a, 0 }, // bind()
                    { 0x10cf289c, 0 }, // bind()
                    { 0x10cf28e4, 0 }, // bind()
                    { 0x10013f33, 1 }, // sendTo()
                    { 0x10c6a692, 2 }, // getAdapersInfo()
                    { 0x100141fc, 3 }, // getHostByName()

                    // Fix: Swap Ubisoft endpoints with our own.
                    { 0x10f05568, patch_endpoint_config_host, ".rdata" },
                    { 0x10f07dd0, patch_endpoint_stun_host, ".rdata" },
                    { 0x10f07de4, patch_endpoint_stun_host, ".rdata" },
                    { 0x10f07df8, patch_endpoint_stun_host, ".rdata" },

                    /* Client */
                    // Tweak: Replace game_id with new for community backend.
                    { 0x10e420c0, agoraIdList[10], ".rdata" }, // game_id // SHOULD BE 4

                    // Fix: Swap Ubisoft endpoints with our own.
                    { 0x10f3fa7c, patch_endpoint_onlineconfig, ".rdata" },

                    // Fix: Hack to avoid verfiying agora certificate with public key from game files.
                    //{ 0x10c24829, get_asm_nop(2) }, // Just importing key no matter if sig verification was success or not :-)

                    // Fix: Patch in our own agora root public key, and use that instead
                    { patch_agora_root_public_key },

                    // Fix: Call rsa_import() with our agora root public key
                    { QByteArray("\x8D\x86\x58\x33\x00\x00"                // lea  eax,dword ptr ds:[esi+3358] ; Get address of rsa_key *key
                                 "\x50"                                    // push eax                         ; rsa_key *key
                                 "\x68\x0E\x02\x00\x00"                    // push 526                         ; unsigned long inlen
                                 "\x68\x00\xD0\x9E\x11"                    // push dunia.119ED000              ; const unsigned char *in
                                 "\xE8\x1A\x1A\x28\xFF"                    // call dunia.10C6EC50              ; Calling rsa_import()
                                 "\xE9\xA6\x6C\x23\xFF", 27) },            // jmp  <dunia.return>
                    { 0x10c23edc, QByteArray("\xE9\x3F\x93\xDC\x00", 5) }, // jmp  dunia.119ED220              ; Jump to codecave because of space constrains
                    { 0x10c23ee1, get_asm_nop(15) },

                    // Fix: Change function call genOneTimeKey() to instead call external.
                    { QByteArray("\xFF\x15\x0C\xBA\x9E\x11"                // call dword ptr ds:[<&_ZN7MPPatch13genOneTimeKeyEPcPyS0_S0_S0_>] ; MPPatch::genOneTimeKey()
                                 "\xE9\xC2\x77\x23\xFF", 11) },            // jmp  <dunia.return>
                    { 0x10c24a08, QByteArray("\xE9\x33\x88\xDC\x00", 5) }, // jmp  dunia.119ED240                                             ; Change function call to instead jump to the .text_p section.

                    // Tweak: Remove mouse clamp
                    { 0x105ffc78, get_asm_nop(8) }, // Replace byte 0x105ffc78 to 0x105ffc7f with "nop" instruction.

                    /* Server */
                    // Fix: Custom map download
                    { 0x10cebaf2, asm_jmp }, // change JZ (74) to JMP (EB)
                    { QByteArray("\xE8\x9B\x03\xD9\xFE"                    // call dunia.1077D600 ; GetNetFileServerAddress()
                                 "\x51"                                    // push ecx
                                 "\x50"                                    // push eax
                                 "\xE8\xC4\x06\xD9\xFE"                    // call dunia.1077D930 ; IsSessionTypeLAN()
                                 "\x84\xC0"                                // test al,al
                                 "\x75\x16"                                // jne  <dunia.lan>
                                 "\x90"                                    // nop
                                 "\x90"                                    // nop
                                 "\x90"                                    // nop
                                 "\x90"                                    // nop
                                 "\xFF\x15\x10\xBA\x9E\x11"                // call dword ptr ds:[<&_ZN7MPPatch18getPublicIPAddressEv@0>] ; MPPatch::getPublicIPAddress()
                                 "\x8B\xC8"                                // mov  ecx,eax
                                 "\x58"                                    // pop  eax
                                 "\x89\x48\x08"                            // mov  dword ptr ds:[eax+8],ecx
                                 "\x59"                                    // pop  ecx
                                 "\xE9\x76\x0C\xD9\xFE"                    // jmp  <dunia.return>
                                 "\x58"                                    // pop  eax
                                 "\x59"                                    // pop  ecx
                                 "\xE9\x6F\x0C\xD9\xFE", 45) },            // jmp  <dunia.return>

                    { 0x1077def7, QByteArray("\xE9\x64\xF3\x26\x01", 5) }, // jmp  dunia.119ED260 ; Change function call to instead jump to the .text_p section.
                    { 0x10ceb6c8, get_asm_nop(6) } // bypassing the rate limiting of map downloads by NOP out rate limit jump.
                }
            }
        }
    },
    {
        "FC2ServerLauncher.exe",
        {
            { // Retail (GOG is identical)
                {
                    { "c175d2a1918d3e6d4120a2f6e6254bd04907a5ec10d3c1dfac28100d6fbf9ace",
                      "" }
                },
                {
                    { "ed008348873b2f4f9e25ecb144aabb243b005711d9cd59841f65874c90349325" }, // Hash for Retail version 0.1.17.
                    { "26a9f3a1cf1880df79e395006b0fb595767c10b0265283e4e9928353d30f1e79" }, // Hash for Retail version 0.1.16.
                    { "418b8393f912bb6cdede77360914fc32979964aab1950d240f15ceb35ad80da0" }, // Hash for Retail version 0.1.14.
                    { "20a85ddbff6a6a0fc35aa3ddaff9e4d08ce660000b9568731c39b3d94ac38e8d" }, // Hash for Retail version 0.1.13.
                    { "31738feedf18d2459ce8cb62589bcdc59254be7060dbbdd0d1cbe485efbacdd2" }, // Hash for Retail version 0.1.12.
                    { "9d9bbce845d81ab01821593f45783de5aba886a4133881cad265245e14247732" }, // Hash for Retail version 0.1.11.
                    { "bfb73dffcac987a511be8a7d34f66644e9171dc0fee6a48a17256d6b5e55dc64" }, // Hash for Retail version 0.1.10.
                    { "64c07c4d04d1180f3ff497a6ecc1ce8a48b021e201c640242db32e9a98cb5d1a" }, // Hash for Retail version 0.1.6.
                    { "824fec9e57014f29f7c21fb36a19af33221a8bf37d9a1f85603b2b58df8a976a" }, // Hash for Retail version 0.1.5.
                    { "6ea189c2a6c0834ace7314457d485c4e610ab74ee7335fbc24d55560865b36c4" }, // Hash for Retail version 0.1.4.
                    { "3ced2ee5188b05e4554fec738bb833d639698b1c3473008c8da2678ac66c4433" }  // Hash for Retail version 0.1.2.
                },
                {
                    /* Common */
                    { 0x00425fc4, 0 }, // bind()
                    { 0x0042600b, 0 }, // bind()
                    { 0x004c9d2a, 0 }, // bind()
                    { 0x00ba126e, 0 }, // bind()
                    { 0x00e83eda, 0 }, // bind()
                    { 0x00ba4a33, 1 }, // sendTo()
                    { 0x00c444a6, 2 }, // getAdapersInfo()
                    { 0x00ba4cfc, 3 }, // getHostByName()

                    // Fix: Swap Ubisoft endpoints with our own.
                    { 0x0105eb28, patch_endpoint_config_host, ".rdata" },
                    { 0x0105fdd0, patch_endpoint_stun_host, ".rdata" },
                    { 0x0105fde4, patch_endpoint_stun_host, ".rdata" },
                    { 0x0105fdf8, patch_endpoint_stun_host, ".rdata" },

                    /* Server */
                    // Tweak: Replace game_id with new for community backend.
                    { 0x01042f50, agoraIdList[5], ".rdata" }, // game_id

                    // Fix: Custom map download
                    { 0x004ecda5, asm_jmp }, // change JZ (74) to JMP (EB)
                    { QByteArray("\xE8\x4B\x4C\x30\xFF"                    // call fc2serverlauncher.AB3C50 ; GetNetFileServerAddress()
                                 "\x51"                                    // push ecx
                                 "\x50"                                    // push eax
                                 "\xE8\x04\x01\x30\xFF"                    // call fc2serverlauncher.AAF110 ; IsSessionTypeLAN()
                                 "\x84\xC0"                                // test al,al
                                 "\x75\x16"                                // jne  <fc2serverlauncher.lan>
                                 "\x90"                                    // nop
                                 "\x90"                                    // nop
                                 "\x90"                                    // nop
                                 "\x90"                                    // nop
                                 "\xFF\x15\xE8\xDB\x7A\x01"                // call dword ptr ds:[<&_ZN7MPPatch18getPublicIPAddressEv@0>] ; MPPatch::getPublicIPAddress()
                                 "\x8B\xC8"                                // mov  ecx,eax
                                 "\x58"                                    // pop  eax
                                 "\x89\x48\x08"                            // mov  dword ptr ds:[eax+8],ecx
                                 "\x59"                                    // pop  ecx
                                 "\xE9\x3F\x91\x30\xFF"                    // jmp  <fc2serverlauncher.return>
                                 "\x58"                                    // pop  eax
                                 "\x59"                                    // pop  ecx
                                 "\xE9\x38\x91\x30\xFF", 45) },            // jmp  <fc2serverlauncher.return>
                    { 0x00ab8160, QByteArray("\xE9\x9B\x6E\xCF\x00", 5) }, // jmp  fc2serverlauncher.17AF000 ; Change function call to instead jump to the .text_p section.
                    { 0x004ecb38, get_asm_nop(6) }, // bypassing the rate limiting of map downloads by NOP out rate limit jump.

                    // Fix: Possibility to disable PunkBuster also for ranked matches.
                    { 0x0094c74b, QByteArray("\xE9\xA9\x00\x00\x00", 5).append(get_asm_nop(1)) }, // change JZ to JMP + NOP, from (0F 84 A8 00 00 00) to (E9 A9 00 00 00 90), bypassing PB checks for ranked matches.
                    { 0x0094c943, asm_jmp }, // change JZ to JMP in order to bypass autoenable of PB on ranked matches.
                    { 0x00661eac, get_asm_nop(2) } // change JNZ to NOP in order to prevent PB from starting autostarting after match is started.
                }
            },
            { // Steam (R2 is identical) and Uplay
                {
                    // Steam (R2 is identical)
                    { "5cd5d7b6e6e0b1d25843fdee3e9a743ed10030e89ee109b121109f4a146a062e",
                      "" },

                    // Uplay
                    { "948a8626276a6689c0125f2355b6a820c104f20dee36977973b39964a82f2703",
                      "" }
                },
                {
                    { "18902ffab0a8227d762d0ea0bbc0ea780e52ac81b25fd6a95f41a926c9e07929" }, // Hash for Steam version 0.1.17.
                    { "97dcbe2f34e21bc68560acacd4febba956abb76d3b136fcae5fdbbac5527b183" }, // Hash for Uplay version 0.1.17.
                    { "db5cfd6e70ea8abe54c221aff028132ec581684755a687e21aad7173cd57856c" }, // Hash for Steam version 0.1.16.
                    { "258d1c9461c751f245d8b8c93894ffa9957e064b2b8a87a9dec9303c21ff595f" }, // Hash for Uplay version 0.1.16.
                    { "b3d4c57e6ad51c6e4615106329a5c33d0121c25f5c5e22da80201c2488f78c5c" }, // Hash for Steam version 0.1.14.
                    { "86a5701effea1bc0f51dc8dfe19b007bce73998b37956b3079eb58b4652c96ff" }, // Hash for Uplay version 0.1.14.
                    { "0eeba5e94d07b225eca772575283897169b893b755d481e0184c20952e515442" }, // Hash for Steam version 0.1.13.
                    { "ef30f22cff98914d408ba69a6778c00e793c0b1eae128718091df206c3c1bd85" }, // Hash for Uplay version 0.1.13.
                    { "04df9d30bce8f7e22788a2fc7c6bad6719caf0f22de42f936ed7e3ed6cc1dda6" }, // Hash for Steam version 0.1.12.
                    { "e90dc85ea2229b038278f6ac45805658fc12fd00ceeca05093d271c885584d4c" }, // Hash for Uplay version 0.1.12.
                    { "cb5036fccf38a2a8cb73afc36df7844e160657c7f93c35554909b0e52e09ccee" }, // Hash for Steam version 0.1.11.
                    { "346c6b3a292a352b6ca88ff563f25564d501af632fcb09a07f6a19a1415236c6" }, // Hash for Uplay version 0.1.11.
                    { "bfb73dffcac987a511be8a7d34f66644e9171dc0fee6a48a17256d6b5e55dc64" }, // Hash for Steam version 0.1.10.
                    { "38f33dfd74b9483fb7db7703dffe61d61fa51444730d38ed2b61fc6e20855d9a" }, // Hash for Uplay version 0.1.10.
                    { "1c8152bfa04283b699acfe81d09a023fd19b0c2c73fe20e6d83b15e24c946653" }, // Hash for Steam version 0.1.6.
                    { "bf97b551b3219d5223dd9fdab961a3821edb2e3fbc759857695f661ebca0a1d7" }, // Hash for Uplay version 0.1.8.
                    { "6b3c243f414e9cc9142b52c143cce2bc963fc82ab60b8bbf041c87266a64ee9c" }, // Hash for Steam version 0.1.5.
                    { "a0b5ec1dce04094e821d466185efc11716198f24d965222a3fe4a0c3f3f6471b" }, // Hash for Steam version 0.1.4.
                    { "2db5a3c609410eae86b6d6c0fb33f4cbb65196b4fc1915d2a19f150cde238252" }  // Hash for Steam version 0.1.2.
                },
                {
                    /* Common */
                    { 0x004263d4, 0 }, // bind()
                    { 0x0042641b, 0 }, // bind()
                    { 0x004c9d2a, 0 }, // bind()
                    { 0x00ba36be, 0 }, // bind()
                    { 0x00e85ffa, 0 }, // bind()
                    { 0x00ba6e83, 1 }, // sendTo()
                    { 0x00c46a66, 2 }, // getAdapersInfo()
                    { 0x00ba714c, 3 }, // getHostByName()

                    // Fix: Swap Ubisoft endpoints with our own.
                    { 0x01061b78, patch_endpoint_config_host, ".rdata" },
                    { 0x01062e20, patch_endpoint_stun_host, ".rdata" },
                    { 0x01062e34, patch_endpoint_stun_host, ".rdata" },
                    { 0x01062e48, patch_endpoint_stun_host, ".rdata" },

                    /* Server */
                    // Tweak: Replace game_id with new for community backend.
                    { 0x01045fb0, agoraIdList[5], ".rdata" }, // game_id

                    // Fix: Custom map download
                    { 0x004eca95, asm_jmp }, // change JZ (74) to JMP (EB)
                    { QByteArray("\xE8\x4B\xCB\x2F\xFF"                    // call fc2serverlauncher.AAEB50 ; GetNetFileServerAddress()
                                 "\x51"                                    // push ecx
                                 "\x50"                                    // push eax
                                 "\xE8\x14\x72\x2F\xFF"                    // call fc2serverlauncher.AA9220 ; IsSessionTypeLAN()
                                 "\x84\xC0"                                // test al,al
                                 "\x75\x16"                                // jne  <fc2serverlauncher.lan>
                                 "\x90"                                    // nop
                                 "\x90"                                    // nop
                                 "\x90"                                    // nop
                                 "\x90"                                    // nop
                                 "\xFF\x15\xA0\x0D\x7B\x01"                // call dword ptr ds:[<&_ZN7MPPatch18getPublicIPAddressEv@0>] ; MPPatch::getPublicIPAddress()
                                 "\x8B\xC8"                                // mov  ecx,eax
                                 "\x58"                                    // pop  eax
                                 "\x89\x48\x08"                            // mov  dword ptr ds:[eax+8],ecx
                                 "\x59"                                    // pop  ecx
                                 "\xE9\xDF\x10\x30\xFF"                    // jmp  <fc2serverlauncher.return>
                                 "\x58"                                    // pop  eax
                                 "\x59"                                    // pop  ecx
                                 "\xE9\xD8\x10\x30\xFF", 45) },            // jmp  <fc2serverlauncher.return>
                    { 0x00ab3100, QByteArray("\xE9\xFB\xEE\xCF\x00", 5) }, // jmp  fc2serverlauncher.17B2000 ; Change function call to instead jump to the .text_p section.
                    { 0x004ec828, get_asm_nop(6) }, // bypassing the rate limiting of map downloads by NOP out rate limit jump.

                    // Fix: Possibility to disable PunkBuster also for ranked matches.
                    { 0x0094d39b, QByteArray("\xE9\xA9\x00\x00\x00", 5).append(get_asm_nop(1)) }, // change JZ to JMP + NOP, from (0F 84 A8 00 00 00) to (E9 A9 00 00 00 90), bypassing PB checks for ranked matches.
                    { 0x0094d593, asm_jmp }, // change JZ to JMP in order to bypass autoenable of PB on ranked matches.
                    { 0x0067552c, get_asm_nop(2) }, // change JNZ to NOP in order to prevent PB from starting autostarting after match is started.

                    // Fix: Unintentional line break due to UTF-16 character being printed on client join.
                    { QByteArray("\x68\x78\x3F\x04\x01"         // push   fc2serverlauncher.1043F78
                                 "\x57"                         // push   edi
                                 "\x50"                         // push   eax
                                 "\x51"                         // push   ecx
                                 "\x66\x8B\x08"                 // mov    cx,word ptr ds:[eax]
                                 "\x66\x85\xC9"                 // test   cx,cx
                                 "\x74\x29"                     // je     <fc2serverlauncher.exit_joined>
                                 "\x90\x90\x90\x90"             // nop    nop nop nop
                                 "\x80\xE1\x80"                 // and    cl,80
                                 "\x75\x11"                     // jne    <fc2serverlauncher.do_joined>
                                 "\x90\x90\x90\x90"             // nop    nop nop nop
                                 "\x84\xED"                     // test   ch,ch
                                 "\x75\x09"                     // jne    <fc2serverlauncher.do_joined>
                                 "\x90\x90\x90\x90"             // nop    nop nop nop
                                 "\xEB\x0A"                     // jmp    <fc2serverlauncher.continue_joined>
                                 "\x90\x90\x90"                 // nop    nop nop
                                 "\xB1\x3F"                     // mov    cl,3F
                                 "\x32\xED"                     // xor    ch,ch
                                 "\x66\x89\x08"                 // mov    word ptr ds:[eax],cx
                                 "\x83\xC0\x02"                 // add    eax,2
                                 "\xEB\xD2"                     // jmp    <fc2serverlauncher.loop_joined>
                                 "\x90\x90\x90"                 // nop    nop nop
                                 "\x59"                         // pop    ecx
                                 "\x58"                         // pop    eax
                                 "\xE9\x13\x28\x55\xFF", 64) }, // nop    nop nop
                    { 0x00d0488d, QByteArray("\xE9\xAE\xD7\xAA\x00", 5).append(get_asm_nop(1)) }, // change function call to instead jump to the .text_p section.


                    // Fix: Unintentional line break due to UTF-16 character being printed on client leave.
                    { QByteArray("\x68\xD8\x3F\x04\x01"         // push   fc2serverlauncher.1043FD8
                                 "\x57"                         // push   edi
                                 "\x50"                         // push   eax
                                 "\x51"                         // push   ecx
                                 "\x66\x8B\x08"                 // mov    cx,word ptr ds:[eax]
                                 "\x66\x85\xC9"                 // test   cx,cx
                                 "\x74\x29"                     // je     <fc2serverlauncher.exit_joined>
                                 "\x90\x90\x90\x90"             // nop    nop nop nop
                                 "\x80\xE1\x80"                 // and    cl,80
                                 "\x75\x11"                     // jne    <fc2serverlauncher.do_joined>
                                 "\x90\x90\x90\x90"             // nop    nop nop nop
                                 "\x84\xED"                     // test   ch,ch
                                 "\x75\x09"                     // jne    <fc2serverlauncher.do_joined>
                                 "\x90\x90\x90\x90"             // nop    nop nop nop
                                 "\xEB\x0A"                     // jmp    <fc2serverlauncher.continue_joined>
                                 "\x90\x90\x90"                 // nop    nop nop
                                 "\xB1\x3F"                     // mov    cl,3F
                                 "\x32\xED"                     // xor    ch,ch
                                 "\x66\x89\x08"                 // mov    word ptr ds:[eax],cx
                                 "\x83\xC0\x02"                 // add    eax,2
                                 "\xEB\xD2"                     // jmp    <fc2serverlauncher.loop_joined>
                                 "\x90\x90\x90"                 // nop    nop nop
                                 "\x59"                         // pop    ecx
                                 "\x58"                         // pop    eax
                                 "\xE9\x23\x29\x55\xFF", 64) }, // nop    nop nop
                    { 0x00d049dd, QByteArray("\xE9\x9E\xD6\xAA\x00", 5).append(get_asm_nop(1)) }, // change function call to instead jump to the .text_p section.

                    /* Experimental / WIP */
                    { 0x0052c8d3, get_asm_nop(2) }, // bypass min player limit enforced in ranked mode.

                    //{ 0x0052c8d3, asm_nop(2) }, // MinPlayers?... 75 03 // works somehow..?
                    //{ 0x00b046ff, asm_nop(2) }, // MinPlayers?... 75 03 // takes care of persist thru refresh??
                    //{ 0x0094df05, asm_nop(2) } // MinPlayers?... 74 0f // bypass join in progress check?
                }
            }
        }
    }
};

#endif // PATCH_DEFS_H
