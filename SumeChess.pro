#-------------------------------------------------
#
# Project created by QtCreator 2013-05-11T21:12:40
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SomeChess
TEMPLATE = app


SOURCES += main.cpp \
        chesswidget.cpp	\
		chesslogic.cpp

HEADERS  += chesswidget.h	\
			chesslogic.h

FORMS    += chesswidget.ui

RESOURCES     += icons.qrc
