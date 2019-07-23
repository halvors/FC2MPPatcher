#ifndef PE_H
#define PE_H

#include <QObject>
#include <QString>

#include <pelib/PeFile.h>

#include "constants.h"

using namespace std;
using namespace PeLib;

class Pe : public QObject
{
    Q_OBJECT
public:
    explicit Pe(QObject *parent = nullptr);
    ~Pe();

    bool open(const QString &fileName);
    bool save();

    bool addLibraryFunction(const QString &libraryName, const QString &functionName);
    void dumpImportDirectory();

private:
    PeFile32 *file;

    MzHeader *mzHeader;
    PeHeader32 *peHeader;
    ImportDirectory32 *importDirectory;
};

#endif // PE_H
