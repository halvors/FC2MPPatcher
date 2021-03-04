#include <QFile>
#include <QMessageBox>
#include <QSettings>

#include "patcher.h"
#include "defs.h"
#include "patch_defs.h"
#include "fileutils.h"
#include "pefile.h"

Patcher::State Patcher::isPatched(const QString &path)
{
    if (path.isEmpty())
        return INVALID;

    QDir dir = path;
    int numInstalled = 0;
    int numLegacyInstalled = 0;

    // Should we be looking in executable directory instead?
    if (dir.exists() | dir.cd(game_executable_directory)) {
        for (const FileEntry &file : files) {
            for (const TargetEntry &target : file.targets) {
                // Check if target file is patched.
                if (FileUtils::isValid(dir, file, target, true)) {
                    numInstalled++;
                    break;
                }

                if (DEBUG_MODE) {
                    const QFile &backupFile = dir.filePath(QString(file.name).prepend(game_hidden_prefix));

                    // Detect old installations of the patch.
                    if (backupFile.exists()) {
                        numInstalled++;

                        break;
                    }
                }

                const QByteArray &checksum = FileUtils::calculateChecksum(dir.filePath(QString(file.name)));

                // Check if there are legacy versions installed.
                for (QByteArray legacyHash : target.legacyHashEntries) {
                    if (checksum == legacyHash) {
                        numLegacyInstalled++;
                        break;
                    }
                }
            }
        }
    }

    if (numLegacyInstalled > 0)
        return UPGRADABLE;
    else if (numInstalled == files.size())
        return INSTALLED;

    return NOT_INSTALLED;
}

bool Patcher::patch(const QDir &dir, QWidget *widget)
{
    int numPatched = 0;

    // Scanning for valid files to start patching.
    for (const FileEntry &fileEntry : files) {
        QFile file = dir.filePath(fileEntry.name);

        if (!file.exists())
            continue;

        QFileInfo fileInfo = file;

        // What permissions should be set for files.
        QFileDevice::Permissions permissions =
                QFileDevice::WriteOther |
                QFileDevice::ReadOther |
                QFileDevice::WriteGroup |
                QFileDevice::ReadGroup |
                QFileDevice::WriteUser |
                QFileDevice::ReadUser |
                QFileDevice::WriteOwner |
                QFileDevice::ReadOwner;

        // If file is not writable, set the proper permissions.
        if (!fileInfo.permission(permissions)) {
            file.setPermissions(permissions);

            qDebug().noquote() << QT_TR_NOOP(QString("Setting write permissions for protected file %1").arg(fileEntry.name));
        }

        for (const TargetEntry &target : fileEntry.targets) {
            // Validate target file against stored checksum.
            if (FileUtils::isValid(dir, fileEntry, target, false)) {
                // Backup original file.
                FileUtils::replicate(dir, fileEntry, true);

                // Patch target file.
                if (!DEBUG_MODE & !patchFile(dir, fileEntry, target)) {
                    undoPatch(dir);
                    log(QT_TR_NOOP(QString("Invalid checksum for patched file %1, aborting!").arg(fileEntry.name)), widget);

                    return false;
                }

                numPatched++;
            }
        }
    }

    // Copy needed libraries.
    if (!DEBUG_MODE & !copyFiles(dir)) {
        undoPatch(dir);
        log(QT_TR_NOOP(QString("Missing patch files, make sure you unzipped the compressed file, aborting!")), widget);

        return false;
    }

    return numPatched == files.size();
}

void Patcher::undoPatch(const QDir &dir) {
    for (const FileEntry &file : files) {
        // Restore patched files.
        FileUtils::replicate(dir, file, false);

        // Delete backed up game files.
        //QFile::remove(dir.filePath(QString(file.getName()).prepend(game_hidden_prefix)));
        //QFile::remove(dir.filePath(FileUtils::appendToName(dir, file.getName(), game_backup_suffix))); // Temp, leave for some iterations, was changed in 0.1.12.
    }

    // Delete patch library file.
    QFile::remove(dir.filePath(patch_library_file));

    // Remove patch runtime dependencies.
    for (const QString &fileName : patch_library_runtime_dependencies)
        QFile::remove(dir.filePath(fileName));

    // Remove network configuration file.
    QFile::remove(dir.filePath(patch_configuration_file));
}

void Patcher::generateConfigurationFile(const QDir &dir, const QNetworkInterface &interface)
{
    QFile file = dir.filePath(patch_configuration_file);
    QSettings settings(file.fileName(), QSettings::IniFormat);

    settings.beginGroup(patch_configuration_network);
        settings.setValue(patch_configuration_network_interface, interface.name());
    settings.endGroup();

    log(QT_TR_NOOP(QString("Generated configuration, saved to: %1").arg(file.fileName())));
}

bool Patcher::copyFiles(const QDir &dir)
{
    bool success = true;

    for (const QString &fileName : patch_library_runtime_dependencies) {
        QFile sourceFile = fileName;
        QFile destinationFile = dir.filePath(fileName);

        if (!destinationFile.exists() || destinationFile.remove())
            success &= sourceFile.copy(destinationFile.fileName());
    }

    return success;
}

bool Patcher::patchFile(const QDir &dir, const FileEntry &fileEntry, const TargetEntry &target)
{
    QFile file = dir.filePath(fileEntry.name);

    log(QT_TR_NOOP(QString("Patching file %1").arg(file.fileName())));

    // Create PeFile instance for this particular target.
    PeFile *peFile = new PeFile(file);

    // Apply PE and binary patches.
    peFile->apply(patch_library_file, patch_library_functions, target.codeEntries);

    // Write PE to file.
    peFile->write();

    delete peFile;

    if (DEBUG_MODE)
        qDebug().noquote() << QT_TR_NOOP(QString("New checksum for file %1 is \"%2\"").arg(fileEntry.name).arg(FileUtils::calculateChecksum(dir.filePath(fileEntry.name)).constData()));

    return FileUtils::isValid(dir, fileEntry, target, true);
}

void Patcher::log(const QString &msg, QWidget *widget)
{
    if (widget)
        QMessageBox::warning(widget, QT_TR_NOOP(QString("Warning")), msg);
    else
        qDebug().noquote() << msg;
}
