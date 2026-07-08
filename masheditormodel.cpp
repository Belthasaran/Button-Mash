#include "masheditormodel.h"
#include "skinutil.h"
#include "skinwriter.h"

#include <QDir>
#include <QFileInfo>

#include "inputprovider.h"

void MashEditorModel::loadFromDir(const QString &dir)
{
    skinDir = QFileInfo(dir).canonicalFilePath();
    metadata = SkinMetadata::load(skinDir);
    dirtyRegular = false;
    dirtyPiano = false;
    activeSubSkin = 0;

    const QString regPath = SkinUtil::regularXmlPath(skinDir);
    if (QFileInfo::exists(regPath)) {
        regular = SkinParser::parseRegularSkin(regPath, false);
        regular.file = regPath;
    } else {
        regular = RegularSkin();
        regular.name = QFileInfo(skinDir).fileName();
        regular.author = QStringLiteral("MashEditor");
    }

    const QString pianoPath = SkinUtil::pianoXmlPath(skinDir);
    if (QFileInfo::exists(pianoPath)) {
        piano = SkinParser::parsePianoSkin(pianoPath);
        piano.file = pianoPath;
    } else {
        piano = PianoSkin();
        piano.name = QFileInfo(skinDir).fileName();
        piano.author = QStringLiteral("MashEditor");
    }
    snapshotLoadedKeys();
}

QString MashEditorModel::regularBackgroundKeyForVariant(const RegularSkin &variant) const
{
    if (!regular.subSkins.isEmpty()) {
        const QString vname = variant.backgroundName.isEmpty() ? variant.name : variant.backgroundName;
        return QStringLiteral("background:") + vname;
    }
    return QStringLiteral("background");
}

QString MashEditorModel::regularBackgroundKey() const
{
    return regularBackgroundKeyForVariant(*activeRegularVariant());
}

QString MashEditorModel::regularButtonKey(const QString &name) const
{
    return QStringLiteral("button:") + name;
}

QString MashEditorModel::pianoMainareaKey() const
{
    return QStringLiteral("mainarea");
}

QString MashEditorModel::pianoButtonKey(const QString &name) const
{
    return QStringLiteral("button:") + name;
}

QString MashEditorModel::canvasBackgroundKey() const
{
    return QStringLiteral("background");
}

bool MashEditorModel::isObjectLocked(const QString &key) const
{
    const auto &map = (mode == MashEditMode::Regular) ? metadata.regularObjects : metadata.pianoObjects;
    const auto &loaded = (mode == MashEditMode::Regular) ? loadedRegularKeys : loadedPianoKeys;
    if (map.contains(key))
        return map.value(key).locked;
    if (loaded.contains(key))
        return true;
    return false;
}

void MashEditorModel::snapshotLoadedKeys()
{
    loadedRegularKeys.clear();
    loadedPianoKeys.clear();

    auto snapshotVariant = [this](const RegularSkin &v) {
        if (!v.background.isEmpty())
            loadedRegularKeys.insert(regularBackgroundKeyForVariant(v));
        for (auto it = v.buttons.constBegin(); it != v.buttons.constEnd(); ++it)
            loadedRegularKeys.insert(regularButtonKey(it.key()));
    };

    if (regular.subSkins.isEmpty()) {
        snapshotVariant(regular);
    } else {
        for (const RegularSkin &v : regular.subSkins)
            snapshotVariant(v);
    }

    loadedPianoKeys.insert(pianoMainareaKey());
    for (auto it = piano.buttons.constBegin(); it != piano.buttons.constEnd(); ++it)
        loadedPianoKeys.insert(pianoButtonKey(it.key()));
}

RegularSkin *MashEditorModel::activeRegularVariant()
{
    if (regular.subSkins.isEmpty())
        return &regular;
    if (activeSubSkin < 0 || activeSubSkin >= regular.subSkins.size())
        activeSubSkin = 0;
    return &regular.subSkins[activeSubSkin];
}

const RegularSkin *MashEditorModel::activeRegularVariant() const
{
    if (regular.subSkins.isEmpty())
        return &regular;
    if (activeSubSkin < 0 || activeSubSkin >= regular.subSkins.size())
        return &regular.subSkins.constFirst();
    return &regular.subSkins[activeSubSkin];
}

bool MashEditorModel::saveCurrentMode()
{
    if (mode == MashEditMode::Regular) {
        SkinWriter::backupXml(SkinUtil::regularXmlPath(skinDir));
        if (!SkinWriter::writeRegularSkin(SkinUtil::regularXmlPath(skinDir), regular))
            return false;
        dirtyRegular = false;
    } else {
        SkinWriter::backupXml(SkinUtil::pianoXmlPath(skinDir));
        if (!SkinWriter::writePianoSkin(SkinUtil::pianoXmlPath(skinDir), piano))
            return false;
        dirtyPiano = false;
    }
    metadata.lastMode = (mode == MashEditMode::Regular) ? QStringLiteral("regular") : QStringLiteral("piano");
    metadata.save(skinDir);
    return true;
}

bool MashEditorModel::saveAll()
{
    bool ok = true;
    if (dirtyRegular || QFileInfo::exists(SkinUtil::regularXmlPath(skinDir))) {
        SkinWriter::backupXml(SkinUtil::regularXmlPath(skinDir));
        ok = SkinWriter::writeRegularSkin(SkinUtil::regularXmlPath(skinDir), regular) && ok;
        dirtyRegular = false;
    }
    if (dirtyPiano || QFileInfo::exists(SkinUtil::pianoXmlPath(skinDir))) {
        SkinWriter::backupXml(SkinUtil::pianoXmlPath(skinDir));
        ok = SkinWriter::writePianoSkin(SkinUtil::pianoXmlPath(skinDir), piano) && ok;
        dirtyPiano = false;
    }
    metadata.save(skinDir);
    return ok;
}

QStringList MashEditorModel::snesButtonNames()
{
    return {QStringLiteral("b"), QStringLiteral("y"), QStringLiteral("select"), QStringLiteral("start"),
            QStringLiteral("up"), QStringLiteral("down"), QStringLiteral("left"), QStringLiteral("right"),
            QStringLiteral("a"), QStringLiteral("x"), QStringLiteral("l"), QStringLiteral("r")};
}

QString MashEditorModel::snesNameForEnum(int snesButtonEnum)
{
    switch (static_cast<InputProvider::SNESButton>(snesButtonEnum)) {
    case InputProvider::B: return QStringLiteral("b");
    case InputProvider::Y: return QStringLiteral("y");
    case InputProvider::Select: return QStringLiteral("select");
    case InputProvider::Start: return QStringLiteral("start");
    case InputProvider::Up: return QStringLiteral("up");
    case InputProvider::Down: return QStringLiteral("down");
    case InputProvider::Left: return QStringLiteral("left");
    case InputProvider::Right: return QStringLiteral("right");
    case InputProvider::A: return QStringLiteral("a");
    case InputProvider::X: return QStringLiteral("x");
    case InputProvider::L: return QStringLiteral("l");
    case InputProvider::R: return QStringLiteral("r");
    }
    return QString();
}
