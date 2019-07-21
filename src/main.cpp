#include <QCoreApplication>
#include <QDebug>

const QString fc2_install_dir = "C:/Program Files/Steam/steamapps/common/Far Cry 2";

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    qDebug() << "Far Cry 2 install directory is: " << fc2_install_dir;

    return a.exec();
}
