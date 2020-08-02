# add_definitions()
set(EXTRA_SQL_LIBS KtCore)

set(SQL_PUBLIC_HEADERS
    QSqlDatabase
    QSqlDriver
    QSqlDriverPlugin
    QSqlError
    QSqlField
    QSqlIndex
    QSqlQuery
    QSqlQueryModel
    QSqlRecord
    QSqlRelationalDelegate
    QSqlRelationalTableModel
    QSqlResult
    QSqlTableModel
)

include_directories(
    ${CMAKE_BINARY_DIR}/include
    ${CMAKE_BINARY_DIR}/privateinclude
    ${CMAKE_BINARY_DIR}/include/QtCore
    ${CMAKE_BINARY_DIR}/privateinclude/QtCore
    ${CMAKE_BINARY_DIR}/include/QtSql
    ${CMAKE_BINARY_DIR}/privateinclude/QtSql
)

set(SQL_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qsql.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qsqlquery.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qsqldatabase.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qsqlfield.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qsqlrecord.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qsqldriver.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qsqlnulldriver_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qsqldriverplugin.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qsqlerror.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qsqlresult.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qsqlcachedresult_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qsqlindex.h
    ${CMAKE_CURRENT_SOURCE_DIR}/models/qsqlquerymodel.h
    ${CMAKE_CURRENT_SOURCE_DIR}/models/qsqlquerymodel_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/models/qsqltablemodel.h
    ${CMAKE_CURRENT_SOURCE_DIR}/models/qsqltablemodel_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/models/qsqlrelationaldelegate.h
    ${CMAKE_CURRENT_SOURCE_DIR}/models/qsqlrelationaltablemodel.h
    ${CMAKE_CURRENT_SOURCE_DIR}/drivers/qsql_sqlite_p.h
)

set(SQL_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qsqlquery.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qsqldatabase.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qsqlfield.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qsqlrecord.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qsqldriver.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qsqldriverplugin.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qsqlerror.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qsqlresult.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qsqlindex.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qsqlcachedresult.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/models/qsqlquerymodel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/models/qsqltablemodel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/models/qsqlrelationaldelegate.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/models/qsqlrelationaltablemodel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/drivers/qsql_sqlite.cpp
)

if(SQLITE_FOUND)
    set(EXTRA_SQL_LIBS
        ${EXTRA_SQL_LIBS}
        ${SQLITE_LIBRARIES}
    )
    include_directories(${SQLITE_INCLUDES})
else()
    set(SQL_HEADERS
        ${SQL_HEADERS}
        ${CMAKE_SOURCE_DIR}/src/3rdparty/sqlite/sqlite3.h
        ${CMAKE_SOURCE_DIR}/src/3rdparty/sqlite/sqlite3ext.h
    )

    set(SQL_SOURCES
        ${SQL_SOURCES}
        ${CMAKE_SOURCE_DIR}/src/3rdparty/sqlite/sqlite3.c
    )

    add_definitions(
        -DSQLITE_OMIT_LOAD_EXTENSION
        -DSQLITE_OMIT_COMPLETE
    )
    include_directories(${CMAKE_SOURCE_DIR}/src/3rdparty/sqlite)
endif()


katie_generate_misc("${SQL_HEADERS}" QtSql)
katie_generate_public("${SQL_PUBLIC_HEADERS}" QtSql)
katie_generate_package(KtSql "KtCore")
katie_setup_target(KtSql ${SQL_SOURCES} ${SQL_HEADERS})

add_library(KtSql ${KATIE_TYPE} ${KtSql_SOURCES})
target_link_libraries(KtSql PRIVATE ${EXTRA_SQL_LIBS})
set_target_properties(KtSql PROPERTIES
    VERSION ${KATIE_MAJOR}.${KATIE_MINOR}
    SOVERSION ${KATIE_VERSION}
    EXPORT_NAME Sql
)

install(
    TARGETS KtSql
    EXPORT KatieLibraryTargets ${INSTALL_TARGETS_DEFAULT_ARGS}
    DESTINATION ${KATIE_LIBRARIES_PATH}
    COMPONENT Runtime
)

install(
    DIRECTORY ${CMAKE_BINARY_DIR}/include/QtSql
    DESTINATION ${KATIE_HEADERS_PATH}
    COMPONENT Devel
)

katie_optimize_headers(${KATIE_HEADERS_PATH}/QtSql)
