#include "inputsessionlogger.h"
#include "snesbitorder.h"
#include <QDateTime>
#include <QFileInfo>
#include <QIODevice>
#include <QSaveFile>
#include <QStringList>
#include <QtEndian>
#include <cstring>

#include "third_party/xxhash.h"

static const char kSyncMagic[8] = {
    'B', 'M', char(0xAA), char(0x55), char(0xAA), char(0x55), char(0xAA), char(0x55)
};

static void appendU16BE(QByteArray &out, quint16 v)
{
    quint16 be = qToBigEndian(v);
    out.append(reinterpret_cast<const char *>(&be), 2);
}

static void appendU32BE(QByteArray &out, quint32 v)
{
    quint32 be = qToBigEndian(v);
    out.append(reinterpret_cast<const char *>(&be), 4);
}

static void appendU64BE(QByteArray &out, quint64 v)
{
    quint64 be = qToBigEndian(v);
    out.append(reinterpret_cast<const char *>(&be), 8);
}

static QByteArray xxh3_64(const QByteArray &data)
{
    XXH64_hash_t h = XXH3_64bits(data.constData(), size_t(data.size()));
    QByteArray out(8, '\0');
    quint64 be = qToBigEndian(quint64(h));
    std::memcpy(out.data(), &be, 8);
    return out;
}

InputSessionLogger::InputSessionLogger(QObject *parent)
    : QObject(parent)
    , m_active(false)
    , m_binEnabled(false)
    , m_fullTextEnabled(false)
    , m_syncOnlyTextEnabled(false)
    , m_lastNEnabled(false)
    , m_lastN(10)
    , m_ordinarySinceSync(0)
    , m_lastSyncMs(0)
    , m_lastHeartbeatMs(0)
    , m_lastInputEntryMs(0)
    , m_inputChangesAtLastSync(0)
    , m_syncHeaderWriteCount(0)
{
    m_prevEntryHash = QByteArray(8, '\0');
    m_pressAtLastSync.fill(0);
    m_releaseAtLastSync.fill(0);
    m_heldAtLastSync.fill(0);
    m_timer.setInterval(1000);
    connect(&m_timer, &QTimer::timeout, this, &InputSessionLogger::onTick);
}

InputSessionLogger::~InputSessionLogger()
{
    stopLogging();
    wipeSessionKeys();
}

void InputSessionLogger::setBinLog(bool enabled, const QString &path)
{
    m_binEnabled = enabled;
    m_binPath = path;
}

void InputSessionLogger::setFullTextLog(bool enabled, const QString &path)
{
    m_fullTextEnabled = enabled;
    m_fullTextPath = path;
}

void InputSessionLogger::setSyncOnlyTextLog(bool enabled, const QString &path)
{
    m_syncOnlyTextEnabled = enabled;
    m_syncOnlyTextPath = path;
}

void InputSessionLogger::setLastNDashboard(bool enabled, const QString &basePath, int n)
{
    m_lastNEnabled = enabled;
    m_lastNBasePath = basePath;
    m_lastN = qBound(1, n, 500);
}

void InputSessionLogger::openLogsFresh()
{
    closeLogs();
    if (m_binEnabled && !m_binPath.isEmpty()) {
        m_binFile.setFileName(m_binPath);
        m_binFile.open(QIODevice::WriteOnly | QIODevice::Truncate);
    }
    if (m_fullTextEnabled && !m_fullTextPath.isEmpty()) {
        m_fullTextFile.setFileName(m_fullTextPath);
        m_fullTextFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text);
    }
    if (m_syncOnlyTextEnabled && !m_syncOnlyTextPath.isEmpty()) {
        m_syncTextFile.setFileName(m_syncOnlyTextPath);
        m_syncTextFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text);
    }
}

