#ifndef PATCH_DEFS_H
#define PATCH_DEFS_H

#include <QByteArray>
#include <QList>

#include "entry.h"

// Currently only applies for dedicated server, changes lobby server to that of the client because the server endpoint is currently down.
constexpr char patch_network_lobbyserver_address[] = "216.98.48.56";
constexpr uint16_t patch_network_lobbyserver_port = 3035;

const QStringList patch_library_functions = {
    "_ZN7MPPatch10bind_patchEjPK8sockaddri@12",                   // bind()
    "_ZN7MPPatch13connect_patchEjPK8sockaddri@12",                // connect()
    "_ZN7MPPatch12sendTo_patchEjPKciiPK8sockaddri@24",            // sendTo()
    "_ZN7MPPatch21getAdaptersInfo_patchEP16_IP_ADAPTER_INFOPm@8", // getAdapersInfo()
    "_ZN7MPPatch19getHostByName_patchEPKc@4",                     // getHostByName()
    "_ZN7MPPatch18getPublicIPAddressEv@0"                         // getPublicIpAddress()
};

// Currently only applies to Steam and Uplay editions, changes game id sent to Ubisoft to that of the Retail edition.
const QByteArray patch_game_id = QByteArray("2c66b725e7fb0697c0595397a14b0bc8");

