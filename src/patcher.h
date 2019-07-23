#ifndef PATCHER_H
#define PATCHER_H

#include <QObject>
#include <QString>

#include "constants.h"
#include "pe.h"

class Patcher : public QObject
{
    Q_OBJECT

public:
    explicit Patcher(QObject *parent = nullptr);
    ~Patcher();

    void applyPatch(const QString &installDirectory);

private:
    Pe *pe;

};

#endif // PATCHER_H
