#include "cryptoutils.h"

#include <QByteArray>
#include <argon2.h>

#define HASH_LENGTH 32
#define SALT_LENGTH 16

QByteArray CryptoUtils::hashPassword(const QByteArray &password)
{
    if (!password.isEmpty()) {
        QByteArray hash(HASH_LENGTH, 0);

        const uint32_t timeCost = 3;      // 3-pass computation.
        const uint32_t memoryCost = 4096; // 4 mebibytes memory usage.
        const uint32_t parallelism = 1;   // number of threads and lanes.
        //const uint8_t salt[SALT_LENGTH] = { 0x9f, 0xfa, 0x6a, 0x64, 0x23, 0x39, 0xf7, 0x95, 0xb0, 0x87, 0x2f, 0x07, 0x0a, 0x02, 0x78, 0xe8 };
        const char salt[SALT_LENGTH] = { 'Y', 'q', '1', 'c', 'K', 'x', 'q', 'z', 'H', 'N', '3', 's', 'k', 'C', '9', 'l' };

        if (argon2i_hash_raw(timeCost, memoryCost, parallelism, password.constData(), password.size(), salt, SALT_LENGTH, hash.data(), hash.size()) == ARGON2_OK)
            return hash;
    }

    return QByteArray();
}
