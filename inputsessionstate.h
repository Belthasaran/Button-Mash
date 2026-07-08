#ifndef INPUTSESSIONSTATE_H
#define INPUTSESSIONSTATE_H

#include "inputprovider.h"
#include <QByteArray>
#include <QtGlobal>
#include <array>

class InputSessionState
{
public:
    InputSessionState();

    void resetSession(quint64 nowMs);
    QByteArray sessionId() const { return m_sessionId; }
    QString sessionIdHex() const;

    quint16 currentBits() const { return m_current; }
    quint32 sequence() const { return m_sequence; }
    quint64 entryCount() const { return m_entryCount; }
    quint64 inputChangeCount() const { return m_inputChangeCount; }
    quint64 lastChangeMs() const { return m_lastChangeMs; }
    quint64 lastCheckpointMs() const { return m_lastCheckpointMs; }
    quint64 sessionStartMs() const { return m_sessionStartMs; }

    const std::array<quint16, 16> &timesPressed() const { return m_timesPressed; }
    const std::array<quint16, 16> &timesReleased() const { return m_timesReleased; }
    const std::array<quint32, 16> &timesHeld() const { return m_timesHeld; }
    const std::array<quint64, 16> &totalHeldMs() const { return m_totalHeldMs; }
    const std::array<quint64, 16> &pressStartedMs() const { return m_pressStartedMs; }

    quint64 heldFramesAt(quint64 nowMs, int bit) const;

    // Apply a button edge. Returns true if state changed.
    bool applyButton(InputProvider::SNESButton button, bool pressed, quint64 nowMs,
                     quint16 *beforeOut, quint16 *afterOut, quint16 *pressedOut, quint16 *releasedOut);

    // Heartbeat / forced checkpoint without change (before==after).
    void noteCheckpoint(quint64 nowMs, bool countAsInputChange);

    // Advance sequence for a recorded entry (sync or input).
    void bumpSequenceAfterEntry(bool isInputChange);

    bool hasPendingChange(quint64 nowMs, quint64 idleMs) const;
    bool buttonsHeld() const { return m_current != 0; }

private:
    void accumulateHold(quint64 nowMs);
    void generateSessionId();

    QByteArray m_sessionId;
    quint16 m_current;
    quint32 m_sequence;
    quint64 m_entryCount;
    quint64 m_inputChangeCount;
    quint64 m_lastChangeMs;
    quint64 m_lastCheckpointMs;
    quint64 m_sessionStartMs;
    std::array<quint16, 16> m_timesPressed;
    std::array<quint16, 16> m_timesReleased;
    std::array<quint32, 16> m_timesHeld;
    std::array<quint64, 16> m_totalHeldMs;
    std::array<quint64, 16> m_pressStartedMs;
};

#endif
