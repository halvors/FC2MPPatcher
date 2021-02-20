#ifndef GLOBAL_H
#define GLOBAL_H

#include <QString>
#include <QStringList>
#include <QByteArray>
#include <QList>

#include "entry.h"

#include <QDebug>
#include <QtEndian>

// Set true for debugging mode without checksum verification.
#define DEBUG_MODE true

constexpr char app_name[] = "FC2MPPatcher";
const QString app_organization = app_name;
const QString app_configuration_file = QString(app_name).toLower() + ".ini";

constexpr char settings_install_directory[] = "installDirectory";
constexpr char settings_interface_index[] = "interfaceIndex";
constexpr char settings_group_window[] = "Window";
constexpr char settings_group_window_size[] = "size";
constexpr char settings_group_window_position[] = "position";
constexpr char settings_group_window_isMaximized[] = "isMaximized";

constexpr char game_name[] = "Far Cry 2";
constexpr char game_publisher[] = "Ubisoft";
constexpr char game_executable_directory[] = "bin";
constexpr char game_backup_suffix[] = "_original";

constexpr char game_steam_name[] = "Steam";
constexpr char game_steam_publisher[] = "Valve";
constexpr char game_steam_app_directory[] = "steamapps";
constexpr char game_steam_app_directory_common[] = "common";
constexpr uint16_t game_steam_app_id = 19900;
constexpr char game_steam_app_manifest_name[] = "appmanifest";
constexpr char game_steam_app_manifest_suffix[] = "acf";
constexpr char game_steam_app_manifest_key[] = "installdir";
constexpr char game_steam_app_library[] = "libraryfolders.vdf";

constexpr uint32_t pe_base_address = 0x400000;
constexpr char pe_patch_rdata_section[] = ".rdata_p";
constexpr char pe_patch_text_section[] = ".text_p";
constexpr uint32_t pe_patch_text_section_virtual_address = 0x13b2000; // Pre-defined place to insert section.

constexpr char patch_library_name[] = "MPPatch";
const QString patch_library_file = QString(patch_library_name).toLower() + ".dll";
const QStringList patch_library_functions = {
    "_ZN7MPPatch10bind_patchEjPK8sockaddri@12",                   // bind()
    "_ZN7MPPatch13connect_patchEjPK8sockaddri@12",                // connect()
    "_ZN7MPPatch12sendTo_patchEjPKciiPK8sockaddri@24",            // sendTo()
    "_ZN7MPPatch21getAdaptersInfo_patchEP16_IP_ADAPTER_INFOPm@8", // getAdapersInfo()
    "_ZN7MPPatch19getHostByName_patchEPKc@4",                     // getHostByName()
    "_ZN7MPPatch18getPublicIPAddressEv@0"                         // getPublicIpAddress()
};

const QString patch_configuration_file = QString(patch_library_name).toLower() + ".cfg";
constexpr char patch_configuration_network[] = "Network";
constexpr char patch_configuration_network_interface_index[] = "InterfaceIndex";
const QStringList patch_library_runtime_dependencies = {
    patch_library_file,
    "libgcc_s_dw2-1.dll",
    "libstdc++-6.dll",
    "libwinpthread-1.dll",
    "Qt5Core.dll",
    "Qt5Network.dll"
};

// Currently only applies for dedicated server, changes lobby server to that of game clients because server endpoint is down.
constexpr char patch_network_lobbyserver_address[] = "216.98.48.56";
constexpr uint16_t patch_network_lobbyserver_port = 3035;

// Currently only applies to Steam and Uplay editions, changes game id sent to Ubisoft to that of the Retail edition.
const QByteArray patch_game_id = QString("2c66b725e7fb0697c0595397a14b0bc8").toUtf8();

