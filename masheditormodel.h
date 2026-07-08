#ifndef MASHEDITORMODEL_H
#define MASHEDITORMODEL_H

#include "skinmetadata.h"
#include "skinparser.h"
#include <QSet>
#include <QString>

enum class MashEditMode { Regular, Piano };

class MashEditorModel
{
public:
    QString skinDir;
    QString skinsRoot;
    MashEditMode mode = MashEditMode::Regular;

    RegularSkin regular;
    PianoSkin piano;
    SkinMetadata metadata;

    QSet<QString> loadedRegularKeys;
    QSet<QString> loadedPianoKeys;

    int activeSubSkin = 0;
    bool dirtyRegular = false;
    bool dirtyPiano = false;

    bool isDirty() const { return dirtyRegular || dirtyPiano; }

    void loadFromDir(const QString &dir);
    bool saveCurrentMode();
    bool saveAll();

    RegularSkin *activeRegularVariant();
    const RegularSkin *activeRegularVariant() const;

    QString regularBackgroundKey() const;
    QString regularBackgroundKeyForVariant(const RegularSkin &variant) const;
    QString regularButtonKey(const QString &name) const;
    QString pianoMainareaKey() const;
    QString pianoButtonKey(const QString &name) const;
    QString canvasBackgroundKey() const;
    bool isObjectLocked(const QString &key) const;

    static QStringList snesButtonNames();
    static QString snesNameForEnum(int snesButtonEnum);

private:
    void snapshotLoadedKeys();
};

#endif
