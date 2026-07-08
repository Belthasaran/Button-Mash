#ifndef SKINUTIL_H
#define SKINUTIL_H

#include <QString>

namespace SkinUtil {

QString uniqueFileName(const QString &dir, const QString &baseName);
bool copyFileUnique(const QString &srcPath, const QString &destDir, QString *outDestName = nullptr);
bool copyDirRecursive(const QString &srcDir, const QString &destDir);
bool createTransparentPng(const QString &path, int w, int h);

/** Create new skin folder with skeleton XML + placeholder assets. */
bool createSkeletonSkin(const QString &skinsRoot, const QString &folderName, QString *outSkinDir = nullptr);

QString regularXmlPath(const QString &skinDir);
QString pianoXmlPath(const QString &skinDir);

} // namespace SkinUtil

#endif
