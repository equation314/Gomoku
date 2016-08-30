#-------------------------------------------------
#
# Project created by QtCreator 2016-08-30T14:42:15
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Gomoku
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    board.cpp \
    pieces.cpp

HEADERS  += mainwindow.h \
    board.h \
    const.h \
    pieces.h

FORMS    += mainwindow.ui
