#-------------------------------------------------
#
# Project created by QtCreator 2013-05-28T13:40:07
#
#-------------------------------------------------

QT       += core gui\
webkit\
network \
xml \
phonon

QT += xmlpatterns


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QVkPlayer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    vkauth.cpp \
    playingcontrol.cpp

HEADERS  += mainwindow.h \
    vkauth.h \
    vkauth.h \
    playingcontrol.h

FORMS    += mainwindow.ui

RESOURCES += \
    icons.qrc
