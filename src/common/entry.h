#ifndef ENTRY_H
#define ENTRY_H

#include <QList>

class TargetEntry {
public:
    TargetEntry(const char* checkSum, const char* checkSumPatched, const QList<unsigned int> &addresses) :
        checkSum(checkSum),
        checkSumPatched(checkSumPatched),
        addresses(addresses) {}

    const char* getCheckSum() const {
        return checkSum;
    }

    const char* getCheckSumPatched() const {
        return checkSumPatched;
    }

    QList<unsigned int> getAddresses() const {
        return addresses;
    }

private:
    const char* checkSum;
    const char* checkSumPatched;
    QList<unsigned int> addresses;
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
