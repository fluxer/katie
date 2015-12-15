include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}/designer/phonon
)

set(PHONONWIDGETSLUGIN_SOURCES
    ${PHONONWIDGETSLUGIN_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/designer/phonon/phononcollection.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/designer/phonon/seeksliderplugin.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/designer/phonon/videoplayerplugin.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/designer/phonon/videoplayertaskmenu.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/designer/phonon/videowidgetplugin.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/designer/phonon/volumesliderplugin.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/designer/phonon/phononwidgets.qrc
)

set(PHONONWIDGETSLUGIN_HEADERS
    ${PHONONWIDGETSLUGIN_HEADERS}
    ${CMAKE_CURRENT_SOURCE_DIR}/designer/phonon/seeksliderplugin.h
    ${CMAKE_CURRENT_SOURCE_DIR}/designer/phonon/volumesliderplugin.h
    ${CMAKE_CURRENT_SOURCE_DIR}/designer/phonon/videoplayerplugin.h
    ${CMAKE_CURRENT_SOURCE_DIR}/designer/phonon/videowidgetplugin.h
    ${CMAKE_CURRENT_SOURCE_DIR}/designer/phonon/videoplayertaskmenu.h
)

katie_resources(${PHONONWIDGETSLUGIN_SOURCES})
katie_resources(${PHONONWIDGETSLUGIN_HEADERS})

add_library(phononwidgetsplugin ${KATIE_TYPE} ${PHONONWIDGETSLUGIN_SOURCES} ${PHONONWIDGETSLUGIN_HEADERS})
target_link_libraries(phononwidgetsplugin KtCore KtGui KtPhonon KtDesigner)
set_target_properties(phononwidgetsplugin PROPERTIES OUTPUT_NAME phononwidgets)

install(
    TARGETS phononwidgetsplugin
    DESTINATION ${QT_PLUGINS_PATH_INST}/designer
)
