# add_definitions()
set(EXTRA_XML_LIBS KtCore)

set(XML_PUBLIC_HEADERS
    # all headers are aliases
)

include_directories(
    ${CMAKE_BINARY_DIR}/include
    ${CMAKE_BINARY_DIR}/privateinclude
    ${CMAKE_BINARY_DIR}/include/QtCore
    ${CMAKE_BINARY_DIR}/include/QtXml
)

set(XML_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/dom/qdom.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qxmlstream.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qxmlstream_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qxmlutils_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/sax/qxml.h
    ${CMAKE_CURRENT_SOURCE_DIR}/sax/qxml_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qxmlcommon_p.h
)

set(XML_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/dom/qdom.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qxmlstream.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qxmlutils.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sax/qxml.cpp
)

katie_generate_misc("${XML_HEADERS}" QtXml)
katie_generate_public("${XML_PUBLIC_HEADERS}" QtXml)
katie_generate_package(KtXml "KtCore")
katie_setup_target(KtXml ${XML_SOURCES} ${XML_HEADERS})

add_library(KtXml ${KATIE_TYPE} ${KtXml_SOURCES})
target_link_libraries(KtXml PRIVATE ${EXTRA_XML_LIBS})
set_target_properties(KtXml PROPERTIES
    VERSION ${KATIE_MAJOR}.${KATIE_MINOR}
    SOVERSION ${KATIE_VERSION}
    EXPORT_NAME Xml
)

install(
    TARGETS KtXml
    EXPORT KatieTargets ${INSTALL_TARGETS_DEFAULT_ARGS}
    DESTINATION ${KATIE_LIBRARIES_PATH}
    COMPONENT Runtime
)

install(
    DIRECTORY ${CMAKE_BINARY_DIR}/include/QtXml
    DESTINATION ${KATIE_HEADERS_PATH}
    COMPONENT Devel
)

katie_optimize_headers(${KATIE_HEADERS_PATH}/QtXml)
