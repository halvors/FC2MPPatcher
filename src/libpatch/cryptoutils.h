#ifndef CRYPTOUTILS_H
#define CRYPTOUTILS_H

class QByteArray;

struct CryptoUtils
{
    static QByteArray hashPassword(const QByteArray &password);
};

#endif // CRYPTOUTILS_H
