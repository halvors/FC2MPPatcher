#ifndef GLOBAL_H
#define GLOBAL_H

#include <QString>
#include <QStringList>
#include <QList>

#include "entry.h"

// Set true for debugging mode without checksum verification.
#define DEBUG false

constexpr char app_name[] = "FC2MPPatcher";
const QString app_organization = app_name;
constexpr int app_version_major = 0;
constexpr int app_version_minor = 1;
constexpr int app_version_micro = 5;
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
    "_ZN7MPPatch10bind_patchEjPK8sockaddri@12",                  // bind()
    "_ZN7MPPatch13connect_patchEjPK8sockaddri@12",               // connect()
    "_ZN7MPPatch12sendTo_patchEjPKciiPK8sockaddri@24",           // sendTo()
    "_ZN7MPPatch21getAdaptersInfo_patchEP16_IP_ADAPTER_INFOPm@8" // getAdapersInfo()
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
                "db4ba60a1d2520aa89c4371ebb89df231a69b3d13793eec28dc0b4d1e79da0eb",
                {
                    { 0x1001088e, 0 }, // bind()
                    { 0x10213d18, 0 }, // bind()
                    { 0x10c4e97a, 0 }, // bind()
                    { 0x10cb9a8c, 0 }, // bind()
                    { 0x10cb9ad4, 0 }, // bind()
                    { 0x10014053, 2 }  // sendTo()
                }
            },
            { // Steam.
                "6353936a54aa841350bb30ff005727859cdef1aa10c209209b220b399e862765",
                "bc56e2e248118365c8bedc68f3998b57c63de606c91c29e9c911cedecbfeacdf",
                {
                    { 0x1001076e, 0 }, // bind()
                    { 0x102161a8, 0 }, // bind()
                    { 0x10c5d10a, 0 }, // bind()
                    { 0x10cf289c, 0 }, // bind()
                    { 0x10cf28e4, 0 }, // bind()
                    { 0x10013f33, 2 }, // sendTo()
                    { 0x10c6a692, 3 }  // getAdapersInfo()
                }
            }
        }
    },
    {
        "FC2ServerLauncher.exe",
        {
            { // Retail (GOG is identical).
                "c175d2a1918d3e6d4120a2f6e6254bd04907a5ec10d3c1dfac28100d6fbf9ace",
                "824fec9e57014f29f7c21fb36a19af33221a8bf37d9a1f85603b2b58df8a976a",
                {
                    { 0x00425fc4, 0 }, // bind()
                    { 0x0042600b, 0 }, // bind()
                    { 0x004c9d2a, 0 }, // bind()
                    { 0x00ba126e, 0 }, // bind()
                    { 0x00e83eda, 0 }, // bind()
                    { 0x00c43ffd, 1 }, // connect()
                    { 0x00ba4a33, 2 }  // sendTo()
                }
            },
            { // Steam (R2 Server is identical).
                "5cd5d7b6e6e0b1d25843fdee3e9a743ed10030e89ee109b121109f4a146a062e",
                "6b3c243f414e9cc9142b52c143cce2bc963fc82ab60b8bbf041c87266a64ee9c",
                {
                    { 0x004263d4, 0 }, // bind()
                    { 0x0042641b, 0 }, // bind()
                    { 0x004c9d2a, 0 }, // bind()
                    { 0x00ba36be, 0 }, // bind()
                    { 0x00e85ffa, 0 }, // bind()
                    { 0x00c465bd, 1 }, // connect()
                    { 0x00ba6e83, 2 }  // sendTo()
                }
            }
        }
    }
};

#endif // GLOBAL_H
