set(ODBCDRIVER_HEADERS
    ${ODBCDRIVER_HEADERS}
    ${CMAKE_CURRENT_SOURCE_DIR}/drivers/odbc/qsql_odbc.h
)

set(ODBCDRIVER_SOURCES
    ${ODBCDRIVER_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/drivers/odbc/odbcmain.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/drivers/odbc/qsql_odbc.cpp
)

if(UNIX)
    add_definitions(-DUNICODE)
endif()

include_directories(${ODBC_INCLUDE_DIRECTORIES})

katie_resources("${ODBCDRIVER_SOURCES}")
katie_resources("${ODBCDRIVER_HEADERS}")

add_library(qsqlodbc ${KATIE_TYPE} ${ODBCDRIVER_SOURCES} ${ODBCDRIVER_HEADERS})
target_link_libraries(qsqlodbc KtSql ${ODBC_LIBRARIES})

install(
    TARGETS qsqlodbc
    DESTINATION ${QT_PLUGINS_PATH_INST}/sqldrivers
)