void InputSessionLogger::closeLogs()
{
    if (m_binFile.isOpen())
        m_binFile.close();
    if (m_fullTextFile.isOpen())
        m_fullTextFile.close();
    if (m_syncTextFile.isOpen())
        m_syncTextFile.close();
}

void InputSessionLogger::startLogging()
{
    if (!(m_binEnabled || m_fullTextEnabled || m_syncOnlyTextEnabled || m_lastNEnabled))
        return;
    const quint64 now = quint64(QDateTime::currentMSecsSinceEpoch());
    if (!beginSessionKeys(now))
        return;
    m_prevEntryHash = QByteArray(8, '\0');
    m_entriesSinceSync.clear();
    m_ordinarySinceSync = 0;
    m_lastSyncHash.clear();
    m_lastSyncSig.clear();
    m_rollingInputs.clear();
    m_rollingSyncs.clear();
    m_lastInputEntryMs = 0;
    m_inputChangesAtLastSync = 0;
    m_pressAtLastSync.fill(0);
    m_releaseAtLastSync.fill(0);
    m_heldAtLastSync.fill(0);
    m_syncHeaderWriteCount = 0;
    openLogsFresh();
    m_active = true;
    m_lastSyncMs = now;
    m_lastHeartbeatMs = now;
    writeSyncHeader(now);
    m_timer.start();
}

void InputSessionLogger::stopLogging()
{
    if (m_active && kExtraIdleSyncsEnabled)
        writeSyncHeader(quint64(QDateTime::currentMSecsSinceEpoch()));
    m_timer.stop();
    m_active = false;
    closeLogs();
    wipeSessionKeys();
}

bool InputSessionLogger::beginSessionKeys(quint64 nowMs)
{
    wipeSessionKeys();
    if (!SessionCrypto::mintKeyPair(&m_keys))
        return false;
    const QByteArray sid = SessionCrypto::sessionIdFromPublicKey(m_keys.publicKey);
    if (sid.size() != SessionCrypto::kSessionIdBytes) {
        wipeSessionKeys();
        return false;
    }
    m_state.resetSession(nowMs);
    m_state.setSessionId(sid);
    return true;
}

void InputSessionLogger::wipeSessionKeys()
{
    SessionCrypto::wipeKeyPair(&m_keys);
}

void InputSessionLogger::appendBin(const QByteArray &entry)
{
    if (m_binFile.isOpen()) {
        m_binFile.write(entry);
        m_binFile.flush();
    }
}

void InputSessionLogger::appendFullText(const QString &line)
{
    if (m_fullTextFile.isOpen()) {
        m_fullTextFile.write(line.toUtf8());
        m_fullTextFile.write("\n");
        m_fullTextFile.flush();
    }
}

void InputSessionLogger::appendSyncText(const QString &line)
{
    if (m_syncTextFile.isOpen()) {
        m_syncTextFile.write(line.toUtf8());
        m_syncTextFile.write("\n");
        m_syncTextFile.flush();
    }
}

QString InputSessionLogger::formatPressedList(const std::array<quint16, 16> &arr) const
{
    QStringList parts;
    for (int bit = 15; bit >= 4; --bit) {
        QString name = SnesBitOrder::nameForBitIndex(bit);
        if (!name.isEmpty())
            parts << QStringLiteral("%1=%2").arg(name).arg(arr[size_t(bit)]);
    }
    return parts.join(QLatin1Char(' '));
}

QString InputSessionLogger::formatHeld32(const std::array<quint32, 16> &arr) const
{
    QStringList parts;
    for (int bit = 15; bit >= 4; --bit) {
        QString name = SnesBitOrder::nameForBitIndex(bit);
        if (!name.isEmpty())
            parts << QStringLiteral("%1=%2").arg(name).arg(arr[size_t(bit)]);
    }
    return parts.join(QLatin1Char(' '));
}

