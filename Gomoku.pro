#-------------------------------------------------
#
# Project created by QtCreator 2016-08-30T14:42:15
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Gomoku
TEMPLATE = app

CONFIG += c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    board.cpp \
    pieces.cpp \
    connectdialog.cpp \
    player.cpp \
    choosecolordialog.cpp \
    connection.cpp \
    server.cpp \
    connectionthread.cpp \
    dialpaddialog.cpp \
    waitdialog.cpp

HEADERS  += mainwindow.h \
    board.h \
    const.h \
    pieces.h \
    connectdialog.h \
    player.h \
    choosecolordialog.h \
    connection.h \
    server.h \
    connectionthread.h \
    dialpaddialog.h \
    waitdialog.h

FORMS    += forms/mainwindow.ui \
    forms/connectdialog.ui \
    forms/choosecolordialog.ui \
    forms/dialpaddialog.ui \
    forms/waitdialog.ui

RESOURCES += \
    resource/icon.qrc

RC_FILE += \
    resource/icon.rc
