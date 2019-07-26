#ifndef PE_H
#define PE_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QHash>

#include <pe_bliss.h>

#include "constants.h"

using namespace pe_bliss;

class PeFile : public QObject
{
    Q_OBJECT

public:
    explicit PeFile(QObject* parent = nullptr);
    ~PeFile();

    FunctionMap buildAddressOfFunctions();
    bool patchCode(const FunctionMap &functions);

    void clear();
    bool load(const QString &path, const QString &target);
    void apply(const QString &libraryName, const FunctionMap &functions);
    bool save();

private:
    QString path, target;
    pe_base* image = nullptr;
};

#endif // PE_H
