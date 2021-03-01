#ifndef ENTRY_H
#define ENTRY_H

#include <QList>
#include <QByteArray>

struct HashEntry {
    const QByteArray original;
    const QByteArray result;
};

class CodeEntry {
public:
    enum Type {
        INJECT_DATA,
        INJECT_SYMBOL,
        NEW_DATA
    };

    CodeEntry(uint32_t address, uint32_t word, const char *section = ".text", const enum Type &type = INJECT_SYMBOL) :
        CodeEntry(address, QByteArray::number(word), section, type) {}

    CodeEntry(const QByteArray &data, const char *section = ".text_mp", const enum Type &type = NEW_DATA) :
        CodeEntry(0, data, section, type) {}

    CodeEntry(uint32_t address, const QByteArray &data, const char *section = ".text", const enum Type &type = INJECT_DATA) :
        address(address),
        data(data),
        section(section),
        type(type) {}

    uint32_t getAddress() const {
        return address;
    }

    const QByteArray &getData() const {
        return data;
    }

    const char *getSection() const {
        return section;
    }

    const Type &getType() const {
        return type;
    }

private:
    const uint32_t address = 0;
    const QByteArray data;
    const char *section;
    const enum Type type;
};

struct TargetEntry {
    const QList<HashEntry> hashEntries;
    const QList<QByteArray> upgradeHashEntries;
    const QList<CodeEntry> codeEntries;
};

struct FileEntry {
    const char *name;
    const QList<TargetEntry> targets;
};

#endif // ENTRY_H
