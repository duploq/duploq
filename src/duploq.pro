TEMPLATE = subdirs
CONFIG += ordered

# engine
SUBDIRS += engine
engine.file = ./engine/engine.pro

# app
SUBDIRS += app
app.file = ./app/app.pro

