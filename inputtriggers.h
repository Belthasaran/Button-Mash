#ifndef INPUTTRIGGERS_H
#define INPUTTRIGGERS_H

#include "inputprovider.h"
#include <QObject>
#include <QString>
#include <QStringList>
#include <QTimer>
#include <QVector>
#include <QSet>

class QSettings;

/** One binding: SNES button or Extra slot → keyboard key or run-program macro. */
struct TriggerBinding
{
    enum class SourceKind { SnesButton, Extra };
    enum class ActionKind { None, Keyboard, RunProgram };

    SourceKind sourceKind = SourceKind::SnesButton;
    InputProvider::SNESButton snesButton = InputProvider::B;
    int extraIndex = 0; // Extra1 → 0
    ActionKind action = ActionKind::None;
    /** Linux KEY_* or Windows scancode (set 1) when action == Keyboard. */
    int keyCode = 0;
    QString keyLabel;
    QString programPath;
    QString programArgs;
    QString workingDirectory;
};

struct TriggerConfig
{
    bool enabled = false;
    bool linuxUinputGamepad = false; // Linux only; creates virtual pad via uinput
    QStringList windowTitleSubstrings; // keyboard maps require match (case-insensitive)
    QStringList windowClassSubstrings;
    QVector<TriggerBinding> bindings;

    static TriggerConfig defaults();
};

class InputTriggersEngine : public QObject
{
    Q_OBJECT
public:
    explicit InputTriggersEngine(QObject *parent = nullptr);
    ~InputTriggersEngine() override;

    void setConfig(const TriggerConfig &cfg);
    TriggerConfig config() const { return m_cfg; }

    void loadSettings(QSettings &settings);
    void saveSettings(QSettings &settings) const;

    void setEnabled(bool enabled);
    bool enabled() const { return m_cfg.enabled; }

    /** Open / recreate inject backends when session starts; tear down on stop. */
    void startSession();
    void stopSession();

    QString statusText() const { return m_status; }

public slots:
    void onButtonPressed(InputProvider::SNESButton button);
    void onButtonReleased(InputProvider::SNESButton button);

private slots:
    void onFocusPoll();

private:
    const TriggerBinding *findSnesBinding(InputProvider::SNESButton button) const;
    bool focusAllowsKeyboard() const;
    QString foregroundWindowTitle() const;
    QString foregroundWindowClass() const;
    void injectKey(int keyCode, bool pressed);
    void releaseAllHeldKeys();
    void runMacro(const TriggerBinding &b);
    bool ensureInjectReady();

    TriggerConfig m_cfg;
    QTimer m_focusTimer;
    bool m_sessionActive = false;
    bool m_focusOk = false;
    QSet<int> m_heldKeys;
    QString m_status;

#ifdef Q_OS_LINUX
    class UInputKeyboard *m_uinputKbd = nullptr;
    class UInputGamepad *m_uinputPad = nullptr;
#endif
};

#endif
