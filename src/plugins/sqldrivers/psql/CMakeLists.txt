set(PSQLDRIVER_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/qsql_psql.h
)

set(PSQLDRIVER_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/psqlmain.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qsql_psql.cpp
)

include_directories(${POSTGRESQL_INCLUDES})

katie_setup_target(qsqlpsqlplugin ${PSQLDRIVER_SOURCES} ${PSQLDRIVER_HEADERS})

add_library(qsqlpsqlplugin MODULE ${qsqlpsqlplugin_SOURCES})
target_link_libraries(qsqlpsqlplugin KtSql ${POSTGRESQL_LIBRARIES})
set_target_properties(qsqlpsqlplugin PROPERTIES OUTPUT_NAME qsqlpsql)

katie_setup_plugin(qsqlpsqlplugin)

install(
    TARGETS qsqlpsqlplugin
    DESTINATION ${KATIE_PLUGINS_PATH}/sqldrivers
    COMPONENT Runtime
)
