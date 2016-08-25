include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}/sqldrivers/psql
)

set(PSQLDRIVER_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/sqldrivers/psql/qsql_psql.h
)

set(PSQLDRIVER_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/sqldrivers/psql/psqlmain.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sqldrivers/psql/qsql_psql.cpp
)

include_directories(${PostgreSQL_INCLUDE_DIRS})

katie_setup_target(qsqlpsqlplugin ${PSQLDRIVER_SOURCES} ${PSQLDRIVER_HEADERS})

add_library(qsqlpsqlplugin MODULE ${qsqlpsqlplugin_SOURCES})
target_link_libraries(qsqlpsqlplugin KtSql ${PostgreSQL_LIBRARIES})
set_target_properties(qsqlpsqlplugin PROPERTIES OUTPUT_NAME qsqlpsql)

install(
    TARGETS qsqlpsqlplugin
    DESTINATION ${QT_PLUGINS_PATH}/sqldrivers
)
