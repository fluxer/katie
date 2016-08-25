set(SHAREDPROPERTYBROWSER_DEFINITIONS)

set(SHAREDPROPERTYBROWSER_INCLUDES
    ${CMAKE_BINARY_DIR}/include
    ${CMAKE_BINARY_DIR}/privateinclude
    ${CMAKE_BINARY_DIR}/include/QtCore
    ${CMAKE_BINARY_DIR}/privateinclude/QtCore
    ${CMAKE_BINARY_DIR}/include/QtGui
    ${CMAKE_BINARY_DIR}/privateinclude/QtGui
    ${CMAKE_SOURCE_DIR}/src/shared/qtpropertybrowser
    ${CMAKE_BINARY_DIR}/src/shared/qtpropertybrowser
)

set(SHAREDPROPERTYBROWSER_SOURCES
    ${CMAKE_SOURCE_DIR}/src/shared/qtpropertybrowser/qtpropertybrowser.cpp
    ${CMAKE_SOURCE_DIR}/src/shared/qtpropertybrowser/qtpropertymanager.cpp
    ${CMAKE_SOURCE_DIR}/src/shared/qtpropertybrowser/qteditorfactory.cpp
    ${CMAKE_SOURCE_DIR}/src/shared/qtpropertybrowser/qtvariantproperty.cpp
    ${CMAKE_SOURCE_DIR}/src/shared/qtpropertybrowser/qttreepropertybrowser.cpp
    ${CMAKE_SOURCE_DIR}/src/shared/qtpropertybrowser/qtbuttonpropertybrowser.cpp
    ${CMAKE_SOURCE_DIR}/src/shared/qtpropertybrowser/qtgroupboxpropertybrowser.cpp
    ${CMAKE_SOURCE_DIR}/src/shared/qtpropertybrowser/qtpropertybrowserutils.cpp
    ${CMAKE_SOURCE_DIR}/src/shared/qtpropertybrowser/qtpropertybrowser.qrc
)

set(SHAREDPROPERTYBROWSER_HEADERS
    ${CMAKE_SOURCE_DIR}/src/shared/qtpropertybrowser/qtpropertybrowser.h
    ${CMAKE_SOURCE_DIR}/src/shared/qtpropertybrowser/qtpropertymanager.h
    ${CMAKE_SOURCE_DIR}/src/shared/qtpropertybrowser/qteditorfactory.h
    ${CMAKE_SOURCE_DIR}/src/shared/qtpropertybrowser/qtvariantproperty.h
    ${CMAKE_SOURCE_DIR}/src/shared/qtpropertybrowser/qttreepropertybrowser.h
    ${CMAKE_SOURCE_DIR}/src/shared/qtpropertybrowser/qtbuttonpropertybrowser.h
    ${CMAKE_SOURCE_DIR}/src/shared/qtpropertybrowser/qtgroupboxpropertybrowser.h
    ${CMAKE_SOURCE_DIR}/src/shared/qtpropertybrowser/qtpropertybrowserutils_p.h
)

katie_setup_target(sharedqtpropertybrowser ${SHAREDPROPERTYBROWSER_SOURCES} ${SHAREDPROPERTYBROWSER_HEADERS})

add_library(sharedqtpropertybrowser OBJECT ${sharedqtpropertybrowser_SOURCES})
target_compile_definitions(sharedqtpropertybrowser PRIVATE ${SHAREDPROPERTYBROWSER_DEFINITIONS})
target_include_directories(sharedqtpropertybrowser PRIVATE ${SHAREDPROPERTYBROWSER_INCLUDES})
