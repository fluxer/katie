set(EXTRA_TRC_LIBS
    KtCore
    ${GETTEXTPO_LIBRARIES}
)

include_directories(
    ${CMAKE_BINARY_DIR}/include
    ${CMAKE_BINARY_DIR}/privateinclude
    ${CMAKE_BINARY_DIR}/include/QtCore
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${GETTEXTPO_INCLUDES}
)

set(TRC_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/trcmain.cpp
)

katie_setup_target(trc ${TRC_SOURCES})

add_executable(trc ${trc_SOURCES})
target_link_libraries(trc ${EXTRA_TRC_LIBS})

set_target_properties(trc PROPERTIES
    EXPORT_NAME trc
    OUTPUT_NAME trc${KATIE_TOOLS_SUFFIX}
)

install(
    TARGETS trc
    EXPORT KatieTargets
    RUNTIME DESTINATION ${KATIE_BINARIES_PATH}
    COMPONENT Tools
)

install(
    FILES ${CMAKE_CURRENT_SOURCE_DIR}/trc.1
    DESTINATION ${KATIE_MAN_PATH}/man1
    RENAME trc${KATIE_TOOLS_SUFFIX}.1
    COMPONENT Doc
)
