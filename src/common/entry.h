#ifndef ENTRY_H
#define ENTRY_H

#include <QString>
#include <QList>

class TargetEntry {
public:
    TargetEntry(const QString &checkSum, const QString &checkSumPatched, const QList<unsigned int> &addresses) :
        checkSum(checkSum),
        checkSumPatched(checkSumPatched),
        addresses(addresses) {}

    QString getCheckSum() const {
        return checkSum;
    }

    QString getCheckSumPatched() const {
        return checkSumPatched;
    }

    QList<unsigned int> getAddresses() const {
        return addresses;
    }

private:
    QString checkSum;
    QString checkSumPatched;
    QList<unsigned int> addresses;
};

class FileEntry {
public:
    FileEntry(const QString &name, const QList<TargetEntry> &targets) :
        name(name),
        targets(targets) {}

    QString getName() const {
        return name;
    }

    QList<TargetEntry> getTargets() const {
        return targets;
    }

private:
    QString name;
    QList<TargetEntry> targets;
};

#endif // ENTRY_H
