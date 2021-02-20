#ifndef ENTRY_H
#define ENTRY_H

#include <QList>
#include <QByteArray>

class CodeEntry {
public:
    enum Type {
        INJECT_DATA,
        INJECT_SYMBOL,
        NEW_DATA
    };

    CodeEntry(unsigned int address, unsigned int word, const QString &section = ".text", Type type = INJECT_SYMBOL) :
        CodeEntry(address, QByteArray::number(word), section, type) {}

    CodeEntry(unsigned int address, const QByteArray &data, const QString &section = ".text", Type type = INJECT_DATA) :
        address(address),
        data(data),
        section(section),
        type(type) {}

    CodeEntry(const QByteArray &data, const QString &section = ".text_mp", Type type = NEW_DATA) :
        data(data),
        section(section),
        type(type) {}

    unsigned int getAddress() const {
        return address;
    }

    QByteArray getData() const {
        return data;
    }

    QString getSection() const {
        return section;
    }

    Type getType() const {
        return type;
    }

private:
    unsigned int address = 0;
    QByteArray data;
    QString section;
    Type type;
};

class TargetEntry {
public:
    TargetEntry(const char *checkSum, const char *checkSumPatched, const QList<CodeEntry> &functions) :
        checkSum(checkSum),
        checkSumPatched(checkSumPatched),
        addresses(functions) {}

    const char *getCheckSum() const {
        return checkSum;
    }

    const char *getCheckSumPatched() const {
        return checkSumPatched;
    }

    QList<CodeEntry> getCodeEntries() const {
        return addresses;
    }

private:
    const char* checkSum;
    const char* checkSumPatched;
    QList<CodeEntry> addresses;
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
