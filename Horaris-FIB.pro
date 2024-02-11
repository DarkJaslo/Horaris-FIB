TEMPLATE = app
TARGET = Horaris-FIB

QT += widgets

CONFIG += qt warn_on release

FORMS += ui/MyForm.ui ui/HorariWin.ui

INCLUDEPATH += /usr/include/x86_64-linux-gnu/qt5 \
            ./src \
            ./ui \
            ./lib

SOURCES += src/main.cc \
        src/Data.cc \
        src/Horari.cc \
        src/HTTPSGetter.cc \
        src/Parser.cc \
        ui/MyForm.cc \
        src/App.cc \
        ui/MyComboBox.cc \
        ui/HorariWin.cc

HEADERS += src/Data.hh \
        src/Horari.hh \
        src/HTTPSGetter.hh \
        src/Parser.hh \
        lib/httplib.h \
        ui/MyForm.h \
        src/App.hh \
        ui/MyComboBox.hh \
        ui/HorariWin.h

QMAKE_CXXFLAGS += -Wall -Wno-sign-compare -g -D_GLIBCXX_DEBUG -O3
LIBS += -lssl -lcrypto





