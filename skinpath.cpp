#include "skinpath.h"

#include "skinparser.h"

#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QPixmap>

namespace SkinPath {

namespace {

bool isAbsoluteSkinPath(const QString &path)
{
    if (QDir::isAbsolutePath(path))
        return true;
    if (path.size() >= 2 && path.at(1) == QLatin1Char(':'))
        return true;
    return false;
}

bool hasParentPathSegment(const QString &path)
{
    const QString norm = QDir::fromNativeSeparators(path);
    const QStringList parts = norm.split(QLatin1Char('/'), Qt::SkipEmptyParts);
    for (const QString &part : parts) {
        if (part == QLatin1String(".."))
            return true;
    }
    return false;
}

QString skinRootCanonical(const QString &skinRoot)
{
    const QString canonical = QFileInfo(skinRoot).canonicalFilePath();
    if (!canonical.isEmpty())
        return canonical;
    return QFileInfo(skinRoot).absoluteFilePath();
}

bool isUnderSkinRoot(const QString &canonicalPath, const QString &skinRootCanonical)
{
    if (canonicalPath == skinRootCanonical)
        return true;
    return canonicalPath.startsWith(skinRootCanonical + QLatin1Char('/'));
}

} // namespace

QString normalizeSkinRelativePath(const QString &relativePath, QString *error)
{
    const QString trimmed = relativePath.trimmed();
    if (trimmed.isEmpty()) {
        if (error)
            *error = QStringLiteral("Missing image path");
        return QString();
    }
    if (isAbsoluteSkinPath(trimmed)) {
        if (error)
            *error = QStringLiteral("Absolute paths are not allowed in skins: %1").arg(relativePath);
        return QString();
    }
    if (hasParentPathSegment(trimmed)) {
        if (error)
            *error = QStringLiteral("Directory traversal is not allowed: %1").arg(relativePath);
        return QString();
    }

    QString norm = QDir::fromNativeSeparators(trimmed);
    norm.replace(QLatin1Char('\\'), QLatin1Char('/'));
    QStringList parts = norm.split(QLatin1Char('/'), Qt::SkipEmptyParts);
    QStringList out;
    for (const QString &part : parts) {
        if (part == QLatin1String("."))
            continue;
        if (part == QLatin1String("..")) {
            if (error)
                *error = QStringLiteral("Directory traversal is not allowed: %1").arg(relativePath);
            return QString();
        }
        out.append(part);
    }
    return out.join(QLatin1Char('/'));
}

QString resolveSkinRelativePath(const QString &skinRoot, const QString &relativePath,
                                QString *error, bool requireExists)
{
    const QString relative = normalizeSkinRelativePath(relativePath, error);
    if (relative.isEmpty())
        return QString();

    const QString rootCanonical = skinRootCanonical(skinRoot);
    if (rootCanonical.isEmpty()) {
        if (error)
            *error = QStringLiteral("Invalid skin directory: %1").arg(skinRoot);
        return QString();
    }

    const QString candidate = QDir(skinRoot).filePath(relative);
    QFileInfo fi(candidate);
    if (requireExists && !fi.exists()) {
        if (error)
            *error = QStringLiteral("Can't open %1").arg(relative);
        return QString();
    }

    const QString canonical = fi.canonicalFilePath();
    if (canonical.isEmpty()) {
        const QString absolute = fi.absoluteFilePath();
        if (!isUnderSkinRoot(absolute, rootCanonical)) {
            if (error)
                *error = QStringLiteral("Path escapes skin folder: %1").arg(relativePath);
            return QString();
        }
        if (requireExists) {
            if (error)
                *error = QStringLiteral("Can't open %1").arg(relative);
            return QString();
        }
        return absolute;
    }

    if (!isUnderSkinRoot(canonical, rootCanonical)) {
        if (error)
            *error = QStringLiteral("Path escapes skin folder: %1").arg(relativePath);
        return QString();
    }
    return canonical;
}

int selfTest()
{
    const QString appDir = QCoreApplication::applicationDirPath();
    QStringList skinCandidates = {
        appDir + QStringLiteral("/Skins/snes-strip/skin.xml"),
        appDir + QStringLiteral("/../Skins/snes-strip/skin.xml"),
        appDir + QStringLiteral("/../../Skins/snes-strip/skin.xml")
    };
    QString skinXml;
    for (const QString &candidate : skinCandidates) {
        if (QFileInfo::exists(candidate)) {
            skinXml = candidate;
            break;
        }
    }
    if (skinXml.isEmpty())
        return 1;

    RegularSkin strip = SkinParser::parseRegularSkin(skinXml, true);
    if (!SkinParser::xmlError.isEmpty() || strip.buttons.size() != 12)
        return 2;

    QString err;
    if (!SkinParser::validateRegularSkin(strip, true, &err))
        return 3;

    const QString skinRoot = QFileInfo(skinXml).absolutePath();
    if (resolveSkinRelativePath(skinRoot, QStringLiteral("../../../etc/passwd"), &err, false) != QString())
        return 4;
    if (!err.contains(QStringLiteral("traversal"), Qt::CaseInsensitive))
        return 5;

    err.clear();
    if (resolveSkinRelativePath(skinRoot, QStringLiteral("/absolute.png"), &err, false) != QString())
        return 6;
    if (!err.contains(QStringLiteral("Absolute"), Qt::CaseInsensitive))
        return 7;

    err.clear();
    if (resolveSkinRelativePath(skinRoot, QStringLiteral("C:\\outside.png"), &err, false) != QString())
        return 8;
    if (!err.contains(QStringLiteral("Absolute"), Qt::CaseInsensitive))
        return 9;

    err.clear();
    const QString resolved = resolveSkinRelativePath(skinRoot, QStringLiteral("v2/circle-a.png"), &err, true);
    if (resolved.isEmpty())
        return 10;

    QPixmap pix(resolved);
    if (pix.isNull())
        return 11;

    return 0;
}

} // namespace SkinPath
