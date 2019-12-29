# add_definitions()
set(EXTRA_CORE_LIBS
    ${ZLIB_LIBRARIES}
    ${ICU_LIBRARIES}
    ${OPENSSL_LIBRARIES}
    ${ZSTD_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
)

set(CORE_PUBLIC_HEADERS
    QVarLengthArray
    QList
    QSystemSemaphore
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
    QFuture
    QCryptographicHash
    QWaitCondition
    QMimeData
    QtConcurrentMap
    QBitArray
    QChar
    QTextCodecPlugin
    QMap
    QTextStream
    QDebug
    QContiguousCache
    QFileInfo
    QFutureInterface
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
    QFutureWatcherBase
    QPoint
    QByteArray
    QHashData
    QSharedData
    QPointer
    QSetIterator
    QtConcurrentRun
    QSignalMapper
    QEventLoop
    QHashNode
    QDataStream
    QAbstractFileEngineIterator
    QScopedValueRollback
    QUuid
    QStdWString
    QAbstractItemModel
    QTextBoundaryFinder
    QTimer
    QElapsedTimer
    QUrl
    QFutureWatcher
    QVector
    QLibraryInfo
    QTextStreamFunction
    QLinkedList
    QEvent
    QVectorIterator
    QObjectData
    QTextEncoder
    QSemaphore
    QFutureSynchronizer
    QNoDebug
    QListData
    QTextCodecFactoryInterface
    QStack
    QScopedPointerPodDeleter
    QTemporaryFile
    QQueue
    QMutex
    QMargins
    QByteArrayMatcher
    QDirIterator
    QtMsgHandler
    QSharedMemory
    QReadWriteLock
    QFileSystemWatcher
    QContiguousCacheTypedData
    QtCleanUpFunction
    QSharedPointer
    QInternal
    QSize
    QRect
    QCoreApplication
    QRegExp
    QMetaObject
    QMapData
    QStringRef
    QGlobalStaticDeleter
    QMetaTypeId
    QMapNode
    QLinkedListData
    QDir
    QObject
    QFactoryInterface
    QString
    QScopedPointerArrayDeleter
    QReturnArgument
    QMetaObjectAccessor
    QLocale
    QMetaType
    QVariant
    QFile
    QtConcurrentFilter
    QCache
    QAbstractEventDispatcher
    QResource
    QRunnable
    QGenericReturnArgument
    QTimeLine
    QMutableFutureIterator
    QtPluginInstanceFunction
    QTextCodec
    QStringList
    QEasingCurve
    QSystemLocale
    QVectorTypedData
    QMapPayloadNode
    QBasicTimer
    QThread
    QSocketNotifier
    QJsonArray
    QJsonDocument
    QJsonObject
    QJsonValue
    QCommandLineOption
    QCommandLineParser
)

include_directories(
    ${CMAKE_BINARY_DIR}/include
    ${CMAKE_BINARY_DIR}/privateinclude
    ${CMAKE_BINARY_DIR}/include/QtCore
    ${CMAKE_BINARY_DIR}/privateinclude/QtCore
    ${ZLIB_INCLUDE_DIRS}
    ${ICU_INCLUDES}
    ${OPENSSL_INCLUDE_DIR}
    ${ZSTD_INCLUDES}
)

include(codecs/codecs.cmake)
include(concurrent/concurrent.cmake)
include(global/global.cmake)
include(io/io.cmake)
include(json/json.cmake)
include(kernel/kernel.cmake)
include(plugin/plugin.cmake)
include(thread/thread.cmake)
include(tools/tools.cmake)

if(${KATIE_PLATFORM} STREQUAL "linux")
    set(EXTRA_CORE_LIBS
        ${EXTRA_CORE_LIBS}
        dl
        rt
    )
endif()

if(WITH_UNWIND AND UNWIND_FOUND AND HAVE_CXXABI)
    set(EXTRA_CORE_LIBS
        ${EXTRA_CORE_LIBS}
        ${UNWIND_LIBRARIES}
    )
    include_directories(${UNWIND_INCLUDES})
endif()
     

set(CORE_HEADERS
    ${CORE_HEADERS}
    ${CMAKE_CURRENT_SOURCE_DIR}/qcorecommon_p.h
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
    EXPORT KatieLibraryTargets ${INSTALL_TARGETS_DEFAULT_ARGS}
    DESTINATION ${KATIE_LIBRARIES_RELATIVE}
    COMPONENT Runtime
)

install(
    DIRECTORY ${CMAKE_BINARY_DIR}/include/QtCore
    DESTINATION ${KATIE_HEADERS_RELATIVE}
    COMPONENT Devel
)

katie_optimize_headers(${KATIE_HEADERS_FULL}/QtCore)
