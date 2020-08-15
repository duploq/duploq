TARGET = duploq

# output
CONFIG(debug, debug|release){
	DESTDIR = $$OUT_PWD/../bin.debug
}
else{
	DESTDIR = $$OUT_PWD/../bin
}

# temp dirs (unix)
unix{
	MOC_DIR = $$OUT_PWD/_generated
	OBJECTS_DIR = $$OUT_PWD/_generated
	UI_DIR = $$OUT_PWD/_generated
	RCC_DIR = $$OUT_PWD/_generated
}

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

win32{
    VERSION = 0.1.0
    RC_ICONS = $$PWD/Icons/MainIcon.ico
    QMAKE_TARGET_COPYRIGHT = (C) 2020 A.Masyuk
    QMAKE_TARGET_DESCRIPTION = DuploQ - source code duplicate finder frontend
    QMAKE_TARGET_PRODUCT = DuploQ
}

include($$PWD/../engine/version.pri)

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

UI_DIR = $$OUT_PWD/$$TARGET
MOC_DIR = $$OUT_PWD/$$TARGET
OBJECTS_DIR = $$OUT_PWD/$$TARGET
RCC_DIR = $$OUT_PWD/$$TARGET

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    app.qrc
