#ifndef DEFS_H
#define DEFS_H

#include <QString>
#include <QStringList>
#include <QByteArray>
#include <QList>

#include "entry.h"

// Set true for debugging mode without checksum verification.
#define DEBUG_MODE true

constexpr char app_name[] = "FC2MPPatcher";
const QString app_organization = app_name;
const QString app_configuration_file = QString(app_name).toLower() + ".ini";

constexpr char settings_install_directory[] = "installDirectory";
constexpr char settings_network_interface[] = "networkInterface";
constexpr char settings_group_window[] = "Window";
constexpr char settings_group_window_size[] = "size";
constexpr char settings_group_window_position[] = "position";
constexpr char settings_group_window_isMaximized[] = "isMaximized";

constexpr char game_name[] = "Far Cry 2";
constexpr char game_publisher[] = "Ubisoft";
constexpr char game_executable_directory[] = "bin";
constexpr char game_backup_prefix[] = ".";

constexpr char game_steam_name[] = "Steam";
constexpr char game_steam_publisher[] = "Valve";
constexpr char game_steam_app_directory[] = "steamapps";
constexpr char game_steam_app_directory_common[] = "common";
constexpr uint16_t game_steam_app_id = 19900;
constexpr char game_steam_app_manifest_name[] = "appmanifest";
constexpr char game_steam_app_manifest_suffix[] = "acf";
constexpr char game_steam_app_manifest_key[] = "installdir";
constexpr char game_steam_app_library[] = "libraryfolders.vdf";

constexpr char pe_patch_rdata_section[] = ".rdata_p";
constexpr char pe_patch_text_section[] = ".text_p";

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
constexpr char patch_configuration_network_interface[] = "Interface";
const QStringList patch_library_runtime_dependencies = {
    patch_library_file,
    "libgcc_s_dw2-1.dll",
    "libstdc++-6.dll",
    "libwinpthread-1.dll",
    "Qt5Core.dll",
    "Qt5Network.dll"
};

#endif // DEFS_H
