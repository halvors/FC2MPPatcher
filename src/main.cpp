#include <QCoreApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    //qDebug("Far Cry 2 install directory is:  %s", fc2_install_dir);

    return a.exec();
}