static const QByteArray buildIPAddressPatchFuncCall(uint32_t origin, uint32_t callerAddr, uint32_t oldFuncAddr, uint32_t patchFuncPtr) {
    origin += pe_base_address;

    //origin = //017b2000 //0x13b2000
    //callerAddr = 0x00ab3100;
    //oldFuncAddr = 0x00aaeb50; // GetNetFileServerAddress
    //patchFuncPtr = 0x6d242068

    // relative oldFuncAddr = oldFuncAddr - (origin + 5) = 0xff2fcb50
    // patchFuncPtr = 0x6D242068
    // relative callerAddr = callerAddr - (origin + 20)

    uint32_t relOldFuncAddr = oldFuncAddr - (origin + 5);
    uint32_t relCallerAddr = callerAddr - (origin + 20);

    uint32_t revRelOldFuncAddr = qFromBigEndian(relOldFuncAddr);

    qDebug() << QString("OldFuncAddr is: 0x%1, should be 0x%2").arg(relOldFuncAddr, 0, 16).arg(0xff2fcb50, 0, 16);
    qDebug() << QString("OldFuncAddr is: 0x%1 (rev)").arg(revRelOldFuncAddr, 0, 16);

    qDebug() << QString("CallerAddr is: 0x%1, should be 0x%2").arg(relCallerAddr, 0, 16).arg(0xff301105, 0, 16);

    QByteArray ba = QString("\xE8%1"                     // call   0xff2fcb50 // oldFunc
                            "\x51"                       // push   ecx
                            "\x50"                       // push   eax
                            "\xFF\x15\xA4\x0D\x7B\x01"   // call   [0x6D242068] // patchFunc
                            "\x8B\xC8"                   // mov    ecx,eax
                            "\x58"                       // pop    eax
                            "\x89\x48\x08"               // mov    DWORD PTR [eax+0x8],ecx
                            "\x59"                       // pop    ecx
                            "\xE9%3") // jmp    0xff301105 // origin + 5
                    .arg(qFromBigEndian(relOldFuncAddr))
                    .arg(patchFuncPtr)
                    .arg(qFromBigEndian(relCallerAddr))
                    .toUtf8();

    /*
    return QByteArray("\xE8\x4B\xCB\x2F\xFF"       // call   0xff2fcb50 // oldFunc
                      "\x51"                       // push   ecx
                      "\x50"                       // push   eax
                      "\xFF\x15\xA4\x0D\x7B\x01"   // call   [0x6D242068] // patchFunc
                      "\x8B\xC8"                   // mov    ecx,eax
                      "\x58"                       // pop    eax
                      "\x89\x48\x08"               // mov    DWORD PTR [eax+0x8],ecx
                      "\x59"                       // pop    ecx
                      "\xE9\xEC\x10\x30\xFF", 25); // jmp    0xff301105 // origin + 5
    */

    return ba;
}

const QList<CodeEntry> steamEntry = {
    // Common
    { 0x004263d4, 0 }, // bind()
    { 0x0042641b, 0 }, // bind()
    { 0x004c9d2a, 0 }, // bind()
    { 0x00ba36be, 0 }, // bind()
    { 0x00e85ffa, 0 }, // bind()
    { 0x00ba6e83, 2 }, // sendTo()
    { 0x00c46a66, 3 }, // getAdapersInfo()
    { 0x00ba714c, 4 }, // getHostByName()

    // Server
    { 0x00c465bd, 1 },  // connect()
    { 0x004eca95, QByteArray("\xEB", 1) }, // change JZ (74) to JMP (EB)
    { 0x00ab3100, QByteArray("\xE9\xFB\xEE\xCF\x00", 5) } // change function call to instead jump to .text_p section.
};

