# TODO: check for gds library

set(IBASEDRIVER_HEADERS
    ${IBASEDRIVER_HEADERS}
    ${CMAKE_CURRENT_SOURCE_DIR}/drivers/ibase/qsql_ibase.h
)

set(IBASEDRIVER_SOURCES
    ${IBASEDRIVER_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/drivers/ibase/ibasemain.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/drivers/ibase/qsql_ibase.cpp
)

if(UNIX)
    set(EXTRA_IBASEDRIVER_LIBS
        ${EXTRA_IBASEDRIVER_LIBS}
        gds
    )
else()
    set(EXTRA_IBASEDRIVER_LIBS
        ${EXTRA_IBASEDRIVER_LIBS}
        gds32_ms
    )
endif()

set(EXTRA_SQL_LIBS
    ${EXTRA_SQL_LIBS}
    ${EXTRA_IBASEDRIVERS_LIBS}
)

katie_resources("${IBASEDRIVER_SOURCES}")
katie_resources("${IBASEDRIVER_HEADERS}")

add_library(qsqlibase ${KATIE_TYPE} ${IBASEDRIVER_SOURCES} ${IBASEDRIVER_HEADERS})
target_link_libraries(qsqlibase KtSql ${EXTRA_IBASEDRIVER_LIBS})

install(
    TARGETS qsqlibase
    DESTINATION ${QT_PLUGINS_PATH_INST}/sqldrivers
)
