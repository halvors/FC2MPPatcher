#ifndef DIRUTILS_H
#define DIRUTILS_H

#include <QDir>
#include <QString>
#include <QStringList>

class DirUtils
{
public:
    static bool isGameDir(QDir &dir);
    static QString findInstallDir();
    static QString getSteamInstallDir(int appId);
    static QString getRetailInstallDir();

private:
    static QString getSteamDir();
    static QString getJsonFromAcf(const QStringList &lines);
};

#endif // DIRUTILS_H
