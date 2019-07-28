#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QHash>
#include <QString>
#include <QMetaType>

typedef QHash<QString, unsigned int> FunctionMap;

enum TargetType {
    STEAM,
    RETAIL
};

// Does Steam version differ from retail version?
struct TargetEntry {
    TargetType type;
    const QString fileName;
    const QString checkSum;
    const FunctionMap functions;
};

Q_DECLARE_METATYPE(TargetEntry)

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

    const QString game_name = "Far Cry 2";
    const QString game_install_directory = { "C:/Program Files/Steam/steamapps/common/Far Cry 2" };
    const QString game_executable_directory = "bin";

    const QString patch_library_name = "MPPatch";
    const QString patch_library_file = patch_library_name + ".dll";
    const QString patch_library_function_getAdaptersInfo = "_Z17GetAdaptersInfoPXP16_IP_ADAPTER_INFOPm@8";
    const QString patch_library_function_getHostbyname = "_Z14getHostbyname2Pc@4";
    const QString patch_pe_section = ".text";
    const QString patch_network_configuration_file = "network.cfg";

    const TargetEntry targets[] = {
        { TargetType::STEAM,
          "Dunia.dll",
          "b99ea707e1bba5ae964effd2cec94eed6b865739",
          {
              { patch_library_function_getAdaptersInfo, 0x10C6A692 },
              { patch_library_function_getHostbyname, 0x100141FC }
          }
        },
        { TargetType::STEAM,
          "FC2ServerLauncher.exe",
          "a9ba7b50f1c541254a27299dd471fa0ebc1db02b",
          {
              { patch_library_function_getAdaptersInfo, 0x00495BCB }, // Verify these addresses!!!
              { patch_library_function_getAdaptersInfo, 0x00495BEC },
              { patch_library_function_getAdaptersInfo, 0x00E63AAA },
              { patch_library_function_getAdaptersInfo, 0x00E63ACC },
              { patch_library_function_getHostbyname, 0x00BA7148 }
          }
        }
    };

    const QString target_backup_suffix = "_Original.dll";
}

#endif // CONSTANTS_H
