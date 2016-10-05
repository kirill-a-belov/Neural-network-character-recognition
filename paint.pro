#-------------------------------------------------
#
# Project created by QtCreator 2016-08-23T16:10:52
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = paint
TEMPLATE = app


SOURCES += main.cpp\
        paint.cpp \
    paintscene.cpp \
    neuron.cpp \
    dbhelper.cpp

HEADERS  += paint.h \
    paintscene.h \
    neuron.h \
    dbhelper.h

FORMS    += paint.ui
