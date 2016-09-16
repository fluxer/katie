include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}/sqldrivers/sqlite
)

set(SQLDRIVER_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/sqldrivers/sqlite/qsql_sqlite.h
)

set(SQLDRIVER_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/sqldrivers/sqlite/sqlitemain.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sqldrivers/sqlite/qsql_sqlite.cpp
)

include_directories(${SQLITE_INCLUDES})

katie_setup_target(qsqliteplugin ${SQLDRIVER_SOURCES} ${SQLDRIVER_HEADERS})

add_library(qsqliteplugin MODULE ${qsqliteplugin_SOURCES})
target_link_libraries(qsqliteplugin KtSql ${SQLITE_LIBRARIES})
set_target_properties(qsqliteplugin PROPERTIES OUTPUT_NAME qsqlite)

install(
    TARGETS qsqliteplugin
    DESTINATION ${QT_PLUGINS_PATH}/sqldrivers
)
