#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QString>
#include <QList>
#include <QMetaType>

enum TargetType {
    RETAIL, // GOG is identical.
    STEAM
};

Q_DECLARE_METATYPE(TargetType)

class FunctionEntry {
public:
    FunctionEntry(unsigned int address, const QString &name) :
        address(address),
        name(name) {}

    unsigned int getAddress() const {
        return address;
    }

    QString getName() const {
        return name;
    }

private:
    unsigned int address;
    QString name;
};

class TargetEntry {
public:
    TargetEntry() {}
    TargetEntry(TargetType type, const QString &fileName, const QString &fileCheckSum, QList<FunctionEntry> functions) :
        type(type),
        fileName(fileName),
        fileCheckSum(fileCheckSum),
        functions(functions) {}

    TargetType getType() const {
        return type;
    }

    QString getFileName() const {
        return fileName;
    }

    QString getFileCheckSum() const {
        return fileCheckSum;
    }

    QList<FunctionEntry> getFunctions() const {
        return functions;
    }

private:
    TargetType type;
    QString fileName;
    QString fileCheckSum;
    QList<FunctionEntry> functions;
};

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
    const QString game_install_directory = "C:/Program Files/Steam/steamapps/common/Far Cry 2";
    const QString game_executable_directory = "bin";

    const QString patch_pe_section = ".text";
    const QString patch_library_name = "MPPatch";
    const QString patch_library_file = patch_library_name + ".dll";
    const QString patch_library_function_getAdaptersInfo = "_Z17GetAdaptersInfoPXP16_IP_ADAPTER_INFOPm@8";
    const QString patch_library_function_getHostbyname = "_Z14getHostbyname2Pc@4";
    const QString patch_network_configuration_file = "network.cfg";
    const QString patch_network_configuration_address = "Address";
    const QString patch_network_configuration_broadcast = "Broadcast";
    const QString patch_network_configuration_netmask = "Netmask";

    const QStringList patch_library_functions = {
        "_Z17GetAdaptersInfoPXP16_IP_ADAPTER_INFOPm@8",
        "_Z14getHostbyname2Pc@4"
    };

    const TargetEntry targets[] = {
        { TargetType::RETAIL,
          "Dunia.dll",
          "3905709d89d75b1e1928c94685e70b22b25843fa",
          {
              { 0x00000000, patch_library_function_getAdaptersInfo },
              { 0x00000000, patch_library_function_getHostbyname }
          }
        },
        { TargetType::RETAIL,
          "FC2ServerLauncher.exe",
          "0949e9dc6fd2934673005a0baff4b30b843ada02",
          {
              { 0x00000000, patch_library_function_getAdaptersInfo },
              { 0x00000000, patch_library_function_getHostbyname }
          }
        },
        { TargetType::STEAM,
          "Dunia.dll",
          "b99ea707e1bba5ae964effd2cec94eed6b865739",
          {
              { 0x10C6A692, patch_library_function_getAdaptersInfo },
              { 0x100141FC, patch_library_function_getHostbyname }
          }
        },
        { TargetType::STEAM,
          "FC2ServerLauncher.exe",
          "a9ba7b50f1c541254a27299dd471fa0ebc1db02b",
          {
              { 0x00C46A66, patch_library_function_getAdaptersInfo },
              { 0x00BA714C, patch_library_function_getHostbyname }
          }
        }
    };

    const QString target_backup_suffix = "_Original";
}

#endif // CONSTANTS_H
