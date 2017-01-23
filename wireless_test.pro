#-------------------------------------------------
#
# Project created by QtCreator 2017-01-22T01:45:47
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = wireless_test
TEMPLATE = app


SOURCES += main.cpp\
        output.cpp \
    mysniff.cpp

HEADERS  += output.h \
    mysniff.h

FORMS    += output.ui

LIBS += -ltins
