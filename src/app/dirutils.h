#ifndef DIRUTILS_H
#define DIRUTILS_H

#include <QDir>
#include <QString>
#include <QStringList>

class DirUtils
{
public:
    static bool isGameDir(QDir &dir);
    static bool isGameDir(const QString &path);
    static QString findInstallDir();
    static QString getSteamGameDir(int appId);
    static QString getRetailGameDir();

private:
    static QString getSteamDir();
    static QStringList findSteamLibraries(QDir dir);
    static QJsonObject getJsonFromFile(QFile &file);
    static QString getJsonFromAcf(const QStringList &lines);
};

#endif // DIRUTILS_H
