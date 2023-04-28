#include "cryptoutils.h"

#include <argon2.h>

const char salt[] = "Yq1cKxqzHN3skC9l";

QByteArray hashPassword(const QByteArray &password)
{
    if (!password.isEmpty()) {
        QByteArray hash(HASH_LENGTH, 0);

        if (argon2id_hash_raw(TIME_COST, MEMORY_COST, PARALLELISM, password.constData(), password.size(), salt, SALT_LENGTH, hash.data(), hash.size()) == ARGON2_OK)
            return hash;
    }

    return QByteArray();
}
