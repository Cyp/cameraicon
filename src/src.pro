SOURCES += main.cpp \
 cameratrayicon.cpp
TEMPLATE = app
CONFIG += warn_on \
	  thread \
          qt
TARGET = cameraicon
DESTDIR = ../bin
HEADERS += cameratrayicon.h

RESOURCES += icons.qrc
