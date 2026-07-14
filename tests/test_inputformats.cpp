#include <QCoreApplication>
#include <QDateTime>
#include <QDebug>
#include <QFile>
#include <QIODevice>
#include <QtEndian>
#include <array>
#include <cstring>

#include "snesbitorder.h"
#include "sessioncrypto.h"
#include "inputsessionlogger.h"
#include "buttonmashremoteprovider.h"
#include "retroarchremotepadprovider.h"
#include "inputmirrormanager.h"
#include "inputtriggers.h"
#include "uinputdevice.h"

#include <QSettings>
#include <QTemporaryFile>

static int failures = 0;

static void expect(bool cond, const char *msg)
{
    if (!cond) {
        qWarning("FAIL: %s", msg);
        failures++;
    } else {
        qDebug("OK: %s", msg);
    }
}

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);

    expect(SnesBitOrder::bitIndexForButton(InputProvider::B) == 15, "B is bit 15");
    expect(SnesBitOrder::bitIndexForButton(InputProvider::Y) == 14, "Y is bit 14");
    expect(SnesBitOrder::bitIndexForButton(InputProvider::Select) == 13, "Select is bit 13");
    expect(SnesBitOrder::bitIndexForButton(InputProvider::R) == 4, "R is bit 4");
    expect(SnesBitOrder::maskForButton(InputProvider::B) == 0x8000, "B mask 0x8000");

    // Session crypto: mint, derive id, sign/verify
    {
        SessionCrypto::KeyPair kp;
        expect(SessionCrypto::mintKeyPair(&kp), "mint ed25519 keypair");
        const QByteArray sid = SessionCrypto::sessionIdFromPublicKey(kp.publicKey);
        expect(sid.size() == 16, "derived session id 16 bytes");
        expect(sid == SessionCrypto::sessionIdFromPublicKey(kp.publicKey), "session id deterministic");
        const QByteArray msg = QByteArrayLiteral("sync-body");
        const QByteArray sig = SessionCrypto::signEd25519(kp.privateKey, msg);
        expect(sig.size() == 64, "ed25519 sig 64 bytes");
        expect(SessionCrypto::verifyEd25519(kp.publicKey, msg, sig), "ed25519 verify ok");
        QByteArray bad = sig;
        bad[0] = char(uchar(bad[0]) ^ 0x01);
        expect(!SessionCrypto::verifyEd25519(kp.publicKey, msg, bad), "ed25519 verify rejects tamper");
        SessionCrypto::wipeKeyPair(&kp);
        expect(!kp.valid(), "keypair wiped");
    }

    InputSessionLogger logger;
    logger.setBinLog(true, QStringLiteral("/tmp/buttonmash_test.binlog"));
    logger.setFullTextLog(true, QStringLiteral("/tmp/buttonmash_test.csv"));
    logger.startLogging();
    expect(logger.isLogging(), "logger started");
    expect(logger.state().sessionId().size() == 16, "session id 16 bytes");
    expect(logger.state().sessionId()
               == SessionCrypto::sessionIdFromPublicKey(logger.publicKeyForTest()),
           "session id matches SHA3(pubkey)[:16]");
    expect(logger.lastSyncHashForTest() == QByteArray(32, '\0'), "opening sync hash zeros");
    expect(logger.lastSyncSigForTest().size() == 64, "opening sync sig present");
    {
        const QByteArray pub = logger.publicKeyForTest();
        QByteArray encoded = logger.encodeSyncHeaderForTest(quint64(QDateTime::currentMSecsSinceEpoch()));
        expect(encoded.startsWith(QByteArray("BM", 2)), "sync magic BM");
        expect(uchar(encoded.at(2)) == 0xAA, "sync magic third byte");
        expect(uchar(encoded.at(8)) == 0xAA, "sync type 0xAA");
        expect(uchar(encoded.at(9)) == 0x02, "sync subtype 0x02");
        expect(SessionCrypto::verifyEd25519(pub, encoded.left(encoded.size() - 64), encoded.right(64)),
               "opening-style encoded sync verifies");
    }
    logger.onButton(InputProvider::B, true);
    logger.onButton(InputProvider::B, false);
    logger.forceSyncHeader();
    expect(logger.lastSyncHashForTest() != QByteArray(32, '\0'), "sync hash non-zero after inputs");
    {
        const QByteArray pub = logger.publicKeyForTest();
        QByteArray encoded = logger.encodeSyncHeaderForTest(quint64(QDateTime::currentMSecsSinceEpoch()));
        expect(SessionCrypto::verifyEd25519(pub, encoded.left(encoded.size() - 64), encoded.right(64)),
               "encoded sync signature verifies");
    }

    QByteArray inputEntry = logger.encodeInputEntryForTest(2000, 0, 0x8000, 0x8000, 0);
    expect(inputEntry.size() == 54, "ordinary entry 54 bytes");
    expect(uchar(inputEntry.at(1)) == 0x02, "input subtype 0x02");

    logger.stopLogging();
    expect(!logger.hasSigningKeys(), "keys wiped after stop");

    // Force sync at 200 ordinary entries
    {
        InputSessionLogger force;
        force.setLastNDashboard(true, QStringLiteral("/tmp/buttonmash_force200"), 5);
        force.startLogging();
        const int syncs0 = force.syncHeaderWriteCount();
        const quint64 t0 = quint64(QDateTime::currentMSecsSinceEpoch());
        for (int i = 0; i < InputSessionLogger::kMaxOrdinaryBetweenSync; ++i)
            force.onButtonAt(InputProvider::A, (i % 2) == 0, t0 + quint64(i));
        expect(force.syncHeaderWriteCount() > syncs0, "forced sync after 200 ordinary entries");
        force.stopLogging();
    }

    QByteArray pkt = ButtonMashRemoteProvider::buildPacket(0x8000, 0x8000, 0, 1234, QByteArray(16, 'A'));
    expect(pkt.startsWith("BMIR"), "BMIR magic");
    expect(pkt.size() == ButtonMashRemoteProvider::kBmirBodyBytes + ButtonMashRemoteProvider::kBmirXxh3Bytes,
           "BMIR packet includes XXH3 trailer");
    ButtonMashRemoteProvider recv(39999);
    expect(recv.handleDatagram(pkt), "BMIR datagram parsed");
    QByteArray corrupt = pkt;
    corrupt[10] = char(uchar(corrupt[10]) ^ 0xFF);
    expect(!recv.handleDatagram(corrupt), "BMIR rejects bad XXH3");
    QByteArray zeroTrail = pkt.left(ButtonMashRemoteProvider::kBmirBodyBytes);
    zeroTrail.append(QByteArray(8, '\0'));
    expect(recv.handleDatagram(zeroTrail), "BMIR accepts zero XXH3 trailer");
    expect(!recv.handleDatagram(QByteArray()), "BMIR rejects empty");
    expect(!recv.handleDatagram(QByteArray(20, 'x')), "BMIR rejects short");
    expect(!recv.handleDatagram(QByteArrayLiteral("XXXX") + pkt.mid(4)), "BMIR rejects bad magic");
    QByteArray badVer = pkt;
    badVer[4] = char(0x02);
    expect(!recv.handleDatagram(badVer), "BMIR rejects bad version");
    // Sizes 37–43: trailing junk past body, no XXH3 trailer required (legacy).
    QByteArray bodyPlusJunk = pkt.left(ButtonMashRemoteProvider::kBmirBodyBytes);
    bodyPlusJunk.append(QByteArray(4, 'Z'));
    expect(recv.handleDatagram(bodyPlusJunk), "BMIR accepts 36+junk without XXH3 trailer");
    // Size >= 44 with non-zero invalid trailer: reject
    QByteArray bodyBadTrail = pkt.left(ButtonMashRemoteProvider::kBmirBodyBytes);
    bodyBadTrail.append(QByteArray(8, 'Z'));
    expect(!recv.handleDatagram(bodyBadTrail), "BMIR rejects invalid non-zero XXH3 trailer");
    expect(!recv.handleDatagram(QByteArray(ButtonMashRemoteProvider::kMaxUdpDatagram + 1, 'A')),
           "BMIR rejects oversize buffer");

    // Idle resync: duplicate full-state BMIR is silent when already in sync
    {
        int pressCount = 0;
        int releaseCount = 0;
        ButtonMashRemoteProvider bmirResync(39996);
        QObject::connect(&bmirResync, &InputProvider::buttonPressed, &app,
                         [&](InputProvider::SNESButton) { ++pressCount; });
        QObject::connect(&bmirResync, &InputProvider::buttonReleased, &app,
                         [&](InputProvider::SNESButton) { ++releaseCount; });

        const QByteArray pressPkt = ButtonMashRemoteProvider::buildPacket(
            0x8000, 0x8000, 0, 0, QByteArray(16, '\0'), true);
        expect(bmirResync.handleDatagram(pressPkt), "BMIR resync test press applied");
        expect(pressCount == 1 && releaseCount == 0, "BMIR one press edge");

        const QByteArray sameState = ButtonMashRemoteProvider::buildPacket(
            0x8000, 0, 0, 0, QByteArray(16, '\0'), true);
        expect(bmirResync.handleDatagram(sameState), "BMIR resync same state accepted");
        expect(pressCount == 1 && releaseCount == 0, "BMIR in-sync resync silent");

        const QByteArray repair = ButtonMashRemoteProvider::buildPacket(
            0, 0, 0, 0, QByteArray(16, '\0'), true);
        expect(bmirResync.handleDatagram(repair), "BMIR resync repair applied");
        expect(pressCount == 1 && releaseCount == 1, "BMIR resync repairs stuck press");
    }

    expect(InputMirrorManager::nextResyncIntervalMs(300) == 600, "resync interval doubles");
    expect(InputMirrorManager::nextResyncIntervalMs(20000) == 30000, "resync interval caps at 30s");
    expect(InputMirrorManager::nextResyncIntervalMs(30000) == 30000, "resync interval stays at cap");

    RetroArchRemotePadProvider ra(39998);
    // Explicit LE wire bytes (18 usable; often sent as 20 with pad)
    QByteArray rapkt(20, '\0');
    qToLittleEndian<qint32>(0, rapkt.data() + 0);      // port
    qToLittleEndian<qint32>(1, rapkt.data() + 4);      // JOYPAD
    qToLittleEndian<qint32>(0, rapkt.data() + 8);      // index
    qToLittleEndian<qint32>(0, rapkt.data() + 12);     // id B
    qToLittleEndian<quint16>(1, rapkt.data() + 16);    // pressed
    expect(ra.handleDatagram(rapkt), "RetroArch B press parsed");
    qToLittleEndian<quint16>(0, rapkt.data() + 16);
    expect(ra.handleDatagram(rapkt), "RetroArch B release parsed");
    expect(!ra.handleDatagram(QByteArray()), "RetroArch rejects empty");
    expect(!ra.handleDatagram(QByteArray(17, '\0')), "RetroArch rejects 17 bytes");
    expect(ra.handleDatagram(rapkt.left(18)), "RetroArch accepts exact 18-byte payload");
    QByteArray ra24 = rapkt;
    ra24.append(QByteArray(4, 'x'));
    qToLittleEndian<quint16>(1, ra24.data() + 16);
    expect(ra.handleDatagram(ra24), "RetroArch accepts 24 with valid first 18");
    QByteArray badId = rapkt;
    qToLittleEndian<qint32>(-1, badId.data() + 12);
    expect(!ra.handleDatagram(badId), "RetroArch rejects id -1");
    qToLittleEndian<qint32>(12, badId.data() + 12);
    expect(!ra.handleDatagram(badId), "RetroArch rejects id 12");
    QByteArray badDev = rapkt;
    qToLittleEndian<qint32>(5, badDev.data() + 4); // ANALOG
    expect(!ra.handleDatagram(badDev), "RetroArch rejects non-joypad device");
    expect(!ra.handleDatagram(QByteArray(RetroArchRemotePadProvider::kMaxUdpDatagram + 1, '\0')),
           "RetroArch rejects oversize buffer");

    // Outbound RetroArch packet: fixed 20-byte LE wire
    {
        const QByteArray out = InputMirrorManager::buildRetroArchPacket(InputProvider::B, true);
        expect(out.size() == 20, "outbound RetroArch packet size 20");
        expect(qFromLittleEndian<qint32>(reinterpret_cast<const uchar *>(out.constData()) + 4) == 1,
               "outbound device JOYPAD");
        expect(qFromLittleEndian<qint32>(reinterpret_cast<const uchar *>(out.constData()) + 12) == 0,
               "outbound id B");
        expect(qFromLittleEndian<quint16>(reinterpret_cast<const uchar *>(out.constData()) + 16) == 1,
               "outbound pressed state");
        RetroArchRemotePadProvider ra2(39997);
        expect(ra2.handleDatagram(out), "outbound packet accepted by receiver");
    }

    // Idle resync: RetroArch full-state blast dedups in sync; repairs stuck press
    {
        int pressCount = 0;
        int releaseCount = 0;
        RetroArchRemotePadProvider raResync(39995);
        QObject::connect(&raResync, &InputProvider::buttonPressed, &app,
                         [&](InputProvider::SNESButton) { ++pressCount; });
        QObject::connect(&raResync, &InputProvider::buttonReleased, &app,
                         [&](InputProvider::SNESButton) { ++releaseCount; });

        const QByteArray bPress = InputMirrorManager::buildRetroArchPacket(InputProvider::B, true);
        expect(raResync.handleDatagram(bPress), "RA resync test B press");
        expect(pressCount == 1 && releaseCount == 0, "RA one press edge");

        for (int bit = 4; bit < 16; ++bit) {
            const InputProvider::SNESButton btn = SnesBitOrder::buttonForBitIndex(bit);
            const bool held = (bit == SnesBitOrder::B);
            const QByteArray pkt = InputMirrorManager::buildRetroArchPacket(btn, held);
            expect(raResync.handleDatagram(pkt), "RA in-sync resync datagram");
        }
        expect(pressCount == 1 && releaseCount == 0, "RA in-sync resync silent");

        for (int bit = 4; bit < 16; ++bit) {
            const InputProvider::SNESButton btn = SnesBitOrder::buttonForBitIndex(bit);
            const QByteArray pkt = InputMirrorManager::buildRetroArchPacket(btn, false);
            raResync.handleDatagram(pkt);
        }
        expect(releaseCount == 1, "RA resync repairs stuck B");
    }

    // Trigger config round-trip + defaults
    {
        InputTriggersEngine eng;
        TriggerConfig c = TriggerConfig::defaults();
        c.enabled = true;
        c.windowTitleSubstrings = QStringList{QStringLiteral("RetroArch"), QStringLiteral("Game")};
        TriggerBinding macro;
        macro.sourceKind = TriggerBinding::SourceKind::Extra;
        macro.extraIndex = 0;
        macro.action = TriggerBinding::ActionKind::RunProgram;
        macro.programPath = QStringLiteral("/usr/bin/true");
        c.bindings.append(macro);
        eng.setConfig(c);

        QTemporaryFile tmp;
        expect(tmp.open(), "temp settings file");
        const QString path = tmp.fileName();
        tmp.close();
        {
            QSettings s(path, QSettings::IniFormat);
            eng.saveSettings(s);
            s.sync();
        }
        InputTriggersEngine eng2;
        {
            QSettings s(path, QSettings::IniFormat);
            eng2.loadSettings(s);
        }
        expect(eng2.config().enabled, "triggers enabled restored");
        expect(eng2.config().windowTitleSubstrings.size() == 2, "window titles restored");
        bool foundMacro = false;
        for (const TriggerBinding &b : eng2.config().bindings) {
            if (b.sourceKind == TriggerBinding::SourceKind::Extra
                && b.action == TriggerBinding::ActionKind::RunProgram
                && b.programPath == QLatin1String("/usr/bin/true"))
                foundMacro = true;
        }
        expect(foundMacro, "run-program macro restored");
        expect(!TriggerConfig::defaults().bindings.isEmpty(), "default bindings non-empty");
    }

