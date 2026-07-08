#include "skinmetadata.h"

#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

QString SkinMetadata::objectKey(const QString &kind, const QString &name) const
{
    return kind + QLatin1Char(':') + name;
}

ObjectMeta SkinMetadata::metaFor(const QString &key, const QMap<QString, ObjectMeta> &map) const
{
    return map.value(key);
}

void SkinMetadata::setMeta(const QString &key, const ObjectMeta &m, QMap<QString, ObjectMeta> &map)
{
    map[key] = m;
}

static QJsonObject metaToJson(const ObjectMeta &m)
{
    QJsonObject o;
    o.insert(QStringLiteral("locked"), m.locked);
    o.insert(QStringLiteral("zIndex"), m.zIndex);
    return o;
}

static ObjectMeta metaFromJson(const QJsonObject &o)
{
    ObjectMeta m;
    m.locked = o.value(QStringLiteral("locked")).toBool(false);
    m.zIndex = o.value(QStringLiteral("zIndex")).toInt(0);
    return m;
}

static void loadMap(const QJsonObject &parent, const QString &key, QMap<QString, ObjectMeta> &out)
{
    const QJsonObject objs = parent.value(key).toObject();
    for (auto it = objs.begin(); it != objs.end(); ++it)
        out[it.key()] = metaFromJson(it.value().toObject());
}

SkinMetadata SkinMetadata::load(const QString &skinDir)
{
    SkinMetadata m;
    const QString path = QDir(skinDir).filePath(QString::fromLatin1(kMetaFileName));
    QFile f(path);
    if (!f.open(QIODevice::ReadOnly))
        return m;
    const QJsonDocument doc = QJsonDocument::fromJson(f.readAll());
    if (!doc.isObject())
        return m;
    const QJsonObject root = doc.object();
    m.lastMode = root.value(QStringLiteral("lastMode")).toString();
    m.snapToGrid = root.value(QStringLiteral("snapToGrid")).toBool(false);
    m.gridStep = root.value(QStringLiteral("gridStep")).toInt(8);
    loadMap(root, QStringLiteral("regular"), m.regularObjects);
    loadMap(root, QStringLiteral("piano"), m.pianoObjects);
    return m;
}

bool SkinMetadata::save(const QString &skinDir) const
{
    QJsonObject root;
    root.insert(QStringLiteral("lastMode"), lastMode);
    root.insert(QStringLiteral("snapToGrid"), snapToGrid);
    root.insert(QStringLiteral("gridStep"), gridStep);

    auto dumpMap = [](const QMap<QString, ObjectMeta> &map) {
        QJsonObject o;
        for (auto it = map.constBegin(); it != map.constEnd(); ++it)
            o.insert(it.key(), metaToJson(it.value()));
        return o;
    };
    root.insert(QStringLiteral("regular"), dumpMap(regularObjects));
    root.insert(QStringLiteral("piano"), dumpMap(pianoObjects));

    const QString path = QDir(skinDir).filePath(QString::fromLatin1(kMetaFileName));
    QFile f(path);
    if (!f.open(QIODevice::WriteOnly))
        return false;
    f.write(QJsonDocument(root).toJson(QJsonDocument::Indented));
    return true;
}
