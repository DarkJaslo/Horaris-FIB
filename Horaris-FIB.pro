TEMPLATE = app
TARGET = Horaris-FIB

QT += widgets

CONFIG += qt warn_on release

FORMS += ui/MyForm.ui

INCLUDEPATH += /usr/include/x86_64-linux-gnu/qt5 \
            ./src \
            ./ui \
            ./lib \
            ./include

SOURCES += src/main.cc \
        src/Data.cc \
        src/Horari.cc \
        src/HTTPSGetter.cc \
        src/Parser.cc \
        src/MyForm.cc \
        src/App.cc \
        src/MyComboBox.cc

HEADERS += include/Data.hh \
        include/Horari.hh \
        include/HTTPSGetter.hh \
        include/Parser.hh \
        lib/httplib.h \
        include/MyForm.h \
        include/App.hh \
        include/MyComboBox.hh

QMAKE_CXXFLAGS += -Wall -Wno-sign-compare -g -D_GLIBCXX_DEBUG -O3
LIBS += -lssl -lcrypto





