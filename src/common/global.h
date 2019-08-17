#ifndef GLOBAL_H
#define GLOBAL_H

#include <QString>
#include <QStringList>
#include <QList>

#include "entry.h"

constexpr char app_name[] = "FC2MPPatcher";
const QString app_organization = app_name;
constexpr int app_version_major = 0;
constexpr int app_version_minor = 1;
constexpr int app_version_micro = 0;
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
const QString patch_library_pe_section = QString(patch_library_name).toLower();
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
                "b9c679413fd28558d3e14deda8e91c6241e5f8a08003a1938e926a0e6513bb03",
                {
                    0x10C5BDE2, // getAdapersInfo
                    0x1001431C, // getHostByName
                    0x10014053  // sendTo
                }
            },
            { // Steam.
                "6353936a54aa841350bb30ff005727859cdef1aa10c209209b220b399e862765",
                "8c910c8077b8a37acb65f7d7c856265275c794de772b40e7765ab4d2a7a806ad",
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
                "c4d8ea74d7e29f3a0fa30c5bae177d27c3792393535dd0cc8ff4364968ce84e4",
                {
                    0x00C444A6, // getAdapersInfo
                    0x00BA4CFC, // getHostByName
                    0x00BA4A33  // sendTo
                }
            },
            { // Steam.
                "5cd5d7b6e6e0b1d25843fdee3e9a743ed10030e89ee109b121109f4a146a062e",
                "ea612e4e3c46ca477058509be31cf6f34b99b4ed1bb03fbe28e53f9679b047b4",
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
