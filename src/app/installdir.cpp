#include <QSettings>
#include <QJsonValue>
#include <QDebug>
#include <QJsonDocument>
#include <QRegularExpression>
#include <QChar>
#include <QRegularExpressionMatch>

#include "installdir.h"
#include "defs.h"
#include "patch_defs.h"
#include "fileutils.h"

QStringList InstallDir::directories;
QString InstallDir::steamDirectory;
QStringList InstallDir::steamLibraries;

bool InstallDir::isGameDirectory(const QString &path)
{
    if (path.isEmpty())
        return false;

    return isGameDirectory(QDir(path));
}

bool InstallDir::isGameDirectory(QDir dir)
{
    // Trying change to execuatable directory, assuming we're in install directory or that we already is in it.
    if (dir.exists() | dir.cd(game_executable_directory))
        for (const FileEntry &file : files)
            if (dir.exists(file.name)) // TODO: Check against checksum here? using FileUtils::isValid()d?
                return true;

    return false;
}

const QStringList &InstallDir::findInstallDirectories()
{
    // If install directories already detected, return early.
    if (!directories.isEmpty())
        return directories;

    auto append = [&](const QString &path) {
        // Verfiy that path is not empty, already exist or an invalid game directory.
        if (!path.isEmpty() && !directories.contains(path) && isGameDirectory(path))
            directories.append(path);
    };

    // Detect Retail edition.
#if defined(Q_OS_WIN)
    // Look for Far Cry 2 install directory in registry.
    QSettings registry("HKEY_LOCAL_MACHINE\\SOFTWARE", QSettings::Registry32Format);
    registry.beginGroup(game_publisher);
        registry.beginGroup(game_name);
            QDir dir = registry.value("InstallDir").toString();
        registry.endGroup();
    registry.endGroup();

    if (InstallDir::isGameDirectory(dir.absolutePath())) {
        qDebug().noquote() << QT_TR_NOOP(QString("Found game install directory: %1").arg(dir.absolutePath()));

        append(dir.absolutePath());
    }
#endif

    // Search for other steam libraries.
    const QStringList &steamLibraries = findSteamLibraries(getSteamDirectory());

    for (QDir dir : steamLibraries) {
        // Entering directory where steamapps is stored.
        if (!dir.cd(game_steam_app_directory))
            continue;

        // Assemble manifest file using provided appId.
        QFile manifestFile = dir.filePath(QString("%1_%2.%3").arg(game_steam_app_manifest_name).arg(game_steam_app_id).arg(game_steam_app_manifest_suffix));

        if (!manifestFile.exists())
            continue;

        // Only continue parsing json if we need it.
        if (!dir.cd(game_steam_app_directory_common))
            continue;

        const QJsonObject &object = getJsonFromFile(manifestFile);
        const QJsonValue &value = object.value(game_steam_app_manifest_key);

        // Enter found game directory.
        if (!dir.cd(value.toString()))
            continue;

        const QString &absolutePath = dir.absolutePath();

        // Verify that this is a game directory.
        if (isGameDirectory(absolutePath)) {
            qDebug().noquote() << QT_TR_NOOP(QString("Found game install directory: %1").arg(absolutePath));

            append(absolutePath);
        }
    }

    return directories;
}

QString &InstallDir::getSteamDirectory()
{
    // If install directory already detected, return early.
    if (!steamDirectory.isEmpty())
        return steamDirectory;

#if defined(Q_OS_LINUX)
    QDir dir = QDir::home();

    if (dir.cd(QString(".%1/%1").arg(game_steam_name).toLower()))
        steamDirectory = dir.absolutePath();

#elif defined(Q_OS_WIN)
    // Find Steam install directory in registry.
    QSettings registry("HKEY_LOCAL_MACHINE\\SOFTWARE", QSettings::Registry32Format);
    registry.beginGroup(game_steam_publisher);
        registry.beginGroup(game_steam_name);
            steamDirectory = QDir(registry.value("InstallPath").toString()).absolutePath();
        registry.endGroup();
    registry.endGroup();
#endif

    if (!steamDirectory.isEmpty())
        qDebug().noquote() << QT_TR_NOOP(QString("Found steam installation directory: %1").arg(steamDirectory));

    return steamDirectory;
}

QStringList &InstallDir::findSteamLibraries(const QString &steamDirectory)
{
    QDir dir = steamDirectory;

    // If libraries already found, return early.
    if (!steamLibraries.isEmpty())
        return steamLibraries;

    // Add this directory as the default steam library.
    steamLibraries.prepend(dir.absolutePath());

    // Entering directory where steamapps is stored.
    if (!dir.cd(game_steam_app_directory))
        return steamLibraries;

    QFile libraryFile = dir.filePath("libraryfolders.vdf");

    if (!libraryFile.exists())
        return steamLibraries;

    QJsonObject object = getJsonFromFile(libraryFile);
    QJsonObject::iterator it;

    for (it = object.begin(); it != object.end(); it++) {
        bool valid = false;

        // Check if key is of integer type.
        it.key().toInt(&valid);

        // Only read valid values.
        if (valid) {
            const QDir &dir = it.value().toString();
            steamLibraries.append(dir.absolutePath());

            qDebug().noquote() << QT_TR_NOOP(QString("Found steam library: %1").arg(dir.absolutePath()));
        }
    }

    return steamLibraries;
}

QJsonObject InstallDir::getJsonFromFile(QFile &file)
{
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return QJsonObject();

    const QString &data = file.readAll();
    file.close();

    // Convert from acf to json format.
    const QString &json = getJsonFromAcf(data.split(QChar::LineFeed));

    // Parse json.
    return QJsonDocument::fromJson(json.toUtf8()).object();
}

QString InstallDir::getJsonFromAcf(const QStringList &lines)
{
    static const QRegularExpression singleLine("^(\\t+\".+\")\\t\\t(\".*\")$");
    static const QRegularExpression startOfObject("^\\t+\".+\"$");
    QString json;

    auto appendLine = [&](const QString &string = QString()) {
        return json.append(QChar::LineFeed + string);
    };

    auto endLine = [&](int nextIndex) {
        if (nextIndex < lines.length() && lines[nextIndex].endsWith("}"))
            appendLine();
        else
            appendLine(",");
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
