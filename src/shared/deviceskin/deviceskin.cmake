set(SHAREDDEVICESKIN_DEFINITIONS)

set(SHAREDDEVICESKIN_INCLUDES
    ${CMAKE_BINARY_DIR}/include
    ${CMAKE_BINARY_DIR}/privateinclude
    ${CMAKE_BINARY_DIR}/include/QtCore
    ${CMAKE_BINARY_DIR}/privateinclude/QtCore
    ${CMAKE_BINARY_DIR}/include/QtGui
    ${CMAKE_BINARY_DIR}/privateinclude/QtGui
    ${CMAKE_SOURCE_DIR}/src/shared/deviceskin
    ${CMAKE_BINARY_DIR}/src/shared/deviceskin
    ${CMAKE_BINARY_DIR}/src/shared/deviceskin/skins
)

set(SHAREDDEVICESKIN_SOURCES
    ${CMAKE_SOURCE_DIR}/src/shared/deviceskin/deviceskin.cpp
    ${CMAKE_SOURCE_DIR}/src/shared/deviceskin/skins/ClamshellPhone.qrc
    ${CMAKE_SOURCE_DIR}/src/shared/deviceskin/skins/SmartPhone2.qrc
    ${CMAKE_SOURCE_DIR}/src/shared/deviceskin/skins/SmartPhone.qrc
    ${CMAKE_SOURCE_DIR}/src/shared/deviceskin/skins/SmartPhoneWithButtons.qrc
    ${CMAKE_SOURCE_DIR}/src/shared/deviceskin/skins/TouchscreenPhone.qrc
    ${CMAKE_SOURCE_DIR}/src/shared/deviceskin/skins/PortableMedia.qrc
    ${CMAKE_SOURCE_DIR}/src/shared/deviceskin/skins/S60-QVGA-Candybar.qrc
    ${CMAKE_SOURCE_DIR}/src/shared/deviceskin/skins/S60-nHD-Touchscreen.qrc
)

set(SHAREDDEVICESKIN_HEADERS
    ${CMAKE_SOURCE_DIR}/src/shared/deviceskin/deviceskin.h
)

katie_setup_target(shareddeviceskin ${SHAREDDEVICESKIN_SOURCES} ${SHAREDDEVICESKIN_HEADERS})

add_library(shareddeviceskin OBJECT ${shareddeviceskin_SOURCES})
target_compile_definitions(shareddeviceskin PRIVATE ${SHAREDDEVICESKIN_DEFINITIONS})
target_include_directories(shareddeviceskin PRIVATE ${SHAREDDEVICESKIN_INCLUDES})
