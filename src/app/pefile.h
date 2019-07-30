#ifndef PEFILE_H
#define PEFILE_H

#include <QObject>
#include <QList>
#include <QString>

#include <pe_bliss.h>

#include "constants.h"

using namespace pe_bliss;

class PeFile : public QObject
{
    Q_OBJECT

public:
    explicit PeFile(const QString &fileName, QObject* parent = nullptr);
    ~PeFile();

    bool apply(const QString &libraryName, QStringList libraryFunctions, QList<FunctionEntry> targetFunctions, const QString &sectionName) const;
    bool write() const;

private:
    QString fileName;
    pe_base* image = nullptr;

    bool read();
    const QList<FunctionEntry> buildAddressOfFunctions(const QString &libraryName) const;
    bool patchFunctions(const QString &libraryName, const QList<FunctionEntry> &functions, const QString &sectionName) const;
};

#endif // PEFILE_H
