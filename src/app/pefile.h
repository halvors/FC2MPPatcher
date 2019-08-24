#ifndef PEFILE_H
#define PEFILE_H

#include <QObject>
#include <QFile>
#include <QString>
#include <QStringList>
#include <QList>

#include <pe_bliss.h>

#include "entry.h"

using namespace pe_bliss;

class PeFile : public QObject
{
    Q_OBJECT

public:
    explicit PeFile(const QFile &file, QObject* parent = nullptr);
    ~PeFile();

    bool apply(const QString &libraryName, const QString &libraryFile, const QStringList &libraryFunctions, const QList<unsigned int> &addresses, const QString &sectionName) const;
    bool write() const;

private:
    const QFile &file;
    pe_base* image = nullptr;

    bool read();
    QList<unsigned int> getFunctionAddresses(const QString &libraryFile) const;
    bool patchFunctions(const QString &libraryFile, const QStringList &libraryFunctions, const QList<unsigned int> &addresses, const QString &sectionName) const;
};

#endif // PEFILE_H
