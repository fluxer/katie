set(GUI_HEADERS
    ${GUI_HEADERS}
    ${CMAKE_CURRENT_SOURCE_DIR}/styles/qstyle.h
    ${CMAKE_CURRENT_SOURCE_DIR}/styles/qstylefactory.h
    ${CMAKE_CURRENT_SOURCE_DIR}/styles/qstyleoption.h
    ${CMAKE_CURRENT_SOURCE_DIR}/styles/qstyleplugin.h
    ${CMAKE_CURRENT_SOURCE_DIR}/styles/qcommonstylepixmaps_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/styles/qcommonstyle.h
    ${CMAKE_CURRENT_SOURCE_DIR}/styles/qstylehelper_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/styles/qproxystyle.h
    ${CMAKE_CURRENT_SOURCE_DIR}/styles/qproxystyle_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/styles/qstylesheetstyle_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/styles/qcdestyle.h
    ${CMAKE_CURRENT_SOURCE_DIR}/styles/qplastiquestyle.h
    ${CMAKE_CURRENT_SOURCE_DIR}/styles/qmotifstyle.h
    ${CMAKE_CURRENT_SOURCE_DIR}/styles/qwindowsstyle.h
)

set(GUI_SOURCES
    ${GUI_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/styles/qstyle.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/styles/qstylefactory.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/styles/qstyleoption.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/styles/qstyleplugin.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/styles/qstylehelper.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/styles/qcommonstyle.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/styles/qproxystyle.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/styles/qstylesheetstyle.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/styles/qstylesheetstyle_default.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/styles/qcdestyle.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/styles/qplastiquestyle.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/styles/qmotifstyle.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/styles/qwindowsstyle.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/styles/qstyle.qrc
)

if(WITH_GTK2 AND GTK2_FOUND)
    set(GUI_HEADERS
        ${GUI_HEADERS}
        ${CMAKE_CURRENT_SOURCE_DIR}/styles/qgtkstyle.h
        ${CMAKE_CURRENT_SOURCE_DIR}/styles/qgtkpainter_p.h
        ${CMAKE_CURRENT_SOURCE_DIR}/styles/qgtkstyle_p.h
        ${CMAKE_CURRENT_SOURCE_DIR}/styles/qcleanlooksstyle.h
        ${CMAKE_CURRENT_SOURCE_DIR}/styles/qcleanlooksstyle_p.h
    )
    set(GUI_SOURCES
        ${GUI_SOURCES}
        ${CMAKE_CURRENT_SOURCE_DIR}/styles/qgtkstyle.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/styles/qgtkpainter.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/styles/qgtkstyle_p.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/styles/qcleanlooksstyle.cpp
    )
    set(EXTRA_GUI_LIBS
        ${EXTRA_GUI_LIBS}
        ${GTK2_LIBRARIES}
    )
    add_definitions(-DQT_STYLE_CLEANLOOKS ${GTK2_DEFINITIONS})
    include_directories(${GTK2_INCLUDE_DIRS})
else()
    add_definitions(-DQT_NO_STYLE_GTK -DQT_NO_STYLE_CLEANLOOKS)
endif()

add_definitions(-DQT_STYLE_MOTIF -DQT_STYLE_WINDOWS)

# TODO: get rid of these conditionals
# add_definitions(-DQT_NO_STYLE_CDE -DQT_NO_STYLE_PLASTIQUE -DQT_NO_STYLE_MOTIF)