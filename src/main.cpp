#include <QCoreApplication>

#include "fc2mppatcher.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    new FC2MPPatcher();

    return a.exec();
}
