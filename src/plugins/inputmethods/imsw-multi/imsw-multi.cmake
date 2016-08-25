include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}/inputmethods/imsw-multi
)

set(QIMSWMULTIPLUGIN_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/inputmethods/imsw-multi/qmultiinputcontext.h
    ${CMAKE_CURRENT_SOURCE_DIR}/inputmethods/imsw-multi/qmultiinputcontextplugin.h
)

set(QIMSWMULTIPLUGIN_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/inputmethods/imsw-multi/qmultiinputcontext.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/inputmethods/imsw-multi/qmultiinputcontextplugin.cpp
)

katie_setup_target(qimsw-multiplugin ${QIMSWMULTIPLUGIN_SOURCES} ${QIMSWMULTIPLUGIN_HEADERS})

add_library(qimsw-multiplugin MODULE ${qimsw-multiplugin_SOURCES})
target_link_libraries(qimsw-multiplugin KtCore KtGui)
set_target_properties(qimsw-multiplugin PROPERTIES OUTPUT_NAME qimsw-multi)

install(
    TARGETS qimsw-multiplugin
    DESTINATION ${QT_PLUGINS_PATH}/inputmethods
)
