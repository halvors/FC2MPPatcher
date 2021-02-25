#include <QDebug>

#include "console.h"
#include "netutils.h"
#include "dirutils.h"
#include "defs.h"
#include "patcher.h"

Console::Console(const QString &installDir, const QString &interfaceName, QObject *parent) :
    QObject(parent)
{
    settings = new QSettings(app_configuration_file, QSettings::IniFormat, this);
    loadSettings();

    if (DirUtils::isGameDirectory(installDir))
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
        qDebug().noquote() << tr("Error: No %1 installation directory specified, aborting!").arg(game_name);

        return false;
    } else if (!DirUtils::isGameDirectory(installDir)) {
        qDebug().noquote() << QString("Error: Directory \"%1\" does not contain a %2 installation, aborting!").arg(installDir, game_name);

        return false;
    }

    QDir dir = installDir;
    dir.cd(game_executable_directory);

    // Only show option to patch if not already patched.
    if (Patcher::isPatched(dir.absolutePath())) {
        Patcher::undoPatch(dir);

        qDebug().noquote() << tr("Successfully uninstalled patch.");
    } else {
        // Apply patch to files, if successful continue.
        if (Patcher::patch(dir))
            Patcher::generateConfigurationFile(dir, interface);

        qDebug().noquote() << tr("Successfully installed patch.");
    }

    saveSettings();

    return true;
}

void Console::loadSettings()
{
    QDir dir = settings->value(settings_install_directory).toString();

    if (!DirUtils::isGameDirectory(dir))
        return;

    // If we're in executable directory, cd up to install directory.
    if (dir.dirName() == game_executable_directory)
        dir.cdUp();

    installDir = dir.absolutePath();
}

void Console::saveSettings()
{
    if (DirUtils::isGameDirectory(installDir))
        settings->setValue(settings_install_directory, installDir);
}
