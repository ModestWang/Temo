#-------------------------------------------------
#
# Project created by QtCreator 2024-06-25T11:07:32
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Demo
TEMPLATE = app

SOURCES += main.cpp mainwindow.cpp

HEADERS  += mainwindow.h \
    include/types.h \
    include/system.h \
    include/string.h \
    include/stdio.h \
    include/kernel.h \
    include/gcclib.h \
    include/ctype.h \
    common.h \
    hardware.h

FORMS    += mainwindow.ui

RESOURCES += resources.qrc

DESTDIR = ./bin

OBJECTS_DIR = ./obj
