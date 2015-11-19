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
    settingssection.cpp \
    statisticssection.cpp \
    comparisondialog.cpp

HEADERS  += mainwindow.h \
    connection.h \
    pricedialog.h \
    recipedialog.h \
    menudialog.h \
    comparisondialog.h

FORMS    += mainwindow.ui \
    pricedialog.ui \
    recipedialog.ui \
    menudialog.ui \
    comparisondialog.ui

TRANSLATIONS = \
    translations/sukaldu_es.ts

OTHER_FILES += \
    data/database_definition.sql \
    data/database_sample.sql

!win32 {
    copydata.commands = $(COPY_DIR) $$PWD/data $$OUT_PWD
    copytranslations.commands = $(COPY_DIR) $$PWD/translations $$OUT_PWD

    first.depends = $(first) copydata copytranslations
    export(first.depends)
    export(copydata.commands)
    export(copytranslations.commands)
    QMAKE_EXTRA_TARGETS += first copydata copytranslations
}
