#ifndef FC2MPPATCHER_H
#define FC2MPPATCHER_H

#include <QObject>
#include <QString>

#include <pelib/PeLib.h>

#include "constants.h"

class FC2MPPatcher : public QObject
{
    Q_OBJECT

public:
    explicit FC2MPPatcher(QObject *parent = nullptr);
    ~FC2MPPatcher();

    bool open(const QString &filename);
    bool save();

    void patch(const QString &installDir);

    bool addFunction(const QString &libraryName, const QString &functionName);
    void dumpImportDirectory();

private:
    PeLib::PeFile32 *peFile;

};

#endif // FC2MPPATCHER_H
