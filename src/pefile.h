#ifndef PE_H
#define PE_H

#include <QObject>
#include <QString>
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

    void addFunction(const QString &libraryName, const QString &functionName);
    void applyFunctions(imported_functions_list &imports);
    void patchCode();
    FunctionMap buildAddressOfFunctions();

    void clear();
    bool load(const QString &path, const QString &target);
    void apply();
    bool save();

private:
    QString path, target;
    pe_base* image;

    QHash<QString, import_library*> functions;
    FunctionMap addressOfFunctions;

};

#endif // PE_H
