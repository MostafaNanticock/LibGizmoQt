QT += core opengl gui

TEMPLATE = lib
TARGET = LibGizmoQt
CONFIG += staticlib
DEFINES += LIBGIZMOQT_LIB BUILD_STATIC

DESTDIR = {PWD}/../../../lib

DEPENDPATH += $$PWD/../inc
INCLUDEPATH += $$PWD/../inc

LIBS += -lopengl32  -lglu32

HEADERS += ./GizmoTransform.h \
    ./GizmoTransformMove.h \
    ./GizmoTransformRender.h \
    ./GizmoTransformRotate.h \
    ./GizmoTransformScale.h \
    ./IGizmo.h \
    ./LibBase.h \
    ./libgizmoqt_global.h \
    ./stdafx.h \
    ./targetver.h \
    ./Utilities.h \
    ./ZBaseDefs.h \
    ./ZBaseMaths.h \
    ./ZCollisionsUtils.h \
    ./ZMathsFunc.h

SOURCES += ./GizmoTransformMove.cpp \
    ./GizmoTransformRender.cpp \
    ./GizmoTransformRotate.cpp \
    ./GizmoTransformScale.cpp \
    ./stdafx.cpp \
    ./Utilities.cpp \
    ./ZBaseMaths.cpp \
    ./ZMathsFunc.cpp
