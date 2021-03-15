#include <QApplication>
#include <QCommandLineParser>

#include "console.h"
#include "widget.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setOrganizationName(app_name);
    app.setApplicationName(app_organization);
    app.setApplicationVersion(QString("%1%2").arg(APP_VERSION).arg(DEBUG_MODE ? "-dev" : QString()));

    QCommandLineOption cliOption({{ "n", "no-gui" }, "Run the application in headless mode." });
    QCommandLineOption installDirectoryOption({{ "d", "install-directory" }, QString("Path to the %1 installation directory.").arg(game_name), "directory" });
    QCommandLineOption networkInterfaceOption({{ "i", "interface-name" }, QString("Network interface name to be used for LAN matches."), "name" });

    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addOption(cliOption);
    parser.addOption(installDirectoryOption);
    parser.addOption(networkInterfaceOption);
    parser.process(app);

    bool cliMode = parser.isSet(cliOption);
    QString installDir = parser.value(installDirectoryOption);
    QString networkInterfaceName = parser.value(networkInterfaceOption);

    if (cliMode) {
        // Do task in background without GUI.
        Console console(installDir, networkInterfaceName);
        return !console.exec();
    } else {
        // Display the GUI widget.
        Widget widget(installDir, networkInterfaceName);
        widget.show();
        return app.exec();
    }
}