QString InputSessionLogger::formatHeld64(const std::array<quint64, 16> &arr) const
{
    QStringList parts;
    for (int bit = 15; bit >= 4; --bit) {
        QString name = SnesBitOrder::nameForBitIndex(bit);
        if (!name.isEmpty())
            parts << QStringLiteral("%1=%2").arg(name).arg(arr[size_t(bit)]);
    }
    return parts.join(QLatin1Char(' '));
}

QString InputSessionLogger::hex4(quint16 v)
{
    return QString::number(v, 16).rightJustified(4, QLatin1Char('0'));
}

QString InputSessionLogger::lastNStem() const
{
    QFileInfo fi(m_lastNBasePath);
    QString dir = fi.path();
    QString base = fi.completeBaseName();
    if (base.isEmpty())
        base = fi.fileName();
    if (dir.isEmpty() || dir == QLatin1String("."))
        return base;
    return dir + QLatin1Char('/') + base;
}

bool InputSessionLogger::writeAtomicText(const QString &path, const QByteArray &data)
{
    QSaveFile f(path);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
        return false;
    if (f.write(data) != data.size())
        return false;
    return f.commit();
}

QString InputSessionLogger::buttonCountJsonObjectU16(const std::array<quint16, 16> &arr)
{
    QStringList parts;
    for (int bit = 15; bit >= 4; --bit) {
        const QString name = SnesBitOrder::nameForBitIndex(bit);
        if (name.isEmpty())
            continue;
        parts << QStringLiteral("\"%1\":%2").arg(name).arg(arr[size_t(bit)]);
    }
    return QLatin1Char('{') + parts.join(QLatin1Char(',')) + QLatin1Char('}');
}

QString InputSessionLogger::buttonCountJsonObjectU32(const std::array<quint32, 16> &arr)
{
    QStringList parts;
    for (int bit = 15; bit >= 4; --bit) {
        const QString name = SnesBitOrder::nameForBitIndex(bit);
        if (name.isEmpty())
            continue;
        parts << QStringLiteral("\"%1\":%2").arg(name).arg(arr[size_t(bit)]);
    }
    return QLatin1Char('{') + parts.join(QLatin1Char(',')) + QLatin1Char('}');
}

QString InputSessionLogger::formatSyncTimestamp(quint64 ms)
{
    const QDateTime dt = QDateTime::fromMSecsSinceEpoch(qint64(ms), Qt::UTC);
    return dt.toString(QStringLiteral("yy-MM-dd HH:mm:ss.zzz"));
}

int InputSessionLogger::digitWidth(quint64 v)
{
    int w = 1;
    while (v >= 10) {
        v /= 10;
        ++w;
    }
    return w;
}

void InputSessionLogger::snapshotSyncCounters()
{
    m_pressAtLastSync = m_state.timesPressed();
    m_releaseAtLastSync = m_state.timesReleased();
    m_heldAtLastSync = m_state.timesHeld();
    m_inputChangesAtLastSync = m_state.inputChangeCount();
}

void InputSessionLogger::pushInputRolling(const RollingInputRecord &rec)
{
    m_rollingInputs.append(rec);
    while (m_rollingInputs.size() > m_lastN)
        m_rollingInputs.removeFirst();
}

void InputSessionLogger::pushSyncRolling(const RollingSyncRecord &rec)
{
    m_rollingSyncs.append(rec);
    while (m_rollingSyncs.size() > m_lastN)
        m_rollingSyncs.removeFirst();
}

