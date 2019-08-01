#include <QFile>
#include <QCryptographicHash>
#include <QMessageBox>
#include <QSettings>

#include "patcher.h"
#include "constants.h"
#include "file.h"
#include "pefile.h"

bool Patcher::isPatched(const QString &path)
{
    const QList<FileEntry> &files = Constants::files;
    int length = files.length();
    bool* patched = new bool[length];
    int index = 0;

    for (const FileEntry &file : files) {
        for (const TargetEntry &target : file.getTargets()) {
            // Check if target is patched.
            if (File::isValid(path, file, target, true)) {
                patched[index] = true;

                break;
            }
        }

        index++;
    }

    // Verify that all files is patched.
    for (int i = 0; i < length; i++) {
        if (!patched[i]) {
            return false;
        }
    }

    return true;
}


bool Patcher::patchFile(const QString &path, const FileEntry &file, const TargetEntry &target)
{
    QString fileName = path + file.getFileName();

    qDebug() << "Patching:" << fileName;

    // Create PeFile instance for this particular target.
    PeFile* peFile = new PeFile(fileName);

    // Apply PE and binary patches.
    peFile->apply(Constants::patch_library_name, Constants::patch_library_file, Constants::patch_library_functions, target.getFunctions(), Constants::patch_pe_section);

    // Write PE to file.
    peFile->write();

    delete peFile;

    return target.getFileCheckSumPatched() == File::getCheckSum(fileName);
}

bool Patcher::patch(QWidget* parent, const QString &path)
{
    // Scanning for valid files to start patching.
    for (const FileEntry &file : Constants::files) {
        for (const TargetEntry &target : file.getTargets()) {
            // Validate target file against stored checksum.
            if (File::isValid(path, file, target, false)) {
                // Backup original file.
                File::backup(path, file);

                // Patch target file.
                if (!patchFile(path, file, target)) {
                    QMessageBox::warning(parent, "Warning", "Invalid checksum for patched file " + file.getFileName() + ", aborting!");

                    //return false;
                }
            }
        }
    }

    return true;
}

void Patcher::generateNetworkConfigFile(const QString &path, const QNetworkAddressEntry &address)
{
    QSettings settings(path + Constants::patch_network_configuration_file, QSettings::IniFormat);

    settings.beginGroup(Constants::patch_library_name);
        settings.setValue(Constants::patch_network_configuration_address, address.ip().toString());
        settings.setValue(Constants::patch_network_configuration_broadcast, address.broadcast().toString());
        settings.setValue(Constants::patch_network_configuration_netmask, address.netmask().toString());
    settings.endGroup();
}
