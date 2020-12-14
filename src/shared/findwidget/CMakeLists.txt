set(SHAREDFINDWIDGET_INCLUDES
    ${CMAKE_BINARY_DIR}/include
    ${CMAKE_BINARY_DIR}/privateinclude
    ${CMAKE_BINARY_DIR}/include/QtCore
    ${CMAKE_BINARY_DIR}/include/QtGui
    ${CMAKE_SOURCE_DIR}/src/shared/findwidget
    ${CMAKE_BINARY_DIR}/src/shared/findwidget
)

set(SHAREDFINDWIDGET_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/abstractfindwidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/texteditfindwidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/itemviewfindwidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/findwidget.qrc
)

set(SHAREDFINDWIDGET_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/abstractfindwidget.h
    ${CMAKE_CURRENT_SOURCE_DIR}/texteditfindwidget.h
    ${CMAKE_CURRENT_SOURCE_DIR}/itemviewfindwidget.h
)

katie_setup_target(sharedfindwidget ${SHAREDFINDWIDGET_SOURCES} ${SHAREDFINDWIDGET_HEADERS})

add_library(sharedfindwidget OBJECT ${sharedfindwidget_SOURCES})
target_include_directories(sharedfindwidget PUBLIC ${SHAREDFINDWIDGET_INCLUDES})
