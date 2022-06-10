# add_definitions()
set(EXTRA_CORE_LIBS
    ${ICU_LIBRARIES}
    ${JANSSON_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
    # libm should be present by default becaue this is C++
    m
)

set(CORE_PUBLIC_HEADERS
    QVarLengthArray
    QList
    QProcess
    QBuffer
    QStringMatcher
    QScopedPointer
    QSet
    QHash
    QLibrary
    QObjectCleanupHandler
    QAbstractFileEngine
    QTranslator
    QWaitCondition
    QMimeData
    QBitArray
    QChar
    QMap
    QTextStream
    QDebug
    QContiguousCache
    QFileInfo
    QDateTime
    QThreadPool
    QSettings
    QStandardPaths
    QPlugin
    QPluginLoader
    QFSFileEngine
    QLine
    QIODevice
    QPair
    QPoint
    QByteArray
    QSharedData
    QPointer
    QSignalMapper
    QEventLoop
    QDataStream
    QScopedValueRollback
    QAbstractItemModel
    QTextBoundaryFinder
    QTimer
    QElapsedTimer
    QUrl
    QVector
    QLibraryInfo
    QSemaphore
    QStack
    QTemporaryFile
    QQueue
    QMutex
    QMargins
    QByteArrayMatcher
    QDirIterator
    QFileSystemWatcher
    QSharedPointer
    QSize
    QRect
    QCoreApplication
    QRegExp
    QMetaObject
    QDir
    QObject
    QFactoryInterface
    QString
    QLocale
    QMetaType
    QVariant
    QFile
    QCache
    QAbstractEventDispatcher
    QResource
    QRunnable
    QTimeLine
    QTextCodec
    QStringList
    QEasingCurve
    QBasicTimer
    QThread
    QSocketNotifier
    QJsonDocument
)

include_directories(
    ${CMAKE_BINARY_DIR}/include
    ${CMAKE_BINARY_DIR}/privateinclude
    ${CMAKE_BINARY_DIR}/include/QtCore
    ${ICU_INCLUDES}
    ${JANSSON_INCLUDES}
)

set(CORE_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/codecs/qtextcodec_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/codecs/qtextcodec.h
    ${CMAKE_CURRENT_SOURCE_DIR}/codecs/qicucodec_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/global/qendian.h
    ${CMAKE_CURRENT_SOURCE_DIR}/global/qglobal.h
    ${CMAKE_CURRENT_SOURCE_DIR}/global/qlibraryinfo.h
    ${CMAKE_CURRENT_SOURCE_DIR}/global/qnamespace.h
    ${CMAKE_CURRENT_SOURCE_DIR}/global/qnumeric.h
    ${CMAKE_CURRENT_SOURCE_DIR}/global/qplatformdefs.h
    ${CMAKE_CURRENT_SOURCE_DIR}/io/qabstractfileengine.h
    ${CMAKE_CURRENT_SOURCE_DIR}/io/qabstractfileengine_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/io/qbuffer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/io/qdatastream.h
    ${CMAKE_CURRENT_SOURCE_DIR}/io/qdataurl_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/io/qdebug.h
    ${CMAKE_CURRENT_SOURCE_DIR}/io/qdir.h
    ${CMAKE_CURRENT_SOURCE_DIR}/io/qdir_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/io/qdiriterator.h
    ${CMAKE_CURRENT_SOURCE_DIR}/io/qfile.h
    ${CMAKE_CURRENT_SOURCE_DIR}/io/qfileinfo.h
    ${CMAKE_CURRENT_SOURCE_DIR}/io/qfileinfo_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/io/qiodevice.h
    ${CMAKE_CURRENT_SOURCE_DIR}/io/qiodevice_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/io/qnoncontiguousbytedevice_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/io/qprocess.h
    ${CMAKE_CURRENT_SOURCE_DIR}/io/qprocess_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/io/qtextstream.h
    ${CMAKE_CURRENT_SOURCE_DIR}/io/qtemporaryfile.h
    ${CMAKE_CURRENT_SOURCE_DIR}/io/qresource.h
    ${CMAKE_CURRENT_SOURCE_DIR}/io/qresource_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/io/qresource_iterator_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/io/qurl.h
    ${CMAKE_CURRENT_SOURCE_DIR}/io/qtldurl_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/io/qsettings.h
    ${CMAKE_CURRENT_SOURCE_DIR}/io/qsettings_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/io/qstandardpaths.h
    ${CMAKE_CURRENT_SOURCE_DIR}/io/qfsfileengine.h
    ${CMAKE_CURRENT_SOURCE_DIR}/io/qfsfileengine_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/io/qfsfileengine_iterator_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/io/qfilesystemwatcher.h
    ${CMAKE_CURRENT_SOURCE_DIR}/io/qfilesystemwatcher_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/io/qfilesystemwatcher_unix_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/io/qfilesystementry_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/io/qfilesystemengine_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/io/qfilesystemmetadata_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/io/qfilesystemiterator_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qabstracteventdispatcher.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qabstractitemmodel.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qabstractitemmodel_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qbasictimer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qeventloop.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qpointer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qcoreapplication.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qcoreevent.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qmetaobject.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qmetatype.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qmimedata.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qobject.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qobjectdefs.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qsignalmapper.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qsocketnotifier.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qtimer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qtranslator.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qvariant.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qabstracteventdispatcher_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qcoreapplication_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qobjectcleanuphandler.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qvariant_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qmetaobject_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qobject_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qmath.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qcore_unix_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qeventdispatcher_unix_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/plugin/qfactoryinterface.h
    ${CMAKE_CURRENT_SOURCE_DIR}/plugin/qpluginloader.h
    ${CMAKE_CURRENT_SOURCE_DIR}/plugin/qlibrary.h
    ${CMAKE_CURRENT_SOURCE_DIR}/plugin/qlibrary_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/plugin/qplugin.h
    ${CMAKE_CURRENT_SOURCE_DIR}/plugin/qfactoryloader_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/thread/qmutex.h
    ${CMAKE_CURRENT_SOURCE_DIR}/thread/qsemaphore.h
    ${CMAKE_CURRENT_SOURCE_DIR}/thread/qthread.h
    ${CMAKE_CURRENT_SOURCE_DIR}/thread/qwaitcondition.h
    ${CMAKE_CURRENT_SOURCE_DIR}/thread/qatomic.h
    ${CMAKE_CURRENT_SOURCE_DIR}/thread/qmutexpool_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/thread/qorderedmutexlocker_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/thread/qthread_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/thread/qrunnable.h
    ${CMAKE_CURRENT_SOURCE_DIR}/thread/qthreadpool.h
    ${CMAKE_CURRENT_SOURCE_DIR}/thread/qthreadpool_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/qalgorithms.h
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/qbitarray.h
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/qbytearray.h
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/qbytearraymatcher.h
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/qcache.h
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/qchar.h
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/qcontainerfwd.h
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/qdatetime.h
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/qdatetime_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/qeasingcurve.h
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/qhash.h
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/qjsondocument.h
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/qline.h
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/qlist.h
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/qlocale.h
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/qlocale_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/qlocale_tools_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/qlocale_data_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/qmap.h
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/qmargins.h
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/qcontiguouscache.h
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/qpoint.h
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/qqueue.h
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/qrect.h
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/qregexp.h
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/qringbuffer_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/qscopedpointer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/qscopedpointer_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/qscopedvaluerollback.h
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/qshareddata.h
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/qsharedpointer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/qset.h
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/qsize.h
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/qstack.h
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/qstring.h
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/qstringlist.h
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/qstringmatcher.h
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/qtextboundaryfinder.h
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/qtimeline.h
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/qelapsedtimer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/qvarlengtharray.h
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/qvector.h
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/qiterator.h
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/qpair.h
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/qstdcontainers_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qcorecommon_p.h
)

set(CORE_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/codecs/qtextcodec.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/codecs/qicucodec.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/global/qglobal.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/global/qlibraryinfo.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/global/qt_error_string.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/io/qabstractfileengine.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/io/qbuffer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/io/qdatastream.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/io/qdataurl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/io/qtldurl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/io/qdebug.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/io/qdir.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/io/qdiriterator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/io/qfile.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/io/qfileinfo.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/io/qiodevice.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/io/qnoncontiguousbytedevice.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/io/qprocess.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/io/qtextstream.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/io/qtemporaryfile.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/io/qresource.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/io/qresource_iterator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/io/qurl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/io/qsettings.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/io/qstandardpaths.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/io/qfsfileengine.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/io/qfsfileengine_iterator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/io/qfilesystemwatcher.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/io/qfilesystemwatcher_unix.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/io/qfilesystementry.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/io/qfsfileengine_unix.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/io/qfilesystemengine_unix.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/io/qprocess_unix.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/io/qfilesystemiterator_unix.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/io/qstandardpaths_unix.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qabstracteventdispatcher.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qabstractitemmodel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qbasictimer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qeventloop.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qcoreapplication.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qcoreevent.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qmetaobject.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qmetatype.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qmimedata.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qobject.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qobjectcleanuphandler.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qsignalmapper.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qsocketnotifier.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qtimer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qtranslator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qvariant.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qpointer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qcore_unix.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qeventdispatcher_unix.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/plugin/qpluginloader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/plugin/qfactoryloader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/plugin/qlibrary.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/plugin/qlibrary_unix.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/thread/qatomic.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/thread/qmutex.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/thread/qrunnable.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/thread/qsemaphore.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/thread/qthread.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/thread/qthreadpool.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/thread/qthread_unix.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/thread/qwaitcondition_unix.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/qbitarray.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/qbytearray.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/qbytearraymatcher.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/qchar.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/qdatetime.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/qeasingcurve.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/qelapsedtimer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/qhash.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/qjsondocument.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/qline.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/qlist.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/qlocale.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/qlocale_tools.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/qpoint.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/qmap.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/qmargins.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/qcontiguouscache.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/qrect.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/qregexp.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/qshareddata.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/qsharedpointer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/qsize.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/qstring.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/qstringlist.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/qstringmatcher.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/qtextboundaryfinder.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/qtimeline.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/qvector.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/qelapsedtimer_unix.cpp
)

# on some platforms these are not needed as the functionality may be part of
# the standard C library
foreach(lib dl rt)
    find_library(lib${lib}_path NAMES ${lib})
    if(lib${lib}_path)
        set(EXTRA_CORE_LIBS ${EXTRA_CORE_LIBS} ${lib${lib}_path})
    endif()
endforeach()

if(WITH_INTL AND INTL_FOUND)
    set(EXTRA_CORE_LIBS
        ${EXTRA_CORE_LIBS}
        ${INTL_LIBRARIES}
    )
    include_directories(${INTL_INCLUDES})
endif()

if(WITH_EXECINFO AND EXECINFO_FOUND)
    set(EXTRA_CORE_LIBS
        ${EXTRA_CORE_LIBS}
        ${EXECINFO_LIBRARIES}
    )
    include_directories(${EXECINFO_INCLUDES})
endif()

if(WITH_DEFLATE AND DEFLATE_FOUND)
    set(EXTRA_CORE_LIBS
        ${EXTRA_CORE_LIBS}
        ${DEFLATE_LIBRARIES}
    )
    include_directories(${DEFLATE_INCLUDES})
else()
    set(CORE_SOURCES
        ${CORE_SOURCES}
        # common files
        ${CMAKE_SOURCE_DIR}/src/3rdparty/libdeflate/lib/deflate_decompress.c
        ${CMAKE_SOURCE_DIR}/src/3rdparty/libdeflate/lib/deflate_compress.c
        ${CMAKE_SOURCE_DIR}/src/3rdparty/libdeflate/lib/utils.c
        ${CMAKE_SOURCE_DIR}/src/3rdparty/libdeflate/lib/arm/cpu_features.c
        ${CMAKE_SOURCE_DIR}/src/3rdparty/libdeflate/lib/x86/cpu_features.c
        # zlib wrapper files
        ${CMAKE_SOURCE_DIR}/src/3rdparty/libdeflate/lib/adler32.c
        ${CMAKE_SOURCE_DIR}/src/3rdparty/libdeflate/lib/zlib_decompress.c
        ${CMAKE_SOURCE_DIR}/src/3rdparty/libdeflate/lib/zlib_compress.c
        # gzip wrapper files
        ${CMAKE_SOURCE_DIR}/src/3rdparty/libdeflate/lib/crc32.c
        ${CMAKE_SOURCE_DIR}/src/3rdparty/libdeflate/lib/gzip_decompress.c
        ${CMAKE_SOURCE_DIR}/src/3rdparty/libdeflate/lib/gzip_compress.c
    )
    include_directories(${CMAKE_SOURCE_DIR}/src/3rdparty/libdeflate)
endif()

katie_unity_exclude(
    ${CMAKE_CURRENT_SOURCE_DIR}/global/qt_error_string.cpp
)

katie_generate_misc("${CORE_HEADERS}" QtCore)
katie_generate_public("${CORE_PUBLIC_HEADERS}" QtCore)
katie_generate_package(KtCore "")
katie_setup_target(KtCore ${CORE_SOURCES} ${CORE_HEADERS})

add_library(KtCore ${KATIE_TYPE} ${KtCore_SOURCES})
target_link_libraries(KtCore PRIVATE ${EXTRA_CORE_LIBS})
set_target_properties(KtCore PROPERTIES
    VERSION ${KATIE_MAJOR}.${KATIE_MINOR}
    SOVERSION ${KATIE_VERSION}
    EXPORT_NAME Core
)

install(
    TARGETS KtCore
    EXPORT KatieTargets ${INSTALL_TARGETS_DEFAULT_ARGS}
    DESTINATION ${KATIE_LIBRARIES_PATH}
    COMPONENT Runtime
)

install(
    DIRECTORY ${CMAKE_BINARY_DIR}/include/QtCore
    DESTINATION ${KATIE_HEADERS_PATH}
    COMPONENT Devel
)

katie_optimize_headers(${KATIE_HEADERS_PATH}/QtCore)
