#ifndef DEFS_H
#define DEFS_H

#include <QString>
#include <QStringList>

// Set true for debugging mode without checksum verification.
#define DEBUG_MODE true
#define USE_DEV_BACKEND true
// App
constexpr char app_name[] = "FC2MPPatcher";
const QString app_organization = app_name;
const QString app_configuration_file = QString(app_organization).toLower() + ".ini";

// App settings
constexpr char settings_install_directory[] = "installDirectory";
constexpr char settings_network_interface[] = "networkInterface";
constexpr char settings_group_window[] = "Window";
constexpr char settings_group_window_size[] = "size";
constexpr char settings_group_window_position[] = "position";
constexpr char settings_group_window_isMaximized[] = "isMaximized";

// Game
constexpr char game_name[] = "Far Cry 2";
constexpr char game_publisher[] = "Ubisoft";
constexpr char game_executable_directory[] = "bin";
constexpr char game_hidden_prefix = '.';
constexpr char game_backup_suffix[] = "_original"; // Temp, leave for some iterations, was changed in 0.1.12.

// Steam
constexpr char game_steam_name[] = "Steam";
constexpr char game_steam_publisher[] = "Valve";
constexpr char game_steam_app_directory[] = "steamapps";
constexpr char game_steam_app_directory_common[] = "common";
constexpr uint16_t game_steam_app_id = 19900;
constexpr char game_steam_app_manifest_name[] = "appmanifest";
constexpr char game_steam_app_manifest_suffix[] = "acf";
constexpr char game_steam_app_manifest_key[] = "installdir";
constexpr char game_steam_app_library[] = "libraryfolders.vdf";

// PE
constexpr char pe_patch_rdata_section[] = ".rdata_p";
constexpr char pe_patch_text_section[] = ".text_p";

// Assembly
constexpr uint8_t asm_nop = 0x90;

// Patch
constexpr char patch_library_name[] = "MPPatch";
const QString patch_library_file = QString(patch_library_name).toLower() + ".dll";

const QString patch_configuration_file = QString(patch_library_name).toLower() + ".cfg";
constexpr char patch_configuration_network[] = "Network";
constexpr char patch_configuration_network_interface[] = "Interface";
const std::string patch_library_runtime_dependencies[] = {
    patch_library_file.toStdString(),
    "libgcc_s_dw2-1.dll",
    "libstdc++-6.dll",
    "libwinpthread-1.dll",
    "Qt5Core.dll",
    "Qt5Network.dll"
};

#endif // DEFS_H
