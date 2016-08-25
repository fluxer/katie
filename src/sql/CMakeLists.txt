add_definitions(-DQT_BUILD_SQL_LIB)
set(EXTRA_SQL_LIBS KtCore)

set(SQL_PUBLIC_HEADERS
    ${SQL_PUBLIC_HEADERS}
    QDB2Driver
    QDB2Result
    QIBaseDriver
    QIBaseResult
    QMYSQLDriver
    QMYSQLResult
    QOCIDriver
    QOCIResult
    QODBCDriver
    QODBCResult
    QPSQLDriver
    QPSQLResult
    QSqlDatabase
    QSqlDriver
    QSqlDriverCreator
    QSqlDriverCreatorBase
    QSqlDriverFactoryInterface
    QSqlDriverPlugin
    QSqlError
    QSqlField
    QSqlIndex
    QSQLiteDriver
    QSQLiteResult
    QSqlQuery
    QSqlQueryModel
    QSqlRecord
    QSqlRelation
    QSqlRelationalDelegate
    QSqlRelationalTableModel
    QSqlResult
    QSqlTableModel
    QTDSDriver
    QTDSResult
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
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel
    ${CMAKE_CURRENT_SOURCE_DIR}/drivers
    ${CMAKE_CURRENT_SOURCE_DIR}/models
    ${CMAKE_CURRENT_BINARY_DIR}/kernel
    ${CMAKE_CURRENT_BINARY_DIR}/drivers
    ${CMAKE_CURRENT_BINARY_DIR}/models
    ${CMAKE_CURRENT_BINARY_DIR}/drivers/ibase
    ${CMAKE_CURRENT_BINARY_DIR}/drivers/mysql
    ${CMAKE_CURRENT_BINARY_DIR}/drivers/odbc
    ${CMAKE_CURRENT_BINARY_DIR}/drivers/psql
    ${CMAKE_CURRENT_BINARY_DIR}/drivers/sqlite
)

katie_generate_misc("${SQL_HEADERS}" QtSql)
katie_generate_public("${SQL_PUBLIC_HEADERS}" QtSql)
katie_setup_paths()
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
    DESTINATION ${QT_LIBRARIES_PATH}
)

install(
    DIRECTORY ${CMAKE_BINARY_DIR}/include/QtSql
    DESTINATION ${QT_HEADERS_PATH}
    COMPONENT Devel
)

katie_optimize_headers(${QT_HEADERS_PATH}/QtSql)
