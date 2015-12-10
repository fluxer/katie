set(PSQLDRIVER_HEADERS
    ${PSQLDRIVER_HEADERS}
    ${CMAKE_CURRENT_SOURCE_DIR}/drivers/psql/qsql_psql.h
)

set(PSQLDRIVER_SOURCES
    ${PSQLDRIVER_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/drivers/psql/psqlmain.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/drivers/psql/qsql_psql.cpp
)

include_directories(${PostgreSQL_INCLUDE_DIRS})

katie_resources("${PSQLDRIVER_SOURCES}")
katie_resources("${PSQLDRIVER_HEADERS}")

add_library(qsqlpsql ${KATIE_TYPE} ${PSQLDRIVER_SOURCES} ${PSQLDRIVER_HEADERS})
target_link_libraries(qsqlpsql KtSql ${PostgreSQL_LIBRARIES})

install(
    TARGETS qsqlpsql
    DESTINATION ${QT_PLUGINS_PATH_INST}/sqldrivers
)
