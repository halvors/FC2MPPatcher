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

    bool exec();

private:
    QSettings *settings;
    QString installDir;
    QNetworkInterface networkInterface;

    void loadSettings();
    void saveSettings();

};

#endif // CONSOLE_H
