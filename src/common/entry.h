#ifndef ENTRY_H
#define ENTRY_H

#include <QList>

class AddressEntry {
public:
    AddressEntry(unsigned int address, unsigned int code, bool function = true) :
        address(address),
        value(code),
        function(function) {}

    unsigned int getAddress() const {
        return address;
    }

    unsigned int getValue() const {
        return value;
    }

    bool isFunction() const {
        return function;
    }

private:
    unsigned int address;
    unsigned int value;
    bool function;
};

class TargetEntry {
public:
    TargetEntry(const char* checkSum, const char* checkSumPatched, const QList<AddressEntry> &functions) :
        checkSum(checkSum),
        checkSumPatched(checkSumPatched),
        addresses(functions) {}

    const char* getCheckSum() const {
        return checkSum;
    }

    const char* getCheckSumPatched() const {
        return checkSumPatched;
    }

    QList<AddressEntry> getAddresses() const {
        return addresses;
    }

private:
    const char* checkSum;
    const char* checkSumPatched;
    QList<AddressEntry> addresses;
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
