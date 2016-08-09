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

if(SQLITE_FOUND)
    include_directories(${SQLITE_INCLUDES})
else()
    set(SQLDRIVER_SOURCES
        ${SQLDRIVER_SOURCES}
        ${CMAKE_SOURCE_DIR}/src/3rdparty/sqlite3/sqlite3.c
    )
    include_directories(${CMAKE_SOURCE_DIR}/src/3rdparty/sqlite3)
endif()

katie_resources(${SQLDRIVER_SOURCES} ${SQLDRIVER_HEADERS})

add_library(qsqliteplugin MODULE ${SQLDRIVER_SOURCES} ${SQLDRIVER_HEADERS})
if(SQLITE_FOUND)
    target_link_libraries(qsqliteplugin KtSql ${SQLITE_LIBRARIES})
else()
    target_link_libraries(qsqliteplugin KtSql)
endif()
set_target_properties(qsqliteplugin PROPERTIES OUTPUT_NAME qsqlite)

install(
    TARGETS qsqliteplugin
    DESTINATION ${QT_PLUGINS_PATH}/sqldrivers
)
