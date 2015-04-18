#-------------------------------------------------
#
# Project created by QtCreator 2014-11-27T17:13:23
#
#-------------------------------------------------

QT       += sql core gui

CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Sukaldu
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
        pricedialog.cpp \
    productsection.cpp \
    genericcode.cpp \
    recipesection.cpp \
    menusection.cpp \
    recipedialog.cpp \
    menudialog.cpp \
    settingssection.cpp

HEADERS  += mainwindow.h \
    connection.h \
    pricedialog.h \
    recipedialog.h \
    menudialog.h

FORMS    += mainwindow.ui \
    pricedialog.ui \
    recipedialog.ui \
    menudialog.ui

TRANSLATIONS = \
    translations/sukaldu_es.ts

OTHER_FILES += \
    data/database_definition.sql \
    data/database_sample.sql

data.path    = $$OUT_PWD/data
data.files   += \
    data/database_definition.sql \
    data/database_sample.sql

translations.path    = $$OUT_PWD/translations
translations.files   += \
    translations/sukaldu_es.qm

INSTALLS       += \
            data \
            translations
