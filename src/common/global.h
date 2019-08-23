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
constexpr int app_version_micro = 2;
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

constexpr char patch_name[] = "MPPatch";
constexpr char patch_pe_section[] = ".text";
const QString patch_library_file = QString(patch_name).toLower() + ".dll";
const QString patch_library_pe_section = QString(patch_name).toLower();
const QStringList patch_library_functions = {
    "_ZN7MPPatch21getAdaptersInfo_patchEP16_IP_ADAPTER_INFOPm@8", // getAdapersInfo()
    "_ZN7MPPatch19getHostByName_patchEPKc@4",                     // getHostByName()
    "_ZN7MPPatch12sendTo_patchEjPKciiPK8sockaddri@24",            // sendTo()
    "_ZN7MPPatch13connect_patchEjPK8sockaddri@12"                 // connect()
};
const QStringList patch_library_runtime_dependencies = {
    patch_library_file,
    "libgcc_s_dw2-1.dll",
    "libstdc++-6.dll",
    "libwinpthread-1.dll",
    "Qt5Core.dll"
};

const QString patch_configuration_file = QString(patch_name).toLower() + ".cfg";
constexpr char patch_configuration_network[] = "Network";
constexpr char patch_configuration_network_interface_index[] = "InterfaceIndex";
//constexpr char patch_configuration_network_address[] = "Address";
//constexpr char patch_configuration_network_broadcast[] = "Broadcast";
//constexpr char patch_configuration_network_netmask[] = "Netmask";

// Currently only applies for dedicated server.
constexpr char patch_network_lobbyserver_address[] = "216.98.48.56";
constexpr unsigned short patch_network_lobbyserver_port = 3035;

const QList<FileEntry> files = {
    {
        "Dunia.dll",
        {
            { // Retail (GOG is identical).
                "7b82f20088e5c046a99fcaed65dc8bbb8202fd622a69737be83e00686b172d53",
                "1fc060b4c1abd28a91d8b9442aa46b72aa28433f6c02611c9ee9d520024bce0b",
                {
                    0x10c5bde2, // getAdapersInfo()
                    0x1001431c, // getHostByName()
                    0x10014053, // sendTo()
                    0           // connect()
                }
            },
            { // Steam.
                "6353936a54aa841350bb30ff005727859cdef1aa10c209209b220b399e862765",
                "64c82e039ab72b30563d94e2d8072a946ef9733302c99c8cf20e3b8503fbafec",
                {
                    0x10c6a692, // getAdapersInfo()
                    0x100141fc, // getHostByName()
                    0x10013f33, // sendTo()
                    0           // connect()
                }
            }
        }

    },
    {
        "FC2ServerLauncher.exe",
        {
            { // Retail (GOG is identical).
                "c175d2a1918d3e6d4120a2f6e6254bd04907a5ec10d3c1dfac28100d6fbf9ace",
                "3ced2ee5188b05e4554fec738bb833d639698b1c3473008c8da2678ac66c4433",
                {
                    0x00c444a6, // getAdapersInfo()
                    0x00ba4cfc, // getHostByName()
                    0x00ba4a33, // sendTo()
                    0x00c43ffd  // connect()
                }
            },
            { // Steam.
                "5cd5d7b6e6e0b1d25843fdee3e9a743ed10030e89ee109b121109f4a146a062e",
                "2db5a3c609410eae86b6d6c0fb33f4cbb65196b4fc1915d2a19f150cde238252",
                {
                    0x00c46a66, // getAdapersInfo()
                    0x00ba714c, // getHostByName()
                    0x00ba6e83, // sendTo()
                    0x00c465bd  // connect()
                }
            }
        }
    }
};

#endif // GLOBAL_H
