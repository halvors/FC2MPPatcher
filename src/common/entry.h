#ifndef ENTRY_H
#define ENTRY_H

#include <QList>

class FunctionEntry {
public:
    FunctionEntry(unsigned int address, int index) :
        address(address),
        index(index) {}

    unsigned int getAddress() const {
        return address;
    }

    int getIndex() const {
        return index;
    }

private:
    const unsigned int address;
    const int index;

};

class TargetEntry {
public:
    TargetEntry(const char* checkSum, const char* checkSumPatched, const QList<FunctionEntry> &functions) :
        checkSum(checkSum),
        checkSumPatched(checkSumPatched),
        functions(functions) {}

    const char* getCheckSum() const {
        return checkSum;
    }

    const char* getCheckSumPatched() const {
        return checkSumPatched;
    }

    QList<FunctionEntry> getFunctions() const {
        return functions;
    }

private:
    const char* checkSum;
    const char* checkSumPatched;
    QList<FunctionEntry> functions;
};

class FileEntry {
public:
    FileEntry(const char* name, const QList<TargetEntry> &targets) :
        name(name),
        targets(targets) {}

    const char* getName() const {
        return name;
    }

    QList<TargetEntry> getTargets() const {
        return targets;
    }

private:
    const char* name;
    QList<TargetEntry> targets;
};

#endif // ENTRY_H
