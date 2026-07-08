#include "snesbitorder.h"

int SnesBitOrder::bitIndexForButton(InputProvider::SNESButton button)
{
    switch (button) {
    case InputProvider::B: return B;
    case InputProvider::Y: return Y;
    case InputProvider::Select: return Select;
    case InputProvider::Start: return Start;
    case InputProvider::Up: return Up;
    case InputProvider::Down: return Down;
    case InputProvider::Left: return Left;
    case InputProvider::Right: return Right;
    case InputProvider::A: return A;
    case InputProvider::X: return X;
    case InputProvider::L: return L;
    case InputProvider::R: return R;
    }
    return -1;
}

InputProvider::SNESButton SnesBitOrder::buttonForBitIndex(int bit)
{
    switch (bit) {
    case B: return InputProvider::B;
    case Y: return InputProvider::Y;
    case Select: return InputProvider::Select;
    case Start: return InputProvider::Start;
    case Up: return InputProvider::Up;
    case Down: return InputProvider::Down;
    case Left: return InputProvider::Left;
    case Right: return InputProvider::Right;
    case A: return InputProvider::A;
    case X: return InputProvider::X;
    case L: return InputProvider::L;
    case R: return InputProvider::R;
    default: return InputProvider::A; // unused signature bits — callers must filter
    }
}

QString SnesBitOrder::nameForBitIndex(int bit)
{
    switch (bit) {
    case B: return QStringLiteral("B");
    case Y: return QStringLiteral("Y");
    case Select: return QStringLiteral("Select");
    case Start: return QStringLiteral("Start");
    case Up: return QStringLiteral("Up");
    case Down: return QStringLiteral("Down");
    case Left: return QStringLiteral("Left");
    case Right: return QStringLiteral("Right");
    case A: return QStringLiteral("A");
    case X: return QStringLiteral("X");
    case L: return QStringLiteral("L");
    case R: return QStringLiteral("R");
    default: return QString();
    }
}

QString SnesBitOrder::abbrevForBitIndex(int bit)
{
    switch (bit) {
    case B: return QStringLiteral("B");
    case Y: return QStringLiteral("Y");
    case Select: return QStringLiteral("[SEL]");
    case Start: return QStringLiteral("[ST]");
    case Up: return QStringLiteral("U");
    case Down: return QStringLiteral("D");
    case Left: return QStringLiteral("Lf");
    case Right: return QStringLiteral("Rt");
    case A: return QStringLiteral("A");
    case X: return QStringLiteral("X");
    case L: return QStringLiteral("L");
    case R: return QStringLiteral("R");
    default: return QString();
    }
}

QString SnesBitOrder::formatEdgeString(quint16 bits)
{
    QString out;
    for (int bit = 15; bit >= 4; --bit) {
        if (bits & (1u << bit))
            out += abbrevForBitIndex(bit);
    }
    return out;
}

QString SnesBitOrder::formatHeldString(quint16 afterBits, const std::array<quint64, 16> &pressStartedMs, quint64 nowMs)
{
    QStringList parts;
    for (int bit = 15; bit >= 4; --bit) {
        if ((afterBits & (1u << bit)) == 0)
            continue;
        const quint64 frames = heldFrames(pressStartedMs[size_t(bit)], nowMs);
        // Frames==0 means the press started at this entry (not yet held across frames).
        if (frames == 0)
            continue;
        parts << QStringLiteral("%1=%2").arg(abbrevForBitIndex(bit)).arg(frames);
    }
    return parts.join(QLatin1Char(' '));
}
