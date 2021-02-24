#include "console.h"

#include <QDebug>

#include "dirutils.h"
#include "global.h"
#include "patcher.h"

Console::Console(QObject *parent) :
    QObject(parent)
{

}

Console::~Console()
{

}

int Console::exec(const QString &path)
{
    if (!DirUtils::isGameDirectory(path)) {
        qDebug().noquote() << QString("Error: Directory \"%1\" does contain a %1 installation, aborting!").arg(game_name);
        return EXIT_FAILURE;
    }

    if (path.isEmpty()) {
        qDebug().noquote() << QString("Error: Directory \"%1\" does contain a %1 installation!").arg(game_name);
        return EXIT_FAILURE;
    }

    QDir dir = path;
    dir.cd(game_executable_directory);

    // Only show option to patch if not already patched.
    if (Patcher::isPatched(dir.absolutePath())) {
        Patcher::undoPatch(dir);

        qDebug().noquote() << QString("Installed the patch.");
    } else {
        // Apply patch to files, if successful continue.
        if (Patcher::patch(dir)) {
            // Generate network configuration.
            //Patcher::generateConfigurationFile(dir, ui->comboBox_network_interface->currentData().value<QNetworkInterface>());
        }

        qDebug().noquote() << QString("Unistalled the patch.");
    }

    return 0;
}
