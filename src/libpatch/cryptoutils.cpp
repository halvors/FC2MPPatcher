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
        //const char salt[SALT_LENGTH] = { 'Y', 'q', '1', 'c', 'K', 'x', 'q', 'z', 'H', 'N', '3', 's', 'k', 'C', '9', 'l' };
        const char salt[] = "Yq1cKxqzHN3skC9l";

        if (argon2i_hash_raw(timeCost, memoryCost, parallelism, password.constData(), password.size(), salt, SALT_LENGTH, hash.data(), hash.size()) == ARGON2_OK)
            return hash;
    }

    return QByteArray();
}
