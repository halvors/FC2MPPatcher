#ifndef PATCH_DEFS_H
#define PATCH_DEFS_H

#include <cstdint>
#include <string>
#include <vector>

#include "defs.h"
#include "entry.h"

const std::vector<std::string> patch_library_functions = {
    "_ZN7MPPatch10bind_patchEjPK8sockaddri@12",                   // bind()
    "_ZN7MPPatch12sendTo_patchEjPKciiPK8sockaddri@24",            // sendTo()
    "_ZN7MPPatch21getAdaptersInfo_patchEP16_IP_ADAPTER_INFOPm@8", // getAdapersInfo()
    "_ZN7MPPatch19getHostByName_patchEPKc@4",                     // getHostByName()
    "_ZN7MPPatch18generateCdKeyIdHexEPhPjPcS2_",                  // generateCdKeyIdHex()
    "_ZN7MPPatch18generateOneTimeKeyEPhPjPcS2_S2_"                // generateOneTimeKey()
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
const std::string patch_endpoint_config_host = "conf.farcry2.online";
const constexpr uint16_t patch_endpoint_config_port = 3074;
const std::string patch_endpoint_onlineconfig = patch_endpoint_config_host + ":" + std::to_string(patch_endpoint_config_port).append(3, 0x00);
const std::string patch_endpoint_stun_host = "stun.farcry2.online";

// RSA root public key (4096 bits)
const std::string patch_agora_root_public_key(
    "\x30\x82\x02\x0a\x02\x82\x02\x01\x00\xd7\x38\xcd\x45\x34\x20\x1f"
    "\x37\x6d\xec\x48\xb9\x45\x50\x8e\xe9\x67\x37\x25\xb6\x01\x3c\x2a"
    "\x6e\x47\xb4\x5f\xa2\xcc\x50\xb3\xfa\xe0\x56\x09\x8c\x3a\xa0\x12"
    "\x4b\x5c\xc1\x91\x4c\x6d\xa3\x3d\x40\x6a\x3f\x31\x68\xeb\x89\x4a"
    "\x2d\xf8\x4c\x28\xa1\x01\xf6\xc9\xca\xf1\xac\xac\xc9\xae\x27\xa3"
    "\xac\x08\xcf\xff\x3d\x4c\xa2\x10\x94\xc1\xe3\x1c\xdc\xff\xb8\x71"
    "\xe2\x32\x6c\x73\x3f\x7b\xae\x82\x0e\x44\xf1\x61\xda\x21\xf7\x84"
    "\xbc\xa1\x1c\x3d\xf8\x21\x91\x3a\x40\x17\xd1\x93\x5f\x60\xd1\x4c"
    "\xad\x38\x8d\x34\xb0\x8a\x33\x49\xb6\xc1\x22\x72\x04\x90\x84\xc1"
    "\xb5\xe3\xcb\xb6\x8a\x46\xd8\xa2\x69\x6c\x33\xf0\x62\xb3\x86\x43"
    "\x72\x76\xe0\x0a\x4a\x39\xe8\xf3\x9e\x2f\xf1\x05\x2c\x3f\x4a\xc5"
    "\x6d\xcc\x96\xdb\xbc\xf2\xc0\xbc\xc4\xe4\x01\xfd\x62\xab\xad\xd3"
    "\x5b\x6b\xd0\x34\x55\x07\x19\x72\xc6\x8d\x15\x4e\x25\x4d\x1b\xce"
    "\xb1\x3d\xd0\x2b\xf2\x97\x74\x60\xce\x7a\x79\x7d\xda\x98\x3a\x38"
    "\x43\xdb\x48\x65\x30\x1d\x38\xe4\x8d\x2d\xf4\x8c\xd0\xfc\xb7\xef"
    "\xa8\x7d\x63\x8b\x5d\x44\x75\x6e\x05\xbe\x22\x54\x25\x7f\x7d\xd1"
    "\xe4\x0c\xe7\x60\x06\xa1\x3c\xd8\x25\xba\x0c\x08\x64\xaf\xad\x2e"
    "\x63\xde\x09\xe2\xab\xb1\xe6\xc7\x9c\x33\xfa\x08\x09\x11\x28\xb2"
    "\x15\x08\x44\x75\x31\xc7\xf2\xd2\x9b\x7a\x9f\xf6\xcf\xc8\xbb\xd7"
    "\x8b\x5a\xc6\xc0\x8a\xa3\x8a\xd3\xd7\x24\x5b\xf9\xec\xf2\x4d\x90"
    "\x8b\x6e\x84\xfd\x07\xa8\x41\x6b\xf0\xb0\xc3\x1b\xba\xf1\x55\xb2"
    "\x37\xf0\x33\x04\x42\x0d\x0c\x35\x1b\x12\xfb\x23\x58\xe4\x90\x1f"
    "\xfe\x72\x1e\xba\x8a\x4a\x5b\xcb\x77\xa0\xd1\x44\x21\x51\x7e\xf1"
    "\xc4\xf1\xe3\xdb\xeb\xb5\x90\x37\x34\xce\xaf\xa6\x5a\x2b\xe9\x36"
    "\x4a\x56\x69\xe6\x53\x9a\xb1\x38\xb2\x96\xb9\xaf\x06\x7a\x74\xbf"
    "\xe3\x95\x08\x12\x96\xf5\x1b\xaf\xd5\x16\xe0\x6b\x24\xdf\x75\x14"
    "\xc6\xe8\x70\x99\x9d\x33\x10\x1a\xd6\x6a\x04\xf5\xc9\x4f\x68\xcd"
    "\x43\xb5\x7c\xac\x2e\x32\x10\xd1\xe5\xc8\x0c\x49\x7a\x82\x16\x07"
    "\x97\x08\xda\xa5\xcb\x02\x80\x5e\x43\xcc\x39\x0d\xa2\xe2\xf9\x1d"
    "\x5e\x1d\x33\x5d\xf1\x0f\xa2\xa9\xca\xc6\x36\x67\xd4\x95\xfb\xe4"
    "\x12\x90\xbb\xa7\xc9\xf4\xb0\xcc\xb1\x13\x0e\x39\x7e\x5a\x66\xec"
    "\x3c\x4f\xea\x13\x20\x18\xc9\x81\xbc\xf8\x78\x2c\xc2\x89\x5a\x8c"
    "\x7d\x0c\x53\xa8\x19\xf7\x3e\x9d\x67\x02\x03\x01\x00\x01", 526);

const std::string agoraIdList[] = {
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
constexpr uint32_t agoraIdModifier = !USE_DEV_BACKEND ? 0 : 6;

// Reusable assembly constants.
const std::string asm_jmp("\xEB", 1);

inline const std::string get_asm_nop(const uint16_t numBytes) {
    std::string nop;

    for (uint16_t i = 0; i < numBytes; i++)
        nop += asm_nop;

    return nop;
}

const std::vector<FileEntry> files = {
    {
        "Dunia.dll",
        {
            { // Retail (GOG is identical)
                {
                    { "7b82f20088e5c046a99fcaed65dc8bbb8202fd622a69737be83e00686b172d53",
                      "cf77f414a045e98d769c0193402b2f4c2a5e52bf6eb964efb901ce3b266f8bc1" }
                },
                {
                    { "7d71c70809b7153b8e30628a981b096eb6837d024c48eaefbadb64bb21dbdf60" }, // Hash for Retail version 0.2.0.
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

                    // Tweak: Patch Ubisoft endpoints with our own.
                    { 0x10e7cb98, patch_endpoint_config_host, ".rdata" },
                    { 0x10e7f400, patch_endpoint_stun_host, ".rdata" },
                    { 0x10e7f414, patch_endpoint_stun_host, ".rdata" },
                    { 0x10e7f428, patch_endpoint_stun_host, ".rdata" },

                    /* Client */
                    // Tweak: Replace game_id with new for community backend.
                    { 0x10dba4c4, agoraIdList[3 + agoraIdModifier], ".rdata" }, // game_id

                    // Tweak: Patch in our own agora root public key, and use that instead
                    { patch_agora_root_public_key },

                    // Tweak: Call rsa_import() with our agora root public key
                    { std::string("\x8D\x86\x58\x33\x00\x00"                // lea  eax,dword ptr ds:[esi+3358] ; Get address of rsa_key *key
                                  "\x50"                                    // push eax                         ; rsa_key *key
                                  "\x68\x0E\x02\x00\x00"                    // push 526                         ; unsigned long inlen
                                  "\x68\x00\xF0\x92\x11"                    // push dunia.1192F000              ; const unsigned char *in
                                  "\xE8\x1A\x12\x33\xFF"                    // call dunia.10C60450              ; Calling rsa_import()
                                  "\xE9\x94\x39\x2E\xFF", 27) },            // jmp  <dunia.return>
                    { 0x10c12bca, std::string("\xE9\x51\xC6\xD1\x00", 5) }, // jmp  <dunia.text>                ; Jump to codecave because of space constrains
                    { 0x10c12bcf, get_asm_nop(15) },

                    // Tweak: Change function call genCdKeyIdHex() to instead call external.
                    { std::string("\x57"                                    // push edi                                                    ; cd key
                                  "\x56"                                    // push esi                                                    ; serial name
                                  "\x50"                                    // push eax                                                    ; out length
                                  "\x8D\x85\x04\x01\x00\x00"                // lea  eax,dword ptr ss:[ebp+104]
                                  "\x50"                                    // push eax                                                    ; out pointer
                                  "\xFF\x15\x10\xD7\x92\x11"                // call dword ptr ds:[<&_ZN7MPPatch13genCdKeyIdHexEPhPjPcS2_>] ; MPPatch::generateCdKeyIdHex()
                                  "\x59"                                    // pop  ecx                                                    ; clean up cd key
                                  "\x59"                                    // pop  ecx                                                    ; clean up serial name
                                  "\xE9\x8D\x44\x2E\xFF", 23) },            // jmp  <dunia.return>
                    { 0x10c136d1, get_asm_nop(8) },
                    { 0x10c136df, std::string("\xE9\x5C\xBB\xD1\x00", 5) }, // jmp  <dunia.text>                                           ; Change function call to instead jump to the .text_p section.

                    // Tweak: Use OCTETSTRING instead of IA5STRING
                    { std::string("\xC7\x85\x74\xFF\xFF\xFF\x05\x00\x00\x00" // mov dword ptr ss:[ebp-8C],5
                                  "\xE9\xB3\x45\x2E\xFF", 15) },             // jmp dunia.119ED260
                    { 0x10c1381c, std::string("\xE9\x3F\xBA\xD1\x00", 5).append(get_asm_nop(1)) },

                    // Tweak: Change function call genOneTimeKey() to instead call external.
                    { std::string("\xFF\x15\x14\xD7\x92\x11"                // call dword ptr ds:[<&_ZN7MPPatch13genOneTimeKeyEPcPyS0_S0_S0_>] ; MPPatch::genOneTimeKey()
                                  "\xE9\xA5\x44\x2E\xFF", 11) },            // jmp  <dunia.return>
                    { 0x10c1372b, std::string("\xE9\x50\xBB\xD1\x00", 5) }, // jmp  <dunia.text>                                               ; Change function call to instead jump to the .text_p section.

                    // Tweak: Remove mouse clamp
                    { 0x105f2338, get_asm_nop(8) }, // Replace byte 0x105ffc78 to 0x105ffc7f with "nop" instruction.

                    /* Server */
                    // Fix: Custom map download
                    { 0x10cb29e2, asm_jmp }, // change JZ (74) to JMP (EB)
                    { std::string("\xE8\x2B\x19\xE4\xFE"                    // call dunia.10770BD0  ; GetNetFileServerAddress()
                                  "\x52"                                    // push edx
                                  "\x51"                                    // push ecx
                                  "\x50"                                    // push eax
                                  "\xE8\x23\x8C\xEB\xFE"                    // call dunia.107E7ED0  ; IsSessionTypeLAN()
                                  "\x84\xC0"                                // test al,al
                                  "\x75\x1B", 17)                           // jne  <dunia.end>
                      .append(get_asm_nop(4)).append(
                                  "\xE8\x81\xBD\x2D\xFF"                    // call dunia.10C0B03B  ; Os::Agora::Network::getSingleton()
                                  "\x8B\x10"                                // mov  edx,dword ptr ds:[eax]
                                  "\x8B\x52\x14"                            // mov  edx,dword ptr ds:[edx+14]
                                  "\x3E\x8B\x0C\x24"                        // mov  ecx,dword ptr ds:[esp]
                                  "\x36\x8D\x49\x08"                        // lea  ecx,dword ptr ds:[ecx+8]
                                  "\x51"                                    // push ecx
                                  "\x8B\xC8"                                // mov  ecx,eax
                                  "\xFF\xD2"                                // call edx
                                  "\x58"                                    // pop  eax
                                  "\x59"                                    // pop  ecx
                                  "\x5A"                                    // pop  edx
                                  "\xE9\x48\x22\xE4\xFE", 31) },            // jmp  <dunia.return>
                    { 0x10771517, std::string("\xE9\x84\xDD\x1B\x01", 5) }, // jmp  <dunia.text>    ; Change function call to instead jump to the .text_p section.
                    { 0x10cb2588, get_asm_nop(6) } // bypassing the rate limiting of map downloads by NOP out rate limit jump.
                }
            },
            { // Steam, Uplay and Epic Games
                {
                    // Steam
                    { "6353936a54aa841350bb30ff005727859cdef1aa10c209209b220b399e862765",
                      "bc5122bb07248861677c1a35fef25cff4912a51714126c5f02505b52d4351383" },

                    // Uplay and Epic Games
                    { "b7219dcd53317b958c8a31c9241f6855cab660a122ce69a0d88cf4c356944e92",
                      "24f52200e9e42a4c8428aabd205b8ad791fa59171848b30d0a59a842cfe0ab9e" }
                },
                {
                    { "31c0784efadfd6b526379eba24be939f79a4c93ac0f1e1e8565243ff38319f4e" }, // Hash for Steam version 0.2.0.
                    { "5b03e1da81f19abc7e5b9750daae89b28591087228fb104673b8078f2e29b98c" }, // Hash for Uplay version 0.2.0.
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

                    // Tweak: Swap Ubisoft endpoints with our own.
                    { 0x10f05568, patch_endpoint_config_host, ".rdata" },
                    { 0x10f07dd0, patch_endpoint_stun_host, ".rdata" },
                    { 0x10f07de4, patch_endpoint_stun_host, ".rdata" },
                    { 0x10f07df8, patch_endpoint_stun_host, ".rdata" },

                    /* Client */
                    // Tweak: Replace game_id with new for community backend.
                    { 0x10e420c0, agoraIdList[4 + agoraIdModifier], ".rdata" }, // game_id

                    // Tweak: Swap Ubisoft endpoints with our own.
                    { 0x10f3fa7c, patch_endpoint_onlineconfig, ".rdata" },

                    // Tweak: Patch in our own agora root public key, and use that instead
                    { patch_agora_root_public_key },

                    // Tweak: Call rsa_import() with our agora root public key
                    { std::string("\x8D\x86\x58\x33\x00\x00"                // lea  eax,dword ptr ds:[esi+3358] ; Get address of rsa_key *key
                                  "\x50"                                    // push eax                         ; rsa_key *key
                                  "\x68\x0E\x02\x00\x00"                    // push 526                         ; unsigned long inlen
                                  "\x68\x00\xD0\x9E\x11"                    // push dunia.119ED000              ; const unsigned char *in
                                  "\xE8\x1A\x1A\x28\xFF"                    // call dunia.10C6EC50              ; Calling rsa_import()
                                  "\xE9\xA6\x6C\x23\xFF", 27) },            // jmp  <dunia.return>
                    { 0x10c23edc, std::string("\xE9\x3F\x93\xDC\x00", 5) }, // jmp  <dunia.text>                ; Jump to codecave because of space constrains
                    { 0x10c23ee1, get_asm_nop(15) },

                    // Tweak: Change function call genCdKeyIdHex() to instead call external.
                    { std::string("\x57"                                    // push edi                                                    ; cd key
                                  "\x56"                                    // push esi                                                    ; serial name
                                  "\x50"                                    // push eax                                                    ; out length
                                  "\x8D\x85\x04\x01\x00\x00"                // lea eax,dword ptr ss:[ebp+104]
                                  "\x50"                                    // push eax                                                    ; out pointer
                                  "\xFF\x15\x18\xBA\x9E\x11"                // call dword ptr ds:[<&_ZN7MPPatch13genCdKeyIdHexEPhPjPcS2_>] ; MPPatch::generateCdKeyIdHex()
                                  "\x59"                                    // pop ecx                                                     ; clean up cd key
                                  "\x59"                                    // pop ecx                                                     ; clean up serial name
                                  "\xE9\x6A\x77\x23\xFF", 23) },            // jmp <dunia.return>
                    { 0x10c249ae, get_asm_nop(8) },
                    { 0x10c249bc, std::string("\xE9\x7F\x88\xDC\x00", 5) }, // jmp <dunia.text>                                            ; Change function call to instead jump to the .text_p section.

                    // Tweak: Use OCTETSTRING instead of IA5STRING
                    { std::string("\xC7\x85\x74\xFF\xFF\xFF\x05\x00\x00\x00" // mov dword ptr ss:[ebp-8C],5
                                  "\xE9\x90\x78\x23\xFF", 15) },             // jmp dunia.119ED260
                    { 0x10c24af9, std::string("\xE9\x62\x87\xDC\x00", 5).append(get_asm_nop(1)) },

                    // Tweak: Change function call genOneTimeKey() to instead call external.
                    { std::string("\xFF\x15\x1C\xBA\x9E\x11"                // call dword ptr ds:[<&_ZN7MPPatch13genOneTimeKeyEPcPyS0_S0_S0_>] ; MPPatch::genOneTimeKey()
                                  "\xE9\x82\x77\x23\xFF", 11) },            // jmp  <dunia.return>
                    { 0x10c24a08, std::string("\xE9\x73\x88\xDC\x00", 5) }, // jmp  <dunia.text>                                               ; Change function call to instead jump to the .text_p section.

                    // Tweak: Remove mouse clamp
                    { 0x105ffc78, get_asm_nop(8) }, // Replace byte 0x105ffc78 to 0x105ffc7f with "nop" instruction.

                    /* Server */
                    // Fix: Custom map download
                    { 0x10cebaf2, asm_jmp }, // change JZ (74) to JMP (EB)
                    { std::string("\xE8\x5B\x03\xD9\xFE"                    // call dunia.1077D600  ; GetNetFileServerAddress()
                                  "\x52"                                    // push edx
                                  "\x51"                                    // push ecx
                                  "\x50"                                    // push eax
                                  "\xE8\x83\x06\xD9\xFE"                    // call dunia.1077D930  ; IsSessionTypeLAN()
                                  "\x84\xC0"                                // test al,al
                                  "\x75\x1B", 17)                           // jne  <dunia.end>
                      .append(get_asm_nop(4)).append(
                                  "\xE8\x99\xEF\x22\xFF"                    // call dunia.10C1C253  ; Os::Agora::Network::getSingleton()
                                  "\x8B\x10"                                // mov  edx,dword ptr ds:[eax]
                                  "\x8B\x52\x14"                            // mov  edx,dword ptr ds:[edx+14]
                                  "\x3E\x8B\x0C\x24"                        // mov  ecx,dword ptr ds:[esp]
                                  "\x36\x8D\x49\x08"                        // lea  ecx,dword ptr ds:[ecx+8]
                                  "\x51"                                    // push ecx
                                  "\x8B\xC8"                                // mov  ecx,eax
                                  "\xFF\xD2"                                // call edx
                                  "\x58"                                    // pop  eax
                                  "\x59"                                    // pop  ecx
                                  "\x5A"                                    // pop  edx
                                  "\xE9\x28\x0C\xD9\xFE", 31) },            // jmp  <dunia.return>
                    { 0x1077def7, std::string("\xE9\xA4\xF3\x26\x01", 5) }, // jmp  <dunia.text>    ; Change function call to instead jump to the .text_p section.
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
                      "b671face0db269fd6e269d75db3996f55a5371d839a2aaca121439edc701f277" }
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

                    // Tweak: Swap Ubisoft endpoints with our own.
                    { 0x0105eb28, patch_endpoint_config_host, ".rdata" },
                    { 0x0105fdd0, patch_endpoint_stun_host, ".rdata" },
                    { 0x0105fde4, patch_endpoint_stun_host, ".rdata" },
                    { 0x0105fdf8, patch_endpoint_stun_host, ".rdata" },

                    /* Server */
                    // Tweak: Replace game_id with new for community backend.
                    { 0x01042f50, agoraIdList[5 + agoraIdModifier], ".rdata" }, // game_id

                    // Fix: Custom map download
                    { 0x004ecda5, asm_jmp }, // change JZ (74) to JMP (EB)
                    { std::string("\xE8\x4B\x4C\x30\xFF"                    // call fc2serverlauncher.AB3C50  ; GetNetFileServerAddress()
                                  "\x52"                                    // push edx
                                  "\x51"                                    // push ecx
                                  "\x50"                                    // push eax
                                  "\xE8\x03\x01\x30\xFF"                    // call fc2serverlauncher.AAF110  ; IsSessionTypeLAN()
                                  "\x84\xC0"                                // test al,al
                                  "\x75\x1B", 17)                           // jne  <fc2serverlauncher.end>
                      .append(get_asm_nop(4)).append(
                                  "\xE8\x07\x42\x6B\xFF"                    // call fc2serverlauncher.E63221  ; Os::Agora::Network::getSingleton()
                                  "\x8B\x10"                                // mov  edx,dword ptr ds:[eax]
                                  "\x8B\x52\x14"                            // mov  edx,dword ptr ds:[edx+14]
                                  "\x3E\x8B\x0C\x24"                        // mov  ecx,dword ptr ds:[esp]
                                  "\x36\x8D\x49\x08"                        // lea  ecx,dword ptr ds:[ecx+8]
                                  "\x51"                                    // push ecx
                                  "\x8B\xC8"                                // mov  ecx,eax
                                  "\xFF\xD2"                                // call edx
                                  "\x58"                                    // pop  eax
                                  "\x59"                                    // pop  ecx
                                  "\x5A"                                    // pop  edx
                                  "\xE9\x31\x91\x30\xFF", 31) },            // jmp  <fc2serverlauncher.return>
                    { 0x00ab8160, std::string("\xE9\x9B\x6E\xCF\x00", 5) }, // jmp  <fc2serverlauncher.text> ; Change function call to instead jump to the .text_p section.
                    { 0x004ecb38, get_asm_nop(6) }, // bypassing the rate limiting of map downloads by NOP out rate limit jump.

                    // Fix: Possibility to disable PunkBuster also for ranked matches.
                    { 0x0094c74b, std::string("\xE9\xA9\x00\x00\x00", 5).append(get_asm_nop(1)) }, // change JZ to JMP + NOP, from (0F 84 A8 00 00 00) to (E9 A9 00 00 00 90), bypassing PB checks for ranked matches.
                    { 0x0094c943, asm_jmp }, // change JZ to JMP in order to bypass autoenable of PB on ranked matches.
                    { 0x00661eac, get_asm_nop(2) }, // change JNZ to NOP in order to prevent PB from starting after match is started.

                    // Fix: Unintentional line break due to UTF-16 character being printed on client join.
                    { std::string("\x68\x10\x0F\x04\x01"        // push fc2serverlauncher.1040f10
                                  "\x57"                        // push edi
                                  "\x50"                        // push eax
                                  "\x51"                        // push ecx
                                  "\x66\x8B\x08"                // mov  cx,word ptr ds:[eax]
                                  "\x66\x85\xC9"                // test cx,cx
                                  "\x74\x29", 16)               // je   <fc2serverlauncher.exit_joined>
                      .append(get_asm_nop(4)).append(
                                  "\x80\xE1\x80"                // and  cl,80
                                  "\x75\x11", 5)                // jne  <fc2serverlauncher.do_joined>
                      .append(get_asm_nop(4)).append(
                                  "\x84\xED"                    // test ch,ch
                                  "\x75\x09", 4)                // jne  <fc2serverlauncher.do_joined>
                      .append(get_asm_nop(4)).append(
                                  "\xEB\x0A", 2)                // jmp  <fc2serverlauncher.continue_joined>
                      .append(get_asm_nop(3)).append(
                                  "\xB1\x3F"                    // mov  cl,3F
                                  "\x32\xED"                    // xor  ch,ch
                                  "\x66\x89\x08"                // mov  word ptr ds:[eax],cx
                                  "\x83\xC0\x02"                // add  eax,2
                                  "\xEB\xD2", 12)               // jmp  <fc2serverlauncher.loop_joined>
                      .append(get_asm_nop(3)).append(
                                  "\x59"                        // pop    ecx
                                  "\x58"                        // pop    eax
                                  "\xE9\x93\x2B\x55\xFF", 7) }, // jmp <fc2serverlauncher.return_joined>
                    { 0x00d01c0d, std::string("\xE9\x2E\xD4\xAA\x00", 5).append(get_asm_nop(1)) }, // change function call to instead jump to the .text_p section.

                    // Fix: Unintentional line break due to UTF-16 character being printed on client leave.
                    { std::string("\x68\x70\x0F\x04\x01"        // push fc2serverlauncher.1040F70
                                  "\x57"                        // push edi
                                  "\x50"                        // push eax
                                  "\x51"                        // push ecx
                                  "\x66\x8B\x08"                // mov  cx,word ptr ds:[eax]
                                  "\x66\x85\xC9"                // test cx,cx
                                  "\x74\x29", 16)               // je   <fc2serverlauncher.exit_joined>
                      .append(get_asm_nop(4)).append(
                                  "\x80\xE1\x80"                // and  cl,80
                                  "\x75\x11", 5)                // jne  <fc2serverlauncher.do_joined>
                      .append(get_asm_nop(4)).append(
                                  "\x84\xED"                    // test ch,ch
                                  "\x75\x09", 4)                // jne  <fc2serverlauncher.do_joined>
                      .append(get_asm_nop(4)).append(
                                  "\xEB\x0A", 2)                // jmp  <fc2serverlauncher.continue_joined>
                      .append(get_asm_nop(3)).append(
                                  "\xB1\x3F"                    // mov  cl,3F
                                  "\x32\xED"                    // xor  ch,ch
                                  "\x66\x89\x08"                // mov  word ptr ds:[eax],cx
                                  "\x83\xC0\x02"                // add  eax,2
                                  "\xEB\xD2", 12)               // jmp  <fc2serverlauncher.loop_joined>
                      .append(get_asm_nop(3)).append(
                                  "\x59"                        // pop  ecx
                                  "\x58"                        // pop  eax
                                  "\xE9\xA3\x2C\x55\xFF", 7) }, // jmp <fc2serverlauncher.return_left>
                    { 0x00d01d5d, std::string("\xE9\x1E\xD3\xAA\x00", 5).append(get_asm_nop(1)) } // change function call to instead jump to the .text_p section.
                }
            },
            { // Steam (R2 is identical) and Uplay
                {
                    // Steam (R2 is identical)
                    { "5cd5d7b6e6e0b1d25843fdee3e9a743ed10030e89ee109b121109f4a146a062e",
                      "66273498444f82cc2c0ef01c0fca539034abb397a3fbcefbb595b3df0c135e8c" },

                    // Uplay
                    { "948a8626276a6689c0125f2355b6a820c104f20dee36977973b39964a82f2703",
                      "19a8f132b6dbfd7173f3e3ea5a2d084a03bbb9aa189ce4237aeb80effea0abe8" }
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

                    // Tweak: Swap Ubisoft endpoints with our own.
                    { 0x01061b78, patch_endpoint_config_host, ".rdata" },
                    { 0x01062e20, patch_endpoint_stun_host, ".rdata" },
                    { 0x01062e34, patch_endpoint_stun_host, ".rdata" },
                    { 0x01062e48, patch_endpoint_stun_host, ".rdata" },

                    /* Server */
                    // Tweak: Replace game_id with new for community backend.
                    { 0x01045fb0, agoraIdList[5 + agoraIdModifier], ".rdata" }, // game_id

                    // Fix: Custom map download
                    { 0x004eca95, asm_jmp }, // change JZ (74) to JMP (EB)
                    { std::string("\xE8\x4B\xCB\x2F\xFF"                    // call fc2serverlauncher.AAEB50  ; GetNetFileServerAddress()
                                  "\x52"                                    // push edx
                                  "\x51"                                    // push ecx
                                  "\x50"                                    // push eax
                                  "\xE8\x13\x72\x2F\xFF"                    // call fc2serverlauncher.AA9220  ; IsSessionTypeLAN()
                                  "\x84\xC0"                                // test al,al
                                  "\x75\x1B", 17)                           // jne  <fc2serverlauncher.end>
                      .append(get_asm_nop(4)).append(
                                  "\xE8\x7D\x34\x6B\xFF"                    // call fc2serverlauncher.E65497  ; Os::Agora::Network::getSingleton()
                                  "\x8B\x10"                                // mov  edx,dword ptr ds:[eax]
                                  "\x8B\x52\x14"                            // mov  edx,dword ptr ds:[edx+14]
                                  "\x3E\x8B\x0C\x24"                        // mov  ecx,dword ptr ds:[esp]
                                  "\x36\x8D\x49\x08"                        // lea  ecx,dword ptr ds:[ecx+8]
                                  "\x51"                                    // push ecx
                                  "\x8B\xC8"                                // mov  ecx,eax
                                  "\xFF\xD2"                                // call edx
                                  "\x58"                                    // pop  eax
                                  "\x59"                                    // pop  ecx
                                  "\x5A"                                    // pop  edx
                                  "\xE9\xD1\x10\x30\xFF", 31) },            // jmp  <fc2serverlauncher.return>
                    { 0x00ab3100, std::string("\xE9\xFB\xEE\xCF\x00", 5) }, // jmp  <fc2serverlauncher.text> ; Change function call to instead jump to the .text_p section.
                    { 0x004ec828, get_asm_nop(6) }, // bypassing the rate limiting of map downloads by NOP out rate limit jump.

                    // Fix: Possibility to disable PunkBuster also for ranked matches.
                    { 0x0094d39b, std::string("\xE9\xA9\x00\x00\x00", 5).append(get_asm_nop(1)) }, // change JZ to JMP + NOP, from (0F 84 A8 00 00 00) to (E9 A9 00 00 00 90), bypassing PB checks for ranked matches.
                    { 0x0094d593, asm_jmp }, // change JZ to JMP in order to bypass autoenable of PB on ranked matches.
                    { 0x0067552c, get_asm_nop(2) }, // change JNZ to NOP in order to prevent PB from starting autostarting after match is started.

                    // Fix: Unintentional line break due to UTF-16 character being printed on client join.
                    { std::string("\x68\x78\x3F\x04\x01"        // push fc2serverlauncher.1043F78
                                  "\x57"                        // push edi
                                  "\x50"                        // push eax
                                  "\x51"                        // push ecx
                                  "\x66\x8B\x08"                // mov  cx,word ptr ds:[eax]
                                  "\x66\x85\xC9"                // test cx,cx
                                  "\x74\x29", 16)               // je   <fc2serverlauncher.exit_joined>
                      .append(get_asm_nop(4)).append(
                                  "\x80\xE1\x80"                // and  cl,80
                                  "\x75\x11", 5)                // jne  <fc2serverlauncher.do_joined>
                      .append(get_asm_nop(4)).append(
                                  "\x84\xED"                    // test ch,ch
                                  "\x75\x09", 4)                // jne  <fc2serverlauncher.do_joined>
                      .append(get_asm_nop(4)).append(
                                  "\xEB\x0A", 2)                // jmp  <fc2serverlauncher.continue_joined>
                      .append(get_asm_nop(3)).append(
                                  "\xB1\x3F"                    // mov  cl,3F
                                  "\x32\xED"                    // xor  ch,ch
                                  "\x66\x89\x08"                // mov  word ptr ds:[eax],cx
                                  "\x83\xC0\x02"                // add  eax,2
                                  "\xEB\xD2", 12)               // jmp  <fc2serverlauncher.loop_joined>
                      .append(get_asm_nop(3)).append(
                                  "\x59"                        // pop  ecx
                                  "\x58"                        // pop  eax
                                  "\xE9\x13\x28\x55\xFF", 7) }, // jmp <fc2serverlauncher.return_left>
                    { 0x00d0488d, std::string("\xE9\xAE\xD7\xAA\x00", 5).append(get_asm_nop(1)) }, // change function call to instead jump to the .text_p section.

                    // Fix: Unintentional line break due to UTF-16 character being printed on client leave.
                    { std::string("\x68\xD8\x3F\x04\x01"        // push fc2serverlauncher.1043FD8
                                  "\x57"                        // push edi
                                  "\x50"                        // push eax
                                  "\x51"                        // push ecx
                                  "\x66\x8B\x08"                // mov  cx,word ptr ds:[eax]
                                  "\x66\x85\xC9"                // test cx,cx
                                  "\x74\x29", 16)               // je   <fc2serverlauncher.exit_joined>
                      .append(get_asm_nop(4)).append(
                                  "\x80\xE1\x80"                // and  cl,80
                                  "\x75\x11", 5)                // jne  <fc2serverlauncher.do_joined>
                      .append(get_asm_nop(4)).append(
                                  "\x84\xED"                    // test ch,ch
                                  "\x75\x09", 4)                // jne  <fc2serverlauncher.do_joined>
                      .append(get_asm_nop(4)).append(
                                  "\xEB\x0A", 2)                // jmp  <fc2serverlauncher.continue_joined>
                      .append(get_asm_nop(3)).append(
                                  "\xB1\x3F"                    // mov  cl,3F
                                  "\x32\xED"                    // xor  ch,ch
                                  "\x66\x89\x08"                // mov  word ptr ds:[eax],cx
                                  "\x83\xC0\x02"                // add  eax,2
                                  "\xEB\xD2", 12)               // jmp  <fc2serverlauncher.loop_joined>
                      .append(get_asm_nop(3)).append(
                                  "\x59"                        // pop  ecx
                                  "\x58"                        // pop  eax
                                  "\xE9\x23\x29\x55\xFF", 7) }, // jmp <fc2serverlauncher.return_left>
                    { 0x00d049dd, std::string("\xE9\x9E\xD6\xAA\x00", 5).append(get_asm_nop(1)) }, // change function call to instead jump to the .text_p section.

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
