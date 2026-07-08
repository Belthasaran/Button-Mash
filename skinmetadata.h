#ifndef SKINMETADATA_H
#define SKINMETADATA_H

#include <QJsonObject>
#include <QString>

struct ObjectMeta
{
    bool locked = false;
    int zIndex = 0;
};

class SkinMetadata
{
public:
    static constexpr const char *kMetaFileName = "masheditor.meta.json";

    QString lastMode; // "regular" | "piano"
    bool snapToGrid = false;
    int gridStep = 8;
    QMap<QString, ObjectMeta> regularObjects;
    QMap<QString, ObjectMeta> pianoObjects;

    static SkinMetadata load(const QString &skinDir);
    bool save(const QString &skinDir) const;

    QString objectKey(const QString &kind, const QString &name) const;
    ObjectMeta metaFor(const QString &key, const QMap<QString, ObjectMeta> &map) const;
    void setMeta(const QString &key, const ObjectMeta &m, QMap<QString, ObjectMeta> &map);
};

#endif
