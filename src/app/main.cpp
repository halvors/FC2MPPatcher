#include <QApplication>

#include "widget.h"

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    a.setOrganizationName(app_name);
    a.setApplicationName(app_name);

    Widget w;
    w.show();

    return a.exec();
}
