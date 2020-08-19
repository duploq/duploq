# This file is a part of
# DuploQ
#
# (c) 2020 Ars L. Masiuk (ars.masiuk@gmail.com)
#
# It can be used freely, maintaining the information above.

CONFIG += BUILD_DUPLO

# compiler stuff
win32-msvc*{
  	QMAKE_CXXFLAGS += /MP
}