void InputSessionLogger::rewriteInputRollingFiles()
{
    if (!m_lastNEnabled || m_lastNBasePath.isEmpty())
        return;
    const QString stem = lastNStem();
    QByteArray csv;
    QByteArray txt;
    QStringList jsonObjs;
    // Align delta_ms to width of kActivitySyncMs (guaranteed activity sync window); seq to 8 digits.
    const int deltaWidth = digitWidth(kActivitySyncMs);
    const int seqWidth = 8;
    for (const RollingInputRecord &r : m_rollingInputs) {
        csv += QByteArray::number(qulonglong(r.timestampMs)) + ','
             + QByteArray::number(r.seq) + ','
             + hex4(r.before).toLatin1() + ','
             + hex4(r.after).toLatin1() + ','
             + hex4(r.pressed).toLatin1() + ','
             + hex4(r.released).toLatin1() + '\n';

        QString line = QString::number(r.deltaMs).rightJustified(deltaWidth, QLatin1Char(' '))
            + QLatin1Char(' ')
            + QString::number(r.seq).rightJustified(seqWidth, QLatin1Char(' '));
        if (!r.pressedString.isEmpty())
            line += QLatin1String(" +") + r.pressedString;
        if (!r.releasedString.isEmpty())
            line += QLatin1String(" -") + r.releasedString;
        line += QLatin1String(" (") + r.heldString + QLatin1Char(')');
        txt += line.toUtf8() + '\n';

        jsonObjs << QStringLiteral(
            "{\"timestamp_ms\":%1,\"seq\":%2,\"before\":\"%3\",\"after\":\"%4\","
            "\"pressed\":\"%5\",\"released\":\"%6\","
            "\"pressed_string\":\"%7\",\"released_string\":\"%8\",\"held_string\":\"%9\"}")
                         .arg(r.timestampMs)
                         .arg(r.seq)
                         .arg(hex4(r.before), hex4(r.after), hex4(r.pressed), hex4(r.released))
                         .arg(r.pressedString, r.releasedString, r.heldString);
    }
    const QByteArray json = ('[' + jsonObjs.join(QLatin1Char(',')) + ']').toUtf8() + '\n';
    writeAtomicText(stem + QStringLiteral(".csv"), csv);
    writeAtomicText(stem + QStringLiteral(".txt"), txt);
    writeAtomicText(stem + QStringLiteral(".json"), json);
}

void InputSessionLogger::rewriteSyncRollingFiles()
{
    if (!m_lastNEnabled || m_lastNBasePath.isEmpty())
        return;
    const QString stem = lastNStem();
    QByteArray txt;
    QStringList jsonObjs;
    const int countWidth = digitWidth(kActivitySyncMs);
    for (const RollingSyncRecord &r : m_rollingSyncs) {
        const QString ts = formatSyncTimestamp(r.timestampMs);
        txt += ts.toUtf8() + ' '
             + QString::number(r.entriesSinceLastSync).rightJustified(countWidth, QLatin1Char(' ')).toUtf8()
             + '\n';
        jsonObjs << QStringLiteral(
            "{\"timestamp_ms\":%1,\"timestamp_local\":\"%2\",\"entries_since_last_sync\":%3,"
            "\"press_counts_session\":%4,\"release_counts_session\":%5,\"held_checkpoint_counts_session\":%6,"
            "\"press_counts_since_sync\":%7,\"release_counts_since_sync\":%8,\"held_checkpoint_counts_since_sync\":%9}")
                         .arg(r.timestampMs)
                         .arg(ts)
                         .arg(r.entriesSinceLastSync)
                         .arg(buttonCountJsonObjectU16(r.pressSession),
                              buttonCountJsonObjectU16(r.releaseSession),
                              buttonCountJsonObjectU32(r.heldSession),
                              buttonCountJsonObjectU16(r.pressSinceSync),
                              buttonCountJsonObjectU16(r.releaseSinceSync),
                              buttonCountJsonObjectU32(r.heldSinceSync));
    }
    const QByteArray json = ('[' + jsonObjs.join(QLatin1Char(',')) + ']').toUtf8() + '\n';
    writeAtomicText(stem + QStringLiteral("_sync.txt"), txt);
    writeAtomicText(stem + QStringLiteral("_sync.json"), json);
}

