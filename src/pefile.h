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

    void addLibraryFunction(const QString &libraryName, const QString &functionName);
    void fetchLibraryFunctions(imported_functions_list &imports);
    void printLibraryFunctions(const pe_base &imports);
    bool apply(const QString &fileName);


private:
    QHash<QString, import_library*> libraryMap;
    QHash<QString, unsigned int> functionToAddressMap;

};

#endif // PE_H
