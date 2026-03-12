#-------------------------------------------------
#
# Project created by QtCreator 2014-04-10T19:21:07
#
#-------------------------------------------------

QT       += core gui
QT       += core
QT       += charts
QT       += axcontainer
QT       += widgets
QT       += network
DEFINES  += QT_CHARTS_USE_OPENGL
QT       += core gui network charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

greaterThan(QT_MAJOR_VERSION, 5): QT += core5compat

TARGET = NetAssistant
target.path=/usr/local/bin
INSTALLS=target

TEMPLATE = app


SOURCES += main.cpp\
    mainwindow.cpp \
    TcpServer.cpp

HEADERS  += \
    define.h \
    mainwindow.h \
    TcpServer.h

FORMS    += \
    mainwindow.ui

RESOURCES += \
    qrc.qrc

QT  +=network

RC_FILE += icon.rc

DISTFILES += \
    android/AndroidManifest.xml

TRANSLATIONS += language/English.ts \
                language/Chinese.ts

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
