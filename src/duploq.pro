TEMPLATE = subdirs
CONFIG += ordered

include(config.pri)

# engine
BUILD_DUPLO{
	SUBDIRS += engine
	engine.file = ./engine/engine.pro
}

# app
SUBDIRS += app
app.file = ./app/app.pro

