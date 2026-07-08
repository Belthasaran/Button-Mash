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