QByteArray InputSessionLogger::buildSyncHeader(quint64 nowMs)
{
    m_state.noteCheckpoint(nowMs, false);

    QByteArray hash = m_entriesSinceSync.isEmpty()
        ? QByteArray(SessionCrypto::kSha256Bytes, '\0')
        : SessionCrypto::sha256(m_entriesSinceSync);

    QByteArray body;
    body.append(kSyncMagic, 8);
    body.append(char(0xAA));
    body.append(char(0x02));
    appendU64BE(body, nowMs);
    body.append(m_state.sessionId());
    QByteArray pub = m_keys.publicKey;
    if (pub.size() != SessionCrypto::kPubKeyBytes)
        pub = QByteArray(SessionCrypto::kPubKeyBytes, '\0');
    body.append(pub);
    appendU32BE(body, m_state.sequence());
    appendU64BE(body, m_state.entryCount());
    appendU64BE(body, m_state.inputChangeCount());
    for (int i = 0; i < 16; ++i)
        appendU16BE(body, m_state.timesPressed()[size_t(i)]);
    for (int i = 0; i < 16; ++i)
        appendU16BE(body, m_state.timesReleased()[size_t(i)]);
    for (int i = 0; i < 16; ++i)
        appendU32BE(body, m_state.timesHeld()[size_t(i)]);
    for (int i = 0; i < 16; ++i)
        appendU64BE(body, m_state.totalHeldMs()[size_t(i)]);
    body.append(hash);

    QByteArray sig = SessionCrypto::signEd25519(m_keys.privateKey, body);
    if (sig.size() != SessionCrypto::kSigBytes)
        sig = QByteArray(SessionCrypto::kSigBytes, '\0');

    m_lastSyncHash = hash;
    m_lastSyncSig = sig;

    QByteArray entry = body;
    entry.append(sig);
    return entry;
}

QByteArray InputSessionLogger::buildInputEntry(quint64 nowMs, quint16 before, quint16 after,
                                               quint16 pressed, quint16 released)
{
    QByteArray body;
    body.append(char(0x01));
    body.append(char(0x02));
    appendU64BE(body, nowMs);
    appendU32BE(body, m_state.sequence());
    appendU16BE(body, before);
    appendU16BE(body, after);
    appendU16BE(body, pressed);
    appendU16BE(body, released);
    body.append(m_state.sessionId());

    QByteArray prev = m_prevEntryHash.size() == 8 ? m_prevEntryHash : QByteArray(8, '\0');
    QByteArray withPrev = body;
    withPrev.append(prev);
    QByteArray selfHash = xxh3_64(withPrev);

    QByteArray entry = withPrev;
    entry.append(selfHash);
    return entry;
}

