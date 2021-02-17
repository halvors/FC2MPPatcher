#ifndef ENTRY_H
#define ENTRY_H

#include <QList>
#include <QByteArray>

class AddressEntry {
public:
    AddressEntry(unsigned int address, unsigned int word, const QString &section = ".text") :
        AddressEntry(address, QByteArray::number(word), section, true) {}

    AddressEntry(unsigned int address, const QString &data, const QString &section = ".text") :
        AddressEntry(address, data.toUtf8(), section, false) {}

    AddressEntry(unsigned int address, const QByteArray &value, const QString &section, bool symbol) :
        address(address),
        value(value),
        section(section),
        symbol(symbol) {}

    unsigned int getAddress() const {
        return address;
    }

    QByteArray getValue() const {
        return value;
    }

    QString getSection() const {
        return section;
    }

    bool isSymbol() const {
        return symbol;
    }

private:
    unsigned int address;
    QByteArray value;
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
