QT       += core network
CONFIG += console
#LIBS += -lz
TARGET = multicastfiletransferclient
TEMPLATE = app
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    multicastfileclient.cpp

HEADERS += \
    multicastfileclient.h \
    protocol.h




target.path = /usr/bin
service.files = multicastfiletransferclient.service
service.path = /lib/systemd/system/

INSTALLS += target service
