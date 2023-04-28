#ifndef CRYPTOUTILS_H
#define CRYPTOUTILS_H

#define HASH_LENGTH 32

#define TIME_COST 3          // 3-pass computation.
#define MEMORY_COST 4 * 1024 // 4 mebibytes memory usage.
#define PARALLELISM 1        // number of threads and lanes.
#define SALT_LENGTH 16

#include <cstdint>

#include <QByteArray>

QByteArray hashPassword(const QByteArray &password);

#endif // CRYPTOUTILS_H
