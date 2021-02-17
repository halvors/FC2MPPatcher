#ifndef GLOBAL_H
#define GLOBAL_H

#include <QString>
#include <QStringList>
#include <QList>

#include "entry.h"

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
constexpr int game_steam_app_id = 19900;
constexpr char game_steam_app_manifest_name[] = "appmanifest";
constexpr char game_steam_app_manifest_suffix[] = "acf";
constexpr char game_steam_app_manifest_key[] = "installdir";
constexpr char game_steam_app_library[] = "libraryfolders.vdf";

constexpr char patch_library_name[] = "MPPatch";
const QString patch_library_file = QString(patch_library_name).toLower() + ".dll";
const QString patch_library_pe_section = QString(patch_library_name).toLower();
const QStringList patch_library_functions = {
    "_ZN7MPPatch10bind_patchEjPK8sockaddri@12",                   // bind()
    "_ZN7MPPatch13connect_patchEjPK8sockaddri@12",                // connect()
    "_ZN7MPPatch12sendTo_patchEjPKciiPK8sockaddri@24",            // sendTo()
    "_ZN7MPPatch21getAdaptersInfo_patchEP16_IP_ADAPTER_INFOPm@8", // getAdapersInfo()
    "_ZN7MPPatch19getHostByName_patchEPKc@4"                      // getHostByName()
};
const QStringList patch_library_runtime_dependencies = {
    patch_library_file,
    "libgcc_s_dw2-1.dll",
    "libstdc++-6.dll",
    "libwinpthread-1.dll",
    "Qt5Core.dll",
    "Qt5Network.dll"
};

const QString patch_configuration_file = QString(patch_library_name).toLower() + ".cfg";
constexpr char patch_configuration_network[] = "Network";
constexpr char patch_configuration_network_interface_index[] = "InterfaceIndex";

// Currently only applies for dedicated server.
constexpr char patch_network_lobbyserver_address[] = "216.98.48.56";
constexpr unsigned short patch_network_lobbyserver_port = 3035;

// Change game_id sent to ubisoft for Steam and Uplay editions.
constexpr char patch_game_id[] = "2c66b725e7fb0697c0595397a14b0bc8";

const QList<FileEntry> files = {
    {
        "Dunia.dll",
        {
            { // Retail (GOG is identical).
                "7b82f20088e5c046a99fcaed65dc8bbb8202fd622a69737be83e00686b172d53",
                "dde6c2bcd9e333ca68a6c40ab6da8446a53b0721a03700b7302c3d40054cac8a",
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
            { // Steam.
                "6353936a54aa841350bb30ff005727859cdef1aa10c209209b220b399e862765",
                "9eec1b6b2eee87da3a6ec97760359e9afb58fadad506fffbe0db8fea7849d511",
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
                "0dd4f7074debda053b655b6ecde3785bd4e228a2ff15e35836aa300e89258916",
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
            { // Retail (GOG is identical).
                "c175d2a1918d3e6d4120a2f6e6254bd04907a5ec10d3c1dfac28100d6fbf9ace",
                "64c07c4d04d1180f3ff497a6ecc1ce8a48b021e201c640242db32e9a98cb5d1a",
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
                    { 0x00c43ffd, 1 }  // connect()
                }
            },
            { // Steam (R2 Server is identical).
                "5cd5d7b6e6e0b1d25843fdee3e9a743ed10030e89ee109b121109f4a146a062e",
                "1c8152bfa04283b699acfe81d09a023fd19b0c2c73fe20e6d83b15e24c946653",
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
                    { 0x004eca95, "EB" } // change JZ (74) to JMP (EB)
                }
            },
            { // Uplay
                "948a8626276a6689c0125f2355b6a820c104f20dee36977973b39964a82f2703",
                "bf97b551b3219d5223dd9fdab961a3821edb2e3fbc759857695f661ebca0a1d7",
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
                    { 0x00c465bd, 1 }  // connect()
                }
            }
        }
    }
};

#endif // GLOBAL_H
