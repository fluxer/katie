# add_definitions()
set(EXTRA_QTSCRIPT_LIBS KtCore KtScript)

include_directories(
    ${CMAKE_BINARY_DIR}/include
    ${CMAKE_BINARY_DIR}/privateinclude
    ${CMAKE_BINARY_DIR}/include/QtCore
    ${CMAKE_BINARY_DIR}/privateinclude/QtCore
    ${CMAKE_BINARY_DIR}/include/QtScript
    ${CMAKE_BINARY_DIR}/privateinclude/QtScript
)

set(QSCRIPT_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/bytearrayclass.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/bytearrayprototype.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qscript.cpp
)

set(QSCRIPT_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/bytearrayclass.h
    ${CMAKE_CURRENT_SOURCE_DIR}/bytearrayprototype.h
)

katie_resources(${QSCRIPT_SOURCES} ${QSCRIPT_HEADERS})
katie_setup_flags()

add_executable(qscript ${QSCRIPT_SOURCES} ${QSCRIPT_HEADERS})
target_link_libraries(qscript ${EXTRA_QTSCRIPT_LIBS})

install(
    TARGETS qscript
    EXPORT KatieBinaryTargets ${INSTALL_TARGETS_DEFAULT_ARGS}
    RUNTIME DESTINATION ${QT_BINARIES_PATH}
    COMPONENT Devel
)

if(NOT KATIE_PLATFORM MATCHES "(win32|wince|mac)")
    install(
        FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/qscript.1
        DESTINATION ${MAN_INSTALL_PATH}/man1
    )
endif()
