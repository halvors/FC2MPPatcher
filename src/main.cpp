#include <QApplication>

#include "widget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setOrganizationName(Constants::app_name);
    a.setApplicationName(Constants::app_name);

    Widget w;
    w.show();

    return a.exec();
}
