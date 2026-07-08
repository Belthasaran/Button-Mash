#ifndef UINPUTDEVICE_H
#define UINPUTDEVICE_H

#include "inputprovider.h"
#include <QtGlobal>

#ifdef Q_OS_LINUX

#include <QString>

/** Userspace virtual keyboard via /dev/uinput (X11 and Wayland). */
class UInputKeyboard
{
public:
    UInputKeyboard();
    ~UInputKeyboard();

    bool open(QString *errorOut = nullptr);
    void close();
    bool isOpen() const { return m_fd >= 0; }

    /** keyCode is Linux KEY_* from linux/input-event-codes.h */
    bool emitKey(int keyCode, bool pressed);

private:
    int m_fd = -1;
};

/** Userspace virtual gamepad (BTN_* + ABS stubs for joystick recognition). */
class UInputGamepad
{
public:
    UInputGamepad();
    ~UInputGamepad();

    bool open(QString *errorOut = nullptr);
    void close();
    bool isOpen() const { return m_fd >= 0; }

    bool setButton(InputProvider::SNESButton button, bool pressed);
    void releaseAll();

    static int btnCodeForSnes(InputProvider::SNESButton button);

private:
    bool emitEv(unsigned type, unsigned code, int value);
    int m_fd = -1;
    quint16 m_bits = 0;
};

#endif // Q_OS_LINUX

#endif
