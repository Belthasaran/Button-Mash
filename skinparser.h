#ifndef SKINPARSER_H
#define SKINPARSER_H

#include <QColor>
#include <QDebug>
#include <QList>
#include <QMap>
#include <QString>

struct RegularButtonSkin
{
    QString name;
    QString image;
    int x = 0;
    int y = 0;
    int width = 0;
    int height = 0;
};

QDebug operator<<(QDebug debug, const RegularButtonSkin &but);

struct RegularSkin
{
    QString file;
    QString name;
    QString author;
    QString type = QStringLiteral("snes");
    QString background;
    QString backgroundName;
    QList<RegularSkin> subSkins;
    QMap<QString, RegularButtonSkin> buttons;
};

Q_DECLARE_METATYPE(RegularSkin)

QDebug operator<<(QDebug debug, const RegularSkin &req);

struct PianoButton
{
    QString name;
    int x = 0;
    int width = 20;
    QColor color = Qt::gray;
};

QDebug operator<<(QDebug debug, const PianoButton &pb);

struct PianoLabelArea
{
    QColor bgcolor = Qt::black;
    int width = 400;
    int height = 30;
    QString fontName = QStringLiteral("DejaVu Sans Mono");
    bool fontBold = true;
    int fontSize = 12;
};

struct PianoSkin
{
    QString file;
    QString name;
    QString author;
    QString type = QStringLiteral("pianodisplay");
    QColor bgcolor = Qt::black;
    QString background;
    int width = 400;
    int height = 200;
    PianoLabelArea labelArea;
    QMap<QString, PianoButton> buttons;
};

QDebug operator<<(QDebug debug, const PianoSkin &pk);

Q_DECLARE_METATYPE(PianoSkin)

class SkinParser
{
public:
    SkinParser();
    static RegularSkin parseRegularSkin(const QString &filePath, bool requireImages = true);
    static PianoSkin parsePianoSkin(const QString &filePath);
    static bool validateRegularSkin(const RegularSkin &skin, bool requireImages, QString *error);
    static QString xmlError;
    static int lineNumber;
    static int columNumber;
};

#endif // SKINPARSER_H
