#include "cryptoutils.h"

#include <QByteArray>
#include <argon2.h>

const char CryptoUtils::salt[] = "Yq1cKxqzHN3skC9l";

QByteArray CryptoUtils::hashPassword(const QByteArray &password)
{
    if (!password.isEmpty()) {
        QByteArray hash(HASH_LENGTH, 0);

<<<<<<< HEAD
        if (argon2i_hash_raw(TIME_COST, MEMORY_COST, PARALLELISM, password.constData(), password.size(), salt, SALT_LENGTH, hash.data(), hash.size()) == ARGON2_OK)
=======
        const uint32_t timeCost = 3;      // 3-pass computation.
        const uint32_t memoryCost = 4096; // 4 mebibytes memory usage.
        const uint32_t parallelism = 1;   // number of threads and lanes.
        //const char salt[SALT_LENGTH] = { 'Y', 'q', '1', 'c', 'K', 'x', 'q', 'z', 'H', 'N', '3', 's', 'k', 'C', '9', 'l' };
        const char salt[] = "Yq1cKxqzHN3skC9l";

        if (argon2i_hash_raw(timeCost, memoryCost, parallelism, password.constData(), password.size(), salt, SALT_LENGTH, hash.data(), hash.size()) == ARGON2_OK)
>>>>>>> 57c6baa097ca9266751ba1bb0129810f4f9b07df
            return hash;
    }

    return QByteArray();
}
