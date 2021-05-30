set(ACCESSIBLEWIDGETSPLUGIN_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/qaccessiblewidgets.h
    ${CMAKE_CURRENT_SOURCE_DIR}/simplewidgets.h
    ${CMAKE_CURRENT_SOURCE_DIR}/rangecontrols.h
    ${CMAKE_CURRENT_SOURCE_DIR}/complexwidgets.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qaccessiblemenu.h
    ${CMAKE_CURRENT_SOURCE_DIR}/itemviews.h
)

set(ACCESSIBLEWIDGETSPLUGIN_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/simplewidgets.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/rangecontrols.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/complexwidgets.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qaccessiblewidgets.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qaccessiblemenu.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/itemviews.cpp
)

katie_setup_target(qtaccessiblewidgetsplugin ${ACCESSIBLEWIDGETSPLUGIN_SOURCES} ${ACCESSIBLEWIDGETSPLUGIN_HEADERS})

add_library(qtaccessiblewidgetsplugin MODULE ${qtaccessiblewidgetsplugin_SOURCES})
target_link_libraries(qtaccessiblewidgetsplugin KtCore KtGui)
set_target_properties(qtaccessiblewidgetsplugin PROPERTIES OUTPUT_NAME qtaccessiblewidgets)

katie_setup_plugin(qtaccessiblewidgetsplugin)

install(
    TARGETS qtaccessiblewidgetsplugin
    DESTINATION ${KATIE_PLUGINS_PATH}/accessible
    COMPONENT Runtime
)
