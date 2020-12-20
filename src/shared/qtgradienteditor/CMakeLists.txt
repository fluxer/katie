set(SHAREDGRADIENDEDITOR_INCLUDES
    ${CMAKE_BINARY_DIR}/include
    ${CMAKE_BINARY_DIR}/privateinclude
    ${CMAKE_BINARY_DIR}/include/QtCore
    ${CMAKE_BINARY_DIR}/include/QtGui
    ${CMAKE_BINARY_DIR}/include/QtXml
    ${CMAKE_SOURCE_DIR}/src/shared/qtgradienteditor
    ${CMAKE_BINARY_DIR}/src/shared/qtgradienteditor
)

set(SHAREDGRADIENDEDITOR_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/qtgradienteditor.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/qtgradientdialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/qtgradientview.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/qtgradientviewdialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/qtgradientstopsmodel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qtgradientstopswidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qtgradientstopscontroller.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qtgradientwidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qtgradienteditor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qtgradientdialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qtcolorbutton.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qtcolorline.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qtgradientview.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qtgradientviewdialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qtgradientmanager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qtgradientutils.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qtgradienteditor.qrc
)

set(SHAREDGRADIENDEDITOR_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/qtgradientstopsmodel.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qtgradientstopswidget.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qtgradientstopscontroller.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qtgradientwidget.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qtgradienteditor.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qtgradientdialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qtcolorbutton.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qtcolorline.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qtgradientview.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qtgradientviewdialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qtgradientmanager.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qtgradientutils.h
)

katie_setup_target(sharedqtgradienteditor ${SHAREDGRADIENDEDITOR_SOURCES} ${SHAREDGRADIENDEDITOR_HEADERS})

add_library(sharedqtgradienteditor OBJECT ${sharedqtgradienteditor_SOURCES})
target_include_directories(sharedqtgradienteditor PUBLIC ${SHAREDGRADIENDEDITOR_INCLUDES})