const QList<FileEntry> files = {
    {
        "Dunia.dll",
        {
            { // Retail (GOG is identical)
                "7b82f20088e5c046a99fcaed65dc8bbb8202fd622a69737be83e00686b172d53",
                "020ba8709ba7090fa9e29c77f26a66ea230aef92677fe93560d97e391be43c97",
                {
                    // Common
                    { 0x1001088e, 0 }, // bind()
                    { 0x10213d18, 0 }, // bind()
                    { 0x10c4e97a, 0 }, // bind()
                    { 0x10cb9a8c, 0 }, // bind()
                    { 0x10cb9ad4, 0 }, // bind()
                    { 0x10014053, 2 }, // sendTo()
                    { 0x10c5bde2, 3 }, // getAdapersInfo()
                    { 0x1001431c, 4 }  // getHostByName()
                }
            },
            { // Steam
                "6353936a54aa841350bb30ff005727859cdef1aa10c209209b220b399e862765",
                "40f4d55fe0ac6b370798983de2ca1dd09ef0423a7c523b7c424cadddbd894a25",
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
                    { 0x10e420c0, patch_game_id, ".rdata" } // game_id
                }
            },
            { // Uplay
                "b7219dcd53317b958c8a31c9241f6855cab660a122ce69a0d88cf4c356944e92",
                "c7674c14bad4214e547da3d60ccb14225665394f75b941b10c33362b206575c5",
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
                    { 0x10e420c0, patch_game_id, ".rdata" } // game_id
                }
            }
        }
    },
    {
        "FC2ServerLauncher.exe",
        {
            { // Retail (GOG is identical)
                "c175d2a1918d3e6d4120a2f6e6254bd04907a5ec10d3c1dfac28100d6fbf9ace",
                "bfb73dffcac987a511be8a7d34f66644e9171dc0fee6a48a17256d6b5e55dc64",
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
                    { 0x004ecda5, QByteArray("\xEB", 1) } // change JZ (74) to JMP (EB)
                }
            },
            { // Steam (R2 is identical)
                "5cd5d7b6e6e0b1d25843fdee3e9a743ed10030e89ee109b121109f4a146a062e",
                "bfb73dffcac987a511be8a7d34f66644e9171dc0fee6a48a17256d6b5e55dc64",
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

                    // Server
                    { 0x00c465bd, 1 }, // connect()
                    { 0x004eca95, QByteArray("\xEB", 1) }, // change JZ (74) to JMP (EB)
                    {
                        QByteArray("\xE8\x4B\xCB\x2F\xFF"      // call   0xff2fcb50
                                   "\x51"                      // push   ecx
                                   "\x50"                      // push   eax
                                   "\xFF\x15\xA4\x0D\x7B\x01"  // call   DWORD PTR ds:0x17b0da4
                                   "\x8B\xC8"                  // mov    ecx,eax
                                   "\x58"                      // pop    eax
                                   "\x89\x48\x08"              // mov    DWORD PTR [eax+0x8],ecx
                                   "\x59"                      // pop    ecx
                                   "\xE9\xEC\x10\x30\xFF", 25) // jmp    0xff301105
                    },
                    { 0x00ab3100,
                      buildIPAddressPatchFuncCall(pe_patch_text_section_virtual_address,
                                                  0x00ab3100,
                                                  0x00aaeb50, // GetNetFileServerAddress
                                                  0x6d242068)
                    }
                    //{ 0x00ab3100, QByteArray("\xE9\xFB\xEE\xCF\x00", 5) } // change function call to instead jump to the .text_p section.
                }
            },
            { // Uplay
                "948a8626276a6689c0125f2355b6a820c104f20dee36977973b39964a82f2703",
                "38f33dfd74b9483fb7db7703dffe61d61fa51444730d38ed2b61fc6e20855d9a",
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

                    // Server
                    { 0x00c465bd, 1 },  // connect()
                    { 0x004eca95, QByteArray("\xEB", 1) }, // change JZ (74) to JMP (EB)
                    { 0x00ab3100, QByteArray("\xE9\xFB\xEE\xCF\x00", 5) } // change function call to instead jump to .text_p section.
                }
            }
        }
    }
};

#endif // GLOBAL_H
