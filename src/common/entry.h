#ifndef ENTRY_H
#define ENTRY_H

#include <QString>
#include <QList>

class FunctionEntry {
public:
    FunctionEntry(const unsigned int address, const QString &name) :
        address(address),
        name(name) {}

    unsigned int getAddress() const {
        return address;
    }

    QString getName() const {
        return name;
    }

private:
    unsigned int address;
    QString name;
};

class TargetEntry {
public:
    TargetEntry(const QString &checkSum, const QString &checkSumPatched, const QList<FunctionEntry> &functions) :
        checkSum(checkSum),
        checkSumPatched(checkSumPatched),
        functions(functions) {}

    QString getCheckSum() const {
        return checkSum;
    }

    QString getCheckSumPatched() const {
        return checkSumPatched;
    }

    QList<FunctionEntry> getFunctions() const {
        return functions;
    }

private:
    QString checkSum;
    QString checkSumPatched;
    QList<FunctionEntry> functions;
};

class FileEntry {
public:
    FileEntry(const QString &name, const QList<TargetEntry> &targets) :
        name(name),
        targets(targets) {}

    QString getName() const {
        return name;
    }

    QList<TargetEntry> getTargets() const {
        return targets;
    }

private:
    QString name;
    QList<TargetEntry> targets;
};

#endif // ENTRY_H
