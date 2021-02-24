#include <QApplication>

#include "console.h"
#include "widget.h"

#include <QCommandLineParser>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setOrganizationName(app_name);
    app.setApplicationName(app_organization);
    app.setApplicationVersion(APP_VERSION);

    QCommandLineOption cliOption({{ "n", "no-gui" }, "Run the application in headless mode." });
    QCommandLineOption installDirectoryOption({{ "d", "install-dir" }, QString("Path to the %1 installation directory.").arg(game_name), "directory" });

    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addOption(cliOption);
    parser.addOption(installDirectoryOption);
    parser.process(app);

    bool cliMode = parser.isSet(cliOption);
    QString installDir = parser.value(installDirectoryOption);

    if (cliMode) {
        Console console(installDir);

        return !console.exec();
    } else {
        // Display the GUI widget.
        Widget widget(installDir);
        widget.show();
        return app.exec();
    }
}
