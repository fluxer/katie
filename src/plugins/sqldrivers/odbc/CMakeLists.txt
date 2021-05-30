set(ODBCDRIVER_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/qsql_odbc.h
)

set(ODBCDRIVER_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/odbcmain.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qsql_odbc.cpp
)

include_directories(${ODBC_INCLUDES})

katie_setup_target(qsqlodbcplugin ${ODBCDRIVER_SOURCES} ${ODBCDRIVER_HEADERS})

add_library(qsqlodbcplugin MODULE ${qsqlodbcplugin_SOURCES})
target_link_libraries(qsqlodbcplugin KtSql ${ODBC_LIBRARIES})
set_target_properties(qsqlodbcplugin PROPERTIES OUTPUT_NAME qsqlodbc)
target_compile_definitions(qsqlodbcplugin PRIVATE -DUNICODE)

katie_setup_plugin(qsqlodbcplugin)

install(
    TARGETS qsqlodbcplugin
    DESTINATION ${KATIE_PLUGINS_PATH}/sqldrivers
    COMPONENT Runtime
)
