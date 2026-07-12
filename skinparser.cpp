#include "skinparser.h"

#include "skinpath.h"

#include <QDomDocument>
#include <QFile>
#include <QFileInfo>
#include <QPixmap>

QString SkinParser::xmlError = QString();
int SkinParser::lineNumber = 0;
int SkinParser::columNumber = 0;

SkinParser::SkinParser() = default;

static bool validateOneRegularSkin(const RegularSkin &skin, bool requireImages, QString *error)
{
    if (skin.file.isEmpty()) {
        if (error)
            *error = QStringLiteral("Skin has no file path");
        return false;
    }
    const QString skinRoot = QFileInfo(skin.file).absolutePath();
    if (!skin.background.isEmpty()) {
        QString err;
        if (SkinPath::resolveSkinRelativePath(skinRoot, skin.background, &err, requireImages).isEmpty()) {
            if (error)
                *error = QStringLiteral("Background \"%1\": %2").arg(skin.background, err);
            return false;
        }
    }
    for (auto it = skin.buttons.constBegin(); it != skin.buttons.constEnd(); ++it) {
        const RegularButtonSkin &but = it.value();
        if (but.image.isEmpty())
            continue;
        QString err;
        if (SkinPath::resolveSkinRelativePath(skinRoot, but.image, &err, requireImages).isEmpty()) {
            if (error)
                *error = QStringLiteral("Button \"%1\" image \"%2\": %3").arg(but.name, but.image, err);
            return false;
        }
    }
    return true;
}

bool SkinParser::validateRegularSkin(const RegularSkin &skin, bool requireImages, QString *error)
{
    if (!validateOneRegularSkin(skin, requireImages, error))
        return false;
    for (const RegularSkin &sub : skin.subSkins) {
        RegularSkin subSkin = sub;
        if (subSkin.file.isEmpty())
            subSkin.file = skin.file;
        if (!validateOneRegularSkin(subSkin, requireImages, error))
            return false;
    }
    return true;
}

static QString normalizeSkinImageAttribute(const QString &skinRoot, const QString &rawImage,
                                         bool requireImages, QString *errorOut)
{
    QString err;
    const QString normalized = SkinPath::normalizeSkinRelativePath(rawImage, &err);
    if (normalized.isEmpty()) {
        if (errorOut)
            *errorOut = err;
        return QString();
    }
    if (requireImages) {
        if (SkinPath::resolveSkinRelativePath(skinRoot, normalized, &err, true).isEmpty()) {
            if (errorOut)
                *errorOut = err;
            return QString();
        }
    }
    return normalized;
}

RegularSkin SkinParser::parseRegularSkin(const QString &filePath, bool requireImages)
{
    xmlError.clear();
    QDomDocument doc;
    RegularSkin toret;
    QFileInfo fi(filePath);
    QFile skinFile(filePath);
    if (!skinFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        xmlError = QStringLiteral("Cannot open %1").arg(filePath);
        return toret;
    }
    const QString xmlSkin = skinFile.readAll();
    if (!doc.setContent(xmlSkin, true, &xmlError, &lineNumber, &columNumber))
        return toret;
    QDomElement root = doc.documentElement();
    QDomElement child = root.firstChildElement();
    toret.file = fi.absoluteFilePath();
    toret.author = root.attribute(QStringLiteral("author"));
    toret.name = root.attribute(QStringLiteral("name"));
    toret.type = root.attribute(QStringLiteral("type"), QStringLiteral("snes"));

    const QString skinRoot = fi.absolutePath();
    QMap<QString, RegularButtonSkin> defaultBut;
    RegularSkin *currentSkin = &toret;
    QString firstBgName;
    bool notFirstBg = false;
    while (!child.isNull()) {
        if (child.tagName() == QLatin1String("background")) {
            if (notFirstBg) {
                if (toret.subSkins.isEmpty()) {
                    toret.subSkins.append(toret);
                    toret.subSkins.first().name = firstBgName;
                }
                toret.subSkins.append(RegularSkin());
                currentSkin = &toret.subSkins.last();
                currentSkin->name = child.attribute(QStringLiteral("name"));
            }
            if (firstBgName.isEmpty())
                firstBgName = child.attribute(QStringLiteral("name"));
            currentSkin->backgroundName = child.attribute(QStringLiteral("name"));
            currentSkin->file = filePath;
            currentSkin->author = toret.author;
            const QString rawBg = child.attribute(QStringLiteral("image"));
            QString pathErr;
            const QString normalizedBg = normalizeSkinImageAttribute(skinRoot, rawBg, requireImages, &pathErr);
            if (normalizedBg.isEmpty() && !rawBg.isEmpty()) {
                xmlError = pathErr.isEmpty()
                               ? QStringLiteral("Invalid background path: %1").arg(rawBg)
                               : pathErr;
                return toret;
            }
            currentSkin->background = normalizedBg;
            notFirstBg = true;
        }
        if (child.tagName() == QLatin1String("button")) {
            RegularButtonSkin but;
            const QString rawImage = child.attribute(QStringLiteral("image"));
            QString pathErr;
            const QString normalizedImage = normalizeSkinImageAttribute(skinRoot, rawImage, requireImages, &pathErr);
            if (normalizedImage.isEmpty() && !rawImage.isEmpty()) {
                xmlError = pathErr.isEmpty()
                               ? QStringLiteral("Invalid button image path: %1").arg(rawImage)
                               : pathErr;
                return toret;
            }
            but.image = normalizedImage;
            but.width = 16;
            but.height = 16;
            if (requireImages && !but.image.isEmpty()) {
                const QString absPath = SkinPath::resolveSkinRelativePath(skinRoot, but.image, &pathErr, true);
                QPixmap pix(absPath);
                if (pix.isNull()) {
                    xmlError = pathErr.isEmpty()
                                   ? QStringLiteral("Can't open %1").arg(but.image)
                                   : pathErr;
                    return toret;
                }
                but.width = pix.width();
                but.height = pix.height();
            }
            if (!child.attribute(QStringLiteral("width")).isEmpty())
                but.width = child.attribute(QStringLiteral("width")).toUInt();
            if (!child.attribute(QStringLiteral("height")).isEmpty())
                but.height = child.attribute(QStringLiteral("height")).toUInt();
            but.x = child.attribute(QStringLiteral("x")).toInt();
            but.y = child.attribute(QStringLiteral("y")).toInt();
            but.name = child.attribute(QStringLiteral("name"));
            defaultBut[but.name] = but;
            currentSkin->buttons[but.name] = but;
        }
        child = child.nextSiblingElement();
    }
    QMutableListIterator<RegularSkin> it(toret.subSkins);
    while (it.hasNext()) {
        RegularSkin &sk = it.next();
        if (sk.buttons.isEmpty())
            sk.buttons = defaultBut;
    }
    if (toret.buttons.isEmpty() && !toret.subSkins.isEmpty())
        toret.buttons = toret.subSkins.first().buttons;
    if (toret.backgroundName.isEmpty() && !firstBgName.isEmpty())
        toret.backgroundName = firstBgName;
    return toret;
}

