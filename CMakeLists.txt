project(qtTst)
cmake_minimum_required(VERSION 2.6)


find_package(Qt4 REQUIRED)
FIND_PACKAGE( OpenCV REQUIRED )

include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR} ${OPENCV_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR})

INCLUDE_DIRECTORIES(dep/asmlib)
INCLUDE_DIRECTORIES(dep/imgwarp)
#add_subdirectory(tst)

add_subdirectory(dep/asmlib)
add_subdirectory(dep/imgwarp)

set(CPP_SOURCES main.cpp 
    gui_paramdeform.cpp 
    pointspaint.cpp 
    photoview.cpp 
    myimage.cpp 
    widget_modelparamedit.cpp 
    widget_warpcontrolpaint.cpp 
    imageprocess.cpp 
    gui_videocapture.cpp 
    widget_warpparam.cpp )

set (FORM_HDRS gui_paramdeform.h photoview.h 
	widget_warpparam.h gui_videocapture.h widget_modelparamedit.h pointspaint.h)

#qt4_automoc(QOBJ_CPP ${CPP_SOURCES})
qt4_wrap_cpp(QOBJ_CPP ${FORM_HDRS})
qt4_wrap_ui(UI_FORMS gui_paramdeform.ui gui_videocapture.ui widget_modelparamedit.ui widget_warpparam.ui)
add_executable(faceworkshop ${UI_FORMS} ${CPP_SOURCES} ${QOBJ_CPP})

INCLUDE(${QT_USE_FILE})
ADD_DEFINITIONS(${QT_DEFINITIONS})
target_link_libraries(faceworkshop ${QT_LIBRARIES} ${OpenCV_LIBS} asm imgwarp-lib)

#target_link_libraries(faceworkshop ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY}  ${OpenCV_LIBS} asm imgwarp-lib)
