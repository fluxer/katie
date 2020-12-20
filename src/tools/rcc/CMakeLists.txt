# add_definitions()
set(EXTRA_RCC_LIBS
    KtCore KtXml
)

include_directories(
    ${CMAKE_BINARY_DIR}/include
    ${CMAKE_BINARY_DIR}/privateinclude
    ${CMAKE_BINARY_DIR}/include/QtCore
    ${CMAKE_BINARY_DIR}/include/QtXml
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set(RCC_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/rccmain.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/rcc.cpp
)

set(RCC_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/rcc.h
)

katie_setup_target(rcc ${RCC_SOURCES} ${RCC_HEADERS})

add_executable(rcc ${rcc_SOURCES})
target_link_libraries(rcc ${EXTRA_RCC_LIBS})

set_target_properties(rcc PROPERTIES
    EXPORT_NAME rcc
    OUTPUT_NAME rcc${KATIE_TOOLS_SUFFIX}
)

install(
    TARGETS rcc
    EXPORT KatieTargets ${INSTALL_TARGETS_DEFAULT_ARGS}
    RUNTIME DESTINATION ${KATIE_BINARIES_PATH}
    COMPONENT Tools
)

install(
    FILES ${CMAKE_CURRENT_SOURCE_DIR}/rcc.1
    DESTINATION ${KATIE_MAN_PATH}/man1
    RENAME rcc${KATIE_TOOLS_SUFFIX}.1
    COMPONENT Doc
)
