TEMPLATE = app
TARGET = QtExample
QT += core opengl gui widgets

DESTDIR = {PWD}/../../../bin

DEPENDPATH += $$PWD/../inc
INCLUDEPATH += $$PWD/../inc

LIBS += -lopengl32 \
    -l$${PWD}/../../lib/LibGizmoQt \
    -lglu32 \
    -lGdi32 \
    -lUser32

SOURCES += ./main.cpp
