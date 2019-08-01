#ifndef ENTRY_H
#define ENTRY_H

#include <QString>
#include <QList>

class FunctionEntry {
public:
    FunctionEntry(unsigned int address, const QString &name) :
        address(address),
        name(name) {}

    unsigned int getAddress() const {
        return address;
    }

    QString getName() const {
        return name;
    }

private:
    unsigned int address;
    QString name;
};

class TargetEntry {
public:
    TargetEntry(const QString &fileCheckSum, const QString &fileCheckSumPatched, QList<FunctionEntry> functions) :
        fileCheckSum(fileCheckSum),
        fileCheckSumPatched(fileCheckSumPatched),
        functions(functions) {}

    QString getFileCheckSum() const {
        return fileCheckSum;
    }

    QString getFileCheckSumPatched() const {
        return fileCheckSumPatched;
    }

    QList<FunctionEntry> getFunctions() const {
        return functions;
    }

private:
    QString fileCheckSum;
    QString fileCheckSumPatched;
    QList<FunctionEntry> functions;
};

class FileEntry {
public:
    FileEntry(const QString &fileName, QList<TargetEntry> targets) :
        fileName(fileName),
        targets(targets) {}

    QString getFileName() const {
        return fileName;
    }

    QList<TargetEntry> getTargets() const {
        return targets;
    }

private:
    QString fileName;
    QList<TargetEntry> targets;
};


#endif // ENTRY_H
