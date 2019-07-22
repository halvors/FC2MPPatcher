#ifndef FC2MPPATCHER_H
#define FC2MPPATCHER_H

#include <QObject>
#include <QString>

#include <pelib/PeLib.h>

class FC2MPPatcher : public QObject
{
    Q_OBJECT

public:
    explicit FC2MPPatcher(QObject *parent = nullptr);
    ~FC2MPPatcher();

    bool open(const QString &filename);
    bool save();

signals:

public slots:

private:
    PeLib::PeFile32 *peFile;

    const QString filename = "/home/halvors/Dokumenter/Prosjekter/FC2MPPatcher/FarCry2.exe";
    const QString fc2_install_dir = "C:/Program Files/Steam/steamapps/common/Far Cry 2";

    void addImportFunction(const QString &libraryName, const QString &functionName);
    void dumpImportDirectory();

};

#endif // FC2MPPATCHER_H
