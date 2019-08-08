#include <QFile>
#include <QCryptographicHash>
#include <QMessageBox>
#include <QSettings>

#include "patcher.h"
#include "constants.h"
#include "fileutils.h"
#include "pefile.h"

bool Patcher::isPatched(const QDir &path)
{
    QList<FileEntry> files = Constants::files;
    int count = 0;

    for (const FileEntry &file : files) {
        for (const TargetEntry &target : file.getTargets()) {
            // Check if target file is patched.
            if (FileUtils::isValid(path, file, target, true)) {
                count++;

                break;
            }
        }
    }

    return count == files.length();
}

void Patcher::copyFiles(const QDir &path)
{
    bool success = false;

    for (const QString &fileName : Constants::patch_library_runtime_dependencies) {
        QFile sourceFile = QFile(fileName);
        QFile destinationFile = path.filePath(fileName);

        if (!destinationFile.exists() || destinationFile.remove()) {
            success = sourceFile.copy(destinationFile.fileName());
        }
    }

    if (success) {
        qDebug() << "Copying runtime dependencies.";
    } else {
        qDebug() << "Error: Could not copy runtime dependencies, missing from application directory.";
    }
}

bool Patcher::patchFile(const QDir &path, const FileEntry &fileEntry, const TargetEntry &target)
{
    QFile file = path.filePath(fileEntry.getName());

    qDebug() << "Patching:" << file.fileName();

    // Create PeFile instance for this particular target.
    PeFile* peFile = new PeFile(file);

    // Apply PE and binary patches.
    peFile->apply(Constants::patch_library_name, Constants::patch_library_file, Constants::patch_library_functions, target.getAddresses(), Constants::patch_pe_section);

    // Write PE to file.
    peFile->write();

    delete peFile;

    return FileUtils::isValid(path, fileEntry, target, true);
}

bool Patcher::patch(QWidget* parent, const QDir &path)
{
    QList<FileEntry> files = Constants::files;
    int count = 0;

    // Scanning for valid files to start patching.
    for (const FileEntry &file : files) {
        for (const TargetEntry &target : file.getTargets()) {
            // Validate target file against stored checksum.
            if (FileUtils::isValid(path, file, target, false)) {
                // Backup original file.
                FileUtils::backup(path, file);

                // Patch target file.
                if (!patchFile(path, file, target)) {
                    QMessageBox::warning(parent, "Warning", QString("Invalid checksum for patched file %1, aborting!").arg(file.getName()));
                    //undoPatch(path);

                    return false;
                }

                count++;
            }
        }
    }

    // Something is not right, reverting back to backup files.
    if (count < files.length()) {
        QMessageBox::warning(parent, "Warning", "Not all files where patched, files have been restored, please try patching again.");
        //undoPatch(path);

        return false;
    }

    // Copy needed libraries.
    copyFiles(path);

    return true;
}

void Patcher::undoPatch(const QDir &path) {
    // Restore patched files.
    for (const FileEntry &file : Constants::files) {
        FileUtils::restore(path, file);
    }

    // TODO: Delete files (mppatch.dll, dlls and _Original)?
}

void Patcher::generateNetworkConfigFile(const QDir &path, const QNetworkAddressEntry &address)
{
    QFile file = path.filePath(Constants::patch_network_configuration_file);
    QSettings settings(file.fileName(), QSettings::IniFormat);

    settings.beginGroup(Constants::patch_library_name);
        settings.setValue(Constants::patch_network_configuration_address, address.ip().toString());
        settings.setValue(Constants::patch_network_configuration_broadcast, address.broadcast().toString());
        settings.setValue(Constants::patch_network_configuration_netmask, address.netmask().toString());
    settings.endGroup();

    qDebug() << "Generated network configuration, saved to:" << file.fileName();
}
