set(SQLDRIVER_HEADERS
    ${SQLDRIVER_HEADERS}
    ${CMAKE_CURRENT_SOURCE_DIR}/drivers/sqlite/qsql_sqlite.h
)

set(SQLDRIVER_SOURCES
    ${SQLDRIVER_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/drivers/sqlite/sqlitemain.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/drivers/sqlite/qsql_sqlite.cpp
)

include_directories(${SQLITE_INCLUDE_DIRS})

katie_resources("${SQLDRIVER_SOURCES}")
katie_resources("${SQLDRIVER_HEADERS}")

add_library(qsqlite ${KATIE_TYPE} ${SQLDRIVER_SOURCES} ${SQLDRIVER_HEADERS})
target_link_libraries(qsqlite KtSql ${SQLITE_LIBRARIES})

install(
    TARGETS qsqlite
    DESTINATION ${QT_PLUGINS_PATH_INST}/sqldrivers
)
