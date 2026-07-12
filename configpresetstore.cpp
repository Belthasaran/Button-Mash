#include "configpresetstore.h"

#include <QSettings>
#include <QTemporaryFile>
#include <QVariantMap>

namespace {

const char kNamesKey[] = "presets/names";
const char kLastSelectedKey[] = "presets/lastSelected";
const char kPresetsGroup[] = "presets";

const char *kPresetTreeGroups[] = {
    "inputSource",
    "mirror",
    "triggers",
    "lastSkin",
};

const char *kPresetRootScalars[] = {
    "delai",
    "skinFolder",
};

QString presetGroupPath(const QString &name)
{
    return QString::fromLatin1(kPresetsGroup) + QLatin1Char('/') + name;
}

QVariantMap readSettingsGroup(QSettings &settings)
{
    QVariantMap out;
    const QStringList keys = settings.childKeys();
    for (const QString &key : keys)
        out.insert(key, settings.value(key));

    const QStringList groups = settings.childGroups();
    for (const QString &group : groups) {
        settings.beginGroup(group);
        out.insert(group, readSettingsGroup(settings));
        settings.endGroup();
    }
    return out;
}

void writeSettingsGroup(QSettings &settings, const QVariantMap &map)
{
    for (auto it = map.constBegin(); it != map.constEnd(); ++it) {
        if (it.value().type() == QVariant::Map) {
            settings.beginGroup(it.key());
            writeSettingsGroup(settings, it.value().toMap());
            settings.endGroup();
        } else {
            settings.setValue(it.key(), it.value());
        }
    }
}

void removeSettingsGroup(QSettings &settings, const QString &group)
{
    settings.beginGroup(group);

    const QStringList groups = settings.childGroups();
    for (const QString &sub : groups)
        removeSettingsGroup(settings, sub);

    settings.remove(QString());
    settings.endGroup();
}

QVariantMap capturePresetData(QSettings &settings, const QString &prefix)
{
    QVariantMap data;
    if (!prefix.isEmpty())
        settings.beginGroup(prefix);

    for (const char *group : kPresetTreeGroups) {
        settings.beginGroup(QString::fromLatin1(group));
        data.insert(QString::fromLatin1(group), readSettingsGroup(settings));
        settings.endGroup();
    }
    for (const char *scalar : kPresetRootScalars)
        data.insert(QString::fromLatin1(scalar), settings.value(QString::fromLatin1(scalar)));

    if (!prefix.isEmpty())
        settings.endGroup();
    return data;
}

void applyPresetData(QSettings &settings, const QString &prefix, const QVariantMap &data)
{
    if (!prefix.isEmpty())
        settings.beginGroup(prefix);

    for (const char *group : kPresetTreeGroups) {
        const QString groupName = QString::fromLatin1(group);
        removeSettingsGroup(settings, groupName);
        if (data.contains(groupName)) {
            settings.beginGroup(groupName);
            writeSettingsGroup(settings, data.value(groupName).toMap());
            settings.endGroup();
        }
    }
    for (const char *scalar : kPresetRootScalars) {
        const QString scalarName = QString::fromLatin1(scalar);
        if (data.contains(scalarName))
            settings.setValue(scalarName, data.value(scalarName));
        else
            settings.remove(scalarName);
    }

    if (!prefix.isEmpty())
        settings.endGroup();
}

void removePresetGroup(QSettings &settings, const QString &name)
{
    removeSettingsGroup(settings, presetGroupPath(name));
}

} // namespace

QStringList ConfigPresetStore::names(QSettings &settings)
{
    return settings.value(QString::fromLatin1(kNamesKey)).toStringList();
}

bool ConfigPresetStore::isValidPresetName(const QString &name, QString *error)
{
    const QString trimmed = name.trimmed();
    if (trimmed.isEmpty()) {
        if (error)
            *error = QStringLiteral("Preset name cannot be empty.");
        return false;
    }
    if (trimmed.contains(QLatin1Char('/')) || trimmed.contains(QLatin1Char('\\'))) {
        if (error)
            *error = QStringLiteral("Preset name cannot contain / or \\.");
        return false;
    }
    return true;
}

