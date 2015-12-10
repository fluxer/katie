set(PHONONWIDGETSLUGIN_SOURCES
    ${PHONONWIDGETSLUGIN_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/designer/phonon/phononcollection.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/designer/phonon/seeksliderplugin.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/designer/phonon/videoplayerplugin.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/designer/phonon/videoplayertaskmenu.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/designer/phonon/videowidgetplugin.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/designer/phonon/volumesliderplugin.cpp
)

add_library(phononwidgetsplugin ${KATIE_TYPE} ${QTIFFPLUGIN_SOURCES})
target_link_libraries(phononwidgetsplugin KtCore KtGui KtPhonon KtDesigner)
set_target_properties(phononwidgetsplugin PROPERTIES OUTPUT_NAME phononwidgets)

install(
    TARGETS phononwidgetsplugin
    DESTINATION ${QT_PLUGINS_PATH_INST}/designer
)
