#ifndef SKINPATH_H
#define SKINPATH_H

#include <QString>

namespace SkinPath {

QString normalizeSkinRelativePath(const QString &relativePath, QString *error = nullptr);

QString resolveSkinRelativePath(const QString &skinRoot, const QString &relativePath,
                                QString *error = nullptr, bool requireExists = true);

int selfTest();

} // namespace SkinPath

#endif
