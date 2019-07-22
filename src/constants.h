#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QString>

namespace Constants {
    const QString app_name = "FC2MPPatcher";
    const QString app_version = "0.0.1";

    const QString defualt_install_dir = "C:/Program Files/Steam/steamapps/common/Far Cry 2";
    const QString target_client = "Dunia.dll";
    const QString target_server = "FC2ServerLauncher.exe";

    const QString library_name = "fix.dll";
    const QString library_function_getAdaptersInfo = "_Z17GetAdaptersInfoPXP16_IP_ADAPTER_INFOPm@8";
    const QString library_function_getHostbyname = "_Z14getHostbyname2Pc@4";
}

#endif // CONSTANTS_H
