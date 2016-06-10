# add_definitions()
set(EXTRA_GESTURES_LIBS KtDeclarative)

set(GESTURES_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/gestures/qdeclarativegesturearea.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gestures/gestures.cpp
)

set(GESTURES_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/gestures/qdeclarativegesturearea_p.h
)

katie_resources("${GESTURES_SOURCES}")
katie_resources("${GESTURES_HEADERS}")
katie_setup_flags()

add_library(qmlgesturesplugin ${KATIE_TYPE} ${GESTURES_SOURCES} ${GESTURES_HEADERS})
target_link_libraries(qmlgesturesplugin ${EXTRA_GESTURES_LIBS})

install(
    TARGETS qmlgesturesplugin
    DESTINATION ${QT_IMPORTS_PATH}/Qt/labs/gestures
)
install(
    FILES ${CMAKE_CURRENT_SOURCE_DIR}/gestures/qmldir
    DESTINATION ${QT_IMPORTS_PATH}/Qt/labs/gestures
)
