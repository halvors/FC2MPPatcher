#ifndef ENTRY_H
#define ENTRY_H

#include <QList>
#include <QByteArray>

struct HashEntry {
    QByteArray original;
    QByteArray result;
};

class CodeEntry {
public:
    enum Type {
        INJECT_DATA,
        INJECT_SYMBOL,
        NEW_DATA
    };

    CodeEntry(const uint32_t address, const uint32_t word, const char *section = ".text", Type type = INJECT_SYMBOL) :
        CodeEntry(address, QByteArray::number(word), section, type) {}

    CodeEntry(const QByteArray &data, const char *section = ".text_mp", Type type = NEW_DATA) :
        CodeEntry(0, data, section, type) {}

    CodeEntry(const uint32_t address, const QByteArray &data, const char *section = ".text", Type type = INJECT_DATA) :
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
    uint32_t address = 0;
    QByteArray data;
    const char *section;
    Type type;
};

struct TargetEntry {
    QList<HashEntry> hashEntries;
    QList<QByteArray> legacyHashEntries;
    QList<CodeEntry> codeEntries;
};

struct FileEntry {
    const char *name;
    QList<TargetEntry> targets;
};

#endif // ENTRY_H
