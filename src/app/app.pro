TEMPLATE = app

TARGET = duploq

include(../config.pri)

# output
CONFIG(debug, debug|release){
	DESTDIR = $$OUT_PWD/../bin.debug
}
else{
	DESTDIR = $$OUT_PWD/../bin
}

# config
QT += core gui widgets xml

CONFIG += c++17

win32-msvc*{
    QMAKE_CXXFLAGS_WARN_ON -= -w34100
    QMAKE_CXXFLAGS += /MP
    QMAKE_CXXFLAGS += /Zi
    QMAKE_CXXFLAGS += /Gy
    QMAKE_CXXFLAGS += /FS
    QMAKE_CXXFLAGS += /fp:fast
    QMAKE_CXXFLAGS += -D_CRT_SECURE_NO_WARNINGS
    contains(QMAKE_HOST.arch, x86) {QMAKE_CXXFLAGS += /arch:SSE2}
}

# version
include($$PWD/../engine/version.pri)

win32{
    RC_ICONS = $$PWD/Icons/MainIcon.ico
}

VERSION = 0.1.0
QMAKE_TARGET_COPYRIGHT = (C) 2020 Ars L. Masiuk
QMAKE_TARGET_DESCRIPTION = DuploQ - source code duplicate finder frontend
QMAKE_TARGET_PRODUCT = DuploQ

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += $$files($$PWD/*.cpp) $$files($$PWD/3rdparty/*.cpp)
HEADERS += $$files($$PWD/*.h) $$files($$PWD/3rdparty/*.h)
FORMS += $$files($$PWD/*.ui)

INCLUDEPATH += $$PWD/3rdparty

TRANSLATIONS += $$files($$PWD/../translations/*.ts)

RESOURCES += \
    app.qrc

# temp dirs (unix)
unix{
	MOC_DIR = $$OUT_PWD/_generated
	OBJECTS_DIR = $$OUT_PWD/_generated
	UI_DIR = $$OUT_PWD/_generated
	RCC_DIR = $$OUT_PWD/_generated
}
else{
	UI_DIR = $$OUT_PWD/$$TARGET
	MOC_DIR = $$OUT_PWD/$$TARGET
	OBJECTS_DIR = $$OUT_PWD/$$TARGET
	RCC_DIR = $$OUT_PWD/$$TARGET
}

# install
unix{
    PREFIX_DIR = $${PREFIX}

#message($$PREFIX_DIR)

    isEmpty(PREFIX_DIR) {
        PREFIX_DIR = /usr/local
    }

#message($$PREFIX_DIR)

    target.path = $$PREFIX_DIR/bin
    INSTALLS += target

#message($$target.path)

BUILD_DUPLO{
    engine.path = $$PREFIX_DIR/bin
    engine.files = $$DESTDIR/duplo
    INSTALLS += engine
}

    desktop.path = $$PREFIX_DIR/share/applications/
    desktop.files = $$PWD/linux/duploq.desktop
    INSTALLS += desktop

    icon.path = $PREFIX_DIR/share/pixmaps
    icon.files = $$PWD/linux/duploq.png
    INSTALLS += icon

    appdata.path = $$PREFIX_DIR/share/appdata/
    appdata.files = $$PWD/linux/duploq.appdata.xml
    INSTALLS += appdata
}

