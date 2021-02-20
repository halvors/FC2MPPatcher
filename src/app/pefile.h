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
    explicit PeFile(const QFile &file, QObject *parent = nullptr);
    ~PeFile();

    bool apply(const QString &libraryFile, const QStringList &libraryFunctions, const QList<CodeEntry> &codeEntries) const;
    bool write() const;

private:
    const QFile &file;
    pe_base *image = nullptr;

    bool read();
    QList<unsigned int> buildSymbolAddressList(const QString &libraryFile) const;
    bool patchCode(const QString &libraryFile, const QStringList &libraryFunctions, const QList<CodeEntry> &codeEntries) const;
};

#endif // PEFILE_H
