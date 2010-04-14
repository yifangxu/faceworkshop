# -------------------------------------------------
# Project created by QtCreator 2010-03-16T16:39:15
# -------------------------------------------------
TARGET = FaceWorkshop
TEMPLATE = app
LIBS += ../../../aamlib-opencv/src/build/libaam.a \
    ../../../aamlib-opencv/src/build/imgwarp/libimgwarp.a \
    -lcxcore \
    -lcv \
    -lhighgui \
    -lcvaux \
    -lml
SOURCES += main.cpp \
    gui_paramdeform.cpp \
    pointspaint.cpp \
    photoview.cpp \
    myimage.cpp \
    widget_modelparamedit.cpp \
    widget_warpcontrolpaint.cpp \
    imageprocess.cpp \
    gui_videocapture.cpp \
    widget_warpparam.cpp \
    widget_facemorph.cpp \
    widget_facephotoview.cpp
HEADERS += gui_paramdeform.h \
    pointspaint.h \
    photoview.h \
    myimage.h \
    widget_modelparamedit.h \
    widget_warpcontrolpaint.h \
    imageprocess.h \
    gui_videocapture.h \
    widget_warpparam.h \
    widget_facemorph.h \
    widget_facephotoview.h
FORMS += gui_paramdeform.ui \
    widget_modelparamedit.ui \
    gui_videocapture.ui \
    widget_warpparam.ui \
    widget_facemorph.ui
INCLUDEPATH += ../../../aamlib-opencv/src \
    ../../../aamlib-opencv/src/imgwarp \
    /usr/include/opencv
