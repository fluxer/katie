# add_definitions()
set(EXTRA_GESTURES_LIBS
    KtCore KtDeclarative
)

set(GESTURES_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/qdeclarativegesturearea.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gestures.cpp
)

set(GESTURES_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/qdeclarativegesturearea_p.h
)

katie_setup_target(qmlgesturesplugin ${GESTURES_SOURCES} ${GESTURES_HEADERS})

add_library(qmlgesturesplugin MODULE ${qmlgesturesplugin_SOURCES})
target_link_libraries(qmlgesturesplugin ${EXTRA_GESTURES_LIBS})

install(
    TARGETS qmlgesturesplugin
    DESTINATION ${KATIE_IMPORTS_PATH}/Qt/labs/gestures
    COMPONENT Runtime
)
install(
    FILES ${CMAKE_CURRENT_SOURCE_DIR}/qmldir
    DESTINATION ${KATIE_IMPORTS_PATH}/Qt/labs/gestures
    COMPONENT Runtime
)