static QColor parseColorAttr(const QDomElement &el, const char *attr1, const char *attr2, QColor fallback)
{
    QString v = el.attribute(QLatin1String(attr1));
    if (v.isEmpty() && attr2)
        v = el.attribute(QLatin1String(attr2));
    if (v.isEmpty())
        return fallback;
    QColor c(v);
    return c.isValid() ? c : fallback;
}

PianoSkin SkinParser::parsePianoSkin(const QString &filePath)
{
    xmlError.clear();
    QDomDocument doc;
    PianoSkin toret;
    QFileInfo fi(filePath);
    QFile skinFile(filePath);
    if (!skinFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        xmlError = QStringLiteral("Cannot open %1").arg(filePath);
        return toret;
    }
    const QString xmlSkin = skinFile.readAll();
    if (!doc.setContent(xmlSkin, true, &xmlError, &lineNumber, &columNumber))
        return toret;
    QDomElement root = doc.documentElement();
    toret.file = fi.absoluteFilePath();
    toret.author = root.attribute(QStringLiteral("author"));
    toret.name = root.attribute(QStringLiteral("name"));
    toret.type = root.attribute(QStringLiteral("type"), QStringLiteral("pianodisplay"));

    QDomElement mainarea = root.firstChildElement(QStringLiteral("mainarea"));
    if (mainarea.isNull()) {
        xmlError = QStringLiteral("Missing mainarea");
        return toret;
    }
    toret.width = mainarea.attribute(QStringLiteral("width"), QStringLiteral("400")).toInt();
    toret.height = mainarea.attribute(QStringLiteral("height"), QStringLiteral("200")).toInt();
    toret.bgcolor = parseColorAttr(mainarea, "backgroundcolor", nullptr, Qt::black);

    QDomElement button = mainarea.firstChildElement(QStringLiteral("button"));
    while (!button.isNull()) {
        PianoButton but;
        but.name = button.attribute(QStringLiteral("name"));
        but.x = button.attribute(QStringLiteral("x")).toInt();
        but.width = button.attribute(QStringLiteral("width"), QStringLiteral("20")).toInt();
        QColor c2(button.attribute(QStringLiteral("color")));
        if (!c2.isValid()) {
            xmlError = QStringLiteral("Invalid button color");
            lineNumber = button.lineNumber();
            return toret;
        }
        but.color = c2;
        toret.buttons[but.name] = but;
        button = button.nextSiblingElement(QStringLiteral("button"));
    }

    QDomElement labelarea = root.firstChildElement(QStringLiteral("labelarea"));
    if (!labelarea.isNull()) {
        toret.labelArea.width = labelarea.attribute(QStringLiteral("width"), QStringLiteral("400")).toInt();
        toret.labelArea.height = labelarea.attribute(QStringLiteral("height"), QStringLiteral("30")).toInt();
        toret.labelArea.bgcolor = parseColorAttr(labelarea, "background-color", "backgroundcolor", Qt::black);
        QDomElement font = labelarea.firstChildElement(QStringLiteral("font"));
        if (!font.isNull()) {
            toret.labelArea.fontName = font.attribute(QStringLiteral("name"), toret.labelArea.fontName);
            toret.labelArea.fontBold = font.attribute(QStringLiteral("bold"), QStringLiteral("1")) != QLatin1String("0");
            toret.labelArea.fontSize = font.attribute(QStringLiteral("size"), QStringLiteral("12")).toInt();
        }
    }
    return toret;
}

QDebug operator<<(QDebug debug, const RegularSkin &req)
{
    debug << "Author:" << req.author << "Name:" << req.name;
    return debug;
}

QDebug operator<<(QDebug debug, const RegularButtonSkin &but)
{
    debug << but.name << but.image << but.x << but.y << but.width << but.height;
    return debug;
}

QDebug operator<<(QDebug debug, const PianoButton &pb)
{
    debug << pb.name << pb.x << pb.width << pb.color;
    return debug;
}

QDebug operator<<(QDebug debug, const PianoSkin &pk)
{
    debug << pk.name << pk.author;
    return debug;
}
