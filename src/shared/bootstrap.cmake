set(BOOTSTRAP_DEFINITIONS
    ${BOOTSTRAP_DEFINITIONS}
    -DQT_BOOTSTRAPPED
    -DQT_LITE_UNICODE
    -DQT_TEXTCODEC
    # -DQT_NO_CAST_FROM_ASCII
    # -DQT_NO_CAST_TO_ASCII
    -DQT_NO_CODECS
    -DQT_NO_DATASTREAM
    -DQT_NO_GEOM_VARIANT
    -DQT_NO_LIBRARY
    -DQT_NO_QOBJECT
    -DQT_NO_STL
    -DQT_NO_SYSTEMLOCALE
    -DQT_NO_TEXTSTREAM
    -DQT_NO_THREAD
    -DQT_NO_UNICODETABLES
    -DQT_NO_USING_NAMESPACE
    -DQT_NO_DEPRECATED
    -DQT_NO_COMPRESS
    -DQMAKE_OPENSOURCE_EDITION
)

set(BOOTSTRAP_INCLUDES
    ${BOOTSTRAP_INCLUDES}
    ${CMAKE_BINARY_DIR}/include
    ${CMAKE_BINARY_DIR}/privateinclude
    ${CMAKE_BINARY_DIR}/include/QtCore
    ${CMAKE_BINARY_DIR}/privateinclude/QtCore
    ${CMAKE_BINARY_DIR}/include/QtXml
    ${CMAKE_BINARY_DIR}/privateinclude/QtXml
    ${CMAKE_SOURCE_DIR}/src/xml
    ${CMAKE_SOURCE_DIR}/src/xml/dom
    ${CMAKE_SOURCE_DIR}/src/xml/sax
    ${CMAKE_SOURCE_DIR}/src/core/arch
    ${CMAKE_SOURCE_DIR}/src/core/global
    ${CMAKE_SOURCE_DIR}/src/core/kernel
    ${CMAKE_SOURCE_DIR}/src/core/tools
    ${CMAKE_SOURCE_DIR}/src/core/io
    ${CMAKE_SOURCE_DIR}/src/core/codecs
)

