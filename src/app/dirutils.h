#ifndef DIRUTILS_H
#define DIRUTILS_H

#include <QDir>
#include <QString>
#include <QStringList>

class DirUtils
{
public:
    static bool isGameDirectory(QDir dir);
    static bool isGameDirectory(const QString &path);
    static QStringList &findInstallDirectories();

private:
    static QStringList installDirectories;
};

class RetailUtils
{
public:
    static QString getGameDirectory();

private:
    static QString installDirectory;
};

class SteamUtils
{
public:
    static QString getGameDirectory();

private:
    static QString installDirectory;
    static QStringList libraries;

    static QString getInstallDirectory();
    static QStringList &findLibraries(QDir &dir);
    static QJsonObject getJsonFromFile(QFile &file);
    static QString getJsonFromAcf(const QStringList &lines);
};

#endif // DIRUTILS_H
