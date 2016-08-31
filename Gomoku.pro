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
    choosecolordialog.cpp

HEADERS  += mainwindow.h \
    board.h \
    const.h \
    pieces.h \
    connectdialog.h \
    player.h \
    choosecolordialog.h

FORMS    += mainwindow.ui \
    connectdialog.ui \
    choosecolordialog.ui

RESOURCES +=
