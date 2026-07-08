#include "uinputdevice.h"

#ifdef Q_OS_LINUX

#include "snesbitorder.h"

#include <fcntl.h>
#include <linux/input.h>
#include <linux/uinput.h>
#include <sys/time.h>
#include <cstring>
#include <unistd.h>
#include <QByteArray>

namespace {

bool writeAll(int fd, const void *buf, size_t len)
{
    const char *p = static_cast<const char *>(buf);
    size_t left = len;
    while (left > 0) {
        const ssize_t n = ::write(fd, p, left);
        if (n < 0)
            return false;
        p += n;
        left -= size_t(n);
    }
    return true;
}

bool emitEvent(int fd, quint16 type, quint16 code, qint32 value)
{
    input_event ev;
    memset(&ev, 0, sizeof(ev));
    gettimeofday(&ev.time, nullptr);
    ev.type = type;
    ev.code = code;
    ev.value = value;
    return writeAll(fd, &ev, sizeof(ev));
}

} // namespace

UInputKeyboard::UInputKeyboard() = default;

UInputKeyboard::~UInputKeyboard()
{
    close();
}

bool UInputKeyboard::open(QString *errorOut)
{
    close();
    m_fd = ::open("/dev/uinput", O_WRONLY | O_NONBLOCK);
    if (m_fd < 0) {
        if (errorOut)
            *errorOut = QStringLiteral("Cannot open /dev/uinput (need access / input group)");
        return false;
    }
    if (ioctl(m_fd, UI_SET_EVBIT, EV_KEY) < 0) {
        if (errorOut)
            *errorOut = QStringLiteral("UI_SET_EVBIT EV_KEY failed");
        close();
        return false;
    }
    // Enable a broad set of KEY_* so any mapped scancode works.
    for (int k = 1; k < KEY_MAX; ++k) {
        if (ioctl(m_fd, UI_SET_KEYBIT, k) < 0) {
            // ignore individual failures
        }
    }
    if (ioctl(m_fd, UI_SET_EVBIT, EV_SYN) < 0) {
        close();
        return false;
    }

    uinput_user_dev uud;
    memset(&uud, 0, sizeof(uud));
    snprintf(uud.name, UINPUT_MAX_NAME_SIZE, "Button Mash Keyboard");
    uud.id.bustype = BUS_VIRTUAL;
    uud.id.vendor = 0xB7A5;
    uud.id.product = 0x0001;
    uud.id.version = 1;
    if (!writeAll(m_fd, &uud, sizeof(uud))) {
        if (errorOut)
            *errorOut = QStringLiteral("uinput write device failed");
        close();
        return false;
    }
    if (ioctl(m_fd, UI_DEV_CREATE) < 0) {
        if (errorOut)
            *errorOut = QStringLiteral("UI_DEV_CREATE keyboard failed");
        close();
        return false;
    }
    return true;
}

void UInputKeyboard::close()
{
    if (m_fd >= 0) {
        ioctl(m_fd, UI_DEV_DESTROY);
        ::close(m_fd);
        m_fd = -1;
    }
}

bool UInputKeyboard::emitKey(int keyCode, bool pressed)
{
    if (m_fd < 0 || keyCode <= 0)
        return false;
    if (!emitEvent(m_fd, EV_KEY, quint16(keyCode), pressed ? 1 : 0))
        return false;
    return emitEvent(m_fd, EV_SYN, SYN_REPORT, 0);
}

UInputGamepad::UInputGamepad() = default;

UInputGamepad::~UInputGamepad()
{
    close();
}

int UInputGamepad::btnCodeForSnes(InputProvider::SNESButton button)
{
    switch (button) {
    case InputProvider::B: return BTN_SOUTH;
    case InputProvider::A: return BTN_EAST;
    case InputProvider::Y: return BTN_WEST;
    case InputProvider::X: return BTN_NORTH;
    case InputProvider::L: return BTN_TL;
    case InputProvider::R: return BTN_TR;
    case InputProvider::Select: return BTN_SELECT;
    case InputProvider::Start: return BTN_START;
    case InputProvider::Up: return BTN_DPAD_UP;
    case InputProvider::Down: return BTN_DPAD_DOWN;
    case InputProvider::Left: return BTN_DPAD_LEFT;
    case InputProvider::Right: return BTN_DPAD_RIGHT;
    }
    return -1;
}

