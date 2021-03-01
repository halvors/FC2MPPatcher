#include <QDebug>

#include "console.h"
#include "netutils.h"
#include "installdir.h"
#include "defs.h"
#include "patcher.h"

Console::Console(const QString &installDir, const QString &interfaceName, QObject *parent) :
    QObject(parent)
{
    settings = new QSettings(app_configuration_file, QSettings::IniFormat, this);
    loadSettings();

    if (InstallDir::isGameDirectory(installDir))
        this->installDir = installDir;

    this->interface = NetUtils::findValidInterface(interfaceName);
}

Console::~Console()
{
    delete settings;
}

bool Console::exec()
{
    if (installDir.isEmpty()) {
        qDebug().noquote() << tr("No %1 installation directory specified, attempting auto detection...").arg(game_name);

        installDir = InstallDir::findInstallDirectories().first();

        if (installDir.isEmpty()) {
            qDebug().noquote() << tr("Error: Auto detection failed, aborting!").arg(game_name);

            return false;
        }
    } else if (!InstallDir::isGameDirectory(installDir)) {
        qDebug().noquote() << QString("Error: Directory \"%1\" does not contain a %2 installation, aborting!").arg(installDir, game_name);

        return false;
    }

    QDir dir = installDir;
    dir.cd(game_executable_directory);

    // Only show option to patch if not already patched.
    if (Patcher::isPatched(dir.absolutePath()) == Patcher::INSTALLED) {
        Patcher::undoPatch(dir);

        qDebug().noquote() << tr("Successfully uninstalled patch.");
    } else {
        // Apply patch to files, if successful continue.
        if (Patcher::patch(dir)) {
            Patcher::generateConfigurationFile(dir, interface);

            qDebug().noquote() << tr("Successfully installed patch.");
        } else {
            qDebug().noquote() << tr("Error: Something went wrong while applying patch, aborting!");
        }
    }

    switch (Patcher::isPatched(dir.absolutePath())) {
    case Patcher::INSTALLED:
        Patcher::undoPatch(dir);
        qDebug().noquote() << tr("Successfully uninstalled patch.");
        break;

    case Patcher::UPGRADABLE:
        Patcher::undoPatch(dir);
        [[fallthrough]];

    default:
        // Apply patch to files, if successful continue.
        if (Patcher::patch(dir)) {
            // Generate network configuration.
            Patcher::generateConfigurationFile(dir, interface);

            qDebug().noquote() << tr("Successfully installed patch.");
        } else {
            qDebug().noquote() << tr("Error: Something went wrong while applying patch, aborting!");
        }
        break;
    }

    saveSettings();

    return true;
}

void Console::loadSettings()
{
    QDir dir = settings->value(settings_install_directory).toString();

    if (!InstallDir::isGameDirectory(dir))
        return;

    // If we're in executable directory, cd up to install directory.
    if (dir.dirName() == game_executable_directory)
        dir.cdUp();

    installDir = dir.absolutePath();
}

void Console::saveSettings()
{
    if (InstallDir::isGameDirectory(installDir))
        settings->setValue(settings_install_directory, installDir);
}
