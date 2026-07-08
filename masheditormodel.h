#ifndef MASHEDITORMODEL_H
#define MASHEDITORMODEL_H

#include "skinmetadata.h"
#include "skinparser.h"
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

    int activeSubSkin = 0;
    bool dirtyRegular = false;
    bool dirtyPiano = false;

    bool isDirty() const { return dirtyRegular || dirtyPiano; }

    void loadFromDir(const QString &dir);
    bool saveCurrentMode();
    bool saveAll();

    RegularSkin *activeRegularVariant();
    const RegularSkin *activeRegularVariant() const;

    static QStringList snesButtonNames();
    static QString snesNameForEnum(int snesButtonEnum);
};

#endif
