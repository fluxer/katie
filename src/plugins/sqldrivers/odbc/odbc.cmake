include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}/sqldrivers/odbc
)

set(ODBCDRIVER_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/sqldrivers/odbc/qsql_odbc.h
)

set(ODBCDRIVER_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/sqldrivers/odbc/odbcmain.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sqldrivers/odbc/qsql_odbc.cpp
)

include_directories(${ODBC_INCLUDES})

katie_setup_target(qsqlodbcplugin ${ODBCDRIVER_SOURCES} ${ODBCDRIVER_HEADERS})

add_library(qsqlodbcplugin MODULE ${qsqlodbcplugin_SOURCES})
target_link_libraries(qsqlodbcplugin KtSql ${ODBC_LIBRARIES})
set_target_properties(qsqlodbcplugin PROPERTIES OUTPUT_NAME qsqlodbc)

install(
    TARGETS qsqlodbcplugin
    DESTINATION ${KATIE_PLUGINS_RELATIVE}/sqldrivers
)
