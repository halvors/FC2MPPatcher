#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QString>

namespace Constants {
    const QString app_name = "FC2MPPatcher";
    const QString app_version = "0.0.1";
    const QString app_configuration_file = app_name.toLower() + ".ini";

    const QString defualt_install_dir = "C:/Program Files/Steam/steamapps/common/Far Cry 2";
    const QString target_executable_client = "Dunia.dll";
    const QString target_executable_server = "FC2ServerLauncher.exe";

    const QString library_name = "fix.dll";
    const QString library_function_getAdaptersInfo = "_Z17GetAdaptersInfoPXP16_IP_ADAPTER_INFOPm@8";
    const QString library_function_getHostbyname = "_Z14getHostbyname2Pc@4";

    const QString network_configuration_file = "network.cfg";

    const QString settings_install_dir = "install_dir";
    const QString settings_interface_index = "interface_index";
}

#endif // CONSTANTS_H
