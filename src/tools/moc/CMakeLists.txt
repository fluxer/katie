add_definitions(-DQT_MOC)
set(EXTRA_MOC_LIBS
    KtCore
    # possible pthread reference from the global static mutex used in qmetaobject_p.h
    ${CMAKE_THREAD_LIBS_INIT}
)

include_directories(
    ${CMAKE_BINARY_DIR}/include
    ${CMAKE_BINARY_DIR}/privateinclude
    ${CMAKE_BINARY_DIR}/include/QtCore
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${ICU_INCLUDES}
)

set(MOC_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/mocmain.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/moc.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/preprocessor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/generator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/parser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/token.cpp
)

set(BOOTSTRAP_DEFINITIONS
    -DQT_BOOTSTRAPPED
    -DQT_NO_CAST_FROM_BYTEARRAY
    -DQT_NO_CAST_FROM_ASCII
    -DQT_NO_CAST_TO_ASCII
    -DQT_NO_DATASTREAM
    -DQT_NO_LIBRARY
    -DQT_NO_QOBJECT
    -DQT_NO_TEXTSTREAM
    -DQT_NO_THREAD
    -DQT_NO_PROCESS
    -DQT_NO_USING_NAMESPACE
    -DQT_NO_DEPRECATED
    -DQT_NO_COMPRESS
    -DQT_NO_EXCEPTIONS
    -DQT_NO_REGEXP
    -DQT_NO_TEXTCODEC
)

if(WITH_EXECINFO AND EXECINFO_FOUND)
    set(BOOTSTRAP_DEFINITIONS
        ${BOOTSTRAP_DEFINITIONS}
        -DQT_NO_EXECINFO
    )
endif()

set(BOOTSTRAP_SOURCES
    ${CMAKE_SOURCE_DIR}/src/core/codecs/qicucodec.cpp
    ${CMAKE_SOURCE_DIR}/src/core/global/qglobal.cpp
    ${CMAKE_SOURCE_DIR}/src/core/global/qt_error_string.cpp
    ${CMAKE_SOURCE_DIR}/src/core/io/qabstractfileengine.cpp
    ${CMAKE_SOURCE_DIR}/src/core/io/qbuffer.cpp
    ${CMAKE_SOURCE_DIR}/src/core/io/qdir.cpp
    ${CMAKE_SOURCE_DIR}/src/core/io/qdiriterator.cpp
    ${CMAKE_SOURCE_DIR}/src/core/io/qfile.cpp
    ${CMAKE_SOURCE_DIR}/src/core/io/qfileinfo.cpp
    ${CMAKE_SOURCE_DIR}/src/core/io/qfilesystementry.cpp
    ${CMAKE_SOURCE_DIR}/src/core/io/qfsfileengine.cpp
    ${CMAKE_SOURCE_DIR}/src/core/io/qfsfileengine_iterator.cpp
    ${CMAKE_SOURCE_DIR}/src/core/io/qiodevice.cpp
    ${CMAKE_SOURCE_DIR}/src/core/io/qtemporaryfile.cpp
    ${CMAKE_SOURCE_DIR}/src/core/io/qtextstream.cpp
    ${CMAKE_SOURCE_DIR}/src/core/io/qfilesystemengine_unix.cpp
    ${CMAKE_SOURCE_DIR}/src/core/io/qfilesystemiterator_unix.cpp
    ${CMAKE_SOURCE_DIR}/src/core/io/qfsfileengine_unix.cpp
    ${CMAKE_SOURCE_DIR}/src/core/kernel/qmetatype.cpp
    ${CMAKE_SOURCE_DIR}/src/core/tools/qbitarray.cpp
    ${CMAKE_SOURCE_DIR}/src/core/tools/qbytearray.cpp
    ${CMAKE_SOURCE_DIR}/src/core/tools/qbytearraymatcher.cpp
    ${CMAKE_SOURCE_DIR}/src/core/tools/qchar.cpp
    ${CMAKE_SOURCE_DIR}/src/core/tools/qdatetime.cpp
    ${CMAKE_SOURCE_DIR}/src/core/tools/qhash.cpp
    ${CMAKE_SOURCE_DIR}/src/core/tools/qlist.cpp
    ${CMAKE_SOURCE_DIR}/src/core/tools/qlocale.cpp
    ${CMAKE_SOURCE_DIR}/src/core/tools/qlocale_tools.cpp
    ${CMAKE_SOURCE_DIR}/src/core/tools/qmap.cpp
    ${CMAKE_SOURCE_DIR}/src/core/tools/qstring.cpp
    ${CMAKE_SOURCE_DIR}/src/core/tools/qstringlist.cpp
    ${CMAKE_SOURCE_DIR}/src/core/tools/qstringmatcher.cpp
    ${CMAKE_SOURCE_DIR}/src/core/tools/qvector.cpp
)

add_executable(bootstrap_moc ${BOOTSTRAP_SOURCES} ${MOC_SOURCES})
target_compile_definitions(bootstrap_moc PRIVATE ${BOOTSTRAP_DEFINITIONS})
target_link_libraries(bootstrap_moc ${ICU_LIBRARIES})

add_executable(moc ${MOC_SOURCES})
target_link_libraries(moc ${EXTRA_MOC_LIBS})

set_target_properties(moc PROPERTIES
    EXPORT_NAME moc
    OUTPUT_NAME moc${KATIE_TOOLS_SUFFIX}
)

install(
    TARGETS moc
    EXPORT KatieTargets ${INSTALL_TARGETS_DEFAULT_ARGS}
    RUNTIME DESTINATION ${KATIE_BINARIES_PATH}
    COMPONENT Tools
)

install(
    FILES ${CMAKE_CURRENT_SOURCE_DIR}/moc.1
    DESTINATION ${KATIE_MAN_PATH}/man1
    RENAME moc${KATIE_TOOLS_SUFFIX}.1
    COMPONENT Doc
)
