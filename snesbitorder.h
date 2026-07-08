#ifndef SNESBITORDER_H
#define SNESBITORDER_H

#include "inputprovider.h"
#include <QStringList>

// Hardware JOY1 / serial poll bit positions (bit 15 = B, first bit read).
namespace SnesBitOrder {
enum Bit : int {
    Sig0 = 0,
    Sig1 = 1,
    Sig2 = 2,
    Sig3 = 3,
    R = 4,
    L = 5,
    X = 6,
    A = 7,
    Right = 8,
    Left = 9,
    Down = 10,
    Up = 11,
    Start = 12,
    Select = 13,
    Y = 14,
    B = 15,
    Count = 16
};

constexpr double kSnesFrameMs = 1000.0 / 60.0;

int bitIndexForButton(InputProvider::SNESButton button);
InputProvider::SNESButton buttonForBitIndex(int bit);
QString nameForBitIndex(int bit);
QString abbrevForBitIndex(int bit);

/** Concatenate abbreviations for set bits, scanning 15→4 (hardware order). */
QString formatEdgeString(quint16 bits);

/**
 * Held display "B=3 R=5" for currently-down buttons using continuous hold start times.
 * pressStartedMs[bit] == 0 means not held / unknown.
 * Buttons with 0 held frames (just pressed this entry) are omitted.
 */
QString formatHeldString(quint16 afterBits, const std::array<quint64, 16> &pressStartedMs, quint64 nowMs);

inline quint64 heldFrames(quint64 holdStartMs, quint64 nowMs)
{
    if (holdStartMs == 0 || nowMs <= holdStartMs)
        return 0;
    return quint64((nowMs - holdStartMs) / kSnesFrameMs);
}

inline quint16 maskForButton(InputProvider::SNESButton button)
{
    int b = bitIndexForButton(button);
    if (b < 0)
        return 0;
    return quint16(1u << b);
}

inline bool isPressed(quint16 bits, InputProvider::SNESButton button)
{
    return (bits & maskForButton(button)) != 0;
}

} // namespace SnesBitOrder

#endif // SNESBITORDER_H
