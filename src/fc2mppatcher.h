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

    void openFile(const QString &filename);

signals:

public slots:

private:
    PeLib::PeFile32 *peFile;
    const QString fc2_install_dir = "C:/Program Files/Steam/steamapps/common/Far Cry 2";

    void dumpImportDirectory(PeLib::PeFile32 *pef);

};

#endif // FC2MPPATCHER_H
