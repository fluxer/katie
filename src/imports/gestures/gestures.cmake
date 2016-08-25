# add_definitions()
set(EXTRA_GESTURES_LIBS KtDeclarative)

set(GESTURES_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/gestures/qdeclarativegesturearea.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gestures/gestures.cpp
)

set(GESTURES_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/gestures/qdeclarativegesturearea_p.h
)

katie_setup_target(qmlgesturesplugin ${GESTURES_SOURCES} ${GESTURES_HEADERS})
katie_setup_paths()

add_library(qmlgesturesplugin ${KATIE_TYPE} ${qmlgesturesplugin_SOURCES})
target_link_libraries(qmlgesturesplugin ${EXTRA_GESTURES_LIBS})

install(
    TARGETS qmlgesturesplugin
    DESTINATION ${QT_IMPORTS_PATH}/Qt/labs/gestures
)
install(
    FILES ${CMAKE_CURRENT_SOURCE_DIR}/gestures/qmldir
    DESTINATION ${QT_IMPORTS_PATH}/Qt/labs/gestures
)
