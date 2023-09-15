TEMPLATE = app
TARGET = Horaris-FIB.exe

QT += widgets

CONFIG += qt warn_on release

FORMS += ui/Form.ui

INCLUDEPATH += /usr/include/x86_64-linux-gnu/qt5

SOURCES += src/main.cc \
        src/Data.cc \
        src/Horari.cc \
        src/HTTPSGetter.cc \
        src/Parser.cc 

HEADERS += src/Data.hh \
        src/Horari.hh \
        src/HTTPSGetter.hh \
        src/Parser.hh lib/httplib.h

QMAKE_CXXFLAGS += -Wall -Wno-sign-compare -O3
LIBS += -lssl -lcrypto





