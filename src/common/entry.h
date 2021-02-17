#ifndef ENTRY_H
#define ENTRY_H

#include <QList>
#include <QByteArray>

class AddressEntry {
public:
    AddressEntry(unsigned int address, unsigned int word, const QString &section = ".text") :
        AddressEntry(address, QByteArray::number(word), section, true) {}

    AddressEntry(unsigned int address, const QByteArray &data, const QString &section = ".text", bool symbol = false) :
        address(address),
        data(data),
        section(section),
        symbol(symbol) {}

    unsigned int getAddress() const {
        return address;
    }

    QByteArray getData() const {
        return data;
    }

    QString getSection() const {
        return section;
    }

    bool isSymbol() const {
        return symbol;
    }

private:
    unsigned int address;
    QByteArray data;
    QString section;
    bool symbol;
};

class TargetEntry {
public:
    TargetEntry(const char *checkSum, const char *checkSumPatched, const QList<AddressEntry> &functions) :
        checkSum(checkSum),
        checkSumPatched(checkSumPatched),
        addresses(functions) {}

    const char *getCheckSum() const {
        return checkSum;
    }

    const char *getCheckSumPatched() const {
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
    FileEntry(const char *name, const QList<TargetEntry> &targets) :
        name(name),
        targets(targets) {}

    const char *getName() const {
        return name;
    }

    QList<TargetEntry> getTargets() const {
        return targets;
    }

private:
    const char *name;
    QList<TargetEntry> targets;
};

#endif // ENTRY_H
