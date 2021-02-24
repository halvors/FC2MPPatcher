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

    QCommandLineParser parser;
    //parser.setApplicationDescription("Test helper");
    parser.addHelpOption();
    parser.addVersionOption();

    // A boolean option with multiple names (-n, --no-gui)
    QCommandLineOption cliOption({{ "n", "no-gui" }, "Overwrite existing files." });
    parser.addOption(cliOption);

    QCommandLineOption installDirectoryOption({{ "d", "install-dir" }, "Copy all source files into <directory>.", "directory" });
    parser.addOption(installDirectoryOption);

    // Process the actual command line arguments given by the user
    parser.process(app);

    bool cliMode = parser.isSet(cliOption);
    QString installDir = parser.value(installDirectoryOption);

    qDebug().noquote() << QString("Cli is: %1").arg(cliMode);
    qDebug().noquote() << QString("Install directory is: %1").arg(installDir);

    if (cliMode) {
        Console console;
        return console.exec(installDir);
    } else {
        // Display the GUI widget.
        Widget widget;
        widget.show();
        return app.exec();
    }
}
