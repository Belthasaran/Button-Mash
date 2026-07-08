#include "inputtriggers.h"
#include "uinputdevice.h"

#include <QProcess>
#include <QSettings>
#include <QSysInfo>

#ifdef Q_OS_WIN
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif

#ifdef Q_OS_LINUX
#include <linux/input-event-codes.h>
#endif

namespace {

TriggerBinding makeKbd(InputProvider::SNESButton btn, int keyCode, const QString &label)
{
    TriggerBinding b;
    b.sourceKind = TriggerBinding::SourceKind::SnesButton;
    b.snesButton = btn;
    b.action = TriggerBinding::ActionKind::Keyboard;
    b.keyCode = keyCode;
    b.keyLabel = label;
    return b;
}

} // namespace

TriggerConfig TriggerConfig::defaults()
{
    TriggerConfig c;
    c.enabled = false;
    c.linuxUinputGamepad = false;
    c.windowTitleSubstrings = QStringList{QStringLiteral("RetroArch")};
#ifdef Q_OS_WIN
    // Windows set-1 scancodes
    c.bindings = {
        makeKbd(InputProvider::Up, 0x48, QStringLiteral("Up")),
        makeKbd(InputProvider::Down, 0x50, QStringLiteral("Down")),
        makeKbd(InputProvider::Left, 0x4B, QStringLiteral("Left")),
        makeKbd(InputProvider::Right, 0x4D, QStringLiteral("Right")),
        makeKbd(InputProvider::A, 0x1E, QStringLiteral("A")),
        makeKbd(InputProvider::B, 0x1F, QStringLiteral("S")),
        makeKbd(InputProvider::X, 0x10, QStringLiteral("Q")),
        makeKbd(InputProvider::Y, 0x11, QStringLiteral("W")),
        makeKbd(InputProvider::L, 0x2A, QStringLiteral("LShift")),
        makeKbd(InputProvider::R, 0x36, QStringLiteral("RShift")),
        makeKbd(InputProvider::Start, 0x1C, QStringLiteral("Enter")),
        makeKbd(InputProvider::Select, 0x39, QStringLiteral("Space")),
    };
#elif defined(Q_OS_LINUX)
    // Linux KEY_* codes
    c.bindings = {
        makeKbd(InputProvider::Up, KEY_UP, QStringLiteral("Up")),
        makeKbd(InputProvider::Down, KEY_DOWN, QStringLiteral("Down")),
        makeKbd(InputProvider::Left, KEY_LEFT, QStringLiteral("Left")),
        makeKbd(InputProvider::Right, KEY_RIGHT, QStringLiteral("Right")),
        makeKbd(InputProvider::A, KEY_A, QStringLiteral("A")),
        makeKbd(InputProvider::B, KEY_S, QStringLiteral("S")),
        makeKbd(InputProvider::X, KEY_Q, QStringLiteral("Q")),
        makeKbd(InputProvider::Y, KEY_W, QStringLiteral("W")),
        makeKbd(InputProvider::L, KEY_LEFTSHIFT, QStringLiteral("LShift")),
        makeKbd(InputProvider::R, KEY_RIGHTSHIFT, QStringLiteral("RShift")),
        makeKbd(InputProvider::Start, KEY_ENTER, QStringLiteral("Enter")),
        makeKbd(InputProvider::Select, KEY_SPACE, QStringLiteral("Space")),
    };
#endif
    // One empty Extra slot for macros (example Extra1)
    TriggerBinding extra;
    extra.sourceKind = TriggerBinding::SourceKind::Extra;
    extra.extraIndex = 0;
    extra.action = TriggerBinding::ActionKind::None;
    c.bindings.append(extra);
    return c;
}

InputTriggersEngine::InputTriggersEngine(QObject *parent)
    : QObject(parent)
    , m_cfg(TriggerConfig::defaults())
{
    m_focusTimer.setInterval(100);
    connect(&m_focusTimer, &QTimer::timeout, this, &InputTriggersEngine::onFocusPoll);
}

InputTriggersEngine::~InputTriggersEngine()
{
    stopSession();
}

void InputTriggersEngine::setConfig(const TriggerConfig &cfg)
{
    m_cfg = cfg;
}

