set(SQLDRIVER_HEADERS
    ${SQLDRIVER_HEADERS}
    ${CMAKE_CURRENT_SOURCE_DIR}/drivers/sqlite/qsql_sqlite.h
)

set(SQLDRIVER_SOURCES
    ${SQLDRIVER_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/drivers/sqlite/sqlitemain.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/drivers/sqlite/qsql_sqlite.cpp
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

katie_resources("${SQLDRIVER_SOURCES}")
katie_resources("${SQLDRIVER_HEADERS}")

add_library(qsqlite ${KATIE_TYPE} ${SQLDRIVER_SOURCES} ${SQLDRIVER_HEADERS})
if(SQLITE_FOUND)
    target_link_libraries(qsqlite KtSql ${SQLITE_LIBRARIES})
else()
    target_link_libraries(qsqlite KtSql)
endif()

install(
    TARGETS qsqlite
    DESTINATION ${QT_PLUGINS_PATH_INST}/sqldrivers
)
