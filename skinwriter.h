#ifndef SKINWRITER_H
#define SKINWRITER_H

#include "skinparser.h"
#include <QString>

class SkinWriter
{
public:
    static QString lastError;

    static bool writeRegularSkin(const QString &filePath, const RegularSkin &skin);
    static bool writePianoSkin(const QString &filePath, const PianoSkin &skin);

    /** Copy existing file to .bak or .bak.timestamp before overwrite. */
    static bool backupXml(const QString &filePath);
};

#endif
