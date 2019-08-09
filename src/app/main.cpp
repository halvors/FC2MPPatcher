#include <QApplication>

#include "widget.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    app.setOrganizationName(app_name);
    app.setApplicationName(app_organization);

    // Display the GUI widget.
    Widget widget;
    widget.show();

    return app.exec();
}
