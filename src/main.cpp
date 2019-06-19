#include <QCoreApplication>
#include <QDebug>

#define FC2_INSTALL_DIR = "C:/Program Files/Steam/steamapps/common/Far Cry 2";
#define EXECUTABLE_DIR = "bin";

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    return a.exec();
}
