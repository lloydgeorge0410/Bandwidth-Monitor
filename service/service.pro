QT += core network

CONFIG += c++17 cmdline
CONFIG -= app_bundle

LIBS += -ladvapi32

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        config.cpp \
        main.cpp \
        mainthread.cpp \
        monitorthread.cpp \
        rotatingfilelogger.cpp

HEADERS += \
    config.h \
    mainthread.h \
    monitorthread.h \
    rotatingfilelogger.h

LIBS += C:\npcap-sdk-1.15\Lib\x64\wpcap.lib \
        -lws2_32

INCLUDEPATH += C:\npcap-sdk-1.15\Include