set(BOOTSTRAP_SOURCES
    ${BOOTSTRAP_SOURCES}
    ${CMAKE_SOURCE_DIR}/src/core/codecs/qisciicodec.cpp
    ${CMAKE_SOURCE_DIR}/src/core/codecs/qlatincodec.cpp
    ${CMAKE_SOURCE_DIR}/src/core/codecs/qsimplecodec.cpp
    ${CMAKE_SOURCE_DIR}/src/core/codecs/qtextcodec.cpp
    ${CMAKE_SOURCE_DIR}/src/core/codecs/qtsciicodec.cpp
    ${CMAKE_SOURCE_DIR}/src/core/codecs/qutfcodec.cpp
    ${CMAKE_SOURCE_DIR}/src/core/global/qglobal.cpp
    ${CMAKE_SOURCE_DIR}/src/core/global/qmalloc.cpp
    ${CMAKE_SOURCE_DIR}/src/core/global/qnumeric.cpp
    ${CMAKE_SOURCE_DIR}/src/core/io/qabstractfileengine.cpp
    ${CMAKE_SOURCE_DIR}/src/core/io/qbuffer.cpp
    ${CMAKE_SOURCE_DIR}/src/core/io/qdatastream.cpp
    ${CMAKE_SOURCE_DIR}/src/core/io/qdir.cpp
    ${CMAKE_SOURCE_DIR}/src/core/io/qdiriterator.cpp
    ${CMAKE_SOURCE_DIR}/src/core/io/qfile.cpp
    ${CMAKE_SOURCE_DIR}/src/core/io/qfileinfo.cpp
    ${CMAKE_SOURCE_DIR}/src/core/io/qfilesystementry.cpp
    ${CMAKE_SOURCE_DIR}/src/core/io/qfilesystemengine.cpp
    ${CMAKE_SOURCE_DIR}/src/core/io/qfsfileengine.cpp
    ${CMAKE_SOURCE_DIR}/src/core/io/qfsfileengine_iterator.cpp
    ${CMAKE_SOURCE_DIR}/src/core/io/qiodevice.cpp
    ${CMAKE_SOURCE_DIR}/src/core/io/qtemporaryfile.cpp
    ${CMAKE_SOURCE_DIR}/src/core/io/qtextstream.cpp
    ${CMAKE_SOURCE_DIR}/src/core/kernel/qmetatype.cpp
    ${CMAKE_SOURCE_DIR}/src/core/kernel/qvariant.cpp
    ${CMAKE_SOURCE_DIR}/src/core/kernel/qsystemerror.cpp
    ${CMAKE_SOURCE_DIR}/src/core/tools/qbitarray.cpp
    ${CMAKE_SOURCE_DIR}/src/core/tools/qbytearray.cpp
    ${CMAKE_SOURCE_DIR}/src/core/tools/qbytearraymatcher.cpp
    ${CMAKE_SOURCE_DIR}/src/core/tools/qdatetime.cpp
    ${CMAKE_SOURCE_DIR}/src/core/tools/qhash.cpp
    ${CMAKE_SOURCE_DIR}/src/core/tools/qlist.cpp
    ${CMAKE_SOURCE_DIR}/src/core/tools/qlocale.cpp
    ${CMAKE_SOURCE_DIR}/src/core/tools/qlocale_tools.cpp
    ${CMAKE_SOURCE_DIR}/src/core/tools/qmap.cpp
    ${CMAKE_SOURCE_DIR}/src/core/tools/qregexp.cpp
    ${CMAKE_SOURCE_DIR}/src/core/tools/qstring.cpp
    ${CMAKE_SOURCE_DIR}/src/core/tools/qstringlist.cpp
    ${CMAKE_SOURCE_DIR}/src/core/tools/qvector.cpp
    ${CMAKE_SOURCE_DIR}/src/core/tools/qvsnprintf.cpp
    ${CMAKE_SOURCE_DIR}/src/core/xml/qxmlutils.cpp
    ${CMAKE_SOURCE_DIR}/src/core/xml/qxmlstream.cpp
    ${CMAKE_SOURCE_DIR}/src/xml/dom/qdom.cpp
    ${CMAKE_SOURCE_DIR}/src/xml/sax/qxml.cpp
)

if(UNIX)
    set(BOOTSTRAP_SOURCES
        ${BOOTSTRAP_SOURCES}
        ${CMAKE_SOURCE_DIR}/src/core/io/qfilesystemengine_unix.cpp
        ${CMAKE_SOURCE_DIR}/src/core/io/qfilesystemiterator_unix.cpp
        ${CMAKE_SOURCE_DIR}/src/core/io/qfsfileengine_unix.cpp
        ${CMAKE_SOURCE_DIR}/src/core/tools/qlocale_unix.cpp
    )
elseif(KATIE_PLATFORM STREQUAL "win32")
    set(BOOTSTRAP_SOURCES
        ${BOOTSTRAP_SOURCES}
        ${CMAKE_SOURCE_DIR}/src/core/io/qfilesystemengine_win.cpp
        ${CMAKE_SOURCE_DIR}/src/core/io/qfilesystemiterator_win.cpp
        ${CMAKE_SOURCE_DIR}/src/core/io/qfsfileengine_win.cpp
        ${CMAKE_SOURCE_DIR}/src/core/plugin/qsystemlibrary.cpp
        ${CMAKE_SOURCE_DIR}/src/core/tools/qlocale_win.cpp
    )
elseif(KATIE_PLATFORM STREQUAL "mac")
    set(BOOTSTRAP_SOURCES
        ${BOOTSTRAP_SOURCES}
        ${CMAKE_SOURCE_DIR}/src/core/tools/qlocale_mac.mm
    )
endif()

add_library(bootstrap OBJECT ${BOOTSTRAP_SOURCES})
target_compile_definitions(bootstrap PRIVATE ${BOOTSTRAP_DEFINITIONS})
target_include_directories(bootstrap PRIVATE ${BOOTSTRAP_INCLUDES})