bool UInputGamepad::open(QString *errorOut)
{
    close();
    m_fd = ::open("/dev/uinput", O_WRONLY | O_NONBLOCK);
    if (m_fd < 0) {
        if (errorOut)
            *errorOut = QStringLiteral("Cannot open /dev/uinput (need access / input group)");
        return false;
    }
    if (ioctl(m_fd, UI_SET_EVBIT, EV_KEY) < 0
        || ioctl(m_fd, UI_SET_EVBIT, EV_ABS) < 0
        || ioctl(m_fd, UI_SET_EVBIT, EV_SYN) < 0) {
        if (errorOut)
            *errorOut = QStringLiteral("uinput EVBIT setup failed");
        close();
        return false;
    }

    const int btns[] = {
        BTN_SOUTH, BTN_EAST, BTN_WEST, BTN_NORTH,
        BTN_TL, BTN_TR, BTN_SELECT, BTN_START,
        BTN_DPAD_UP, BTN_DPAD_DOWN, BTN_DPAD_LEFT, BTN_DPAD_RIGHT,
        BTN_MODE
    };
    for (int b : btns)
        ioctl(m_fd, UI_SET_KEYBIT, b);

    // ABS stubs so SDL/udev recognize a joystick.
    ioctl(m_fd, UI_SET_ABSBIT, ABS_X);
    ioctl(m_fd, UI_SET_ABSBIT, ABS_Y);

    uinput_user_dev uud;
    memset(&uud, 0, sizeof(uud));
    snprintf(uud.name, UINPUT_MAX_NAME_SIZE, "Button Mash Gamepad");
    uud.id.bustype = BUS_VIRTUAL;
    uud.id.vendor = 0xB7A5;
    uud.id.product = 0x0002;
    uud.id.version = 1;
    uud.absmin[ABS_X] = -32768;
    uud.absmax[ABS_X] = 32767;
    uud.absmin[ABS_Y] = -32768;
    uud.absmax[ABS_Y] = 32767;
    if (!writeAll(m_fd, &uud, sizeof(uud))) {
        if (errorOut)
            *errorOut = QStringLiteral("uinput write gamepad failed");
        close();
        return false;
    }
    if (ioctl(m_fd, UI_DEV_CREATE) < 0) {
        if (errorOut)
            *errorOut = QStringLiteral("UI_DEV_CREATE gamepad failed");
        close();
        return false;
    }
    m_bits = 0;
    return true;
}

void UInputGamepad::close()
{
    if (m_fd >= 0) {
        releaseAll();
        ioctl(m_fd, UI_DEV_DESTROY);
        ::close(m_fd);
        m_fd = -1;
    }
}

bool UInputGamepad::emitEv(unsigned type, unsigned code, int value)
{
    return emitEvent(m_fd, quint16(type), quint16(code), value);
}

bool UInputGamepad::setButton(InputProvider::SNESButton button, bool pressed)
{
    if (m_fd < 0)
        return false;
    const int code = btnCodeForSnes(button);
    if (code < 0)
        return false;
    const quint16 mask = SnesBitOrder::maskForButton(button);
    if (pressed)
        m_bits = quint16(m_bits | mask);
    else
        m_bits = quint16(m_bits & ~mask);
    if (!emitEv(EV_KEY, unsigned(code), pressed ? 1 : 0))
        return false;
    return emitEv(EV_SYN, SYN_REPORT, 0);
}

void UInputGamepad::releaseAll()
{
    if (m_fd < 0 || m_bits == 0)
        return;
    for (int bit = 4; bit < 16; ++bit) {
        if (!(m_bits & (1u << bit)))
            continue;
        InputProvider::SNESButton b = SnesBitOrder::buttonForBitIndex(bit);
        const int code = btnCodeForSnes(b);
        if (code >= 0)
            emitEv(EV_KEY, unsigned(code), 0);
    }
    emitEv(EV_SYN, SYN_REPORT, 0);
    m_bits = 0;
}

#endif // Q_OS_LINUX
