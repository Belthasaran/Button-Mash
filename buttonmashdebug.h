#ifndef BUTTONMASHDEBUG_H
#define BUTTONMASHDEBUG_H

#include <QLoggingCategory>
#include <QProcessEnvironment>
#include <QString>

class QSettings;

Q_DECLARE_LOGGING_CATEGORY(buttonmashLog)

class ButtonMashDebug
{
public:
    static void initFromEnvironment(const QProcessEnvironment &env);
    static void loadFromSettings(QSettings &settings);
    static void setSettingsEnabled(bool enabled);
    static void saveToSettings(QSettings &settings);

    static bool isEnabled();
    static bool settingsEnabled();
    static bool envEnabled();
    static void applyLoggingRules();

    static bool parseTruthyEnv(const QString &value);
    static int selfTest();
};

#endif // BUTTONMASHDEBUG_H
