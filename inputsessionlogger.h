#ifndef INPUTSESSIONLOGGER_H
#define INPUTSESSIONLOGGER_H

#include "inputsessionstate.h"
#include "sessioncrypto.h"
#include <QFile>
#include <QObject>
#include <QTimer>
#include <QVector>
#include <array>

struct RollingInputRecord
{
    quint64 timestampMs = 0;
    quint32 seq = 0;
    quint64 deltaMs = 0;
    quint16 before = 0;
    quint16 after = 0;
    quint16 pressed = 0;
    quint16 released = 0;
    QString pressedString;
    QString releasedString;
    QString heldString;
};

struct RollingSyncRecord
{
    quint64 timestampMs = 0;
    quint64 entriesSinceLastSync = 0;
    std::array<quint16, 16> pressSession {};
    std::array<quint16, 16> releaseSession {};
    std::array<quint32, 16> heldSession {};
    std::array<quint16, 16> pressSinceSync {};
    std::array<quint16, 16> releaseSinceSync {};
    std::array<quint32, 16> heldSinceSync {};
};

class InputSessionLogger : public QObject
{
    Q_OBJECT
public:
    explicit InputSessionLogger(QObject *parent = nullptr);
    ~InputSessionLogger() override;

    InputSessionState &state() { return m_state; }
    const InputSessionState &state() const { return m_state; }

    void setBinLog(bool enabled, const QString &path);
    void setFullTextLog(bool enabled, const QString &path);
    void setSyncOnlyTextLog(bool enabled, const QString &path);
    void setLastNDashboard(bool enabled, const QString &basePath, int n = 10);

    bool binEnabled() const { return m_binEnabled; }
    bool fullTextEnabled() const { return m_fullTextEnabled; }
    bool syncOnlyTextEnabled() const { return m_syncOnlyTextEnabled; }
    bool lastNEnabled() const { return m_lastNEnabled; }
    QString binPath() const { return m_binPath; }
    QString fullTextPath() const { return m_fullTextPath; }
    QString syncOnlyTextPath() const { return m_syncOnlyTextPath; }
    QString lastNBasePath() const { return m_lastNBasePath; }
    int lastN() const { return m_lastN; }

    const QVector<RollingInputRecord> &rollingInputs() const { return m_rollingInputs; }
    const QVector<RollingSyncRecord> &rollingSyncs() const { return m_rollingSyncs; }

    void startLogging();
    void stopLogging();
    bool isLogging() const { return m_active; }

    void onButton(InputProvider::SNESButton button, bool pressed);
    void forceSyncHeader();

    // Test helpers
    QByteArray encodeSyncHeaderForTest(quint64 nowMs);
    QByteArray encodeInputEntryForTest(quint64 nowMs, quint16 before, quint16 after,
                                       quint16 pressed, quint16 released);
    void onButtonAt(InputProvider::SNESButton button, bool pressed, quint64 nowMs);
    void maybeRotateIdleAt(quint64 nowMs);
    bool maybeWriteActivitySyncAt(quint64 nowMs);
    int syncHeaderWriteCount() const { return m_syncHeaderWriteCount; }
    int ordinaryEntriesSinceSync() const { return m_ordinarySinceSync; }
    QByteArray publicKeyForTest() const { return m_keys.publicKey; }
    bool hasSigningKeys() const { return m_keys.valid(); }
    QByteArray lastSyncHashForTest() const { return m_lastSyncHash; }
    QByteArray lastSyncSigForTest() const { return m_lastSyncSig; }
    const QByteArray &entriesSinceSyncBufferForTest() const { return m_entriesSinceSync; }

    // Compile-time: closeout sync + 15s activity syncs (until a user setting exists).
    static constexpr bool kExtraIdleSyncsEnabled = true;
    static constexpr quint64 kIdleSessionMs = 5ull * 60ull * 1000ull;
    static constexpr quint64 kSyncIntervalMs = 5ull * 60ull * 1000ull;
    static constexpr quint64 kHeartbeatMs = 60ull * 1000ull;
    static constexpr quint64 kActivitySyncMs = 15ull * 1000ull;
    static constexpr int kMaxOrdinaryBetweenSync = 200;

private slots:
    void onTick();

private:
    void openLogsFresh();
    void closeLogs();
    bool beginSessionKeys(quint64 nowMs);
    void wipeSessionKeys();
    void writeSyncHeader(quint64 nowMs);
    void writeInputEntry(quint64 nowMs, quint16 before, quint16 after, quint16 pressed, quint16 released, bool isHeartbeat);
    void maybeRotateIdle(quint64 nowMs);
    bool maybeWriteActivitySync(quint64 nowMs);
    void maybeForceSyncByCount(quint64 nowMs);
    QByteArray buildSyncHeader(quint64 nowMs);
    QByteArray buildInputEntry(quint64 nowMs, quint16 before, quint16 after, quint16 pressed, quint16 released);
    void appendBin(const QByteArray &entry);
    void appendFullText(const QString &line);
    void appendSyncText(const QString &line);
    QString formatPressedList(const std::array<quint16, 16> &arr) const;
    QString formatHeld32(const std::array<quint32, 16> &arr) const;
    QString formatHeld64(const std::array<quint64, 16> &arr) const;

    void pushInputRolling(const RollingInputRecord &rec);
    void pushSyncRolling(const RollingSyncRecord &rec);
    void rewriteInputRollingFiles();
    void rewriteSyncRollingFiles();
    QString lastNStem() const;
    static bool writeAtomicText(const QString &path, const QByteArray &data);
    static QString hex4(quint16 v);
    static QString buttonCountJsonObjectU16(const std::array<quint16, 16> &arr);
    static QString buttonCountJsonObjectU32(const std::array<quint32, 16> &arr);
    static QString formatSyncTimestamp(quint64 ms);
    void snapshotSyncCounters();
    static int digitWidth(quint64 v);

    InputSessionState m_state;
    SessionCrypto::KeyPair m_keys;
    QTimer m_timer;
    bool m_active;

    bool m_binEnabled;
    bool m_fullTextEnabled;
    bool m_syncOnlyTextEnabled;
    bool m_lastNEnabled;
    QString m_binPath;
    QString m_fullTextPath;
    QString m_syncOnlyTextPath;
    QString m_lastNBasePath;
    int m_lastN;
    QFile m_binFile;
    QFile m_fullTextFile;
    QFile m_syncTextFile;

    QByteArray m_prevEntryHash;
    QByteArray m_entriesSinceSync;
    int m_ordinarySinceSync;
    QByteArray m_lastSyncHash;
    QByteArray m_lastSyncSig;
    quint64 m_lastSyncMs;
    quint64 m_lastHeartbeatMs;
    quint64 m_lastInputEntryMs;
    quint64 m_inputChangesAtLastSync;
    int m_syncHeaderWriteCount;

    std::array<quint16, 16> m_pressAtLastSync;
    std::array<quint16, 16> m_releaseAtLastSync;
    std::array<quint32, 16> m_heldAtLastSync;

    QVector<RollingInputRecord> m_rollingInputs;
    QVector<RollingSyncRecord> m_rollingSyncs;
};

#endif
