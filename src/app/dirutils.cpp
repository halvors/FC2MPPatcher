#include <QSettings>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

#include "dirutils.h"
#include "constants.h"
#include "entry.h"

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

QString DirUtils::getRetailInstallDir()
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

QString DirUtils::getSteamInstallDir(int appId)
{
    QDir dir = getSteamDir();

    // Entering directory where steamapps is stored.
    if (dir.cd("steamapps")) {
        // Assemble manifest file using provided appId.
        QFile file = dir.filePath("appmanifest_" + QString::number(appId) + ".acf");

        // Try to read the manifest file.
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QString data = file.readAll();
            file.close();

            // Only continue parsing json if we need it.
            if (dir.cd("common")) {
                // Convert Acf to Json.
                QString json = getJsonFromAcf(data.split("\n"));

                // Read Json.
                QJsonDocument jsonDocument = QJsonDocument::fromJson(json.toUtf8());
                QJsonObject jsonObject = jsonDocument.object();
                QJsonValue jsonValue = jsonObject.value("installdir");

                // Enter found directory.
                if (dir.cd(jsonValue.toString())) {
                    qDebug() << "Found install directory: " << dir.absolutePath();

                    return dir.absolutePath();
                }
            }
        }
    }

    return QString();
}

QString DirUtils::getSteamDir()
{
#ifdef Q_OS_WIN
    // Find Steam install directory in registry.
    QSettings registry("HKEY_LOCAL_MACHINE\\SOFTWARE", QSettings::Registry32Format);
    registry.beginGroup("Valve");
        registry.beginGroup("Steam");
            return registry.value("InstallPath").toString();
        registry.endGroup();
    registry.endGroup();
#elif defined(Q_OS_LINUX)
    QDir dir = QDir::home();

    // TODO: Check this?
    if (dir.cd(".steam")) {
        return dir.absolutePath();
    }
#endif

    return QString();
}

QString DirUtils::getJsonFromAcf(const QStringList &lines)
{
    static QRegExp singleLine("^(\\t+\".+\")\\t\\t(\".*\")$");
    static QRegExp startOfObject("^\\t+\".+\"$");

    QString sb;

    for (int i = 1; i < lines.length(); i++) {
        if (singleLine.exactMatch(lines[i])) {
            sb.append(singleLine.cap(1));
            sb.append(": ");
            sb.append(singleLine.cap(2));

            // Last value of object must not have a tailing comma.
            if (i + 1 < lines.length() && lines[i + 1].endsWith("}")) {
                sb.append("\n");
            } else {
                sb.append("\n,");
            }
        } else if (lines[i].startsWith("\t") && lines[i].endsWith("}")) {
            sb.append(lines[i]);

            if (i + 1 < lines.length() && lines[i + 1].endsWith("}")) {
                sb.append("\n");
            } else {
                sb.append("\n,");
            }
        } else if (startOfObject.exactMatch(lines[i])) {
            sb.append(lines[i]);
            sb.append("\n:");
        } else {
            sb.append("\n" + lines[i]);
        }
    }

    return sb;
}
