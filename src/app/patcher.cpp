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
    QList<FileEntry> files = Constants::files;
    int count = 0;

    for (const FileEntry &file : files) {
        for (const TargetEntry &target : file.getTargets()) {
            // Check if target file is patched.
            if (File::isValid(path, file, target, true)) {
                count++;

                break;
            }
        }
    }

    return count == files.length();
}

void Patcher::copyFiles(const QString &path)
{
    // Files to copy.
    QStringList files = {
        "mppatch.dll",
        "QtCore.dll"
    };

    for (const QString &file : files) {
        QString fileName = path + file;

        if (!QFile::exists(fileName)) {
            qDebug() << "Copying file" << file << "to" << fileName;

            QFile::copy(file, fileName);
        }
    }
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

    return File::isValid(path, file, target, true);
}

bool Patcher::patch(QWidget* parent, const QString &path)
{
    QList<FileEntry> files = Constants::files;
    int count = 0;

    // Scanning for valid files to start patching.
    for (const FileEntry &file : files) {
        for (const TargetEntry &target : file.getTargets()) {
            // Validate target file against stored checksum.
            if (File::isValid(path, file, target, false)) {
                // Backup original file.
                File::backup(path, file);

                // Patch target file.
                if (!patchFile(path, file, target)) {
                    QMessageBox::warning(parent, "Warning", "Invalid checksum for patched file " + file.getFileName() + ", aborting!");
                    undoPatch(path);

                    return false;
                }

                count++;
            }
        }
    }

    // Something is not right, reverting back to backup files.
    if (count < files.length()) {
        QMessageBox::warning(parent, "Warning", "Not all files where patched, files have been restored, please try patching again.");
        undoPatch(path);

        return false;
    }

    // Copy needed libraries.
    copyFiles(path);

    return true;
}

void Patcher::undoPatch(const QString &path) {
    // Scanning for valid files to start patching.
    for (const FileEntry &file : Constants::files) {
        File::restore(path, file);

        // TODO: Delete files?
    }
}

void Patcher::generateNetworkConfigFile(const QString &path, const QNetworkAddressEntry &address)
{
    QSettings settings(path + Constants::patch_network_configuration_file, QSettings::IniFormat);

    settings.beginGroup(Constants::patch_library_name);
        settings.setValue(Constants::patch_network_configuration_address, address.ip().toString());
        settings.setValue(Constants::patch_network_configuration_broadcast, address.broadcast().toString());
        settings.setValue(Constants::patch_network_configuration_netmask, address.netmask().toString());
    settings.endGroup();

    qDebug() << "Generated network configuration, saved to:" << path + Constants::patch_network_configuration_file;
}
