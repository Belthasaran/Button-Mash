#include "skinselector.h"
#include "configpresetstore.h"
#include "inputsourceselector.h"
#include "buttonmashdebug.h"
#include "browsersourceserver.h"
#include <QApplication>
#include <QCoreApplication>
#include <QProcessEnvironment>


int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

    QApplication a(argc, argv);
    QCoreApplication::setApplicationName("ButtonMash");
    QCoreApplication::setOrganizationDomain("nyo.fr");

    ButtonMashDebug::initFromEnvironment(QProcessEnvironment::systemEnvironment());
    ButtonMashDebug::applyLoggingRules();

    const QStringList args = a.arguments();
    if (args.contains(QStringLiteral("--self-test-presets"))) {
        return ConfigPresetStore::selfTest();
    }
    if (args.contains(QStringLiteral("--self-test-input-persist"))) {
        return inputSourcePersistSelfTest();
    }
    if (args.contains(QStringLiteral("--self-test-debug"))) {
        return ButtonMashDebug::selfTest();
    }
    if (args.contains(QStringLiteral("--self-test-browser-server"))) {
        return BrowserSourceServer::selfTest();
    }

    SkinSelector w;
    /*SkinEditor e;
    e.show();*/

    w.show();
    /*MapButtonDialog diag;
    diag.show();*/
    return a.exec();
}
