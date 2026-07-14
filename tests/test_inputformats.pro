QT += core network
CONFIG += console c++11
CONFIG -= app_bundle
TARGET = test_inputformats
TEMPLATE = app

INCLUDEPATH += $$PWD/..
DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    test_inputformats.cpp \
    ../snesbitorder.cpp \
    ../inputsessionstate.cpp \
    ../inputsessionlogger.cpp \
    ../sessioncrypto.cpp \
    ../buttonmashremoteprovider.cpp \
    ../retroarchremotepadprovider.cpp \
    ../inputprovider.cpp \
    ../inputmirrormanager.cpp \
    ../inputtriggers.cpp \
    ../uinputdevice.cpp \
    ../third_party/xxhash.c

HEADERS += \
    ../snesbitorder.h \
    ../inputsessionstate.h \
    ../inputsessionlogger.h \
    ../sessioncrypto.h \
    ../buttonmashremoteprovider.h \
    ../retroarchremotepadprovider.h \
    ../inputprovider.h \
    ../inputmirrormanager.h \
    ../inputtriggers.h \
    ../uinputdevice.h \
    ../third_party/xxhash.h

LIBS += -lcrypto
win32: LIBS += -luser32

