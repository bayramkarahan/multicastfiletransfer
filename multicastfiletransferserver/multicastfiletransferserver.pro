QT       += core gui network concurrent
TARGET = multicastfiletransferserver
TEMPLATE = app
#LIBS += -lz

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    multicastfileserver.cpp \
    progressdialog.cpp

HEADERS += \
    multicastfileserver.h \
    progressdialog.h \
    protocol.h


target.path = /usr/bin
service.files = multicastfiletransferclient.service
service.path = /lib/systemd/system/

nemomenu.files = data/nemomenu/*
nemomenu.path = /usr/share/nemo/actions/

nautilusmenu.files = data/nautilusmenu/*
nautilusmenu.path = /usr/share/nautilus/scripts/

dolphinmenu.files = data/dolphinmenu/*
dolphinmenu.path = /usr/share/kservices5/ServiceMenus/

thunarmenu.files = data/thunarmenu/*
thunarmenu.path = /usr/share/Thunar/

INSTALLS += target service nemomenu nautilusmenu dolphinmenu thunarmenu
