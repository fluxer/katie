# add_definitions()
set(EXTRA_SQL_LIBS KtCore)

set(SQL_PUBLIC_HEADERS
    ${SQL_PUBLIC_HEADERS}
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

include(kernel/kernel.cmake)
include(models/models.cmake)

include_directories(
    ${CMAKE_BINARY_DIR}/include
    ${CMAKE_BINARY_DIR}/privateinclude
    ${CMAKE_BINARY_DIR}/include/QtCore
    ${CMAKE_BINARY_DIR}/privateinclude/QtCore
    ${CMAKE_BINARY_DIR}/include/QtSql
    ${CMAKE_BINARY_DIR}/privateinclude/QtSql
)

katie_generate_misc("${SQL_HEADERS}" QtSql)
katie_generate_public("${SQL_PUBLIC_HEADERS}" QtSql)
katie_generate_package(KtSql "KtCore")
katie_setup_target(KtSql ${SQL_SOURCES} ${SQL_HEADERS})

add_library(KtSql ${KATIE_TYPE} ${KtSql_SOURCES})
target_link_libraries(KtSql ${EXTRA_SQL_LIBS})
set_target_properties(KtSql PROPERTIES
    VERSION ${KATIE_MAJOR}.${KATIE_MINOR}
    SOVERSION ${KATIE_VERSION}
    EXPORT_NAME Sql
)

install(
    TARGETS KtSql
    EXPORT KatieLibraryTargets ${INSTALL_TARGETS_DEFAULT_ARGS}
    DESTINATION ${KATIE_LIBRARIES_RELATIVE}
    COMPONENT Runtime
)

install(
    DIRECTORY ${CMAKE_BINARY_DIR}/include/QtSql
    DESTINATION ${KATIE_HEADERS_RELATIVE}
    COMPONENT Devel
)

katie_optimize_headers(${KATIE_HEADERS_FULL}/QtSql)
