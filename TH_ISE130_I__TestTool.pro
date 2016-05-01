#-------------------------------------------------
#
# Project created by QtCreator 2016-04-30T23:32:45
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TH_ISE130_I__TestTool
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    configdialog.cpp \
    convert.cpp \
    th_ic000_i.cpp \
    th_ise130_i.cpp

HEADERS  += mainwindow.h \
    configdialog.h \
    convert.h \
    th_ic000_i.h \
    th_ise130_i.h

FORMS    += mainwindow.ui \
    configdialog.ui

RC_FILE = ico.rc
