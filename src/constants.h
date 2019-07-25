#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QString>
#include <QHash>

typedef QHash<QString, unsigned int> FunctionMap;
typedef QHash<QString, FunctionMap> TargetMap;

namespace Constants {
    const QString app_name = "FC2MPPatcher";
    const QString app_version = "0.0.2";
    const QString app_configuration_file = app_name.toLower() + ".ini";

    const QString settings_install_directory = "install_directory";
    const QString settings_interface_index = "interface_index";
    const QString settings_group_window = "Window";
    const QString settings_group_window_size = "size";
    const QString settings_group_window_position = "position";
    const QString settings_group_window_isMaximized = "isMaximized";

    const QString install_directory = { "C:/Program Files/Steam/steamapps/common/Far Cry 2" };
    const QString executable_directory = "bin";

    const QString patch_library_name = "MPPatch";
    const QString patch_library_file = patch_library_name + ".dll";
    const QString patch_library_function_getAdaptersInfo = "_Z17GetAdaptersInfoPXP16_IP_ADAPTER_INFOPm@8";
    const QString patch_library_function_getHostbyname = "_Z14getHostbyname2Pc@4";
    const QString patch_pe_section = ".text";
    const QString patch_network_configuration_file = "network.cfg";

    const TargetMap targets = {
        { "Dunia.dll",
            {
                { patch_library_function_getAdaptersInfo, 0x10C6A692 },
                { patch_library_function_getHostbyname, 0x100141FC }
            }
        },
        { "FC2ServerLauncher.exe",
            {
                { patch_library_function_getAdaptersInfo, 0 },
                { patch_library_function_getHostbyname, 0 },
                { patch_library_function_getAdaptersInfo, 0 },
                { patch_library_function_getHostbyname, 0 }
            }
        }
    };
}

#endif // CONSTANTS_H
