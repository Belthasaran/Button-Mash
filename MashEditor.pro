#-------------------------------------------------
# MashEditor — visual skin authoring for Button Mash
#-------------------------------------------------

QT += core gui widgets xml

greaterThan(QT_MAJOR_VERSION, 4): CONFIG += c++11

TARGET = MashEditor
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    masheditor_main.cpp \
    masheditorwindow.cpp \
    masheditorcanvas.cpp \
    masheditormodel.cpp \
    masheditorsettings.cpp \
    skinparser.cpp \
    skinwriter.cpp \
    skinmetadata.cpp \
    skinutil.cpp \
    inputprovider.cpp

HEADERS += \
    masheditorwindow.h \
    masheditorcanvas.h \
    masheditormodel.h \
    masheditorsettings.h \
    skinparser.h \
    skinwriter.h \
    skinmetadata.h \
    skinutil.h \
    inputprovider.h \
    sqpath.h

INCLUDEPATH += $$PWD

win32: LIBS += -luser32
