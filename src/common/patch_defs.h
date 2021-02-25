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
const QByteArray patch_game_id = QString("2c66b725e7fb0697c0595397a14b0bc8").toUtf8();
const QByteArray patch_asm_jz_to_jmp = QByteArray("\xEB", 1);

const QList<FileEntry> files = {
    {
        "Dunia.dll",
        {
            { // Retail (GOG is identical)
                {
                    { "7b82f20088e5c046a99fcaed65dc8bbb8202fd622a69737be83e00686b172d53",
                      "a3ae457d942ae499d79c583f32e356cc101ffe1a221aae9220d1ac64695f3c79" }
                },
                {
                    // Common
                    { 0x1001088e, 0 }, // bind()
                    { 0x10213d18, 0 }, // bind()
                    { 0x10c4e97a, 0 }, // bind()
                    { 0x10cb9a8c, 0 }, // bind()
                    { 0x10cb9ad4, 0 }, // bind()
                    { 0x10014053, 2 }, // sendTo()
                    { 0x10c5bde2, 3 }, // getAdapersInfo()
                    { 0x1001431c, 4 }, // getHostByName()

                    // Server
                    { 0x10cb29e2, patch_asm_jz_to_jmp }, // change JZ (74) to JMP (EB)
                    { QByteArray("\xE8\xCB\x1B\xE4\xFE"         // call   dunia.10770BD0
                                 "\x51"                         // push   ecx
                                 "\x50"                         // push   eax
                                 "\xFF\x15\x10\xD7\x92\x11"     // call   dword ptr ds:[<&_ZN7MPPatch18getPublicIPAddressEv@0>]
                                 "\x8B\xC8"                     // mov    ecx,eax
                                 "\x58"                         // pop    eax
                                 "\x89\x48\x08"                 // mov    dword ptr ds:[eax+8],ecx
                                 "\x59"                         // pop    ecx
                                 "\xE9\x03\x25\xE4\xFE", 25) }, // jmp    <dunia.retur>
                    { 0x10771517, QByteArray("\xE9\xE4\xDA\x1B\x01", 5) } // change function call to instead jump to the .text_p section.
                }
            },
            { // Steam and Uplay
                {
                    // Steam
                    { "6353936a54aa841350bb30ff005727859cdef1aa10c209209b220b399e862765",
                      "d4942259d5d61c2f743bed7a0b156ab8489e868aa8cfd2b20fc5e417dd52cefa" },
                    // Uplay
                    { "b7219dcd53317b958c8a31c9241f6855cab660a122ce69a0d88cf4c356944e92",
                      "83e2ea791c6227cc96f2515c4638b24809439944d183ad6b0117d58eb2c8869b" }
                },
                {
                    // Common
                    { 0x1001076e, 0 }, // bind()
                    { 0x102161a8, 0 }, // bind()
                    { 0x10c5d10a, 0 }, // bind()
                    { 0x10cf289c, 0 }, // bind()
                    { 0x10cf28e4, 0 }, // bind()
                    { 0x10013f33, 2 }, // sendTo()
                    { 0x10c6a692, 3 }, // getAdapersInfo()
                    { 0x100141fc, 4 }, // getHostByName()

                    // Client
                    { 0x10e420c0, patch_game_id, ".rdata" }, // game_id

                    // Server
                    { 0x10cebaf2, patch_asm_jz_to_jmp }, // change JZ (74) to JMP (EB)
                    { QByteArray("\xE8\xFB\x05\xD9\xFE"         // call   dunia.1077D600
                                 "\x51"                         // push   ecx
                                 "\x50"                         // push   eax
                                 "\xFF\x15\x14\xBA\x9E\x11"     // call   dword ptr ds:[<&_ZN7MPPatch18getPublicIPAddressEv@0>]
                                 "\x8B\xC8"                     // mov    ecx,eax
                                 "\x58"                         // pop    eax
                                 "\x89\x48\x08"                 // mov    dword ptr ds:[eax+8],ecx
                                 "\x59"                         // pop    ecx
                                 "\xE9\xE3\x0E\xD9\xFE", 25) }, // jmp    <dunia.retur>
                    { 0x1077def7, QByteArray("\xE9\x04\xF1\x26\x01", 5) } // change function call to instead jump to the .text_p section.
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
                      "9d9bbce845d81ab01821593f45783de5aba886a4133881cad265245e14247732" }
                },
                {
                    // Common
                    { 0x00425fc4, 0 }, // bind()
                    { 0x0042600b, 0 }, // bind()
                    { 0x004c9d2a, 0 }, // bind()
                    { 0x00ba126e, 0 }, // bind()
                    { 0x00e83eda, 0 }, // bind()
                    { 0x00ba4a33, 2 }, // sendTo()
                    { 0x00c444a6, 3 }, // getAdapersInfo()
                    { 0x00ba4cfc, 4 }, // getHostByName()

                    // Server
                    { 0x00c43ffd, 1 },  // connect()
                    { 0x004ecda5, patch_asm_jz_to_jmp }, // change JZ (74) to JMP (EB)
                    { QByteArray("\xE8\x4B\x4C\x30\xFF"         // call   fc2serverlauncher.AB3C50
                                 "\x51"                         // push   ecx
                                 "\x50"                         // push   eax
                                 "\xFF\x15\xEC\xDB\x7A\x01"     // call   dword ptr ds:[<&_ZN7MPPatch18getPublicIPAddressEv@0>]
                                 "\x8B\xC8"                     // mov    ecx,eax
                                 "\x58"                         // pop    eax
                                 "\x89\x48\x08"                 // mov    dword ptr ds:[eax+8],ecx
                                 "\x59"                         // pop    ecx
                                 "\xE9\x4C\x91\x30\xFF", 25) }, // jmp    <fc2serverlauncher.retur>
                    { 0x00ab8160, QByteArray("\xE9\x9B\x6E\xCF\x00", 5) } // change function call to instead jump to the .text_p section.
                }
            },
            { // Steam (R2 is identical) and Uplay
                {
                    // Steam (R2 is identical)
                    { "5cd5d7b6e6e0b1d25843fdee3e9a743ed10030e89ee109b121109f4a146a062e",
                      "cb5036fccf38a2a8cb73afc36df7844e160657c7f93c35554909b0e52e09ccee" },
                    // Uplay
                    { "948a8626276a6689c0125f2355b6a820c104f20dee36977973b39964a82f2703",
                      "346c6b3a292a352b6ca88ff563f25564d501af632fcb09a07f6a19a1415236c6" }
                },
                {
                    // Common
                    { 0x004263d4, 0 }, // bind()
                    { 0x0042641b, 0 }, // bind()
                    { 0x004c9d2a, 0 }, // bind()
                    { 0x00ba36be, 0 }, // bind()
                    { 0x00e85ffa, 0 }, // bind()
                    { 0x00ba6e83, 2 }, // sendTo()
                    { 0x00c46a66, 3 }, // getAdapersInfo()
                    { 0x00ba714c, 4 }, // getHostByName()

                    // PunkBuster
                    { 0x0094d39b, QByteArray("\xE9\xA9\x00", 3) }, // change JZ to JMP in order to bypass checking that PB setting and installation is enabled/exists.
                    { 0x00675575, QByteArray("\x77", 1) }, // change EnablePbSv() (E8 17 92 08 00) to DisablePbSv() (E8 77 92 08 00) at address 0x00675574

                    // Server
                    { 0x00c465bd, 1 }, // connect()
                    { 0x004eca95, patch_asm_jz_to_jmp }, // change JZ (74) to JMP (EB)
                    { QByteArray("\xE8\x4B\xCB\x2F\xFF"         // call   fc2serverlauncher.AAEB50
                                 "\x51"                         // push   ecx
                                 "\x50"                         // push   eax
                                 "\xFF\x15\xA4\x0D\x7B\x01"     // call   dword ptr ds:[<&_ZN7MPPatch18getPublicIPAddressEv@0>]
                                 "\x8B\xC8"                     // mov    ecx,eax
                                 "\x58"                         // pop    eax
                                 "\x89\x48\x08"                 // mov    mov dword ptr ds:[eax+8],ecx
                                 "\x59"                         // pop    ecx
                                 "\xE9\xEC\x10\x30\xFF", 25) }, // jmp    <fc2serverlauncher.retur>
                    { 0x00ab3100, QByteArray("\xE9\xFB\xEE\xCF\x00", 5) } // change function call to instead jump to the .text_p section.
                }
            }
        }
    }
};

#endif // PATCH_DEFS_H
