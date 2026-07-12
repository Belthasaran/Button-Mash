#ifndef CONFIGPRESETSTORE_H
#define CONFIGPRESETSTORE_H

#include <QString>
#include <QStringList>

class QSettings;

class ConfigPresetStore
{
public:
    static QStringList names(QSettings &settings);
    static bool isValidPresetName(const QString &name, QString *error = nullptr);
    static bool savePreset(QSettings &settings, const QString &name);
    static bool applyPreset(QSettings &settings, const QString &name);
    static bool deletePreset(QSettings &settings, const QString &name);

    /** Headless self-test; returns 0 on success, non-zero on failure. */
    static int selfTest();
};

#endif // CONFIGPRESETSTORE_H
