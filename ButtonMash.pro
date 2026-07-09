#-------------------------------------------------
#
# Project created by QtCreator 2018-01-01T20:08:03
#
#-------------------------------------------------

QT       += core gui network xml serialport gamepad websockets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ButtonMash
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


win32 {
    LIBS += -ldinput8 -ldxguid -luser32
    CONFIG += c++11
    SOURCES +=  QGameController/src/gamecontroller/qgamecontroller.cpp
    SOURCES +=  QGameController/src/gamecontroller/qgamecontroller_win.cpp
    SOURCES +=  directinputsource.cpp
    HEADERS +=  directinputsource.h
    QGC_PUBLIC_HEADERS += QGameController/src/gamecontroller/qgamecontroller.h
    QGC_PRIVATE_HEADERS +=  QGameController/src/gamecontroller/qgamecontroller_p.h
    HEADERS += $$QGC_PUBLIC_HEADERS $$QGC_PRIVATE_HEADERS
}

SOURCES += \
        main.cpp \
        skinselector.cpp \
        inputdecoder.cpp \
        telnetconnection.cpp \
        inputdisplay.cpp \
        skinparser.cpp \
        arduinocom.cpp \
        inputprovider.cpp \
        inputsourceselector.cpp \
        qgamepadsource.cpp \
        mapbuttondialog.cpp \
        usb2snessource.cpp \
        usb2snes.cpp \
        snesclassictelnet.cpp \
        configurationdialog.cpp \
        localcontroller.cpp \
        localcontrollermanager.cpp \
        snesbitorder.cpp \
        retroarchremotepadprovider.cpp \
        buttonmashremoteprovider.cpp \
        inputsessionstate.cpp \
        inputsessionlogger.cpp \
        sessioncrypto.cpp \
        inputmirrormanager.cpp \
        mirrortargetsdialog.cpp \
        inputtriggers.cpp \
        triggersdialog.cpp
unix {
    SOURCES += uinputdevice.cpp
}

HEADERS += \
        skinselector.h \
        inputdecoder.h \
        telnetconnection.h \
        inputdisplay.h \
        skinparser.h \
        arduinocom.h \
        inputprovider.h \
        inputsourceselector.h \
        qgamepadsource.h \
        mapbuttondialog.h \
        usb2snessource.h \
        usb2snes.h \
        snesclassictelnet.h \
        configurationdialog.h \
        localcontroller.h \
        localcontrollermanager.h \
        sqpath.h \
        snesbitorder.h \
        retroarchremotepadprovider.h \
        buttonmashremoteprovider.h \
        inputsessionstate.h \
        inputsessionlogger.h \
        sessioncrypto.h \
        inputmirrormanager.h \
        mirrortargetsdialog.h \
        inputtriggers.h \
        triggersdialog.h
unix {
    HEADERS += uinputdevice.h
}
        third_party/xxhash.h

FORMS += \
        skinselector.ui \
        inputdisplay.ui \
        inputsourceselector.ui \
        mapbuttondialog.ui \
        configurationdialog.ui \
        mirrortargetsdialog.ui \
        triggersdialog.ui

INCLUDEPATH += $$PWD

unix {
    LIBS += -lcrypto
}
win32 {
    OPENSSL_ROOT = $$(OPENSSL_ROOT_DIR)
    isEmpty(OPENSSL_ROOT): OPENSSL_ROOT = "C:/Program Files/OpenSSL-Win64"
    INCLUDEPATH += $$OPENSSL_ROOT/include
    exists($$OPENSSL_ROOT/lib/VC/x64/MD/libcrypto.lib) {
        LIBS += "$$OPENSSL_ROOT/lib/VC/x64/MD/libcrypto.lib"
    } else:exists($$OPENSSL_ROOT/lib/libcrypto.lib) {
        LIBS += "$$OPENSSL_ROOT/lib/libcrypto.lib"
    } else {
        LIBS += -L$$OPENSSL_ROOT/lib -llibcrypto
    }
}

RC_FILE += \
    buttonmash.rc

macx: {
	QMAKE_INFO_PLIST = Info.plist
	ICON = Icon128x128.icns
}

RESOURCES += \
    ressources.qrc
