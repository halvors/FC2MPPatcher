#include <QSettings>
#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>
#include <QRegularExpression>

#include "dirutils.h"
#include "global.h"

QStringList DirUtils::installDirectories;

bool DirUtils::isGameDirectory(QDir dir)
{
    // Trying change to execuatable directory, assuming we're in install directory or that we already is in it.
    if (dir.exists() | dir.cd(game_executable_directory)) {
        for (const FileEntry &file : files) {
            // TODO: Check against checksum here?
            if (dir.exists(file.getName())) {
                return true;
            }
        }
    }

    return false;
}

bool DirUtils::isGameDirectory(const QString &path)
{
    return isGameDirectory(QDir(path));
}

QStringList& DirUtils::findInstallDirectories()
{
    // If install directories already detected, return early.
    if (!installDirectories.isEmpty()) {
        return installDirectories;
    }

    auto append = [&](const QString &path) {
        // Verfiy that path is not empty, already exist or an invalid game directory.
        if (!path.isEmpty() && !installDirectories.contains(path) && isGameDirectory(path)) {
            installDirectories.append(path);
        }
    };

    append(RetailUtils::getGameDirectory());
    append(SteamUtils::getGameDirectory(game_steam_app_id));
    append(UplayUtils::getGameDirectory());

    return installDirectories;
}

QString RetailUtils::installDirectory;

QString RetailUtils::getGameDirectory()
{
    // If install directory already detected, return early.
    if (!installDirectory.isEmpty()) {
        return installDirectory;
    }

#ifdef Q_OS_WIN
    // Look for Far Cry 2 install directory in registry.
    QSettings registry("HKEY_LOCAL_MACHINE\\SOFTWARE", QSettings::Registry32Format);
    registry.beginGroup(game_publisher);
        registry.beginGroup(game_name);
            QDir dir = registry.value("InstallDir").toString();
        registry.endGroup();
    registry.endGroup();

    if (DirUtils::isGameDirectory(dir.absolutePath())) {
        qDebug() << QT_TR_NOOP(QString("Found installation directory: %1").arg(dir.absolutePath()));

        return dir.absolutePath();
    }
#endif

    return QString();
}

QString SteamUtils::installDirectory;
QStringList SteamUtils::libraries;

QString SteamUtils::getGameDirectory(int appId)
{
    QDir dir = getInstallDirectory();

    // Search for other steam libraries.
    QStringList libraries = findLibraries(dir);

    for (QDir dir : libraries) {
        // Entering directory where steamapps is stored.
        if (!dir.cd(game_steam_app_directory)) {
            continue;
        }

        // Assemble manifest file using provided appId.
        QFile manifestFile = dir.filePath(QString("%1_%2.%3").arg(game_steam_app_manifest_name).arg(appId).arg(game_steam_app_manifest_suffix));

        if (!manifestFile.exists()) {
            continue;
        }

        // Only continue parsing json if we need it.
        if (!dir.cd(game_steam_app_directory_common)) {
            continue;
        }

        QJsonObject object = getJsonFromFile(manifestFile);
        QJsonValue value = object.value(game_steam_app_manifest_key);

        // Enter found game directory.
        if (!dir.cd(value.toString())) {
            continue;
        }

        // Verify that this is a game directory.
        if (DirUtils::isGameDirectory(dir.absolutePath())) {
            qDebug() << QT_TR_NOOP(QString("Found game install directory: %1").arg(dir.absolutePath()));

            return dir.absolutePath();
        }
    }

    return QString();
}

