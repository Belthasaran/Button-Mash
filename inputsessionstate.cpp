#include "inputsessionstate.h"
#include "snesbitorder.h"
#include <QDateTime>

InputSessionState::InputSessionState()
{
    m_sessionId = QByteArray(16, '\0');
    resetSession(quint64(QDateTime::currentMSecsSinceEpoch()));
}

void InputSessionState::setSessionId(const QByteArray &id16)
{
    m_sessionId = id16;
    if (m_sessionId.size() < 16)
        m_sessionId.append(QByteArray(16 - m_sessionId.size(), '\0'));
    else if (m_sessionId.size() > 16)
        m_sessionId = m_sessionId.left(16);
}

void InputSessionState::resetSession(quint64 nowMs)
{
    m_current = 0;
    m_sequence = 0;
    m_entryCount = 0;
    m_inputChangeCount = 0;
    m_lastChangeMs = nowMs;
    m_lastCheckpointMs = nowMs;
    m_sessionStartMs = nowMs;
    m_timesPressed.fill(0);
    m_timesReleased.fill(0);
    m_timesHeld.fill(0);
    m_totalHeldMs.fill(0);
    m_pressStartedMs.fill(0);
}

QString InputSessionState::sessionIdHex() const
{
    return QString::fromLatin1(m_sessionId.toHex().toUpper());
}

quint64 InputSessionState::heldFramesAt(quint64 nowMs, int bit) const
{
    if (bit < 0 || bit >= 16)
        return 0;
    return SnesBitOrder::heldFrames(m_pressStartedMs[size_t(bit)], nowMs);
}

void InputSessionState::accumulateHold(quint64 nowMs)
{
    if (nowMs <= m_lastCheckpointMs)
        return;
    const quint64 delta = nowMs - m_lastCheckpointMs;
    for (int bit = 0; bit < 16; ++bit) {
        const quint16 mask = quint16(1u << bit);
        if ((m_current & mask) != 0)
            m_totalHeldMs[size_t(bit)] += delta;
    }
}

bool InputSessionState::applyButton(InputProvider::SNESButton button, bool pressed, quint64 nowMs,
                                    quint16 *beforeOut, quint16 *afterOut, quint16 *pressedOut, quint16 *releasedOut)
{
    const quint16 mask = SnesBitOrder::maskForButton(button);
    if (mask == 0)
        return false;
    const quint16 before = m_current;
    const bool was = (before & mask) != 0;
    if (was == pressed)
        return false;

    accumulateHold(nowMs);
    quint16 after = before;
    quint16 pressedBits = 0;
    quint16 releasedBits = 0;
    const int bit = SnesBitOrder::bitIndexForButton(button);
    if (pressed) {
        after = quint16(before | mask);
        pressedBits = mask;
        if (bit >= 0 && m_timesPressed[size_t(bit)] < 0xFFFF)
            m_timesPressed[size_t(bit)]++;
        if (bit >= 0)
            m_pressStartedMs[size_t(bit)] = nowMs;
    } else {
        after = quint16(before & ~mask);
        releasedBits = mask;
        if (bit >= 0 && m_timesReleased[size_t(bit)] < 0xFFFF)
            m_timesReleased[size_t(bit)]++;
        if (bit >= 0)
            m_pressStartedMs[size_t(bit)] = 0;
    }
    m_current = after;
    m_lastChangeMs = nowMs;
    m_lastCheckpointMs = nowMs;
    for (int b = 0; b < 16; ++b) {
        if (after & (1u << b))
            m_timesHeld[size_t(b)]++;
    }
    if (beforeOut) *beforeOut = before;
    if (afterOut) *afterOut = after;
    if (pressedOut) *pressedOut = pressedBits;
    if (releasedOut) *releasedOut = releasedBits;
    return true;
}

void InputSessionState::noteCheckpoint(quint64 nowMs, bool /*countAsInputChange*/)
{
    accumulateHold(nowMs);
    m_lastCheckpointMs = nowMs;
    for (int bit = 0; bit < 16; ++bit) {
        if (m_current & (1u << bit))
            m_timesHeld[size_t(bit)]++;
    }
}

void InputSessionState::bumpSequenceAfterEntry(bool isInputChange)
{
    m_sequence++;
    m_entryCount++;
    if (isInputChange)
        m_inputChangeCount++;
}

bool InputSessionState::hasPendingChange(quint64 nowMs, quint64 idleMs) const
{
    Q_UNUSED(nowMs);
    Q_UNUSED(idleMs);
    return false;
}
