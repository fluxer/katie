set(SQLDRIVER_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/qsql_sqlite.h
)

set(SQLDRIVER_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/sqlitemain.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qsql_sqlite.cpp
)

if(SQLITE_FOUND)
    include_directories(${SQLITE_INCLUDES})
else()
    set(SQLDRIVER_HEADERS
        ${SQLDRIVER_HEADERS}
        ${CMAKE_SOURCE_DIR}/src/3rdparty/sqlite/sqlite3.h
        ${CMAKE_SOURCE_DIR}/src/3rdparty/sqlite/sqlite3ext.h
    )

    set(SQLDRIVER_SOURCES
        ${SQLDRIVER_SOURCES}
        ${CMAKE_SOURCE_DIR}/src/3rdparty/sqlite/sqlite3.c
    )

    include_directories(${CMAKE_SOURCE_DIR}/src/3rdparty/sqlite)
endif()

katie_setup_target(qsqliteplugin ${SQLDRIVER_SOURCES} ${SQLDRIVER_HEADERS})

add_library(qsqliteplugin MODULE ${qsqliteplugin_SOURCES})
target_link_libraries(qsqliteplugin KtSql)
set_target_properties(qsqliteplugin PROPERTIES OUTPUT_NAME qsqlite)

if(SQLITE_FOUND)
    target_link_libraries(qsqliteplugin ${SQLITE_LIBRARIES})
else()
    target_compile_definitions(qsqliteplugin PRIVATE
        -DSQLITE_OMIT_LOAD_EXTENSION
        -DSQLITE_OMIT_COMPLETE
    )
endif()

install(
    TARGETS qsqliteplugin
    DESTINATION ${KATIE_PLUGINS_RELATIVE}/sqldrivers
    COMPONENT Runtime
)
