#ifndef PE_H
#define PE_H

#include <QObject>
#include <QString>
#include <QHash>

#include <pe_bliss.h>

#include "constants.h"

using namespace pe_bliss;

class Pe : public QObject
{
    Q_OBJECT
public:
    explicit Pe(QObject *parent = nullptr);
    ~Pe();

    bool addLibraryFunction(const QString &libraryName, const QString &functionName);
    bool apply(const QString &fileName);

private:
    QHash<QString, import_library*> libraryMap;

};

#endif // PE_H
