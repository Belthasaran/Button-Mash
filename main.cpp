#include "skinselector.h"
#include "configpresetstore.h"
#include <QApplication>
#include <QCoreApplication>


int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

    QApplication a(argc, argv);
    QCoreApplication::setApplicationName("ButtonMash");
    QCoreApplication::setOrganizationDomain("nyo.fr");

    const QStringList args = a.arguments();
    if (args.contains(QStringLiteral("--self-test-presets"))) {
        return ConfigPresetStore::selfTest();
    }

    SkinSelector w;
    /*SkinEditor e;
    e.show();*/

    w.show();
    /*MapButtonDialog diag;
    diag.show();*/
    return a.exec();
}
