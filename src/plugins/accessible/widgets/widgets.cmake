set(ACCESSIBLEWIDGETSPLUGIN_HEADERS
    ${ACCESSIBLEWIDGETSPLUGIN_HEADERS}
    ${CMAKE_CURRENT_SOURCE_DIR}/accessible/widgets/qaccessiblewidgets.h
    ${CMAKE_CURRENT_SOURCE_DIR}/accessible/widgets/simplewidgets.h
    ${CMAKE_CURRENT_SOURCE_DIR}/accessible/widgets/rangecontrols.h
    ${CMAKE_CURRENT_SOURCE_DIR}/accessible/widgets/complexwidgets.h
    ${CMAKE_CURRENT_SOURCE_DIR}/accessible/widgets/qaccessiblemenu.h
    ${CMAKE_CURRENT_SOURCE_DIR}/accessible/widgets/itemviews.h
)

set(ACCESSIBLEWIDGETSPLUGIN_SOURCES
    ${ACCESSIBLEWIDGETSPLUGIN_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/accessible/widgets/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/accessible/widgets/simplewidgets.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/accessible/widgets/rangecontrols.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/accessible/widgets/complexwidgets.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/accessible/widgets/qaccessiblewidgets.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/accessible/widgets/qaccessiblemenu.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/accessible/widgets/itemviews.cpp
)

add_library(qtaccessiblewidgetsplugin ${KATIE_TYPE} ${ACCESSIBLEWIDGETSPLUGIN_SOURCES} ${ACCESSIBLEWIDGETSPLUGIN_HEADERS})
target_link_libraries(qtaccessiblewidgetsplugin KtCore KtGui)
set_target_properties(qtaccessiblewidgetsplugin PROPERTIES OUTPUT_NAME qtaccessiblewidgets)
target_include_directories(qtaccessiblewidgetsplugin PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/accessible/widgets)

install(TARGETS qtaccessiblewidgetsplugin DESTINATION ${QT_PLUGINS_PATH_INST}/accessible/)
