#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QString>
#include <QHash>

typedef QHash<QString, unsigned int> FunctionMap;
typedef QHash<QString, FunctionMap> TargetMap;

namespace Constants {
    const QString app_name = "FC2MPPatcher";
    const QString app_version = "0.0.1";
    const QString app_configuration_file = app_name.toLower() + ".ini";

    const QString settings_install_directory = "install_directory";
    const QString settings_interface_index = "interface_index";

    const QString install_directory = { "C:/Program Files/Steam/steamapps/common/Far Cry 2" };
    const QString executable_directory = "bin";

    const QString patch_name = "MPPatch";
    const QString library_name = patch_name + ".dll";
    const QString library_function_getAdaptersInfo = "_Z17GetAdaptersInfoPXP16_IP_ADAPTER_INFOPm@8";
    const QString library_function_getHostbyname = "_Z14getHostbyname2Pc@4";

    const QString network_configuration_file = "network.cfg";

    const TargetMap targets = {
        { "Dunia.dll",
            {
                { library_function_getAdaptersInfo, 0x10C6A692 },
                { library_function_getHostbyname, 0x100141FC }
            }
        },
        { "FC2ServerLauncher.exe",
            {
                { library_function_getAdaptersInfo, 0 },
                { library_function_getHostbyname, 0 },
                { library_function_getAdaptersInfo, 0 },
                { library_function_getHostbyname, 0 }
            }
        }
    };
}

#endif // CONSTANTS_H