#ifdef Q_OS_LINUX
    expect(UInputGamepad::btnCodeForSnes(InputProvider::B) > 0, "uinput B maps to BTN");
    expect(UInputGamepad::btnCodeForSnes(InputProvider::Start) > 0, "uinput Start maps");
#endif

    // Abbreviations / held frames (hardware-order display helpers)
    expect(SnesBitOrder::abbrevForBitIndex(SnesBitOrder::B) == QLatin1String("B"), "abbrev B");
    expect(SnesBitOrder::abbrevForBitIndex(SnesBitOrder::Start) == QLatin1String("[ST]"), "abbrev Start");
    expect(SnesBitOrder::abbrevForBitIndex(SnesBitOrder::Select) == QLatin1String("[SEL]"), "abbrev Select");
    expect(SnesBitOrder::abbrevForBitIndex(SnesBitOrder::Left) == QLatin1String("Lf"), "abbrev Left");
    expect(SnesBitOrder::abbrevForBitIndex(SnesBitOrder::Right) == QLatin1String("Rt"), "abbrev Right");
    {
        const quint16 pressed = SnesBitOrder::maskForButton(InputProvider::B)
            | SnesBitOrder::maskForButton(InputProvider::Start);
        expect(SnesBitOrder::formatEdgeString(pressed) == QLatin1String("B[ST]"), "edge string B[ST]");
    }
    expect(SnesBitOrder::heldFrames(1000, 1050) == 3, "heldFrames 50ms ~= 3 frames @60Hz");
    expect(SnesBitOrder::heldFrames(0, 2000) == 0, "heldFrames zero start");

    // Newly pressed (0 frames) omitted from held parenthesis string
    {
        std::array<quint64, 16> starts {};
        starts[size_t(SnesBitOrder::B)] = 1000; // just pressed at now
        starts[size_t(SnesBitOrder::R)] = 900;  // held 100ms -> 6 frames
        const quint16 after = SnesBitOrder::maskForButton(InputProvider::B)
            | SnesBitOrder::maskForButton(InputProvider::R);
        expect(SnesBitOrder::formatHeldString(after, starts, 1000) == QLatin1String("R=6"),
               "held string omits 0-frame B, keeps R");
        expect(SnesBitOrder::formatHeldString(SnesBitOrder::maskForButton(InputProvider::B), starts, 1000).isEmpty(),
               "held empty when only just-pressed");
    }

    // Last N ring buffer size
    {
        InputSessionLogger roll;
        roll.setLastNDashboard(true, QStringLiteral("/tmp/buttonmash_lastn_test"), 3);
        roll.startLogging();
        expect(roll.isLogging(), "last-N-only starts session tracking");
        const quint64 t0 = quint64(QDateTime::currentMSecsSinceEpoch());
        for (int i = 0; i < 5; ++i) {
            roll.onButtonAt(InputProvider::B, true, t0 + quint64(i * 100));
            roll.onButtonAt(InputProvider::B, false, t0 + 50 + quint64(i * 100));
        }
        expect(roll.rollingInputs().size() == 3, "rolling inputs capped at N=3");
        expect(roll.rollingInputs().last().pressedString == QLatin1String("B")
               || roll.rollingInputs().last().releasedString == QLatin1String("B"),
               "rolling record has abbrev strings");
        // Press entry should not list B=0 in held_string
        bool foundPressWithoutZeroHeld = false;
        for (const RollingInputRecord &r : roll.rollingInputs()) {
            if (r.pressedString == QLatin1String("B")) {
                foundPressWithoutZeroHeld = !r.heldString.contains(QLatin1String("B=0"));
                break;
            }
        }
        expect(foundPressWithoutZeroHeld, "press entry held_string has no B=0");
        roll.stopLogging();

        // TXT numeric column alignment (delta width of kActivitySyncMs digits)
        {
            InputSessionLogger pad;
            pad.setLastNDashboard(true, QStringLiteral("/tmp/buttonmash_pad"), 10);
            pad.startLogging();
            const quint64 t1 = quint64(QDateTime::currentMSecsSinceEpoch());
            pad.onButtonAt(InputProvider::A, true, t1 + 5);
            pad.onButtonAt(InputProvider::A, false, t1 + 25);
            pad.stopLogging();
            QFile tf(QStringLiteral("/tmp/buttonmash_pad.txt"));
            expect(tf.open(QIODevice::ReadOnly | QIODevice::Text), "open pad txt");
            const QByteArray body = tf.readAll();
            tf.close();
            const QList<QByteArray> lines = body.split('\n');
            expect(lines.size() >= 2 && !lines.at(0).isEmpty() && !lines.at(1).isEmpty(),
                   "pad txt has two input lines");
            // Space before first digit of delta, or shared leading-space column boundary before seq
            const int deltaW = QString::number(InputSessionLogger::kActivitySyncMs).size();
            expect(lines.at(0).size() > deltaW + 1 + 8, "first line long enough for padded cols");
            expect(lines.at(1).size() > deltaW + 1 + 8, "second line long enough for padded cols");
            const int seqCol = deltaW + 1;
            expect(lines.at(0).at(seqCol - 1) == ' ' && lines.at(1).at(seqCol - 1) == ' ',
                   "space separator before seq on both rows");
            const quint32 seq0 = QString::fromLatin1(lines.at(0).mid(seqCol, 8)).trimmed().toUInt();
            const quint32 seq1 = QString::fromLatin1(lines.at(1).mid(seqCol, 8)).trimmed().toUInt();
            expect(seq0 > 0 && seq1 > seq0, "aligned seq values increase");
        }
    }

    // Sync "since last sync" button counters
    {
        InputSessionLogger syncLog;
        syncLog.setLastNDashboard(true, QStringLiteral("/tmp/buttonmash_lastn_sync"), 10);
        syncLog.startLogging();
        // startLogging already writes sync #1; use that as baseline
        expect(syncLog.rollingSyncs().size() == 1, "startup sync in rolling buffer");
        const quint64 t0 = quint64(QDateTime::currentMSecsSinceEpoch());
        syncLog.onButtonAt(InputProvider::Y, true, t0 + 10);
        syncLog.onButtonAt(InputProvider::Y, false, t0 + 20);
        syncLog.onButtonAt(InputProvider::Y, true, t0 + 30);
        syncLog.onButtonAt(InputProvider::Y, false, t0 + 40);
        syncLog.forceSyncHeader();
        expect(syncLog.rollingSyncs().size() == 2, "one sync after baseline");
        const RollingSyncRecord &sr = syncLog.rollingSyncs().last();
        expect(sr.entriesSinceLastSync == 4, "four input-change entries since last sync");
        expect(sr.pressSinceSync[size_t(SnesBitOrder::Y)] == 2, "Y pressed twice since sync");
        expect(sr.releaseSinceSync[size_t(SnesBitOrder::Y)] == 2, "Y released twice since sync");
        syncLog.stopLogging();
        // stopLogging may append a closeout sync under kExtraIdleSyncsEnabled
    }

    // Closeout sync on stopLogging
    if (InputSessionLogger::kExtraIdleSyncsEnabled) {
        InputSessionLogger closeLog;
        closeLog.setLastNDashboard(true, QStringLiteral("/tmp/buttonmash_closeout"), 10);
        closeLog.startLogging();
        expect(closeLog.rollingSyncs().size() == 1, "closeout test startup sync");
        const quint64 t0 = quint64(QDateTime::currentMSecsSinceEpoch());
        closeLog.onButtonAt(InputProvider::A, true, t0 + 5);
        closeLog.onButtonAt(InputProvider::A, false, t0 + 10);
        const int beforeStop = closeLog.rollingSyncs().size();
        closeLog.stopLogging();
        expect(closeLog.rollingSyncs().size() == beforeStop + 1, "stopLogging writes closeout sync");
        expect(closeLog.rollingSyncs().last().entriesSinceLastSync == 2, "closeout captures edges since prior sync");
    }

    // Idle rotate: closeout for old session + opening sync for new
    if (InputSessionLogger::kExtraIdleSyncsEnabled) {
        InputSessionLogger rot;
        rot.setLastNDashboard(true, QStringLiteral("/tmp/buttonmash_rotate"), 10);
        rot.startLogging();
        expect(rot.syncHeaderWriteCount() == 1, "rotate test startup write count");
        const quint64 t0 = quint64(QDateTime::currentMSecsSinceEpoch());
        rot.onButtonAt(InputProvider::B, true, t0 + 5);
        rot.onButtonAt(InputProvider::B, false, t0 + 10);
        const int writesBefore = rot.syncHeaderWriteCount();
        const QByteArray oldSid = rot.state().sessionId();
        rot.maybeRotateIdleAt(t0 + 10 + InputSessionLogger::kIdleSessionMs);
        expect(rot.syncHeaderWriteCount() == writesBefore + 2, "idle rotate closeout + opening sync");
        expect(rot.state().sessionId() != oldSid, "idle rotate mints new session id");
        expect(rot.rollingSyncs().size() == 1, "rolling syncs cleared then opening only");
        rot.stopLogging();
    }

    // Activity soft sync (≥15s since sync + edges since last sync)
    if (InputSessionLogger::kExtraIdleSyncsEnabled) {
        InputSessionLogger act;
        act.setLastNDashboard(true, QStringLiteral("/tmp/buttonmash_activity"), 10);
        act.startLogging();
        expect(act.rollingSyncs().size() == 1, "activity test startup sync");
        const quint64 baseline = quint64(QDateTime::currentMSecsSinceEpoch());
        act.onButtonAt(InputProvider::X, true, baseline + 100);
        act.onButtonAt(InputProvider::X, false, baseline + 200);
        expect(!act.maybeWriteActivitySyncAt(baseline + 1000), "no activity sync before 15s");
        expect(act.rollingSyncs().size() == 1, "still one sync before 15s");
        expect(act.maybeWriteActivitySyncAt(baseline + InputSessionLogger::kActivitySyncMs + 100),
               "activity sync at 15s with edges");
        expect(act.rollingSyncs().size() == 2, "activity sync appended");
        expect(act.rollingSyncs().last().entriesSinceLastSync == 2, "activity sync edges since prior");
        expect(!act.maybeWriteActivitySyncAt(baseline + InputSessionLogger::kActivitySyncMs + 20000),
               "no second activity sync without new edges");
        expect(act.rollingSyncs().size() == 2, "still two syncs after no-op");
        act.stopLogging();
    }

    if (failures) {
        qWarning("%d test(s) failed", failures);
        return 1;
    }
    qDebug("All button-mash input format tests passed");
    return 0;
}
