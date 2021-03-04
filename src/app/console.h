#ifndef CONSOLE_H
#define CONSOLE_H

#include <QObject>
#include <QString>
#include <QSettings>
#include <QNetworkInterface>

class Console : public QObject
{
    Q_OBJECT

public:
    explicit Console(const QString &installDir = QString(), const QString &interfaceName = QString(), QObject *parent = nullptr);
    ~Console();

    bool run();

private:
    QSettings *settings;
    QString installDir;
    QNetworkInterface interface;

    void loadSettings();
    void saveSettings();

};

#endif // CONSOLE_H