void InputTriggersEngine::setEnabled(bool enabled)
{
    m_cfg.enabled = enabled;
}

void InputTriggersEngine::loadSettings(QSettings &settings)
{
    TriggerConfig c = TriggerConfig::defaults();
    c.enabled = settings.value(QStringLiteral("triggers/enabled"), false).toBool();
    c.linuxUinputGamepad = settings.value(QStringLiteral("triggers/linuxUinputGamepad"), false).toBool();
    c.windowTitleSubstrings = settings.value(QStringLiteral("triggers/windowTitles"),
                                             c.windowTitleSubstrings).toStringList();
    c.windowClassSubstrings = settings.value(QStringLiteral("triggers/windowClasses"),
                                             c.windowClassSubstrings).toStringList();

    const int n = settings.beginReadArray(QStringLiteral("triggers/bindings"));
    if (n > 0) {
        c.bindings.clear();
        for (int i = 0; i < n; ++i) {
            settings.setArrayIndex(i);
            TriggerBinding b;
            const QString kind = settings.value(QStringLiteral("sourceKind"), QStringLiteral("snes")).toString();
            if (kind == QLatin1String("extra")) {
                b.sourceKind = TriggerBinding::SourceKind::Extra;
                b.extraIndex = settings.value(QStringLiteral("extraIndex"), 0).toInt();
            } else {
                b.sourceKind = TriggerBinding::SourceKind::SnesButton;
                b.snesButton = InputProvider::SNESButton(
                    settings.value(QStringLiteral("snesButton"), int(InputProvider::B)).toInt());
            }
            const QString act = settings.value(QStringLiteral("action"), QStringLiteral("none")).toString();
            if (act == QLatin1String("keyboard"))
                b.action = TriggerBinding::ActionKind::Keyboard;
            else if (act == QLatin1String("runProgram"))
                b.action = TriggerBinding::ActionKind::RunProgram;
            else
                b.action = TriggerBinding::ActionKind::None;
            b.keyCode = settings.value(QStringLiteral("keyCode"), 0).toInt();
            b.keyLabel = settings.value(QStringLiteral("keyLabel")).toString();
            b.programPath = settings.value(QStringLiteral("programPath")).toString();
            b.programArgs = settings.value(QStringLiteral("programArgs")).toString();
            b.workingDirectory = settings.value(QStringLiteral("workingDirectory")).toString();
            c.bindings.append(b);
        }
    }
    settings.endArray();
    m_cfg = c;
}

void InputTriggersEngine::saveSettings(QSettings &settings) const
{
    settings.setValue(QStringLiteral("triggers/enabled"), m_cfg.enabled);
    settings.setValue(QStringLiteral("triggers/linuxUinputGamepad"), m_cfg.linuxUinputGamepad);
    settings.setValue(QStringLiteral("triggers/windowTitles"), m_cfg.windowTitleSubstrings);
    settings.setValue(QStringLiteral("triggers/windowClasses"), m_cfg.windowClassSubstrings);

    settings.beginWriteArray(QStringLiteral("triggers/bindings"), m_cfg.bindings.size());
    for (int i = 0; i < m_cfg.bindings.size(); ++i) {
        settings.setArrayIndex(i);
        const TriggerBinding &b = m_cfg.bindings.at(i);
        if (b.sourceKind == TriggerBinding::SourceKind::Extra) {
            settings.setValue(QStringLiteral("sourceKind"), QStringLiteral("extra"));
            settings.setValue(QStringLiteral("extraIndex"), b.extraIndex);
        } else {
            settings.setValue(QStringLiteral("sourceKind"), QStringLiteral("snes"));
            settings.setValue(QStringLiteral("snesButton"), int(b.snesButton));
        }
        QString act = QStringLiteral("none");
        if (b.action == TriggerBinding::ActionKind::Keyboard)
            act = QStringLiteral("keyboard");
        else if (b.action == TriggerBinding::ActionKind::RunProgram)
            act = QStringLiteral("runProgram");
        settings.setValue(QStringLiteral("action"), act);
        settings.setValue(QStringLiteral("keyCode"), b.keyCode);
        settings.setValue(QStringLiteral("keyLabel"), b.keyLabel);
        settings.setValue(QStringLiteral("programPath"), b.programPath);
        settings.setValue(QStringLiteral("programArgs"), b.programArgs);
        settings.setValue(QStringLiteral("workingDirectory"), b.workingDirectory);
    }
    settings.endArray();
}

