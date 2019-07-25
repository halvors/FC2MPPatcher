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
    QHash<QString, unsigned int> buildAddressOfFunctionMap(const pe_base &image);
    bool apply(const QString &fileName);

    //void printFunctions(const pe_base &imports);

private:
    QHash<QString, import_library*> functionMap;
    QHash<QString, unsigned int> addressOfFunctionMap;

};

#endif // PE_H
