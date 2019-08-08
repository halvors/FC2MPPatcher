#ifndef GLOBAL_H
#define GLOBAL_H

#include <QString>
#include <QStringList>
#include <QList>

#include "entry.h"

constexpr char app_name[] = "FC2MPPatcher";
constexpr int app_version_major = 0;
constexpr int app_version_minor = 0;
constexpr int app_version_micro = 5;
const QString app_configuration_file = QString(app_name).toLower() + ".ini";

constexpr char settings_install_directory[] = "install_directory";
constexpr char settings_interface_index[] = "interface_index";
constexpr char settings_group_window[] = "Window";
constexpr char settings_group_window_size[] = "size";
constexpr char settings_group_window_position[] = "position";
constexpr char settings_group_window_isMaximized[] = "isMaximized";

constexpr char game_name[] = "Far Cry 2";
constexpr char game_publisher[] = "Ubisoft";
constexpr char game_executable_directory[] = "bin";
constexpr char game_backup_file_suffix[] = "_Original";

constexpr char game_steam_name[] = "Steam";
constexpr char game_steam_publisher[] = "Valve";
constexpr char game_steam_app_directory[] = "steamapps";
constexpr char game_steam_app_directory_common[] = "common";
constexpr int game_steam_app_id = 19900;
constexpr char game_steam_app_manifest_name[] = "appmanifest";
constexpr char game_steam_app_manifest_suffix[] = "acf";
constexpr char game_steam_app_manifest_key[] = "installdir";
constexpr char game_steam_app_library[] = "libraryfolders.vdf";

constexpr char patch_pe_section[] = ".text";
constexpr char patch_library_name[] = "MPPatch";
const QString patch_library_file = QString(patch_library_name).toLower() + ".dll";
const QStringList patch_library_functions = {
    "_ZN7MPPatch21getAdaptersInfo_patchEP16_IP_ADAPTER_INFOPm@8", // getAdapersInfo
    "_ZN7MPPatch19getHostByName_patchEPKc@4",                     // getHostByName
    "_ZN7MPPatch12sendTo_patchEjPKciiPK8sockaddri@24"             // sendTo
};
const QStringList patch_library_runtime_dependencies = {
    patch_library_file,
    "libgcc_s_dw2-1.dll",
    "libstdc++-6.dll",
    "libwinpthread-1.dll",
    "Qt5Core.dll"
};
constexpr char patch_network_configuration_file[] = "network.cfg";
constexpr char patch_network_configuration_address[] = "Address";
constexpr char patch_network_configuration_broadcast[] = "Broadcast";
constexpr char patch_network_configuration_netmask[] = "Netmask";

const QList<FileEntry> files = {
    {
        "Dunia.dll",
        {
            { // Retail (GOG is identical).
                "7b82f20088e5c046a99fcaed65dc8bbb8202fd622a69737be83e00686b172d53",
                "7a56f15deab0fde10078ade6fa0d284f8184e7528f2377bfef453cc8f85db86d",
                {
                    0x10C5BDE2, // getAdapersInfo
                    0x1001431C, // getHostByName
                    0x10014053  // sendTo
                }
            },
            { // Steam.
                "6353936a54aa841350bb30ff005727859cdef1aa10c209209b220b399e862765",
                "638fba83ae75cc18ba3d170ce2f61a8b6d3d2a437df9e91461d8cb0c03c7a88c",
                {
                    0x10C6A692, // getAdapersInfo
                    0x100141FC, // getHostByName
                    0x10013F33  // sendTo
                }
            },
        }
    },
    {
        "FC2ServerLauncher.exe",
        {
            { // Retail (GOG is identical).
                "c175d2a1918d3e6d4120a2f6e6254bd04907a5ec10d3c1dfac28100d6fbf9ace",
                "69aa665ca67ef15a6c1e5d6b202c20014f74984d6d279f7d335a68da2c1ee5c6",
                {
                    0x00C444A6, // getAdapersInfo
                    0x00BA4CFC, // getHostByName
                    0x00BA4A33  // sendTo
                }
            },
            { // Steam.
                "5cd5d7b6e6e0b1d25843fdee3e9a743ed10030e89ee109b121109f4a146a062e",
                "ea611c01716a0d07a47abf93f7a2d986c19eb061c2d774f234f686b6f687337c",
                {
                    0x00C46A66, // getAdapersInfo
                    0x00BA714C, // getHostByName
                    0x00BA6E83  // sendTo
                }
            }
        }
    }
};

#endif // GLOBAL_H
