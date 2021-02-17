#include <QApplication>

#include "widget.h"

#include <QDebug>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setOrganizationName(app_name);
    app.setApplicationName(app_organization);
    app.setApplicationVersion(APP_VERSION);

    QNetworkAccessManager *connection = new QNetworkAccessManager();
    QNetworkReply *reply = connection->get(QNetworkRequest(QUrl("http://api.ipify.org")));

    // Trick to do this synchronously.
    QEventLoop waitLoop;
    QObject::connect(reply, SIGNAL(finished()), &waitLoop, SLOT(quit()));
    waitLoop.exec();

    if (reply->error() == QNetworkReply::NoError){
        qDebug().noquote() << QString("Public IP is: \"%1\"").arg(reply->readAll().constData());
    }

    delete reply;
    delete connection;

    // Display the GUI widget.
    //Widget widget;
    //widget.show();

    return app.exec();
}
