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

TARGET = qvkplayer
TEMPLATE = app

unix {
    isEmpty(PREFIX)
    {
        PREFIX = /usr
    }
    target.path = $$PREFIX/bin

    desktop.files = qvkplayer.desktop
    desktop.path =  $$PREFIX/share/applications

    target1.files = icons/qvk64.png
    target1.path =  $$PREFIX/share/pixmaps

    icon_32.files =  icons/LOGOS/qvk32.png
    icon_64.files =  icons/LOGOS/qvk64.png
    icon_32.path =  $$PREFIX/share/icons/hicolor/32x32/apps
    icon_64.path =  $$PREFIX/share/icons/hicolor/64x64/apps

    INSTALLS += target desktop target1
    INSTALLS += icon_32 icon_64
}

win32 {
#OS WIN
RC_FILE = winicon.rc
 }

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
