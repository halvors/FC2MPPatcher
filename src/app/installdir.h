#ifndef DIRUTILS_H
#define DIRUTILS_H

#include <QString>
#include <QDir>
#include <QStringList>
#include <QJsonObject>
#include <QFile>

class InstallDir
{
public:
    static bool isGameDirectory(const QString &path);
    static bool isGameDirectory(QDir dir);

    static const QStringList &findInstallDirectories();

private:
    static QString &getSteamDirectory();
    static QStringList &findSteamLibraries(const QString &steamDirectory);
    static QJsonObject getJsonFromFile(QFile &file);
    static QString getJsonFromAcf(const QStringList &lines);

    static QStringList directories;
    static QString steamDirectory;
    static QStringList steamLibraries;
};

#endif // DIRUTILS_H
