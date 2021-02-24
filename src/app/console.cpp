#include "console.h"

#include <QDebug>

#include "dirutils.h"
#include "global.h"
#include "patcher.h"

Console::Console(QObject *parent) :
    QObject(parent)
{
    settings = new QSettings(app_configuration_file, QSettings::IniFormat, this);
    loadSettings();
}

Console::~Console()
{
    delete settings;
}

bool Console::exec(const QString &path)
{
    if (path.isEmpty()) {
        qDebug().noquote() << QString("Error: No %1 installation directory specified, aborting!").arg(game_name);
        return false;
    }

    if (!DirUtils::isGameDirectory(path)) {
        qDebug().noquote() << QString("Error: Directory \"%1\" does not contain a %2 installation, aborting!").arg(path, game_name);
        return false;
    }

    QDir dir = path;
    dir.cd(game_executable_directory);

    // Only show option to patch if not already patched.
    if (Patcher::isPatched(dir.absolutePath())) {
        Patcher::undoPatch(dir);

        qDebug().noquote() << QString("Successfully unistalled patch.");
    } else {
        // Apply patch to files, if successful continue.
        if (Patcher::patch(dir)) {
            // Generate network configuration.
            //Patcher::generateConfigurationFile(dir, ui->comboBox_network_interface->currentData().value<QNetworkInterface>());
        }

        qDebug().noquote() << QString("Successfully installed patch.");
    }

    saveSettings();

    return true;
}

void Console::loadSettings()
{
    QDir dir = settings->value(settings_install_directory).toString();

    if (DirUtils::isGameDirectory(dir)) {
        // If we're in executable directory, cd up to install directory.
        if (dir.dirName() == game_executable_directory) {
            dir.cdUp();
        }

        installDir = dir.absolutePath();
    }
}

void Console::saveSettings()
{
    if (DirUtils::isGameDirectory(installDir)) {
        settings->setValue(settings_install_directory, installDir);
    }
}