void InputTriggersEngine::startSession()
{
    stopSession();
    m_sessionActive = true;
    m_heldKeys.clear();
    m_focusOk = false;
    if (!m_cfg.enabled) {
        m_status = QStringLiteral("Triggers disabled");
        return;
    }
    if (!ensureInjectReady())
        return;
    m_focusTimer.start();
    onFocusPoll();
    m_status = QStringLiteral("Triggers active");
}

void InputTriggersEngine::stopSession()
{
    m_focusTimer.stop();
    releaseAllHeldKeys();
#ifdef Q_OS_LINUX
    if (m_uinputPad) {
        m_uinputPad->close();
        delete m_uinputPad;
        m_uinputPad = nullptr;
    }
    if (m_uinputKbd) {
        m_uinputKbd->close();
        delete m_uinputKbd;
        m_uinputKbd = nullptr;
    }
#endif
    m_sessionActive = false;
}

bool InputTriggersEngine::ensureInjectReady()
{
#ifdef Q_OS_LINUX
    const bool needKbd = [&]() {
        for (const TriggerBinding &b : m_cfg.bindings) {
            if (b.action == TriggerBinding::ActionKind::Keyboard)
                return true;
        }
        return false;
    }();
    if (needKbd) {
        if (!m_uinputKbd)
            m_uinputKbd = new UInputKeyboard;
        QString err;
        if (!m_uinputKbd->isOpen() && !m_uinputKbd->open(&err)) {
            m_status = err;
            return false;
        }
    }
    if (m_cfg.linuxUinputGamepad) {
        if (!m_uinputPad)
            m_uinputPad = new UInputGamepad;
        QString err;
        if (!m_uinputPad->isOpen() && !m_uinputPad->open(&err)) {
            m_status = err;
            return false;
        }
    }
#else
    Q_UNUSED(this);
#endif
    return true;
}

const TriggerBinding *InputTriggersEngine::findSnesBinding(InputProvider::SNESButton button) const
{
    for (const TriggerBinding &b : m_cfg.bindings) {
        if (b.sourceKind == TriggerBinding::SourceKind::SnesButton && b.snesButton == button)
            return &b;
    }
    return nullptr;
}

bool InputTriggersEngine::focusAllowsKeyboard() const
{
    if (m_cfg.windowTitleSubstrings.isEmpty() && m_cfg.windowClassSubstrings.isEmpty())
        return true; // no restriction configured

    const QString title = foregroundWindowTitle();
    const QString cls = foregroundWindowClass();
    for (const QString &sub : m_cfg.windowTitleSubstrings) {
        if (!sub.isEmpty() && title.contains(sub, Qt::CaseInsensitive))
            return true;
    }
    for (const QString &sub : m_cfg.windowClassSubstrings) {
        if (!sub.isEmpty() && cls.contains(sub, Qt::CaseInsensitive))
            return true;
    }
    return false;
}

QString InputTriggersEngine::foregroundWindowTitle() const
{
#ifdef Q_OS_WIN
    HWND hwnd = GetForegroundWindow();
    if (!hwnd)
        return QString();
    wchar_t buf[512];
    const int n = GetWindowTextW(hwnd, buf, 512);
    if (n <= 0)
        return QString();
    return QString::fromWCharArray(buf, n);
#elif defined(Q_OS_LINUX)
    // Best-effort via xdotool-like env; xprop if DISPLAY set. Fail closed when unknown.
    const QByteArray display = qgetenv("DISPLAY");
    if (display.isEmpty())
        return QString(); // Wayland often has no DISPLAY — focus unknown
    QProcess p;
    p.start(QStringLiteral("xdotool"), {QStringLiteral("getactivewindow"), QStringLiteral("getwindowname")});
    if (!p.waitForFinished(200) || p.exitCode() != 0)
        return QString();
    return QString::fromUtf8(p.readAllStandardOutput()).trimmed();
#else
    return QString();
#endif
}

