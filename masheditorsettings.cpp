#include "masheditorsettings.h"

#include <QCoreApplication>
#include <QProcessEnvironment>

QSettings *mashEditorSettings()
{
    const QString overridePath =
        QProcessEnvironment::systemEnvironment().value(QStringLiteral("BUTTONMASH_CONFIG"));
    if (!overridePath.isEmpty())
        return new QSettings(overridePath, QSettings::IniFormat);
#ifdef Q_OS_WINDOWS
    return new QSettings(QStringLiteral("ButtonMash"), QSettings::IniFormat);
#else
    return new QSettings(QStringLiteral("ButtonMash"));
#endif
}

QString mashEditorSkinsRoot(QSettings *s)
{
    QString path = s->value(QStringLiteral("skinFolder")).toString();
    if (path.isEmpty()) {
        if (QCoreApplication *app = QCoreApplication::instance())
            path = app->applicationDirPath() + QStringLiteral("/Skins");
    }
    return path;
}
