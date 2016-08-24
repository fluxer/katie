add_definitions(-DQT_BUILD_HELP_LIB -DQT_CLUCENE_SUPPORT)
set(EXTRA_HELP_LIBS KtCore KtGui KtNetwork KtSql KtXml)

include(fulltextsearch/fulltextsearch.cmake)

set(HELP_PUBLIC_HEADERS
    ${HELP_PUBLIC_HEADERS}
    QHelpContentItem
    QHelpContentModel
    QHelpContentWidget
    QHelpEngine
    QHelpEngineCore
    QHelpGlobal
    QHelpIndexModel
    QHelpIndexWidget
    QHelpSearchEngine
    QHelpSearchQuery
    QHelpSearchQueryWidget
    QHelpSearchResultWidget
)

set(HELP_HEADERS
    ${HELP_HEADERS}
    ${CMAKE_CURRENT_SOURCE_DIR}/qhelpenginecore.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qhelpengine.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qhelpengine_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qhelp_global.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qhelpdbreader_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qhelpcontentwidget.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qhelpindexwidget.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qhelpgenerator_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qhelpdatainterface_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qhelpprojectdata_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qhelpcollectionhandler_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qhelpsearchengine.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qhelpsearchquerywidget.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qhelpsearchresultwidget.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qhelpsearchindex_default_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qhelpsearchindexwriter_default_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qhelpsearchindexreader_default_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qhelpsearchindexreader_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qclucenefieldnames_p.h
    # access to clucene
    ${CMAKE_CURRENT_SOURCE_DIR}/qhelpsearchindexwriter_clucene_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qhelpsearchindexreader_clucene_p.h
)

set(HELP_SOURCES
    ${HELP_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/helpsystem.qrc
    ${CMAKE_CURRENT_SOURCE_DIR}/qhelpenginecore.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qhelpengine.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qhelpdbreader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qhelpcontentwidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qhelpindexwidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qhelpgenerator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qhelpdatainterface.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qhelpprojectdata.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qhelpcollectionhandler.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qhelpsearchengine.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qhelpsearchquerywidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qhelpsearchresultwidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qhelpsearchindex_default.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qhelpsearchindexwriter_default.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qhelpsearchindexreader_default.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qhelpsearchindexreader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qclucenefieldnames.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qhelp_global.cpp
    # access to clucene
    ${CMAKE_CURRENT_SOURCE_DIR}/qhelpsearchindexwriter_clucene.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qhelpsearchindexreader_clucene.cpp
)

include_directories(
    ${CMAKE_BINARY_DIR}/include
    ${CMAKE_BINARY_DIR}/privateinclude
    ${CMAKE_BINARY_DIR}/include/QtCore
    ${CMAKE_BINARY_DIR}/privateinclude/QtCore
    ${CMAKE_BINARY_DIR}/include/QtGui
    ${CMAKE_BINARY_DIR}/privateinclude/QtGui
    ${CMAKE_BINARY_DIR}/include/QtNetwork
    ${CMAKE_BINARY_DIR}/privateinclude/QtNetwork
    ${CMAKE_BINARY_DIR}/include/QtSql
    ${CMAKE_BINARY_DIR}/privateinclude/QtSql
    ${CMAKE_BINARY_DIR}/include/QtXml
    ${CMAKE_BINARY_DIR}/privateinclude/QtXml
    ${CMAKE_CURRENT_SOURCE_DIR}
)

katie_generate_misc("${HELP_HEADERS}" QtHelp)
katie_generate_public("${HELP_PUBLIC_HEADERS}" QtHelp)
katie_generate_package(KtHelp "KtCore KtGui KtNetwork KtSql KtXml")
katie_setup_paths()

add_library(KtHelp ${KATIE_TYPE} ${HELP_SOURCES} ${HELP_HEADERS})
target_link_libraries(KtHelp ${EXTRA_HELP_LIBS})
set_target_properties(KtHelp PROPERTIES
    VERSION ${KATIE_MAJOR}.${KATIE_MINOR}
    SOVERSION ${KATIE_VERSION}
    EXPORT_NAME Help
)

katie_setup_target(KtHelp)

install(
    TARGETS KtHelp
    EXPORT KatieLibraryTargets ${INSTALL_TARGETS_DEFAULT_ARGS}
    DESTINATION ${QT_LIBRARIES_PATH}
)

install(
    DIRECTORY ${CMAKE_BINARY_DIR}/include/QtHelp
    DESTINATION ${QT_HEADERS_PATH}
    COMPONENT Devel
)

katie_optimize_headers(${QT_HEADERS_PATH}/QtHelp)