void InputSessionLogger::writeSyncHeader(quint64 nowMs)
{
    if (!m_active)
        return;

    const quint64 entriesSince = m_state.inputChangeCount() - m_inputChangesAtLastSync;

    RollingSyncRecord syncRec;
    syncRec.timestampMs = nowMs;
    syncRec.entriesSinceLastSync = entriesSince;

    QByteArray entry = buildSyncHeader(nowMs);
    appendBin(entry);

    const QByteArray hash = entry.mid(entry.size() - SessionCrypto::kSigBytes - SessionCrypto::kSha256Bytes,
                                      SessionCrypto::kSha256Bytes);
    const QByteArray sig = entry.right(SessionCrypto::kSigBytes);
    m_prevEntryHash = QByteArray(8, '\0');
    m_entriesSinceSync.clear();
    m_ordinarySinceSync = 0;

    const QString syncLine =
        QString::fromLatin1("AA02,") + QString::number(nowMs) + QLatin1Char(',')
        + m_state.sessionIdHex() + QLatin1Char(',')
        + QString::fromLatin1(m_keys.publicKey.toHex().toUpper()) + QLatin1Char(',')
        + QString::number(m_state.sequence()) + QLatin1Char(',')
        + QString::number(m_state.entryCount()) + QLatin1Char(',')
        + QString::number(m_state.inputChangeCount()) + QLatin1Char(',')
        + formatPressedList(m_state.timesPressed()) + QLatin1Char(',')
        + formatPressedList(m_state.timesReleased()) + QLatin1Char(',')
        + formatHeld32(m_state.timesHeld()) + QLatin1Char(',')
        + formatHeld64(m_state.totalHeldMs()) + QLatin1Char(',')
        + QString::fromLatin1(hash.toHex().toUpper()) + QLatin1Char(',')
        + QString::fromLatin1(sig.toBase64());

    appendFullText(syncLine);
    appendSyncText(syncLine);

    syncRec.pressSession = m_state.timesPressed();
    syncRec.releaseSession = m_state.timesReleased();
    syncRec.heldSession = m_state.timesHeld();
    for (int i = 0; i < 16; ++i) {
        syncRec.pressSinceSync[size_t(i)] =
            quint16(m_state.timesPressed()[size_t(i)] - m_pressAtLastSync[size_t(i)]);
        syncRec.releaseSinceSync[size_t(i)] =
            quint16(m_state.timesReleased()[size_t(i)] - m_releaseAtLastSync[size_t(i)]);
        syncRec.heldSinceSync[size_t(i)] =
            m_state.timesHeld()[size_t(i)] - m_heldAtLastSync[size_t(i)];
    }
    if (m_lastNEnabled) {
        pushSyncRolling(syncRec);
        rewriteSyncRollingFiles();
    }

    m_state.bumpSequenceAfterEntry(false);
    snapshotSyncCounters();
    m_lastSyncMs = nowMs;
    ++m_syncHeaderWriteCount;
}

void InputSessionLogger::writeInputEntry(quint64 nowMs, quint16 before, quint16 after,
                                         quint16 pressed, quint16 released, bool isHeartbeat)
{
    if (!m_active)
        return;
    QByteArray entry = buildInputEntry(nowMs, before, after, pressed, released);
    appendBin(entry);
    m_entriesSinceSync.append(entry);
    ++m_ordinarySinceSync;

    const QByteArray selfHashBytes = entry.right(8);
    const QByteArray prevHashBytes = entry.mid(entry.size() - 16, 8);
    m_prevEntryHash = selfHashBytes;

    const QString line =
        QString::fromLatin1("0102,") + QString::number(nowMs) + QLatin1Char(',')
        + m_state.sessionIdHex() + QLatin1Char(',')
        + QString::number(m_state.sequence()) + QLatin1Char(',')
        + hex4(before) + QLatin1Char(',')
        + hex4(after) + QLatin1Char(',')
        + hex4(pressed) + QLatin1Char(',')
        + hex4(released) + QLatin1Char(',')
        + m_state.sessionIdHex() + QLatin1Char(',')
        + QString::fromLatin1(prevHashBytes.toBase64()) + QLatin1Char(',')
        + QString::fromLatin1(selfHashBytes.toBase64());
    appendFullText(line);

    if (m_lastNEnabled) {
        RollingInputRecord rec;
        rec.timestampMs = nowMs;
        rec.seq = m_state.sequence();
        rec.deltaMs = (m_lastInputEntryMs == 0) ? 0 : (nowMs - m_lastInputEntryMs);
        rec.before = before;
        rec.after = after;
        rec.pressed = pressed;
        rec.released = released;
        rec.pressedString = SnesBitOrder::formatEdgeString(pressed);
        rec.releasedString = SnesBitOrder::formatEdgeString(released);
        rec.heldString = SnesBitOrder::formatHeldString(after, m_state.pressStartedMs(), nowMs);
        pushInputRolling(rec);
        rewriteInputRollingFiles();
    }

    m_lastInputEntryMs = nowMs;
    m_state.bumpSequenceAfterEntry(!isHeartbeat);
    maybeForceSyncByCount(nowMs);
}

