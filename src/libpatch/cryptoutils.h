#ifndef CRYPTOUTILS_H
#define CRYPTOUTILS_H



#define HASH_LENGTH 32

#define TIME_COST 3         // 3-pass computation.
#define MEMORY_COST 2 << 11 // 4 mebibytes memory usage.
#define PARALLELISM 1       // number of threads and lanes.
#define SALT_LENGTH 16

#include <cstdint>

#include <QByteArray>

class CryptoUtils
{
public:
    static QByteArray hashPassword(const QByteArray &password);

private:
    static const char salt[];
};

#endif // CRYPTOUTILS_H
