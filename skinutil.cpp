#include "skinutil.h"
#include "skinwriter.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QImage>
#include <QStandardPaths>

namespace SkinUtil {

QString regularXmlPath(const QString &skinDir)
{
    return QDir(skinDir).filePath(QStringLiteral("skin.xml"));
}

QString pianoXmlPath(const QString &skinDir)
{
    return QDir(skinDir).filePath(QStringLiteral("pianodisplay.xml"));
}

QString uniqueFileName(const QString &dir, const QString &baseName)
{
    if (!QFileInfo::exists(QDir(dir).filePath(baseName)))
        return baseName;
    const QFileInfo fi(baseName);
    const QString stem = fi.completeBaseName();
    const QString ext = fi.suffix().isEmpty() ? QString() : QStringLiteral(".") + fi.suffix();
    for (int i = 1; i < 10000; ++i) {
        const QString candidate = QStringLiteral("%1_%2%3").arg(stem).arg(i).arg(ext);
        if (!QFileInfo::exists(QDir(dir).filePath(candidate)))
            return candidate;
    }
    return baseName;
}

bool copyFileUnique(const QString &srcPath, const QString &destDir, QString *outDestName)
{
    const QFileInfo srcFi(srcPath);
    const QString destName = uniqueFileName(destDir, srcFi.fileName());
    const QString destPath = QDir(destDir).filePath(destName);
    if (!QFile::copy(srcPath, destPath))
        return false;
    if (outDestName)
        *outDestName = destName;
    return true;
}

bool copyDirRecursive(const QString &srcDir, const QString &destDir)
{
    QDir src(srcDir);
    if (!QDir().mkpath(destDir))
        return false;
    const auto entries = src.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
    for (const QFileInfo &fi : entries) {
        const QString dest = QDir(destDir).filePath(fi.fileName());
        if (fi.isDir()) {
            if (!copyDirRecursive(fi.absoluteFilePath(), dest))
                return false;
        } else {
            if (QFileInfo::exists(dest))
                continue;
            if (!QFile::copy(fi.absoluteFilePath(), dest))
                return false;
        }
    }
    return true;
}

bool createTransparentPng(const QString &path, int w, int h)
{
    QImage img(w, h, QImage::Format_ARGB32);
    img.fill(Qt::transparent);
    return img.save(path, "PNG");
}

static QString defaultTemplateDir()
{
    const QString app = QCoreApplication::applicationDirPath();
    const QString rel = app + QStringLiteral("/Skins/Default");
    if (QFileInfo::exists(rel + QStringLiteral("/skin.xml")))
        return rel;
    const QString dev = app + QStringLiteral("/../Skins/Default");
    if (QFileInfo::exists(dev + QStringLiteral("/skin.xml")))
        return QFileInfo(dev).canonicalFilePath();
    return rel;
}

static RegularSkin defaultRegularSkeleton(const QString &folderName)
{
    RegularSkin s;
    s.name = folderName;
    s.author = QStringLiteral("MashEditor");
    s.type = QStringLiteral("snes");
    s.background = QStringLiteral("background.png");
    s.name = QStringLiteral("Default");
    const QStringList names = {
        QStringLiteral("a"), QStringLiteral("b"), QStringLiteral("x"), QStringLiteral("y"),
        QStringLiteral("l"), QStringLiteral("r"), QStringLiteral("start"), QStringLiteral("select"),
        QStringLiteral("up"), QStringLiteral("down"), QStringLiteral("left"), QStringLiteral("right")
    };
    int i = 0;
    for (const QString &n : names) {
        RegularButtonSkin b;
        b.name = n;
        b.image = n + QStringLiteral(".png");
        b.x = 10 + (i % 4) * 20;
        b.y = 10 + (i / 4) * 20;
        b.width = 16;
        b.height = 16;
        s.buttons[n] = b;
        ++i;
    }
    return s;
}

static PianoSkin defaultPianoSkeleton(const QString &folderName)
{
    PianoSkin p;
    p.name = folderName;
    p.author = QStringLiteral("MashEditor");
    p.width = 400;
    p.height = 200;
    p.bgcolor = Qt::black;
    p.labelArea.width = 400;
    p.labelArea.height = 30;
    p.labelArea.bgcolor = Qt::black;
    const struct { const char *n; int x; QColor c; } defs[] = {
        {"left", 10, Qt::gray}, {"up", 40, Qt::gray}, {"right", 70, Qt::gray}, {"down", 100, Qt::gray},
        {"select", 130, Qt::lightGray}, {"start", 160, Qt::lightGray},
        {"y", 190, Qt::green}, {"b", 220, Qt::yellow}, {"x", 250, Qt::blue}, {"a", 280, Qt::red},
        {"l", 310, Qt::gray}, {"r", 340, Qt::gray},
    };
    for (const auto &d : defs) {
        PianoButton b;
        b.name = QLatin1String(d.n);
        b.x = d.x;
        b.width = 20;
        b.color = d.c;
        p.buttons[b.name] = b;
    }
    return p;
}

bool createSkeletonSkin(const QString &skinsRoot, const QString &folderName, QString *outSkinDir)
{
    const QString skinDir = QDir(skinsRoot).filePath(folderName);
    if (QFileInfo::exists(skinDir))
        return false;
    if (!QDir().mkpath(skinDir))
        return false;

    const QString tmpl = defaultTemplateDir();
    if (QFileInfo::exists(tmpl)) {
        copyDirRecursive(tmpl, skinDir);
        RegularSkin rs = SkinParser::parseRegularSkin(regularXmlPath(skinDir), false);
        rs.name = folderName;
        rs.author = QStringLiteral("MashEditor");
        SkinWriter::writeRegularSkin(regularXmlPath(skinDir), rs);
        PianoSkin ps = SkinParser::parsePianoSkin(pianoXmlPath(skinDir));
        ps.name = folderName;
        ps.author = QStringLiteral("MashEditor");
        SkinWriter::writePianoSkin(pianoXmlPath(skinDir), ps);
    } else {
        createTransparentPng(QDir(skinDir).filePath(QStringLiteral("background.png")), 240, 121);
        for (const QString &n : {QStringLiteral("a"), QStringLiteral("b"), QStringLiteral("x"), QStringLiteral("y"),
                                 QStringLiteral("l"), QStringLiteral("r"), QStringLiteral("start"), QStringLiteral("select"),
                                 QStringLiteral("up"), QStringLiteral("down"), QStringLiteral("left"), QStringLiteral("right")}) {
            createTransparentPng(QDir(skinDir).filePath(n + QStringLiteral(".png")), 16, 16);
        }
        RegularSkin rs = defaultRegularSkeleton(folderName);
        rs.background = QStringLiteral("background.png");
        rs.name = QStringLiteral("Default");
        rs.file = regularXmlPath(skinDir);
        SkinWriter::writeRegularSkin(regularXmlPath(skinDir), rs);
        PianoSkin ps = defaultPianoSkeleton(folderName);
        ps.file = pianoXmlPath(skinDir);
        SkinWriter::writePianoSkin(pianoXmlPath(skinDir), ps);
    }
    if (outSkinDir)
        *outSkinDir = skinDir;
    return true;
}

} // namespace SkinUtil
