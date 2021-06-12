#include "cryptoutils.h"

#include <QByteArray>
#include <argon2.h>

const char CryptoUtils::salt[] = "Yq1cKxqzHN3skC9l";

QByteArray CryptoUtils::hashPassword(const QByteArray &password)
{
    if (!password.isEmpty()) {
        QByteArray hash(HASH_LENGTH, 0);

        if (argon2i_hash_raw(TIME_COST, MEMORY_COST, PARALLELISM, password.constData(), password.size(), salt, SALT_LENGTH, hash.data(), hash.size()) == ARGON2_OK)
            return hash;
    }

    return QByteArray();
}
