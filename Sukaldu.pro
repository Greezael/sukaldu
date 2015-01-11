#-------------------------------------------------
#
# Project created by QtCreator 2014-11-27T17:13:23
#
#-------------------------------------------------

QT       += sql core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Sukaldu
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
        pricedialog.cpp \
    productsection.cpp

HEADERS  += mainwindow.h \
    connection.h \
    pricedialog.h

FORMS    += mainwindow.ui \
    pricedialog.ui

OTHER_FILES += \
    data/database_definition.sql \
    data/database_sample.sql

data.path    = $$OUT_PWD/data
data.files   += \
    data/database_definition.sql \
    data/database_sample.sql

INSTALLS       += data
