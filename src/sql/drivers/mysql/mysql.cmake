set(MYSQLCDRIVER_HEADERS
    ${MYSQLCDRIVER_HEADERS}
    ${CMAKE_CURRENT_SOURCE_DIR}/drivers/mysql/qsql_mysql.h
)

set(MYSQLCDRIVER_SOURCES
    ${MYSQLCDRIVER_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/drivers/mysql/mysqlmain.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/drivers/mysql/qsql_mysql.cpp
)

include_directories(${MYSQL_INCLUDES})

katie_resources("${MYSQLCDRIVER_SOURCES}")
katie_resources("${MYSQLCDRIVER_HEADERS}")

add_library(qsqlmysql ${KATIE_TYPE} ${MYSQLCDRIVER_SOURCES} ${MYSQLCDRIVER_HEADERS})
target_link_libraries(qsqlmysql KtSql ${MYSQL_LIBRARIES})

install(
    TARGETS qsqlmysql
    DESTINATION ${QT_PLUGINS_PATH_INST}/sqldrivers
)
