#ifndef CONSOLE_H
#define CONSOLE_H

#include <QObject>
#include <QString>
#include <QSettings>

class Console : public QObject
{
    Q_OBJECT

public:
    explicit Console(const QString &installDir = QString(), QObject *parent = nullptr);
    ~Console();

    bool exec();

private:
    QSettings *settings;
    QString installDir;

    void loadSettings();
    void saveSettings();

};

#endif // CONSOLE_H