QString SteamUtils::getInstallDirectory()
{
    // If install directory already detected, return early.
    if (!installDirectory.isEmpty()) {
        return installDirectory;
    }

#ifdef Q_OS_WIN
    // Find Steam install directory in registry.
    QSettings registry("HKEY_LOCAL_MACHINE\\SOFTWARE", QSettings::Registry32Format);
    registry.beginGroup(game_steam_publisher);
        registry.beginGroup(game_steam_name);
            installDirectory = QDir(registry.value("InstallPath").toString()).absolutePath();
        registry.endGroup();
    registry.endGroup();
#elif defined(Q_OS_LINUX)
    QDir dir = QDir::home();

    if (dir.cd(QString(".%1/%1").arg(game_steam_name).toLower())) {
        installDirectory = dir.absolutePath();
    }
#endif

    if (!installDirectory.isEmpty()) {
        qDebug() << QT_TR_NOOP(QString("Found steam installation directory: %1").arg(installDirectory));
    }

    return installDirectory;
}

QStringList& SteamUtils::findLibraries(QDir &dir)
{
    // If libraries already found, return early.
    if (!libraries.isEmpty()) {
        return libraries;
    }

    // Add this directory as the default steam library.
    libraries.prepend(dir.absolutePath());

    // Entering directory where steamapps is stored.
    if (!dir.cd(game_steam_app_directory)) {
        return libraries;
    }

    QFile libraryFile = dir.filePath("libraryfolders.vdf");

    if (!libraryFile.exists()) {
        return libraries;
    }

    QJsonObject object = getJsonFromFile(libraryFile);
    QJsonObject::iterator iterator;

    for (iterator = object.begin(); iterator != object.end(); iterator++) {
        bool valid = false;

        // Check if key is of integer type.
        iterator.key().toInt(&valid);

        // Only read valid values.
        if (valid) {
            QDir dir = iterator.value().toString();
            libraries.append(dir.absolutePath());

            qDebug() << QT_TR_NOOP(QString("Found steam library: %1").arg(dir.absolutePath()));
        }
    }

    return libraries;
}

QJsonObject SteamUtils::getJsonFromFile(QFile &file)
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

QString SteamUtils::getJsonFromAcf(const QStringList &lines)
{
    static const QRegularExpression singleLine("^(\\t+\".+\")\\t\\t(\".*\")$");
    static const QRegularExpression startOfObject("^\\t+\".+\"$");
    QString json;

    auto appendLine = [&](const QString &string = QString()) {
        return json.append(QChar::LineFeed + string);
    };

    auto endLine = [&](int nextIndex) {
        if (nextIndex < lines.length() && lines[nextIndex].endsWith("}")) {
            appendLine();
        } else {
            appendLine(",");
        }
    };

    for (int i = 1; i < lines.length(); i++) {
        QRegularExpressionMatch singleLineMatch = singleLine.match(lines[i]);

        if (singleLineMatch.hasMatch()) {
            json.append(singleLineMatch.captured(1));
            json.append(": ");
            json.append(singleLineMatch.captured(2));

            // Last value of object must not have a tailing comma.
            endLine(i + 1);
        } else if (lines[i].startsWith(QChar::Tabulation) && lines[i].endsWith("}")) {
            json.append(lines[i]);

            // Last value of object must not have a tailing comma.
            endLine(i + 1);
        } else if (startOfObject.match(lines[i]).hasMatch()) {
            json.append(lines[i]);
            appendLine(":");
        } else {
            appendLine(lines[i]);
        }
    }

    return json;
}

QString UplayUtils::installDirectory;

QString UplayUtils::getGameDirectory()
{
    // If install directory already detected, return early.
    if (!installDirectory.isEmpty()) {
        return installDirectory;
    }

#ifdef Q_OS_WIN
    // Look for Far Cry 2 install directory in registry.
    QSettings registry("HKEY_LOCAL_MACHINE\\SOFTWARE", QSettings::Registry32Format);
    registry.beginGroup(game_publisher);
        registry.beginGroup(game_uplay_registry_path);
            registry.beginGroup(QString::number(game_uplay_app_id));
                QDir dir = registry.value("InstallDir").toString();
            registry.endGroup();
        registry.endGroup();
    registry.endGroup();

    if (DirUtils::isGameDirectory(dir.absolutePath())) {
        qDebug() << QT_TR_NOOP(QString("Found installation directory: %1").arg(dir.absolutePath()));

        return dir.absolutePath();
    }
#endif

    return QString();
}
