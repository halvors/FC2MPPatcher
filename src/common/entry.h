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

    CodeEntry(uint32_t address, uint32_t word, const char *section = ".text", Type type = INJECT_SYMBOL) :
        CodeEntry(address, QByteArray::number(word), section, type) {}

    CodeEntry(uint32_t address, const QByteArray &data, const char *section = ".text", Type type = INJECT_DATA) :
        address(address),
        data(data),
        section(section),
        type(type) {}

    CodeEntry(const QByteArray &data, const char *section = ".text_mp", Type type = NEW_DATA) :
        data(data),
        section(section),
        type(type) {}

    uint32_t getAddress() const {
        return address;
    }

    QByteArray getData() const {
        return data;
    }

    const char *getSection() const {
        return section;
    }

    Type getType() const {
        return type;
    }

private:
    uint32_t address = 0;
    QByteArray data;
    const char *section;
    Type type;
};

struct HashEntry {
    const char *original;
    const char *result;
};

class TargetEntry {
public:
    TargetEntry(const QList<HashEntry> &hashentries, const QList<CodeEntry> &codeentries) :
        hashentries(hashentries),
        codeentries(codeentries) {}

    const QList<HashEntry> &getHashEntries() const {
        return hashentries;
    }

    const QList<CodeEntry> getCodeEntries() const {
        return codeentries;
    }

private:
    const QList<HashEntry> hashentries;
    const QList<CodeEntry> codeentries;
};

class FileEntry {
public:
    FileEntry(const char *name, const QList<TargetEntry> &targets) :
        name(name),
        targets(targets) {}

    const char *getName() const {
        return name;
    }

    const QList<TargetEntry> &getTargets() const {
        return targets;
    }

private:
    const char *name;
    const QList<TargetEntry> targets;
};

#endif // ENTRY_H