/**
 * Patch values patch related to community backend.
 *
 * Original: gconnect.ubi.com0000
 * New:      gc.farcry2.online000
 *
 * Original: onlineconfigservice.ubi.com
 * New:      gc.farcry2.online:307400000
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
const QByteArray patch_endpoint_gconnect = "gc.farcry2.online";
const QByteArray patch_endpoint_onlineconfig = QByteArray(patch_endpoint_gconnect).append(":3074").append(5, '\0');
const QByteArray patch_endpoint_stun = "stun.farcry2.online";

const QByteArray patch_dev_game_id = QByteArray("88838b37e409143c9319694a6418df42");
const QByteArray patch_dev_dedicated_server_id = QByteArray("3776f77a31dfdb6b34f6e689ee132d02");

// Reusable assembly constants.
const QByteArray asm_jmp = QByteArray("\xEB", 1);

inline const QByteArray asm_nop(uint16_t numBytes) {
    QByteArray nop;

    for (uint16_t i = 0; i < numBytes; i++)
        nop.append('\x90');

    return nop;
}

const QList<FileEntry> files = {
    {
        "Dunia.dll",
        {
            { // Retail (GOG is identical)
                {
                    { "7b82f20088e5c046a99fcaed65dc8bbb8202fd622a69737be83e00686b172d53",
                      "1d7fa188f0aa7046195b6eb7e4494ebd8ae649136719425faad21909df0639a5" }
                },
                {
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
                    { 0x10014053, 2 }, // sendTo()
                    { 0x10c5bde2, 3 }, // getAdapersInfo()
                    { 0x1001431c, 4 }, // getHostByName()

                    // Fix: Patch Ubisoft endpoints with our own.
                    { 0x10e7cb98, patch_endpoint_gconnect, ".rdata" },
                    { 0x10e7f400, patch_endpoint_stun, ".rdata" },
                    { 0x10e7f414, patch_endpoint_stun, ".rdata" },
                    { 0x10e7f428, patch_endpoint_stun, ".rdata" },

                    /* Client */
                    // Fix: Hack to avoid verfiying agora certificate with public key from game files.
                    { 0x10c1354c, asm_nop(2) }, // Just importing key no matter if sig verification was success or not :-)

                    /* Client */
                    // Tweak: Remove mouse clamp
                    { 0x105f2338, asm_nop(8) }, // Replace byte 0x105ffc78 to 0x105ffc7f with "nop" instruction.

                    /* Server */
                    // Fix: Custom map download
                    { 0x10cb29e2, asm_jmp }, // change JZ (74) to JMP (EB)
                    { QByteArray("\xE8\xCB\x1B\xE4\xFE"         // call   dunia.10770BD0
                                 "\x51"                         // push   ecx
                                 "\x50"                         // push   eax
                                 "\xFF\x15\x10\xD7\x92\x11"     // call   dword ptr ds:[<&_ZN7MPPatch18getPublicIPAddressEv@0>]
                                 "\x8B\xC8"                     // mov    ecx,eax
                                 "\x58"                         // pop    eax
                                 "\x89\x48\x08"                 // mov    dword ptr ds:[eax+8],ecx
                                 "\x59"                         // pop    ecx
                                 "\xE9\x03\x25\xE4\xFE", 25) }, // jmp    <dunia.retur>
                    { 0x10771517, QByteArray("\xE9\xE4\xDA\x1B\x01", 5) }, // change function call to instead jump to the .text_p section.
                    { 0x10cb2588, asm_nop(6) } // bypassing the rate limiting of map downloads by NOP out rate limit jump.
                }
            },
            { // Steam and Uplay
                {
                    // Steam
                    { "6353936a54aa841350bb30ff005727859cdef1aa10c209209b220b399e862765",
                      "8258f42dfe094f92f331722dcf687bd02e983ad2a0205de89851334683c38f7b" },
                    // Uplay
                    { "b7219dcd53317b958c8a31c9241f6855cab660a122ce69a0d88cf4c356944e92",
                      "e6163b4d33591af427e962605408ac9bcf5b748603949e91818f004b95087702" }
                },
                {
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
                    { 0x10013f33, 2 }, // sendTo()
                    { 0x10c6a692, 3 }, // getAdapersInfo()
                    { 0x100141fc, 4 }, // getHostByName()

                    // Fix: Patch Ubisoft endpoints with our own.
                    { 0x10f05568, patch_endpoint_gconnect, ".rdata" },
                    { 0x10f07dd0, patch_endpoint_stun, ".rdata" },
                    { 0x10f07de4, patch_endpoint_stun, ".rdata" },
                    { 0x10f07df8, patch_endpoint_stun, ".rdata" },

                    /* Client */
                    // Tweak: Replace game id with dev version.
                    { 0x10e420c0, patch_dev_game_id, ".rdata" }, // game_id

                    // Fix: Patch Ubisoft endpoints with our own.
                    { 0x10f3fa7c, patch_endpoint_onlineconfig, ".rdata" },

                    // Fix: Hack to avoid verfiying agora certificate with public key from game files.
                    { 0x10c24829, asm_nop(2) }, // Just importing key no matter if sig verification was success or not :-)

                    // Tweak: Remove mouse clamp
                    { 0x105ffc78, asm_nop(8) }, // Replace byte 0x105ffc78 to 0x105ffc7f with "nop" instruction.

                    /* Server */
                    // Fix: Custom map download
                    { 0x10cebaf2, asm_jmp }, // change JZ (74) to JMP (EB)
                    { QByteArray("\xE8\xFB\x05\xD9\xFE"         // call   dunia.1077D600
                                 "\x51"                         // push   ecx
                                 "\x50"                         // push   eax
                                 "\xFF\x15\x14\xBA\x9E\x11"     // call   dword ptr ds:[<&_ZN7MPPatch18getPublicIPAddressEv@0>]
                                 "\x8B\xC8"                     // mov    ecx,eax
                                 "\x58"                         // pop    eax
                                 "\x89\x48\x08"                 // mov    dword ptr ds:[eax+8],ecx
                                 "\x59"                         // pop    ecx
                                 "\xE9\xE3\x0E\xD9\xFE", 25) }, // jmp    <dunia.retur>
                    { 0x1077def7, QByteArray("\xE9\x04\xF1\x26\x01", 5) }, // change function call to instead jump to the .text_p section.
                    { 0x10ceb6c8, asm_nop(6) } // bypassing the rate limiting of map downloads by NOP out rate limit jump.
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
                      "1ca2678496e202bd6b86864f6ed21be178a2abab67fc425c0a3aab8625918c25" }
                },
                {
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
                    { 0x00ba4a33, 2 }, // sendTo()
                    { 0x00c444a6, 3 }, // getAdapersInfo()
                    { 0x00ba4cfc, 4 }, // getHostByName()

                    // Fix: Patch Ubisoft endpoints with our own.
                    { 0x0105eb28, patch_endpoint_gconnect, ".rdata" },
                    { 0x0105fdd0, patch_endpoint_stun, ".rdata" },
                    { 0x0105fde4, patch_endpoint_stun, ".rdata" },
                    { 0x0105fdf8, patch_endpoint_stun, ".rdata" },

                    /* Server */
                    // Fix: Servers being able to register with Ubisoft in online mode.
                    //{ 0x00c43ffd, 1 },  // connect()

                    // Fix: Custom map download
                    { 0x004ecda5, asm_jmp }, // change JZ (74) to JMP (EB)
                    { QByteArray("\xE8\x4B\x4C\x30\xFF"         // call   fc2serverlauncher.AB3C50
                                 "\x51"                         // push   ecx
                                 "\x50"                         // push   eax
                                 "\xFF\x15\xEC\xDB\x7A\x01"     // call   dword ptr ds:[<&_ZN7MPPatch18getPublicIPAddressEv@0>]
                                 "\x8B\xC8"                     // mov    ecx,eax
                                 "\x58"                         // pop    eax
                                 "\x89\x48\x08"                 // mov    dword ptr ds:[eax+8],ecx
                                 "\x59"                         // pop    ecx
                                 "\xE9\x4C\x91\x30\xFF", 25) }, // jmp    <fc2serverlauncher.retur>
                    { 0x00ab8160, QByteArray("\xE9\x9B\x6E\xCF\x00", 5) }, // change function call to instead jump to the .text_p section.
                    { 0x004ecb38, asm_nop(6) }, // bypassing the rate limiting of map downloads by NOP out rate limit jump.

                    // Fix: Possibility to disable PunkBuster also for ranked matches.
                    { 0x0094c74b, QByteArray("\xE9\xA9\x00\x00\x00\x90", 6) }, // change JZ to JMP + NOP, from (0F 84 A8 00 00 00) to (E9 A9 00 00 00 90), bypassing PB checks for ranked matches.
                    { 0x0094c943, asm_jmp }, // change JZ to JMP in order to bypass autoenable of PB on ranked matches.
                    { 0x00661eac, asm_nop(2) } // change JNZ to NOP in order to prevent PB from starting autostarting after match is started.
                }
            },
            { // Steam (R2 is identical) and Uplay
                {
                    // Steam (R2 is identical)
                    { "5cd5d7b6e6e0b1d25843fdee3e9a743ed10030e89ee109b121109f4a146a062e",
                      "64af2e7be0fa6413d8b955eaa2c55dadf6d0e250d3b0e4c006dd5ba4ce6bd7cd" },
                    // Uplay
                    { "948a8626276a6689c0125f2355b6a820c104f20dee36977973b39964a82f2703",
                      "4ea78e5a3a94fd95c70f98521c4cfabadb2db6767e5b0edb3ed137dfc3679c7b" }
                },
                {
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
                    { 0x00ba6e83, 2 }, // sendTo()
                    { 0x00c46a66, 3 }, // getAdapersInfo()
                    { 0x00ba714c, 4 }, // getHostByName()

                    // Fix: Patch Ubisoft endpoints with our own.
                    { 0x01061b78, patch_endpoint_gconnect, ".rdata" },
                    { 0x01062e20, patch_endpoint_stun, ".rdata" },
                    { 0x01062e34, patch_endpoint_stun, ".rdata" },
                    { 0x01062e48, patch_endpoint_stun, ".rdata" },

                    /* Server */
                    // Tweak: Replace game id with dev version.
                    { 0x01045fb0, patch_dev_dedicated_server_id, ".rdata" }, // game_id

                    // Fix: Servers being able to register with Ubisoft in online mode.
                    //{ 0x00c465bd, 1 }, // connect()

                    // Fix: Custom map download
                    { 0x004eca95, asm_jmp }, // change JZ (74) to JMP (EB)
                    { QByteArray("\xE8\x4B\xCB\x2F\xFF"         // call   fc2serverlauncher.AAEB50
                                 "\x51"                         // push   ecx
                                 "\x50"                         // push   eax
                                 "\xFF\x15\xA4\x0D\x7B\x01"     // call   dword ptr ds:[<&_ZN7MPPatch18getPublicIPAddressEv@0>]
                                 "\x8B\xC8"                     // mov    ecx,eax
                                 "\x58"                         // pop    eax
                                 "\x89\x48\x08"                 // mov    mov dword ptr ds:[eax+8],ecx
                                 "\x59"                         // pop    ecx
                                 "\xE9\xEC\x10\x30\xFF", 25) }, // jmp    <fc2serverlauncher.retur>
                    { 0x00ab3100, QByteArray("\xE9\xFB\xEE\xCF\x00", 5) }, // change function call to instead jump to the .text_p section.
                    { 0x004ec828, asm_nop(6) }, // bypassing the rate limiting of map downloads by NOP out rate limit jump.

                    // Fix: Possibility to disable PunkBuster also for ranked matches.
                    { 0x0094d39b, QByteArray("\xE9\xA9\x00\x00\x00\x90", 6) }, // change JZ to JMP + NOP, from (0F 84 A8 00 00 00) to (E9 A9 00 00 00 90), bypassing PB checks for ranked matches.
                    { 0x0094d593, asm_jmp }, // change JZ to JMP in order to bypass autoenable of PB on ranked matches.
                    { 0x0067552c, asm_nop(2) } // change JNZ to NOP in order to prevent PB from starting autostarting after match is started.

                    /* Experimental / WIP */
                    //{ 0x0052c8d3, asm_nop(2) } // bypass min player limit enforced in ranked mode.

                    //{ 0x0052c8d3, asm_nop(2) }, // MinPlayers?... 75 03 // works somehow..?
                    //{ 0x00b046ff, asm_nop(2) }, // MinPlayers?... 75 03 // takes care of persist thru refresh??
                    //{ 0x0094df05, asm_nop(2) } // MinPlayers?... 74 0f // bypass join in progress check?
                }
            }
        }
    }
};

#endif // PATCH_DEFS_H
