#include <QSettings>
#include <QDebug>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonDocument>
#include <QRegExp>

#include "dirutils.h"
#include "constants.h"

bool DirUtils::isGameDir(QDir &dir)
{
    // Trying change to execuatable directory, assuming we're in install directory or that we already is in it.
    if (dir.exists() | dir.cd(Constants::game_executable_directory)) {
        for (const FileEntry &file : Constants::files) {
            // TODO: Check against checksum here?
            if (dir.exists(file.getName())) {
                return true;
            }
        }
    }

    return false;
}

QString DirUtils::findInstallDir()
{
    // Look for Far Cry 2 install directory in registry.
    QDir dir = DirUtils::getRetailGameDir();

    if (DirUtils::isGameDir(dir)) {
        //return dir.absolutePath();
    }

    // Look for Far Cry 2 install directory in Steam.
    dir = DirUtils::getSteamGameDir(Constants::game_steam_appId);

    if (DirUtils::isGameDir(dir)) {
        return dir.absolutePath();
    }

    // Fallback to statically set installation directory if autodetection failed.
    return Constants::game_install_directory;
}

QString DirUtils::getRetailGameDir()
{
#ifdef Q_OS_WIN
    // Look for Far Cry 2 install directory in registry.
    QSettings registry("HKEY_LOCAL_MACHINE\\SOFTWARE", QSettings::Registry32Format);
    registry.beginGroup(Constants::game_publisher);
        registry.beginGroup(Constants::game_name);
            QDir dir = registry.value("InstallDir").toString();
        registry.endGroup();
    registry.endGroup();

    if (dir.exists()) {
        qDebug() << "Found install directory: " << dir.absolutePath();

        return dir.absolutePath();
    }
#endif

    return QString();
}

QString DirUtils::getSteamGameDir(int appId)
{
    QDir dir = getSteamDir();

    qDebug() << "Found steam directory:" << dir.absolutePath();

    // Search for other steam libraries.
    QStringList libraries = findSteamLibraries(dir);

    // Add this directory as the default steam library.
    libraries.prepend(".");

    for (QDir dir : libraries) {
        qDebug() << dir.absolutePath();

        // Entering directory where steamapps is stored.
        if (dir.cd("steamapps")) {
            // Assemble manifest file using provided appId.
            QFile manifestFile = dir.filePath("appmanifest_" + QString::number(appId) + ".acf");

            if (manifestFile.exists()) {
                QJsonObject object = getJsonFromFile(manifestFile);

                // Only continue parsing json if we need it.
                if (dir.cd("common")) {
                    QJsonValue value = object.value("installdir");

                    // Enter found game directory.
                    if (dir.cd(value.toString())) {
                        qDebug() << "Found install directory: " << dir.absolutePath();

                        return dir.absolutePath();
                    }
                }
            }
        }
    }

    return QString();
}

QString DirUtils::getSteamDir()
{
    QString steamDir;

#ifdef Q_OS_WIN    
    // Find Steam install directory in registry.
    QSettings registry("HKEY_LOCAL_MACHINE\\SOFTWARE", QSettings::Registry32Format);
    registry.beginGroup("Valve");
        registry.beginGroup("Steam");
            steamDir = registry.value("InstallPath").toString();
        registry.endGroup();
    registry.endGroup();
#elif defined(Q_OS_LINUX)
    QDir dir = QDir::home();

    // TODO: Check this?
    if (dir.cd(".steam")) {
        steamDir = dir.absolutePath();
    }
#endif

    return steamDir;
}

QStringList DirUtils::findSteamLibraries(QDir dir)
{
    QStringList libraries;

    // Entering directory where steamapps is stored.
    if (dir.cd("steamapps")) {
        QFile libraryFile = dir.filePath("libraryfolders.vdf");

        if (libraryFile.exists()) {
            QJsonObject object = getJsonFromFile(libraryFile);
            QJsonObject::iterator iterator;

            for (iterator = object.begin(); iterator != object.end(); iterator++) {
                bool valid = false;

                // Check if key is of integer type.
                iterator.key().toInt(&valid);

                // Only read valid values.
                if (valid) {
                    libraries.append(iterator.value().toString());

                    qDebug() << "Found steam library:" << iterator.value().toString();
                }
            }
        }
    }

    return libraries;
}

QJsonObject DirUtils::getJsonFromFile(QFile &file)
{
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QString data = file.readAll();
        file.close();

        // Convert acf to json.
        QString json = getJsonFromAcf(data.split("\n"));

        // Parse json.
        QJsonDocument document = QJsonDocument::fromJson(json.toUtf8());

        return document.object();
    }

    return QJsonObject();
}

QString DirUtils::getJsonFromAcf(const QStringList &lines)
{
    static QRegExp singleLine("^(\\t+\".+\")\\t\\t(\".*\")$");
    static QRegExp startOfObject("^\\t+\".+\"$");
    QString json;

    for (int i = 1; i < lines.length(); i++) {
        if (singleLine.exactMatch(lines[i])) {
            json.append(singleLine.cap(1));
            json.append(": ");
            json.append(singleLine.cap(2));

            // Last value of object must not have a tailing comma.
            if (i + 1 < lines.length() && lines[i + 1].endsWith("}")) {
                json.append("\n");
            } else {
                json.append("\n,");
            }
        } else if (lines[i].startsWith("\t") && lines[i].endsWith("}")) {
            json.append(lines[i]);

            if (i + 1 < lines.length() && lines[i + 1].endsWith("}")) {
                json.append("\n");
            } else {
                json.append("\n,");
            }
        } else if (startOfObject.exactMatch(lines[i])) {
            json.append(lines[i]);
            json.append("\n:");
        } else {
            json.append("\n" + lines[i]);
        }
    }

    return json;
}
