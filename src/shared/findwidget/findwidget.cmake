set(SHAREDFINDWIDGET_DEFINITIONS)

set(SHAREDFINDWIDGET_INCLUDES
    ${CMAKE_BINARY_DIR}/include
    ${CMAKE_BINARY_DIR}/privateinclude
    ${CMAKE_BINARY_DIR}/include/QtCore
    ${CMAKE_BINARY_DIR}/privateinclude/QtCore
    ${CMAKE_BINARY_DIR}/include/QtGui
    ${CMAKE_BINARY_DIR}/privateinclude/QtGui
    ${CMAKE_SOURCE_DIR}/src/shared/findwidget
    ${CMAKE_BINARY_DIR}/src/shared/findwidget
)

set(SHAREDFINDWIDGET_SOURCES
    ${CMAKE_SOURCE_DIR}/src/shared/findwidget/abstractfindwidget.cpp
    ${CMAKE_SOURCE_DIR}/src/shared/findwidget/texteditfindwidget.cpp
    ${CMAKE_SOURCE_DIR}/src/shared/findwidget/itemviewfindwidget.cpp
    ${CMAKE_SOURCE_DIR}/src/shared/findwidget/findwidget.qrc
)

set(SHAREDFINDWIDGET_HEADERS
    ${CMAKE_SOURCE_DIR}/src/shared/findwidget/abstractfindwidget.h
    ${CMAKE_SOURCE_DIR}/src/shared/findwidget/texteditfindwidget.h
    ${CMAKE_SOURCE_DIR}/src/shared/findwidget/itemviewfindwidget.h
)

katie_setup_target(sharedfindwidget ${SHAREDFINDWIDGET_SOURCES} ${SHAREDFINDWIDGET_HEADERS})

add_library(sharedfindwidget OBJECT ${sharedfindwidget_SOURCES})
target_compile_definitions(sharedfindwidget PRIVATE ${SHAREDFINDWIDGET_DEFINITIONS})
target_include_directories(sharedfindwidget PRIVATE ${SHAREDFINDWIDGET_INCLUDES})
