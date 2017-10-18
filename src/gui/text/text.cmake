set(GUI_HEADERS
    ${GUI_HEADERS}
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qfont.h
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qfontdatabase.h
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qfontengine_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qfontengineglyphcache_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qfontinfo.h
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qfontmetrics.h
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qfont_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qfontsubset_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qtextcontrol_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qtextcontrol_p_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qtextengine_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qtextlayout.h
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qtextformat.h
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qtextformat_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qtextobject.h
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qtextobject_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qtextoption.h
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qfragmentmap_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qtextdocument.h
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qtextdocument_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qtexthtmlparser_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qabstracttextdocumentlayout.h
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qtextdocumentlayout_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qtextcursor.h
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qtextcursor_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qtextdocumentfragment.h
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qtextdocumentfragment_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qtextimagehandler_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qtexttable.h
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qtextlist.h
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qsyntaxhighlighter.h
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qtextdocumentwriter.h
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qcssparser_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qtexttable_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qzipreader_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qzipwriter_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qtextodfwriter_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qstatictext_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qstatictext.h
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qharfbuzz_p.h
)

set(GUI_SOURCES
    ${GUI_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qfont.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qfontengine.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qfontsubset.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qfontmetrics.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qfontdatabase.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qtextcontrol.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qtextengine.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qtextlayout.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qtextformat.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qtextobject.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qtextoption.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qfragmentmap.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qtextdocument.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qtextdocument_p.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qtexthtmlparser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qabstracttextdocumentlayout.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qtextdocumentlayout.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qtextcursor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qtextdocumentfragment.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qtextimagehandler.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qtexttable.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qtextlist.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qtextdocumentwriter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qsyntaxhighlighter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qcssparser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qzip.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qtextodfwriter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qstatictext.cpp
)

if(WITH_FONTCONFIG AND FONTCONFIG_FOUND)
    set(GUI_HEADERS
        ${GUI_HEADERS}
        ${CMAKE_CURRENT_SOURCE_DIR}/text/qfontengine_ft_p.h
    )
    set(GUI_SOURCES
        ${GUI_SOURCES}
        ${CMAKE_CURRENT_SOURCE_DIR}/text/qfontengine_ft.cpp
    )

    set(EXTRA_GUI_LIBS
        ${EXTRA_GUI_LIBS}
        ${FONTCONFIG_LIBRARIES}
    )
    include_directories(${FONTCONFIG_INCLUDES})
    add_definitions(${FONTCONFIG_DEFINITIONS})
endif()

if(WITH_X11 AND X11_FOUND)
    set(GUI_HEADERS
        ${GUI_HEADERS}
        ${CMAKE_CURRENT_SOURCE_DIR}/text/qfontengine_x11_p.h
        ${CMAKE_CURRENT_SOURCE_DIR}/text/qfontdatabase_x11.h
    )
    set(GUI_SOURCES
        ${GUI_SOURCES}
        ${CMAKE_CURRENT_SOURCE_DIR}/text/qfont_x11.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/text/qfontengine_x11.cpp
    )
endif()

if(WITH_HARFBUZZ AND HARFBUZZ_FOUND)
    set(EXTRA_GUI_LIBS
        ${EXTRA_GUI_LIBS}
        ${HARFBUZZ_LIBRARIES}
    )
    include_directories(${HARFBUZZ_INCLUDE_DIRS})
else()
    # TODO: move to main CMakeLists?
    add_definitions(-DHB_EXPORT=Q_CORE_EXPORT -DQT_NO_OPENTYPE)
    set(GUI_SOURCES
        ${GUI_SOURCES}
        ${CMAKE_SOURCE_DIR}/src/3rdparty/harfbuzz/src/harfbuzz-buffer.c
        ${CMAKE_SOURCE_DIR}/src/3rdparty/harfbuzz/src/harfbuzz-gdef.c
        ${CMAKE_SOURCE_DIR}/src/3rdparty/harfbuzz/src/harfbuzz-gsub.c
        ${CMAKE_SOURCE_DIR}/src/3rdparty/harfbuzz/src/harfbuzz-gpos.c
        ${CMAKE_SOURCE_DIR}/src/3rdparty/harfbuzz/src/harfbuzz-impl.c
        ${CMAKE_SOURCE_DIR}/src/3rdparty/harfbuzz/src/harfbuzz-open.c
        ${CMAKE_SOURCE_DIR}/src/3rdparty/harfbuzz/src/harfbuzz-stream.c
        ${CMAKE_SOURCE_DIR}/src/3rdparty/harfbuzz/src/harfbuzz-shaper-all.cpp
        ${CMAKE_SOURCE_DIR}/src/gui/text/qharfbuzz.cpp
    )
    include_directories(
        ${CMAKE_SOURCE_DIR}/src/3rdparty/harfbuzz/src
    )
endif()
