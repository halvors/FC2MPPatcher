#ifndef PEFILE_H
#define PEFILE_H

#include <string>
#include <vector>

#include <QObject>
#include <QFile>

#include <pe_bliss.h>

#include "entry.h"

using namespace pe_bliss;

class PeFile : public QObject
{
    Q_OBJECT

public:
    explicit PeFile(const QFile &file, QObject *parent = nullptr);
    ~PeFile();

    bool apply(const std::string& libraryFile, const std::vector<std::string>& libraryFunctions, const std::vector<CodeEntry> &codeEntries) const;
    bool write() const;

private:
    const QFile &file;
    pe_base *image = nullptr;

    bool read();
    QList<uint32_t> buildSymbolAddressList(const std::string& libraryFile) const;
    bool patchCode(const std::string& libraryFile, const std::vector<std::string>& libraryFunctions, const std::vector<CodeEntry> &codeEntries) const;
};

#endif // PEFILE_H
