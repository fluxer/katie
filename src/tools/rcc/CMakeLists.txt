add_definitions(-DQT_RCC)
set(EXTRA_RCC_LIBS KtCore KtCore KtXml)

include_directories(
    ${CMAKE_BINARY_DIR}/include
    ${CMAKE_BINARY_DIR}/privateinclude
    ${CMAKE_BINARY_DIR}/include/QtCore
    ${CMAKE_BINARY_DIR}/privateinclude/QtCore
    ${CMAKE_BINARY_DIR}/include/QtXml
    ${CMAKE_BINARY_DIR}/privateinclude/QtXml
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set(RCC_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/rccmain.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/rcc.cpp
)

set(RCC_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/rcc.h
    ${CMAKE_SOURCE_DIR}/src/core/kernel/qcorecmdlineargs_p.h
)

katie_setup_flags()

add_executable(rcc ${RCC_SOURCES} ${RCC_HEADERS})
target_link_libraries(rcc ${EXTRA_RCC_LIBS})
set_target_properties(rcc PROPERTIES
    EXPORT_NAME rcc
)

install(
    TARGETS rcc
    EXPORT KatieBinaryTargets ${INSTALL_TARGETS_DEFAULT_ARGS}
    RUNTIME DESTINATION ${QT_BINARIES_PATH_INST}
    COMPONENT Devel
)
