set(SHAREDGRADIENDEDITOR_DEFINITIONS)

set(SHAREDGRADIENDEDITOR_INCLUDES
    ${SHAREDGRADIENDEDITOR_INCLUDES}
    ${CMAKE_BINARY_DIR}/include
    ${CMAKE_BINARY_DIR}/privateinclude
    ${CMAKE_BINARY_DIR}/include/QtCore
    ${CMAKE_BINARY_DIR}/privateinclude/QtCore
    ${CMAKE_BINARY_DIR}/include/QtGui
    ${CMAKE_BINARY_DIR}/privateinclude/QtGui
    ${CMAKE_BINARY_DIR}/include/QtXml
    ${CMAKE_BINARY_DIR}/privateinclude/QtXml
    ${CMAKE_SOURCE_DIR}/src/shared/qtgradienteditor
    ${CMAKE_BINARY_DIR}/src/shared/qtgradienteditor
    ${CMAKE_CURRENT_BINARY_DIR}
)

set(SHAREDGRADIENDEDITOR_SOURCES
    ${SHAREDGRADIENDEDITOR_SOURCES}
    ${CMAKE_SOURCE_DIR}/src/shared/qtgradienteditor/qtgradienteditor.ui
    ${CMAKE_SOURCE_DIR}/src/shared/qtgradienteditor/qtgradientdialog.ui
    ${CMAKE_SOURCE_DIR}/src/shared/qtgradienteditor/qtgradientview.ui
    ${CMAKE_SOURCE_DIR}/src/shared/qtgradienteditor/qtgradientviewdialog.ui
    ${CMAKE_SOURCE_DIR}/src/shared/qtgradienteditor/qtgradientstopsmodel.cpp
    ${CMAKE_SOURCE_DIR}/src/shared/qtgradienteditor/qtgradientstopswidget.cpp
    ${CMAKE_SOURCE_DIR}/src/shared/qtgradienteditor/qtgradientstopscontroller.cpp
    ${CMAKE_SOURCE_DIR}/src/shared/qtgradienteditor/qtgradientwidget.cpp
    ${CMAKE_SOURCE_DIR}/src/shared/qtgradienteditor/qtgradienteditor.cpp
    ${CMAKE_SOURCE_DIR}/src/shared/qtgradienteditor/qtgradientdialog.cpp
    ${CMAKE_SOURCE_DIR}/src/shared/qtgradienteditor/qtcolorbutton.cpp
    ${CMAKE_SOURCE_DIR}/src/shared/qtgradienteditor/qtcolorline.cpp
    ${CMAKE_SOURCE_DIR}/src/shared/qtgradienteditor/qtgradientview.cpp
    ${CMAKE_SOURCE_DIR}/src/shared/qtgradienteditor/qtgradientviewdialog.cpp
    ${CMAKE_SOURCE_DIR}/src/shared/qtgradienteditor/qtgradientmanager.cpp
    ${CMAKE_SOURCE_DIR}/src/shared/qtgradienteditor/qtgradientutils.cpp
    ${CMAKE_SOURCE_DIR}/src/shared/qtgradienteditor/qtgradienteditor.qrc
    ${CMAKE_SOURCE_DIR}/src/shared/qtgradienteditor/qtcolorbutton.cpp
)

set(SHAREDGRADIENDEDITOR_HEADERS
    ${SHAREDGRADIENDEDITOR_HEADERS}
    ${CMAKE_SOURCE_DIR}/src/shared/qtgradienteditor/qtgradientstopsmodel.h
    ${CMAKE_SOURCE_DIR}/src/shared/qtgradienteditor/qtgradientstopswidget.h
    ${CMAKE_SOURCE_DIR}/src/shared/qtgradienteditor/qtgradientstopscontroller.h
    ${CMAKE_SOURCE_DIR}/src/shared/qtgradienteditor/qtgradientwidget.h
    ${CMAKE_SOURCE_DIR}/src/shared/qtgradienteditor/qtgradienteditor.h
    ${CMAKE_SOURCE_DIR}/src/shared/qtgradienteditor/qtgradientdialog.h
    ${CMAKE_SOURCE_DIR}/src/shared/qtgradienteditor/qtcolorbutton.h
    ${CMAKE_SOURCE_DIR}/src/shared/qtgradienteditor/qtcolorline.h
    ${CMAKE_SOURCE_DIR}/src/shared/qtgradienteditor/qtgradientview.h
    ${CMAKE_SOURCE_DIR}/src/shared/qtgradienteditor/qtgradientviewdialog.h
    ${CMAKE_SOURCE_DIR}/src/shared/qtgradienteditor/qtgradientmanager.h
    ${CMAKE_SOURCE_DIR}/src/shared/qtgradienteditor/qtgradientutils.h
    ${CMAKE_SOURCE_DIR}/src/shared/qtgradienteditor/qtcolorbutton.h
)

add_library(sharedqtgradienteditor OBJECT ${SHAREDGRADIENDEDITOR_SOURCES} ${SHAREDGRADIENDEDITOR_HEADERS})
target_compile_definitions(sharedqtgradienteditor PRIVATE ${SHAREDGRADIENDEDITOR_DEFINITIONS})
target_include_directories(sharedqtgradienteditor PRIVATE ${SHAREDGRADIENDEDITOR_INCLUDES})

katie_setup_target(sharedqtgradienteditor)
