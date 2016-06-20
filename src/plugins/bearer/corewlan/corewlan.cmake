set(QCOREWLANBEARERPLUGIN_HEADERS
    ${QCOREWLANBEARERPLUGIN_HEADERS}
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/corewlan/qcorewlanengine.h
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/qnetworksession_impl.h
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/qbearerengine_impl.h
)

set(QCOREWLANBEARERPLUGIN_SOURCES
    ${QCOREWLANBEARERPLUGIN_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/corewlan/qcorewlanmain.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/corewlan/qcorewlanengine.mm
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/qnetworksession_impl.cpp
)

katie_resources(${QCOREWLANBEARERPLUGIN_HEADERS} ${QCOREWLANBEARERPLUGIN_SOURCES})

add_library(qcorewlanbearerplugin MODULE ${QCOREWLANBEARERPLUGIN_SOURCES} ${QCOREWLANBEARERPLUGIN_HEADERS})
set_target_properties(qcorewlanbearerplugin PROPERTIES OUTPUT_NAME qcorewlanbearer)
set_target_properties(qcorewlanbearerplugin PROPERTIES
    LINK_FLAGS "-framework Foundation -framework SystemConfiguration -framework CoreWLAN -framework Security"
)

install(
    TARGETS qcorewlanbearerplugin
    DESTINATION ${QT_PLUGINS_PATH}/bearer
)
