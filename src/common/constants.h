#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QString>
#include <QStringList>
#include <QList>

#include "entry.h"

namespace Constants {
    const QString app_name = "FC2MPPatcher";
    const QString app_version = "0.0.3";
    const QString app_configuration_file = app_name.toLower() + ".ini";

    const QString settings_install_directory = "install_directory";
    const QString settings_interface_index = "interface_index";
    const QString settings_group_window = "Window";
    const QString settings_group_window_size = "size";
    const QString settings_group_window_position = "position";
    const QString settings_group_window_isMaximized = "isMaximized";

    const QString game_name = "Far Cry 2";
    const QString game_install_directory = "C:/Program Files/Steam/steamapps/common/Far Cry 2";
    const QString game_executable_directory = "bin";
    const QString game_backup_suffix = "_Original";

    const QString patch_pe_section = ".text";
    const QString patch_library_name = "MPPatch";
    const QString patch_library_file = patch_library_name.toLower() + ".dll";
    const QString patch_library_function_getAdaptersInfo = "?getAdaptersInfo_patch@MPPatch@@SGKPAU_IP_ADAPTER_INFO@@PAK@Z";
    const QString patch_library_function_getHostByName = "?getHostByName_patch@MPPatch@@SGPAUhostent@@PBD@Z";
    const QString patch_library_function_sendTo = "?sendTo_patch@MPPatch@@SGHIPBDHHPBUsockaddr@@H@Z";
    const QString patch_network_configuration_file = "network.cfg";
    const QString patch_network_configuration_address = "Address";
    const QString patch_network_configuration_broadcast = "Broadcast";
    const QString patch_network_configuration_netmask = "Netmask";

    const QStringList patch_library_functions = {
        patch_library_function_getAdaptersInfo,
        patch_library_function_getHostByName,
        patch_library_function_sendTo
    };

    const QList<FileEntry> files = {
        {
            "Dunia.dll",
            {
                { // Retail (GOG is identical).
                    "3905709d89d75b1e1928c94685e70b22b25843fa",
                    "793bbd2009d886137af4696c42d7bd6d4b3a1130",
                    {
                        { 0x10C5BDE2, patch_library_function_getAdaptersInfo },
                        { 0x1001431C, patch_library_function_getHostByName }
                    }
                },
                { // Steam.
                    "b99ea707e1bba5ae964effd2cec94eed6b865739",
                    "ce4470609070d58cbfa4a1bbccbb3998d8442e7e",
                    {
                        { 0x10C6A692, patch_library_function_getAdaptersInfo },
                        { 0x100141FC, patch_library_function_getHostByName },
                        { 0x10013F33, patch_library_function_sendTo }
                    }
                },
            }
        },
        {
            "FC2ServerLauncher.exe",
            {
                { // Retail (GOG is identical).
                    "0949e9dc6fd2934673005a0baff4b30b843ada02",
                    "26694877b9465b1177f28583098f663ebc70fe85",
                    {
                        { 0x00C444A6, patch_library_function_getAdaptersInfo },
                        { 0x00BA4CFC, patch_library_function_getHostByName }
                    }
                },
                { // Steam.
                    "a9ba7b50f1c541254a27299dd471fa0ebc1db02b",
                    "2eb8294400c8b2d4956a48f4066754ef4eac6e7b",
                    {
                        { 0x00C46A66, patch_library_function_getAdaptersInfo },
                        { 0x00BA714C, patch_library_function_getHostByName }
                    }
                }
            }
        }
    };
}

#endif // CONSTANTS_H
