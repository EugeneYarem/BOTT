#-------------------------------------------------
#
# Project created by QtCreator 2017-11-12T21:04:45
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BOTT
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        widget.cpp \
    rect.cpp \
    view.cpp \
    gamemenu.cpp \
    gamemenuhandler.cpp \
    gameMenus/hospitalmenu.cpp \
    gameMenus/mainmenu.cpp \
    gameMenus/minemenu.cpp \
    gameMenus/sciencemenu.cpp \
    gameMenus/workshopmenu.cpp \
    Military/troop.cpp \
    Military/army.cpp \
    Military/battlefield.cpp \
    town.cpp

HEADERS += \
        widget.h \
    rect.h \
    view.h \
    gamemenu.h \
    gamemenuhandler.h \
    gameMenus/hospitalmenu.h \
    gameMenus/mainmenu.h \
    gameMenus/minemenu.h \
    gameMenus/sciencemenu.h \
    gameMenus/workshopmenu.h \
    Military/troop.h \
    Military/army.h \
    Military/battlefield.h \
    town.h

FORMS += \
    widget.ui

RESOURCES += \
    resources.qrc