void InputSessionLogger::maybeForceSyncByCount(quint64 nowMs)
{
    if (m_ordinarySinceSync >= kMaxOrdinaryBetweenSync)
        writeSyncHeader(nowMs);
}

void InputSessionLogger::onButton(InputProvider::SNESButton button, bool pressed)
{
    onButtonAt(button, pressed, quint64(QDateTime::currentMSecsSinceEpoch()));
}

void InputSessionLogger::onButtonAt(InputProvider::SNESButton button, bool pressed, quint64 nowMs)
{
    if (!m_active)
        return;
    maybeRotateIdle(nowMs);
    if (!m_active)
        return;
    quint16 before = 0, after = 0, pressedBits = 0, releasedBits = 0;
    if (!m_state.applyButton(button, pressed, nowMs, &before, &after, &pressedBits, &releasedBits))
        return;
    writeInputEntry(nowMs, before, after, pressedBits, releasedBits, false);
    m_lastHeartbeatMs = nowMs;
    maybeWriteActivitySync(nowMs);
}

void InputSessionLogger::forceSyncHeader()
{
    if (!m_active)
        return;
    writeSyncHeader(quint64(QDateTime::currentMSecsSinceEpoch()));
}

void InputSessionLogger::maybeRotateIdleAt(quint64 nowMs)
{
    maybeRotateIdle(nowMs);
}

bool InputSessionLogger::maybeWriteActivitySyncAt(quint64 nowMs)
{
    return maybeWriteActivitySync(nowMs);
}

bool InputSessionLogger::maybeWriteActivitySync(quint64 nowMs)
{
    if (!kExtraIdleSyncsEnabled || !m_active)
        return false;
    if (nowMs - m_lastSyncMs < kActivitySyncMs)
        return false;
    if (m_state.inputChangeCount() <= m_inputChangesAtLastSync)
        return false;
    writeSyncHeader(nowMs);
    return true;
}

void InputSessionLogger::maybeRotateIdle(quint64 nowMs)
{
    if (nowMs - m_state.lastChangeMs() < kIdleSessionMs)
        return;

    if (kExtraIdleSyncsEnabled)
        writeSyncHeader(nowMs);

    wipeSessionKeys();
    if (!beginSessionKeys(nowMs)) {
        m_active = false;
        m_timer.stop();
        closeLogs();
        return;
    }
    m_prevEntryHash = QByteArray(8, '\0');
    m_entriesSinceSync.clear();
    m_ordinarySinceSync = 0;
    m_rollingInputs.clear();
    m_rollingSyncs.clear();
    m_lastInputEntryMs = 0;
    m_inputChangesAtLastSync = 0;
    m_pressAtLastSync.fill(0);
    m_releaseAtLastSync.fill(0);
    m_heldAtLastSync.fill(0);
    writeSyncHeader(nowMs);
}

void InputSessionLogger::onTick()
{
    if (!m_active)
        return;
    const quint64 now = quint64(QDateTime::currentMSecsSinceEpoch());
    maybeRotateIdle(now);
    if (!m_active)
        return;
    if (now - m_lastSyncMs >= kSyncIntervalMs)
        writeSyncHeader(now);
    else
        maybeWriteActivitySync(now);
    if (!m_state.buttonsHeld() && (now - m_lastHeartbeatMs >= kHeartbeatMs)
        && (now - m_state.lastChangeMs() >= kHeartbeatMs)) {
        m_state.noteCheckpoint(now, false);
        writeInputEntry(now, 0, 0, 0, 0, true);
        m_lastHeartbeatMs = now;
    }
}

QByteArray InputSessionLogger::encodeSyncHeaderForTest(quint64 nowMs)
{
    return buildSyncHeader(nowMs);
}

QByteArray InputSessionLogger::encodeInputEntryForTest(quint64 nowMs, quint16 before, quint16 after,
                                                       quint16 pressed, quint16 released)
{
    return buildInputEntry(nowMs, before, after, pressed, released);
}
