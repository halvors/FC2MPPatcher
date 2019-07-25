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
    explicit PeFile(QObject *parent = nullptr);
    ~PeFile();

    void addFunction(const QString &libraryName, const QString &functionName);
    void applyFunctions(imported_functions_list &imports);
    void patchCode(const QString &target, const pe_base &image);
    FunctionMap buildAddressOfFunctions(const pe_base &image);
    bool apply(const QString &path, const QString &target);

    //void printFunctions(const pe_base &imports);

private:
    QHash<QString, import_library*> functions;
    FunctionMap addressOfFunctions;

};

#endif // PE_H
