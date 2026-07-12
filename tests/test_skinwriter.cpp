#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QGuiApplication>
#include <QTemporaryDir>

#include "skinparser.h"
#include "skinwriter.h"
#include "skinmetadata.h"
#include "skinutil.h"
#include "masheditormodel.h"

static int failures = 0;

static void expect(bool cond, const char *msg)
{
    if (!cond) {
        qWarning("FAIL: %s", msg);
        failures++;
    } else {
        qDebug("OK: %s", msg);
    }
}

static bool pianoEqualEnough(const PianoSkin &a, const PianoSkin &b)
{
    if (a.name != b.name || a.width != b.width || a.height != b.height)
        return false;
    if (a.buttons.size() != b.buttons.size())
        return false;
    for (auto it = a.buttons.constBegin(); it != a.buttons.constEnd(); ++it) {
        if (!b.buttons.contains(it.key()))
            return false;
        const PianoButton &pb = b.buttons[it.key()];
        if (pb.x != it->x || pb.width != it->width)
            return false;
    }
    return true;
}

int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);

    const QString defaultDir = QDir(QCoreApplication::applicationDirPath())
                                   .absoluteFilePath(QStringLiteral("../../Skins/Default"));
    const QString regPath = defaultDir + QStringLiteral("/skin.xml");
    const QString pianoPath = defaultDir + QStringLiteral("/pianodisplay.xml");

    if (QFileInfo::exists(regPath)) {
        RegularSkin orig = SkinParser::parseRegularSkin(regPath);
        expect(SkinParser::xmlError.isEmpty(), "parse Default skin.xml");
        QTemporaryDir tmp;
        const QString out = tmp.path() + QStringLiteral("/skin.xml");
        expect(SkinWriter::writeRegularSkin(out, orig), "write regular skin");
        RegularSkin again = SkinParser::parseRegularSkin(out, false);
        expect(again.name == orig.name, "regular round-trip name");
        expect(again.buttons.size() == orig.buttons.size(), "regular round-trip button count");
    } else {
        qDebug("SKIP: Default skin.xml not found at %s", qPrintable(regPath));
    }

    if (QFileInfo::exists(pianoPath)) {
        PianoSkin orig = SkinParser::parsePianoSkin(pianoPath);
        expect(SkinParser::xmlError.isEmpty(), "parse Default pianodisplay.xml");
        expect(orig.labelArea.height == 30, "labelarea height parsed");
        QTemporaryDir tmp;
        const QString out = tmp.path() + QStringLiteral("/pianodisplay.xml");
        expect(SkinWriter::writePianoSkin(out, orig), "write piano skin");
        PianoSkin again = SkinParser::parsePianoSkin(out);
        expect(pianoEqualEnough(orig, again), "piano round-trip fields");
    }

    QTemporaryDir skelRoot;
    expect(SkinUtil::createSkeletonSkin(skelRoot.path(), QStringLiteral("TestSkin")), "skeleton create");
    expect(QFileInfo::exists(SkinUtil::regularXmlPath(skelRoot.path() + QStringLiteral("/TestSkin"))),
           "skeleton skin.xml exists");
    expect(QFileInfo::exists(SkinUtil::pianoXmlPath(skelRoot.path() + QStringLiteral("/TestSkin"))),
           "skeleton pianodisplay.xml exists");
    RegularSkin skel = SkinParser::parseRegularSkin(
        SkinUtil::regularXmlPath(skelRoot.path() + QStringLiteral("/TestSkin")));
    expect(SkinParser::xmlError.isEmpty(), "skeleton regular parses");

    const QString u = SkinUtil::uniqueFileName(skelRoot.path(), QStringLiteral("a.png"));
    QFile f(skelRoot.path() + QStringLiteral("/a.png"));
    f.open(QIODevice::WriteOnly);
    f.write("x");
    f.close();
    const QString u2 = SkinUtil::uniqueFileName(skelRoot.path(), QStringLiteral("a.png"));
    expect(u2 == QStringLiteral("a_1.png"), "unique filename a_1");

    SkinMetadata meta;
    meta.lastMode = QStringLiteral("regular");
    ObjectMeta om;
    om.locked = true;
    meta.setMeta(QStringLiteral("button:b"), om, meta.regularObjects);
    expect(meta.save(skelRoot.path() + QStringLiteral("/TestSkin")), "meta save");
    SkinMetadata loaded = SkinMetadata::load(skelRoot.path() + QStringLiteral("/TestSkin"));
    expect(loaded.regularObjects.value(QStringLiteral("button:b")).locked, "meta lock round-trip");

    MashEditorModel model;
    model.mode = MashEditMode::Regular;
    model.loadedRegularKeys.insert(QStringLiteral("button:a"));
    model.loadedRegularKeys.insert(QStringLiteral("background"));
    expect(model.isObjectLocked(QStringLiteral("button:a")), "loaded button locked by default");
    expect(model.isObjectLocked(QStringLiteral("background")), "loaded background locked by default");
    ObjectMeta unlocked;
    unlocked.locked = false;
    model.metadata.regularObjects[QStringLiteral("button:a")] = unlocked;
    expect(!model.isObjectLocked(QStringLiteral("button:a")), "explicit unlock honored");
    expect(!model.isObjectLocked(QStringLiteral("button:z")), "new button not locked");

    if (failures > 0) {
        qWarning("%d test(s) failed", failures);
        return 1;
    }
    qDebug("All skinwriter tests passed");
    return 0;
}
