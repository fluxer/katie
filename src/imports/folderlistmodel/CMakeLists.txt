# add_definitions()
set(EXTRA_FOLDERLISTMODEL_LIBS KtDeclarative)

set(FOLDERLISTMODEL_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/qdeclarativefolderlistmodel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/folderlistmodel.cpp
)

set(FOLDERLISTMODEL_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/qdeclarativefolderlistmodel.h
)

katie_setup_target(qmlfolderlistmodelplugin ${FOLDERLISTMODEL_SOURCES} ${FOLDERLISTMODEL_HEADERS})

add_library(qmlfolderlistmodelplugin ${KATIE_TYPE} ${qmlfolderlistmodelplugin_SOURCES})
target_link_libraries(qmlfolderlistmodelplugin ${EXTRA_FOLDERLISTMODEL_LIBS})

install(
    TARGETS qmlfolderlistmodelplugin
    DESTINATION ${KATIE_IMPORTS_RELATIVE}/Qt/labs/folderlistmodel
)
install(
    FILES ${CMAKE_CURRENT_SOURCE_DIR}/qmldir
    DESTINATION ${KATIE_IMPORTS_RELATIVE}/Qt/labs/folderlistmodel
)
