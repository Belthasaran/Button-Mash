#include "sessioncrypto.h"

#include <QCryptographicHash>
#include <cstring>

#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/crypto.h>

namespace SessionCrypto {

bool mintKeyPair(KeyPair *out)
{
    if (!out)
        return false;
    wipeKeyPair(out);

    QByteArray seed(kSeedBytes, Qt::Uninitialized);
    if (RAND_bytes(reinterpret_cast<unsigned char *>(seed.data()), kSeedBytes) != 1)
        return false;

    EVP_PKEY *pkey = nullptr;
    EVP_PKEY_CTX *pctx = EVP_PKEY_CTX_new_id(EVP_PKEY_ED25519, nullptr);
    if (!pctx)
        return false;
    bool ok = false;
    if (EVP_PKEY_keygen_init(pctx) == 1 && EVP_PKEY_keygen(pctx, &pkey) == 1 && pkey) {
        QByteArray pub(kPubKeyBytes, Qt::Uninitialized);
        size_t pubLen = size_t(kPubKeyBytes);
        QByteArray priv(kPrivSeedBytes, Qt::Uninitialized);
        size_t privLen = size_t(kPrivSeedBytes);
        if (EVP_PKEY_get_raw_public_key(pkey, reinterpret_cast<unsigned char *>(pub.data()), &pubLen) == 1
            && pubLen == size_t(kPubKeyBytes)
            && EVP_PKEY_get_raw_private_key(pkey, reinterpret_cast<unsigned char *>(priv.data()), &privLen) == 1
            && privLen == size_t(kPrivSeedBytes)) {
            out->publicKey = pub;
            out->privateKey = priv;
            out->seed = seed;
            ok = true;
        }
    }
    EVP_PKEY_free(pkey);
    EVP_PKEY_CTX_free(pctx);
    if (!ok)
        secureWipe(&seed);
    return ok;
}

QByteArray sessionIdFromPublicKey(const QByteArray &publicKey)
{
    if (publicKey.size() != kPubKeyBytes)
        return QByteArray();
    QByteArray full = QCryptographicHash::hash(publicKey, QCryptographicHash::Sha3_256);
    return full.left(kSessionIdBytes);
}

QByteArray sha256(const QByteArray &data)
{
    return QCryptographicHash::hash(data, QCryptographicHash::Sha256);
}

QByteArray signEd25519(const QByteArray &privateKey, const QByteArray &message)
{
    if (privateKey.size() != kPrivSeedBytes)
        return QByteArray();
    EVP_PKEY *pkey = EVP_PKEY_new_raw_private_key(
        EVP_PKEY_ED25519, nullptr,
        reinterpret_cast<const unsigned char *>(privateKey.constData()),
        size_t(privateKey.size()));
    if (!pkey)
        return QByteArray();

    EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
    QByteArray sig;
    size_t sigLen = 0;
    bool ok = false;
    if (mdctx && EVP_DigestSignInit(mdctx, nullptr, nullptr, nullptr, pkey) == 1
        && EVP_DigestSign(mdctx, nullptr, &sigLen,
                          reinterpret_cast<const unsigned char *>(message.constData()),
                          size_t(message.size()))
            == 1) {
        sig.resize(int(sigLen));
        if (EVP_DigestSign(mdctx, reinterpret_cast<unsigned char *>(sig.data()), &sigLen,
                           reinterpret_cast<const unsigned char *>(message.constData()),
                           size_t(message.size()))
                == 1
            && sigLen == size_t(kSigBytes)) {
            sig.resize(int(sigLen));
            ok = true;
        }
    }
    EVP_MD_CTX_free(mdctx);
    EVP_PKEY_free(pkey);
    return ok ? sig : QByteArray();
}

bool verifyEd25519(const QByteArray &publicKey, const QByteArray &message, const QByteArray &signature)
{
    if (publicKey.size() != kPubKeyBytes || signature.size() != kSigBytes)
        return false;
    EVP_PKEY *pkey = EVP_PKEY_new_raw_public_key(
        EVP_PKEY_ED25519, nullptr,
        reinterpret_cast<const unsigned char *>(publicKey.constData()),
        size_t(publicKey.size()));
    if (!pkey)
        return false;
    EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
    bool ok = mdctx
        && EVP_DigestVerifyInit(mdctx, nullptr, nullptr, nullptr, pkey) == 1
        && EVP_DigestVerify(mdctx,
                            reinterpret_cast<const unsigned char *>(signature.constData()),
                            size_t(signature.size()),
                            reinterpret_cast<const unsigned char *>(message.constData()),
                            size_t(message.size()))
            == 1;
    EVP_MD_CTX_free(mdctx);
    EVP_PKEY_free(pkey);
    return ok;
}

void secureWipe(QByteArray *buf)
{
    if (!buf || buf->isEmpty())
        return;
    OPENSSL_cleanse(buf->data(), size_t(buf->size()));
    buf->clear();
}

void wipeKeyPair(KeyPair *kp)
{
    if (!kp)
        return;
    secureWipe(&kp->privateKey);
    secureWipe(&kp->seed);
    // Public key is not secret but clear for consistent lifetime.
    kp->publicKey.clear();
}

} // namespace SessionCrypto
