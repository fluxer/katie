# add_definitions()
set(EXTRA_QTSCRIPT_LIBS KtCore KtScript KtGui KtScriptTools)

include_directories(
    ${CMAKE_BINARY_DIR}/include
    ${CMAKE_BINARY_DIR}/privateinclude
    ${CMAKE_BINARY_DIR}/include/QtCore
    ${CMAKE_BINARY_DIR}/privateinclude/QtCore
    ${CMAKE_BINARY_DIR}/include/QtScript
    ${CMAKE_BINARY_DIR}/privateinclude/QtScript
    ${CMAKE_BINARY_DIR}/include/QtGui
    ${CMAKE_BINARY_DIR}/privateinclude/QtGui
    ${CMAKE_BINARY_DIR}/include/QtScriptTools
    ${CMAKE_BINARY_DIR}/privateinclude/QtScriptTools
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

katie_setup_target(qscript ${QSCRIPT_SOURCES} ${QSCRIPT_HEADERS})

add_executable(qscript ${qscript_SOURCES})
target_link_libraries(qscript ${EXTRA_QTSCRIPT_LIBS})

install(
    TARGETS qscript
    EXPORT KatieBinaryTargets ${INSTALL_TARGETS_DEFAULT_ARGS}
    RUNTIME DESTINATION ${KATIE_BINARIES_RELATIVE}
    COMPONENT Devel
)

install(
    FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/qscript.1
    DESTINATION ${KATIE_MAN_RELATIVE}/man1
)
