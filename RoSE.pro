TEMPLATE = app

QT += gui core qml quick serialport sql websockets
CONFIG += \
    c++11 \
#    console

INCLUDEPATH += \
    $$PWD/include

HEADERS += \
    include/hspsprotocol.h \
    include/common.h \
    include/useriface.h \
    include/sender.h \
    include/receiver.h \
    include/swupdater.h \
    include/logger.h \
    include/ymodem.h \
    include/gui.h \
    ../lib/conf/conf.h \
    ../lib/protocol/protocol.h \
    ../lib/protocol/uart.h \
    ../lib/protocol/uartprotocol.h \
    ../lib/w_qtlib/qtlib.h \
    ../lib/global.h \
    include/wifi.h \
    include/patient.h \
    include/websocketthread.h

SOURCES += \
    src/main.cpp \
    src/useriface.cpp \
    src/sender.cpp \
    src/receiver.cpp \
    src/swupdater.cpp \
    src/logger.cpp \
    src/ymodem.cpp \
    src/gentlo_res.cpp \
    src/gentlo_db.cpp \
    src/wifi.cpp \
    src/gui.cpp \
    ../lib/conf/conf.cpp \
    ../lib/protocol/protocol.cpp \
    ../lib/protocol/uart.cpp \
    ../lib/protocol/uartprotocol.cpp \
    ../lib/w_qtlib/qtlib.cpp \
    ../lib/w_qtlib/aria.c \
    ../lib/util.cpp \
    src/patient.cpp \
    src/websocketthread.cpp

RESOURCES += qml.qrc

TARGET = rose

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)
target.path = /home/app/bin

DISTFILES +=
