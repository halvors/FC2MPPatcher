#ifndef PE_H
#define PE_H

#include <QObject>
#include <QList>
#include <QString>

#include <pe_bliss.h>

#include "../common/constants.h"

using namespace pe_bliss;

class PeFile : public QObject
{
    Q_OBJECT

public:
    explicit PeFile(QObject* parent = nullptr);
    ~PeFile();

    QList<FunctionEntry> buildAddressOfFunctions();
    bool patchCode(QList<FunctionEntry> &functions);

    void clear();
    bool load(const QString &path, const QString &target);
    void apply(const QString &libraryName, QList<FunctionEntry> functions);
    bool save();

private:
    QString path, target;
    pe_base* image = nullptr;
};

#endif // PE_H
