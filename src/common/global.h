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
constexpr int app_version_micro = 4;
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

constexpr char patch_pe_section[] = ".text";
constexpr char patch_library_name[] = "MPPatch";
const QString patch_library_file = QString(patch_library_name).toLower() + ".dll";
const QString patch_library_pe_section = QString(patch_library_name).toLower();
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
    "Qt5Core.dll",
    "Qt5Network.dll"
};

const QString patch_configuration_file = QString(patch_library_name).toLower() + ".cfg";
constexpr char patch_configuration_network[] = "Network";
constexpr char patch_configuration_network_interface_index[] = "InterfaceIndex";

// Currently only applies for dedicated server.
constexpr char patch_network_lobbyserver_address[] = "216.98.48.56";
constexpr unsigned short patch_network_lobbyserver_port = 3035;

const QList<FileEntry> files = {
    {
        "Dunia.dll",
        {
            { // Retail (GOG is identical).
                "7b82f20088e5c046a99fcaed65dc8bbb8202fd622a69737be83e00686b172d53",
                "9d32381e05449c845e5fa28245f38ee40ea8154c050dcaee45014547a7703921",
                {
                    { 0x10c5bde2, 0 }, // getAdapersInfo()
                    { 0x1001431c, 1 }, // getHostByName()
                    { 0x10014053, 2 }  // sendTo()
                }
            },
            { // Steam.
                "6353936a54aa841350bb30ff005727859cdef1aa10c209209b220b399e862765",
                "f28878931b7d6c804647545377b064ccf560ee0c6f38cfaf9668373f851114c7",
                {
                    { 0x10c6a692, 0 }, // getAdapersInfo()
                    { 0x100141fc, 1 }, // getHostByName()
                    { 0x10013f33, 2 }  // sendTo()
                }
            }
        }
    },
    {
        "FC2ServerLauncher.exe",
        {
            { // Retail (GOG is identical).
                "c175d2a1918d3e6d4120a2f6e6254bd04907a5ec10d3c1dfac28100d6fbf9ace",
                "6ea189c2a6c0834ace7314457d485c4e610ab74ee7335fbc24d55560865b36c4",
                {
                    { 0x00c444a6, 0 }, // getAdapersInfo()
                    { 0x00ba4cfc, 1 }, // getHostByName()
                    { 0x00ba4a33, 2 }, // sendTo()
                    { 0x00c43ffd, 3 }  // connect()
                }
            },
            { // Steam (R2 is identical).
                "5cd5d7b6e6e0b1d25843fdee3e9a743ed10030e89ee109b121109f4a146a062e",
                "a0b5ec1dce04094e821d466185efc11716198f24d965222a3fe4a0c3f3f6471b",
                {
                    { 0x00c46a66, 0 }, // getAdapersInfo()
                    { 0x00ba714c, 1 }, // getHostByName()
                    { 0x00ba6e83, 2 }, // sendTo()
                    { 0x00c465bd, 3 }  // connect()
                }
            }
        }
    }
};

#endif // GLOBAL_H
