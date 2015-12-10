set(SQL_HEADERS
    ${SQL_HEADERS}
    ${CMAKE_CURRENT_SOURCE_DIR}/models/qsqlquerymodel.h
    ${CMAKE_CURRENT_SOURCE_DIR}/models/qsqlquerymodel_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/models/qsqltablemodel.h
    ${CMAKE_CURRENT_SOURCE_DIR}/models/qsqltablemodel_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/models/qsqlrelationaldelegate.h
    ${CMAKE_CURRENT_SOURCE_DIR}/models/qsqlrelationaltablemodel.h
)

set(SQL_SOURCES
    ${SQL_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/models/qsqlquerymodel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/models/qsqltablemodel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/models/qsqlrelationaldelegate.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/models/qsqlrelationaltablemodel.cpp
)
