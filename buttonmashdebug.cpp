#include "buttonmashdebug.h"

#include <QLoggingCategory>
#include <QSettings>

Q_LOGGING_CATEGORY(buttonmashLog, "buttonmash")

namespace {

const char kSettingsKey[] = "debug/consoleOutput";
const char kEnvKey[] = "BUTTONMASH_DEBUG";

bool s_envEnabled = false;
bool s_settingsEnabled = false;

} // namespace

bool ButtonMashDebug::parseTruthyEnv(const QString &value)
{
    const QString v = value.trimmed().toLower();
    return v == QLatin1String("1")
        || v == QLatin1String("true")
        || v == QLatin1String("yes")
        || v == QLatin1String("on");
}

void ButtonMashDebug::initFromEnvironment(const QProcessEnvironment &env)
{
    if (!env.contains(QLatin1String(kEnvKey))) {
        s_envEnabled = false;
        return;
    }
    s_envEnabled = parseTruthyEnv(env.value(QLatin1String(kEnvKey)));
}

void ButtonMashDebug::loadFromSettings(QSettings &settings)
{
    s_settingsEnabled = settings.value(QLatin1String(kSettingsKey), false).toBool();
}

void ButtonMashDebug::setSettingsEnabled(bool enabled)
{
    s_settingsEnabled = enabled;
}

void ButtonMashDebug::saveToSettings(QSettings &settings)
{
    settings.setValue(QLatin1String(kSettingsKey), s_settingsEnabled);
}

bool ButtonMashDebug::envEnabled()
{
    return s_envEnabled;
}

bool ButtonMashDebug::settingsEnabled()
{
    return s_settingsEnabled;
}

bool ButtonMashDebug::isEnabled()
{
    return s_envEnabled || s_settingsEnabled;
}

void ButtonMashDebug::applyLoggingRules()
{
    if (isEnabled()) {
        QLoggingCategory::setFilterRules(
            QStringLiteral("buttonmash.debug=true\nqt.gamepad.debug=true"));
    } else {
        QLoggingCategory::setFilterRules(
            QStringLiteral("buttonmash.debug=false\nqt.gamepad.debug=false"));
    }
}

int ButtonMashDebug::selfTest()
{
    if (!parseTruthyEnv(QStringLiteral("1")))
        return 1;
    if (!parseTruthyEnv(QStringLiteral("TRUE")))
        return 2;
    if (!parseTruthyEnv(QStringLiteral("yes")))
        return 3;
    if (!parseTruthyEnv(QStringLiteral("on")))
        return 4;
    if (parseTruthyEnv(QStringLiteral("0")))
        return 5;
    if (parseTruthyEnv(QStringLiteral("false")))
        return 6;
    if (parseTruthyEnv(QStringLiteral("")))
        return 7;

    s_envEnabled = false;
    s_settingsEnabled = false;
    if (isEnabled())
        return 8;

    s_settingsEnabled = true;
    if (!isEnabled())
        return 9;

    s_envEnabled = true;
    s_settingsEnabled = false;
    if (!isEnabled())
        return 10;

    applyLoggingRules();
    applyLoggingRules();

    s_envEnabled = false;
    s_settingsEnabled = false;
    return 0;
}
