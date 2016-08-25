set(QCOREWLANBEARERPLUGIN_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/corewlan/qcorewlanengine.h
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/qnetworksession_impl.h
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/qbearerengine_impl.h
)

set(QCOREWLANBEARERPLUGIN_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/corewlan/qcorewlanmain.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/corewlan/qcorewlanengine.mm
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/qnetworksession_impl.cpp
)

katie_setup_target(qcorewlanbearerplugin ${QCOREWLANBEARERPLUGIN_SOURCES} ${QCOREWLANBEARERPLUGIN_HEADERS})

add_library(qcorewlanbearerplugin MODULE ${qcorewlanbearerplugin_SOURCES})
set_target_properties(qcorewlanbearerplugin PROPERTIES OUTPUT_NAME qcorewlanbearer)
set_target_properties(qcorewlanbearerplugin PROPERTIES
    LINK_FLAGS "-framework Foundation -framework SystemConfiguration -framework CoreWLAN -framework Security"
)

install(
    TARGETS qcorewlanbearerplugin
    DESTINATION ${QT_PLUGINS_PATH}/bearer
)
