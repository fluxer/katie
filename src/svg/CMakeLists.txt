add_definitions(-DQT_BUILD_SVG_LIB)
set(EXTRA_SVG_LIBS KtCore KtGui)

set(SVG_PUBLIC_HEADERS
    ${SVG_PUBLIC_HEADERS}
    QGraphicsSvgItem
    QSvgGenerator
    QSvgRenderer
    QSvgWidget
)

set(SVG_HEADERS
    ${SVG_HEADERS}
    ${CMAKE_CURRENT_SOURCE_DIR}/qsvggraphics_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qsvghandler_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qsvgnode_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qsvgstructure_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qsvgstyle_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qsvgfont_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qsvgtinydocument_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qsvgrenderer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qsvgwidget.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qgraphicssvgitem.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qsvggenerator.h
)

set(SVG_SOURCES
    ${SVG_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/qsvggraphics.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qsvghandler.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qsvgnode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qsvgstructure.cpp  
    ${CMAKE_CURRENT_SOURCE_DIR}/qsvgstyle.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qsvgfont.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qsvgtinydocument.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qsvgrenderer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qsvgwidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qgraphicssvgitem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qsvggenerator.cpp
)

include_directories(
    ${CMAKE_BINARY_DIR}/include
    ${CMAKE_BINARY_DIR}/privateinclude
    ${CMAKE_BINARY_DIR}/include/QtCore
    ${CMAKE_BINARY_DIR}/privateinclude/QtCore
    ${CMAKE_BINARY_DIR}/include/QtGui
    ${CMAKE_BINARY_DIR}/privateinclude/QtGui
    ${CMAKE_BINARY_DIR}/include/QtSvg
    ${CMAKE_BINARY_DIR}/privateinclude/QtSvg
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src/core/tools
    ${CMAKE_SOURCE_DIR}/src/gui/painting
    ${CMAKE_SOURCE_DIR}/src/3rdparty/harfbuzz/src
)

if(WITH_ZLIB AND ZLIB_FOUND)
    set(EXTRA_SVG_LIBS
        ${EXTRA_SVG_LIBS}
        ${ZLIB_LIBRARIES}
    )
    include_directories(${ZLIB_INCLUDE_DIRS})
else()
    set(SVG_SOURCES
        ${SVG_SOURCES}
        ${CMAKE_SOURCE_DIR}/src/3rdparty/miniz/miniz.c
    )
    include_directories(${CMAKE_SOURCE_DIR}/src/3rdparty/miniz)
endif()

katie_generate_misc("${SVG_HEADERS}" QtSvg)
katie_generate_public("${SVG_PUBLIC_HEADERS}" QtSvg)
katie_generate_package(KtSvg "KtCore KtGui")
katie_setup_paths()

add_library(KtSvg ${KATIE_TYPE} ${SVG_SOURCES} ${SVG_HEADERS})
target_link_libraries(KtSvg ${EXTRA_SVG_LIBS})
set_target_properties(KtSvg PROPERTIES
    VERSION ${KATIE_MAJOR}.${KATIE_MINOR}
    SOVERSION ${KATIE_VERSION}
    EXPORT_NAME Svg
)

katie_setup_target(KtSvg)

install(
    TARGETS KtSvg
    EXPORT KatieLibraryTargets ${INSTALL_TARGETS_DEFAULT_ARGS}
    DESTINATION ${QT_LIBRARIES_PATH}
)

install(
    DIRECTORY ${CMAKE_BINARY_DIR}/include/QtSvg
    DESTINATION ${QT_HEADERS_PATH}
    COMPONENT Devel
)

katie_optimize_headers(${QT_HEADERS_PATH}/QtSvg)
