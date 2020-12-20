set(SHAREDPROPERTYBROWSER_INCLUDES
    ${CMAKE_BINARY_DIR}/include
    ${CMAKE_BINARY_DIR}/privateinclude
    ${CMAKE_BINARY_DIR}/include/QtCore
    ${CMAKE_BINARY_DIR}/include/QtGui
    ${CMAKE_SOURCE_DIR}/src/shared/qtpropertybrowser
    ${CMAKE_BINARY_DIR}/src/shared/qtpropertybrowser
)

set(SHAREDPROPERTYBROWSER_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/qtpropertybrowser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qtpropertymanager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qteditorfactory.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qtvariantproperty.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qttreepropertybrowser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qtbuttonpropertybrowser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qtgroupboxpropertybrowser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qtpropertybrowserutils.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qtpropertybrowser.qrc
)

set(SHAREDPROPERTYBROWSER_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/qtpropertybrowser.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qtpropertymanager.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qteditorfactory.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qtvariantproperty.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qttreepropertybrowser.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qtbuttonpropertybrowser.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qtgroupboxpropertybrowser.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qtpropertybrowserutils_p.h
)

katie_setup_target(sharedqtpropertybrowser ${SHAREDPROPERTYBROWSER_SOURCES} ${SHAREDPROPERTYBROWSER_HEADERS})

add_library(sharedqtpropertybrowser OBJECT ${sharedqtpropertybrowser_SOURCES})
target_include_directories(sharedqtpropertybrowser PUBLIC ${SHAREDPROPERTYBROWSER_INCLUDES})
