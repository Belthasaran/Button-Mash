#include "skinwriter.h"

#include <QDateTime>
#include <QDomDocument>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>

QString SkinWriter::lastError;

static void writeRegularVariant(QTextStream &out, const RegularSkin &variant, int indent)
{
    const QString pad(indent, QLatin1Char(' '));
    if (!variant.background.isEmpty()) {
        const QString bgName = variant.backgroundName.isEmpty() ? variant.name : variant.backgroundName;
        out << pad << "<background name=\"" << bgName << "\" image=\"" << variant.background << "\" />\n";
    }
    const auto keys = variant.buttons.keys();
    for (const QString &k : keys) {
        const RegularButtonSkin &b = variant.buttons[k];
        out << pad << "<button name=\"" << b.name << "\" image=\"" << b.image << "\""
            << " x=\"" << b.x << "\" y=\"" << b.y << "\"";
        if (b.width > 0)
            out << " width=\"" << b.width << "\"";
        if (b.height > 0)
            out << " height=\"" << b.height << "\"";
        out << " />\n";
    }
}

bool SkinWriter::backupXml(const QString &filePath)
{
    if (!QFileInfo::exists(filePath))
        return true;
    QString bak = filePath + QStringLiteral(".bak");
    if (QFileInfo::exists(bak)) {
        const QString ts = QDateTime::currentDateTime().toString(QStringLiteral("yyyyMMdd_HHmmss"));
        bak = filePath + QStringLiteral(".bak.") + ts;
    }
    if (QFile::exists(bak))
        QFile::remove(bak);
    return QFile::copy(filePath, bak);
}

bool SkinWriter::writeRegularSkin(const QString &filePath, const RegularSkin &skin)
{
    lastError.clear();
    QFile f(filePath);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text)) {
        lastError = QStringLiteral("Cannot write %1").arg(filePath);
        return false;
    }
    QTextStream out(&f);
    out << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    out << "<skin name=\"" << skin.name << "\" author=\"" << skin.author << "\" type=\"" << skin.type << "\">\n\n";

    if (skin.subSkins.isEmpty()) {
        RegularSkin v = skin;
        if (v.name.isEmpty())
            v.name = QStringLiteral("Default");
        writeRegularVariant(out, v, 4);
    } else {
        for (const RegularSkin &sub : skin.subSkins)
            writeRegularVariant(out, sub, 4);
    }

    out << "\n</skin>\n";
    return true;
}

bool SkinWriter::writePianoSkin(const QString &filePath, const PianoSkin &skin)
{
    lastError.clear();
    QFile f(filePath);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text)) {
        lastError = QStringLiteral("Cannot write %1").arg(filePath);
        return false;
    }
    QTextStream out(&f);
    out << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    out << "<skin name=\"" << skin.name << "\" author=\"" << skin.author << "\" type=\"" << skin.type << "\">\n";
    out << "<mainarea backgroundcolor=\"" << skin.bgcolor.name() << "\" width=\"" << skin.width
        << "\" height=\"" << skin.height << "\">\n";
    const auto keys = skin.buttons.keys();
    for (const QString &k : keys) {
        const PianoButton &b = skin.buttons[k];
        out << "    <button name=\"" << b.name << "\" x=\"" << b.x << "\" width=\"" << b.width
            << "\" color=\"" << b.color.name() << "\"/>\n";
    }
    out << "</mainarea>\n";
    const PianoLabelArea &la = skin.labelArea;
    out << "<labelarea background-color=\"" << la.bgcolor.name() << "\" width=\"" << la.width
        << "\" height=\"" << la.height << "\">\n";
    out << "    <font name=\"" << la.fontName << "\" bold=\"" << (la.fontBold ? 1 : 0)
        << "\" size=\"" << la.fontSize << "\"/>\n";
    out << "</labelarea>\n\n</skin>\n";
    return true;
}
