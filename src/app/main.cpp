#include <QApplication>

#include "widget.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    app.setOrganizationName(app_name);
    app.setApplicationName(app_organization);
    app.setApplicationVersion(APP_VERSION);

    // Display the GUI widget.
    Widget widget;
    widget.show();

    return app.exec();
}
