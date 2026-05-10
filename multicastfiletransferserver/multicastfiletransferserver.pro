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

rightmenu.files = data/menu/*
rightmenu.path = /usr/share/nemo/actions/

INSTALLS += target service rightmenu