bool ConfigPresetStore::savePreset(QSettings &settings, const QString &name)
{
    QString err;
    if (!isValidPresetName(name, &err))
        return false;

    const QString trimmed = name.trimmed();
    const QVariantMap data = capturePresetData(settings, QString());
    removePresetGroup(settings, trimmed);
    applyPresetData(settings, presetGroupPath(trimmed), data);

    QStringList presetNames = names(settings);
    if (!presetNames.contains(trimmed)) {
        presetNames.append(trimmed);
        settings.setValue(QString::fromLatin1(kNamesKey), presetNames);
    }
    settings.sync();
    return true;
}

bool ConfigPresetStore::applyPreset(QSettings &settings, const QString &name)
{
    QString err;
    if (!isValidPresetName(name, &err))
        return false;

    const QString trimmed = name.trimmed();
    if (!names(settings).contains(trimmed))
        return false;

    const QVariantMap data = capturePresetData(settings, presetGroupPath(trimmed));
    applyPresetData(settings, QString(), data);

    settings.setValue(QString::fromLatin1(kLastSelectedKey), trimmed);
    settings.sync();
    return true;
}

bool ConfigPresetStore::deletePreset(QSettings &settings, const QString &name)
{
    QString err;
    if (!isValidPresetName(name, &err))
        return false;

    const QString trimmed = name.trimmed();
    QStringList presetNames = names(settings);
    if (!presetNames.removeAll(trimmed))
        return false;

    settings.setValue(QString::fromLatin1(kNamesKey), presetNames);
    removePresetGroup(settings, trimmed);

    if (settings.value(QString::fromLatin1(kLastSelectedKey)).toString() == trimmed)
        settings.remove(QString::fromLatin1(kLastSelectedKey));

    settings.sync();
    return true;
}

int ConfigPresetStore::selfTest()
{
    QTemporaryFile tmp;
    tmp.setAutoRemove(true);
    if (!tmp.open())
        return 1;

    const QString path = tmp.fileName();
    tmp.close();

    QSettings s(path, QSettings::IniFormat);
    s.clear();
    s.setValue(QStringLiteral("skinFolder"), QStringLiteral("/skins/a"));
    s.setValue(QStringLiteral("delai"), 42);
    s.setValue(QStringLiteral("mirror/shareEnabled"), true);
    s.setValue(QStringLiteral("triggers/enabled"), false);
    s.setValue(QStringLiteral("inputSource/inputSource"), QStringLiteral("RemoteServer"));
    s.setValue(QStringLiteral("lastSkin/pianoDisplay"), true);
    s.sync();

    if (!savePreset(s, QStringLiteral("PresetA")))
        return 2;

    if (!names(s).contains(QStringLiteral("PresetA")))
        return 3;

    s.setValue(QStringLiteral("skinFolder"), QStringLiteral("/skins/b"));
    s.setValue(QStringLiteral("delai"), 7);
    s.setValue(QStringLiteral("mirror/shareEnabled"), false);

    if (!applyPreset(s, QStringLiteral("PresetA")))
        return 4;

    if (s.value(QStringLiteral("skinFolder")).toString() != QStringLiteral("/skins/a"))
        return 5;
    if (s.value(QStringLiteral("delai")).toInt() != 42)
        return 6;
    if (!s.value(QStringLiteral("mirror/shareEnabled")).toBool())
        return 7;

    s.setValue(QStringLiteral("skinFolder"), QStringLiteral("/skins/updated"));
    if (!savePreset(s, QStringLiteral("PresetA")))
        return 8;
    if (!applyPreset(s, QStringLiteral("PresetA")))
        return 9;
    if (s.value(QStringLiteral("skinFolder")).toString() != QStringLiteral("/skins/updated"))
        return 10;

    if (!deletePreset(s, QStringLiteral("PresetA")))
        return 11;
    if (!names(s).isEmpty())
        return 12;

    return 0;
}
