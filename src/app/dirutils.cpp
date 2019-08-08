#include <QSettings>
#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>
#include <QRegularExpression>

#include "dirutils.h"
#include "constants.h"

QString DirUtils::steamDir;
QStringList DirUtils::steamLibraries;

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

bool DirUtils::isGameDir(const QString &path)
{
    QDir dir = path;

    return isGameDir(dir);
}

QString DirUtils::findInstallDir()
{
    // Look for Far Cry 2 install directory in registry.
    QDir dir = DirUtils::getRetailGameDir();

    if (DirUtils::isGameDir(dir)) {
        return dir.absolutePath();
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
        qDebug() << QT_TR_NOOP(QString("Found installation directory: %1").arg(dir.absolutePath()));

        return dir.absolutePath();
    }
#endif

    return QString();
}

QString DirUtils::getSteamGameDir(int appId)
{
    QDir dir = getSteamDir();

    qDebug() << QT_TR_NOOP(QString("Found steam directory: %1").arg(dir.absolutePath()));

    // Search for other steam libraries.
    QStringList libraries = findSteamLibraries(dir);

    for (QDir dir : libraries) {
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
                        qDebug() << QT_TR_NOOP(QString("Found game install directory: %1").arg(dir.absolutePath()));

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
    if (steamDir.isEmpty()) {
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

    if (dir.cd(".steam/steam")) {
        steamDir = dir.absolutePath();
    }
#endif
    }

    return steamDir;
}

QStringList DirUtils::findSteamLibraries(QDir dir)
{
    if (steamLibraries.isEmpty()) {
        // Add this directory as the default steam library.
        steamLibraries.prepend(dir.absolutePath());

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
                        steamLibraries.append(iterator.value().toString());

                        qDebug() << QT_TR_NOOP(QString("Found steam library: %1").arg(iterator.value().toString()));
                    }
                }
            }
        }
    }

    return steamLibraries;
}

QJsonObject DirUtils::getJsonFromFile(QFile &file)
{
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QString data = file.readAll();
        file.close();

        // Convert acf to json.
        QString json = getJsonFromAcf(data.split(QChar::LineFeed));

        // Parse json.
        QJsonDocument document = QJsonDocument::fromJson(json.toUtf8());

        return document.object();
    }

    return QJsonObject();
}

QString DirUtils::getJsonFromAcf(const QStringList &lines)
{
    static const QRegularExpression singleLine("^(\\t+\".+\")\\t\\t(\".*\")$");
    static const QRegularExpression startOfObject("^\\t+\".+\"$");
    QString json;

    auto appendLine = [&](const QString &string = QString()) {
        return json.append(QChar::LineFeed + string);
    };

    for (int i = 1; i < lines.length(); i++) {
        QRegularExpressionMatch singleLineMatch = singleLine.match(lines[i]);
        int nextIndex = i + 1;

        if (singleLineMatch.hasMatch()) {
            json.append(singleLineMatch.captured(1));
            json.append(": ");
            json.append(singleLineMatch.captured(2));

            // Last value of object must not have a tailing comma.
            if (nextIndex < lines.length() && lines[nextIndex].endsWith("}")) {
                appendLine();
            } else {
                appendLine(",");
            }
        } else if (lines[i].startsWith(QChar::Tabulation) && lines[i].endsWith("}")) {
            json.append(lines[i]);

            if (nextIndex < lines.length() && lines[nextIndex].endsWith("}")) {
                appendLine();
            } else {
                appendLine(",");
            }
        } else if (startOfObject.match(lines[i]).hasMatch()) {
            json.append(lines[i]);
            appendLine(":");
        } else {
            appendLine(lines[i]);
        }
    }

    return json;
}
