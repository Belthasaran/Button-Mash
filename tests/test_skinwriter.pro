QT += core xml gui widgets
CONFIG += console c++11
CONFIG -= app_bundle
TARGET = test_skinwriter
TEMPLATE = app

INCLUDEPATH += $$PWD/..
DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    test_skinwriter.cpp \
    ../skinparser.cpp \
    ../skinwriter.cpp \
    ../skinmetadata.cpp \
    ../skinutil.cpp \
    ../masheditormodel.cpp \
    ../inputprovider.cpp

HEADERS += \
    ../skinparser.h \
    ../skinwriter.h \
    ../skinmetadata.h \
    ../skinutil.h \
    ../masheditormodel.h \
    ../inputprovider.h
