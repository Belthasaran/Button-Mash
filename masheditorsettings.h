#ifndef MASHEDITORSETTINGS_H
#define MASHEDITORSETTINGS_H

#include <QSettings>
#include <QString>

QSettings *mashEditorSettings();
QString mashEditorSkinsRoot(QSettings *s);

#endif
