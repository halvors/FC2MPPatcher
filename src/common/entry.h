#ifndef ENTRY_H
#define ENTRY_H

#include <string>

#include <QList>

struct HashEntry {
    std::string original;
    std::string result;
};

class CodeEntry {
public:
    enum Type {
        INJECT_DATA,
        INJECT_SYMBOL,
        NEW_DATA
    };

    CodeEntry(const uint32_t address, const uint32_t word, const char *section = ".text", Type type = INJECT_SYMBOL) :
        CodeEntry(address, std::to_string(word), section, type) {}

    CodeEntry(const std::string &data, const char *section = ".text_mp", Type type = NEW_DATA) :
        CodeEntry(0, data, section, type) {}

    CodeEntry(const uint32_t address, const std::string &data, const char *section = ".text", Type type = INJECT_DATA) :
        address(address),
        data(data),
        section(section),
        type(type) {}

    uint32_t getAddress() const {
        return address;
    }

    const std::string &getData() const {
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
    std::string data;
    const char *section;
    Type type;
};

struct TargetEntry {
    QList<HashEntry> hashEntries;
    QList<std::string> legacyHashEntries;
    QList<CodeEntry> codeEntries;
};

struct FileEntry {
    const char *name;
    QList<TargetEntry> targets;
};

#endif // ENTRY_H
