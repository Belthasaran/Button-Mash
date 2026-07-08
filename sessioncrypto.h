#ifndef SESSIONCRYPTO_H
#define SESSIONCRYPTO_H

#include <QByteArray>

// Ed25519 session keying for signed sync headers (OpenSSL libcrypto).
namespace SessionCrypto {

constexpr int kSeedBytes = 32;
constexpr int kPubKeyBytes = 32;
constexpr int kPrivSeedBytes = 32; // OpenSSL raw private key material for Ed25519
constexpr int kSigBytes = 64;
constexpr int kSessionIdBytes = 16;
constexpr int kSha256Bytes = 32;

struct KeyPair {
    QByteArray publicKey;  // 32
    QByteArray privateKey; // 32 seed (OpenSSL raw private)
    QByteArray seed;       // 32 crypto RNG seed used at mint (for wipe tracking)
    bool valid() const
    {
        return publicKey.size() == kPubKeyBytes && privateKey.size() == kPrivSeedBytes;
    }
};

bool mintKeyPair(KeyPair *out);
QByteArray sessionIdFromPublicKey(const QByteArray &publicKey);
QByteArray sha256(const QByteArray &data);
QByteArray signEd25519(const QByteArray &privateKey, const QByteArray &message);
bool verifyEd25519(const QByteArray &publicKey, const QByteArray &message, const QByteArray &signature);
void secureWipe(QByteArray *buf);
void wipeKeyPair(KeyPair *kp);

} // namespace SessionCrypto

#endif
