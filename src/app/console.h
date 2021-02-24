#ifndef CONSOLE_H
#define CONSOLE_H

#include <QObject>

class Console : public QObject
{
    Q_OBJECT

public:
    explicit Console(QObject *parent = nullptr);
    ~Console();

    int exec(const QString &path);
};

#endif // CONSOLE_H