QString InputTriggersEngine::foregroundWindowClass() const
{
#ifdef Q_OS_WIN
    HWND hwnd = GetForegroundWindow();
    if (!hwnd)
        return QString();
    wchar_t buf[256];
    if (GetClassNameW(hwnd, buf, 256) <= 0)
        return QString();
    return QString::fromWCharArray(buf);
#else
    return QString();
#endif
}

void InputTriggersEngine::injectKey(int keyCode, bool pressed)
{
    if (keyCode <= 0)
        return;
#ifdef Q_OS_WIN
    INPUT in;
    ZeroMemory(&in, sizeof(in));
    in.type = INPUT_KEYBOARD;
    in.ki.wVk = 0;
    in.ki.wScan = WORD(keyCode);
    in.ki.dwFlags = KEYEVENTF_SCANCODE | (pressed ? 0 : KEYEVENTF_KEYUP);
    // Extended keys (arrows etc.) need KEYEVENTF_EXTENDEDKEY for some scancodes.
    if (keyCode == 0x48 || keyCode == 0x50 || keyCode == 0x4B || keyCode == 0x4D)
        in.ki.dwFlags |= KEYEVENTF_EXTENDEDKEY;
    SendInput(1, &in, sizeof(INPUT));
#elif defined(Q_OS_LINUX)
    if (m_uinputKbd)
        m_uinputKbd->emitKey(keyCode, pressed);
#else
    Q_UNUSED(pressed);
#endif
    if (pressed)
        m_heldKeys.insert(keyCode);
    else
        m_heldKeys.remove(keyCode);
}

void InputTriggersEngine::releaseAllHeldKeys()
{
    const QSet<int> copy = m_heldKeys;
    for (int k : copy)
        injectKey(k, false);
    m_heldKeys.clear();
#ifdef Q_OS_LINUX
    if (m_uinputPad)
        m_uinputPad->releaseAll();
#endif
}

void InputTriggersEngine::runMacro(const TriggerBinding &b)
{
    if (b.action != TriggerBinding::ActionKind::RunProgram || b.programPath.isEmpty())
        return;
    QStringList args = b.programArgs.split(QLatin1Char(' '), Qt::SkipEmptyParts);
    QProcess::startDetached(b.programPath, args,
                            b.workingDirectory.isEmpty() ? QString() : b.workingDirectory);
}

void InputTriggersEngine::onFocusPoll()
{
    if (!m_sessionActive || !m_cfg.enabled)
        return;
    const bool ok = focusAllowsKeyboard();
    if (m_focusOk && !ok)
        releaseAllHeldKeys();
    m_focusOk = ok;
}

void InputTriggersEngine::onButtonPressed(InputProvider::SNESButton button)
{
    if (!m_sessionActive || !m_cfg.enabled)
        return;

#ifdef Q_OS_LINUX
    if (m_uinputPad && m_uinputPad->isOpen())
        m_uinputPad->setButton(button, true);
#endif

    const TriggerBinding *b = findSnesBinding(button);
    if (!b || b->action == TriggerBinding::ActionKind::None)
        return;

    if (b->action == TriggerBinding::ActionKind::RunProgram) {
        runMacro(*b);
        return;
    }
    if (b->action == TriggerBinding::ActionKind::Keyboard) {
        if (!m_focusOk)
            return;
        injectKey(b->keyCode, true);
    }
}

void InputTriggersEngine::onButtonReleased(InputProvider::SNESButton button)
{
    if (!m_sessionActive || !m_cfg.enabled)
        return;

#ifdef Q_OS_LINUX
    if (m_uinputPad && m_uinputPad->isOpen())
        m_uinputPad->setButton(button, false);
#endif

    const TriggerBinding *b = findSnesBinding(button);
    if (!b || b->action != TriggerBinding::ActionKind::Keyboard)
        return;
    // Always release our key if we were holding it (even if focus already lost —
    // releaseAllHeldKeys may have already cleared).
    if (m_heldKeys.contains(b->keyCode) || m_focusOk)
        injectKey(b->keyCode, false);
}
