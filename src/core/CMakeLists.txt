add_definitions(-DQT_BUILD_CORE_LIB)
set(EXTRA_CORE_LIBS ${ZLIB_LIBRARIES} ${OPENSSL_LIBRARIES})

set(CORE_PUBLIC_HEADERS
    ${CORE_PUBLIC_HEADERS}
    QVarLengthArray
    QThreadStorageData
    QList
    QMutableStringListIterator
    QSystemSemaphore
    QProcess
    QTextDecoder
    QVectorData
    QBuffer
    QStringMatcher
    QFutureIterator
    QScopedPointer
    QIncompatibleFlag
    QSet
    QLinkedListIterator
    QDynamicPropertyChangeEvent
    QHash
    QThreadStorage
    QTextStreamManipulator
    QMutableMapIterator
    QLinkedListNode
    QFutureInterfaceBase
    QLibrary
    QEventTransition
    QStringListIterator
    QMutableLinkedListIterator
    QMultiHash
    QObjectCleanupHandler
    QAbstractFileEngine
    QTranslator
    QFuture
    QCryptographicHash
    QWaitCondition
    QContiguousCacheData
    QMimeData
    QTypeInfo
    QtConcurrentMap
    QBitArray
    QAbstractAnimation
    QChar
    QTextCodecPlugin
    QParallelAnimationGroup
    QMap
    QTextStream
    QDebug
    QScopedPointerDeleter
    QAbstractTransition
    QMetaTypeId2
    QContiguousCache
    QFileInfo
    QFutureInterface
    QAbstractFileEngineHandler
    QMetaObjectExtraData
    QSharedDataPointer
    QMultiMap
    QDateTime
    QThreadPool
    QSettings
    QStandardPaths
    QPlugin
    QPluginLoader
    QAtomicPointer
    QSequentialAnimationGroup
    QFSFileEngine
    QLine
    QIODevice
    QtConfig
    QPair
    QHashDummyNode
    QHashDummyValue
    QFutureWatcherBase
    QPoint
    QByteArray
    QWriteLocker
    QHashData
    QSharedData
    QReadLocker
    QPointer
    QSetIterator
    QtConcurrentRun
    QSignalMapper
    QEventLoop
    QHashNode
    QDataStream
    QPointF
    QAbstractFileEngineIterator
    QScopedValueRollback
    QUuid
    QStdWString
    QtAlgorithms
    QAbstractItemModel
    QStringBuilder
    QSignalTransition
    QTextBoundaryFinder
    QExplicitlySharedDataPointer
    QTimerEvent
    QObjectUserData
    QTimer
    QElapsedTimer
    QUrl
    QMutableHashIterator
    QFutureWatcher
    QVector
    QLibraryInfo
    QTextStreamFunction
    QVariantAnimation
    QLinkedList
    QEvent
    QVectorIterator
    QAnimationGroup
    QGenericArgument
    QObjectData
    QTextEncoder
    QSemaphore
    QFutureSynchronizer
    QNoDebug
    QListData
    QTextCodecFactoryInterface
    QPropertyAnimation
    QStack
    QMutableListIterator
    QScopedPointerPodDeleter
    QTemporaryFile
    QQueue
    QMutex
    QMargins
    QtGlobal
    QByteArrayMatcher
    QDirIterator
    QtMsgHandler
    QSharedMemory
    QAbstractConcatenable
    QReadWriteLock
    QFileSystemWatcher
    QObjectList
    QContiguousCacheTypedData
    QForeachContainer
    QtCleanUpFunction
    QSharedPointer
    QInternal
    QSize
    QRect
    QFileInfoList
    QCoreApplication
    QRegExp
    QVariantComparisonHelper
    QMetaObject
    QPauseAnimation
    QMapData
    QByteRef
    QIntegerForSize
    QStringRef
    QChildEvent
    QGlobalStaticDeleter
    QFlag
    QMetaTypeId
    QMapNode
    QLinkedListData
    QLatin1Char
    QAnimationDriver
    QBBSystemLocaleData
    QDir
    QObject
    QFactoryInterface
    QMutableSetIterator
    QString
    QMetaProperty
    QScopedPointerArrayDeleter
    QCharRef
    QReturnArgument
    QMetaObjectAccessor
    QConcatenable
    QLocale
    QMetaType
    QVariant
    QFile
    QtConcurrentFilter
    QCache
    QLatin1String
    QAbstractEventDispatcher
    QResource
    QRunnable
    QtEndian
    QArgument
    QGenericReturnArgument
    QAtomicInt
    QtContainerFwd
    QTimeLine
    QForeachContainerBase
    QMutableFutureIterator
    QtPluginInstanceFunction
    QTextCodec
    QStringList
    QGlobalStatic
    QEasingCurve
    QMutexData
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
    # aliases
    QtPlugin
    QtDebug
    # mixed
    QVariantMap
    QVariantList
    QVariantHash
    QAbstractListModel
    QAbstractTableModel
    QProcessEnvironment
    QModelIndex
    QModelIndexList
    QSizeF
    QRectF
    QLineF
    QWeakPointer
    QDate
    QTime
    QListIterator
    QMetaMethod
    QMetaEnum
    QMutexLocker
    QScopedArrayPointer
    QJsonParseError
    # obsolete
    QFlags
    Qt
    # both in core and xml
    QXmlStreamAttribute
    QXmlStreamAttributes
    QXmlStreamNotationDeclaration
    QXmlStreamNotationDeclarations
    QXmlStreamEntityDeclaration
    QXmlStreamEntityDeclarations
    QXmlStreamNamespaceDeclaration
    QXmlStreamNamespaceDeclarations
    QXmlStreamEntityResolver
    QXmlStreamReader
    QXmlStreamWriter
)

include(animation/animation.cmake)
include(codecs/codecs.cmake)
include(concurrent/concurrent.cmake)
include(global/global.cmake)
include(io/io.cmake)
include(json/json.cmake)
include(kernel/kernel.cmake)
include(plugin/plugin.cmake)
include(thread/thread.cmake)
include(tools/tools.cmake)
include(xml/xml.cmake)

include_directories(
    ${CMAKE_BINARY_DIR}/include
    ${CMAKE_BINARY_DIR}/privateinclude
    ${CMAKE_BINARY_DIR}/include/QtCore
    ${CMAKE_BINARY_DIR}/privateinclude/QtCore
    ${CMAKE_CURRENT_SOURCE_DIR}/animation
    ${CMAKE_CURRENT_SOURCE_DIR}/codecs
    ${CMAKE_CURRENT_SOURCE_DIR}/concurrent
    ${CMAKE_CURRENT_SOURCE_DIR}/global
    ${CMAKE_CURRENT_SOURCE_DIR}/io
    ${CMAKE_CURRENT_SOURCE_DIR}/json
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel
    ${CMAKE_CURRENT_SOURCE_DIR}/plugin
    ${CMAKE_CURRENT_SOURCE_DIR}/thread
    ${CMAKE_CURRENT_SOURCE_DIR}/tools
    ${CMAKE_CURRENT_SOURCE_DIR}/xml
    ${CMAKE_CURRENT_BINARY_DIR}/animation
    ${CMAKE_CURRENT_BINARY_DIR}/codecs
    ${CMAKE_CURRENT_BINARY_DIR}/concurrent
    ${CMAKE_CURRENT_BINARY_DIR}/global
    ${CMAKE_CURRENT_BINARY_DIR}/io
    ${CMAKE_CURRENT_BINARY_DIR}/json
    ${CMAKE_CURRENT_BINARY_DIR}/kernel
    ${CMAKE_CURRENT_BINARY_DIR}/plugin
    ${CMAKE_CURRENT_BINARY_DIR}/thread
    ${CMAKE_CURRENT_BINARY_DIR}/tools
    ${CMAKE_CURRENT_BINARY_DIR}/xml
    ${ZLIB_INCLUDE_DIRS}
    ${OPENSSL_INCLUDE_DIR}
)

if(WITH_GLIB2 AND GLIB2_FOUND)
    set(CORE_HEADERS
        ${CORE_HEADERS}
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qeventdispatcher_glib_p.h
    )
    set(EXTRA_CORE_LIBS
        ${EXTRA_CORE_LIBS}
        ${GLIB2_LIBRARIES}
    )
    include_directories(${GLIB2_INCLUDES})
endif()

if(${KATIE_PLATFORM} STREQUAL "linux")
    set(EXTRA_CORE_LIBS
        ${EXTRA_CORE_LIBS}
        dl
        rt
    )
endif()

if(WITH_THREADS AND THREADS_FOUND)
    set(EXTRA_CORE_LIBS
        ${EXTRA_CORE_LIBS}
        ${CMAKE_THREAD_LIBS_INIT}
    )
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
target_link_libraries(KtCore ${EXTRA_CORE_LIBS})
set_target_properties(KtCore PROPERTIES
    VERSION ${KATIE_MAJOR}.${KATIE_MINOR}
    SOVERSION ${KATIE_VERSION}
    EXPORT_NAME Core
)

install(
    TARGETS KtCore
    EXPORT KatieLibraryTargets ${INSTALL_TARGETS_DEFAULT_ARGS}
    DESTINATION ${KATIE_LIBRARIES_RELATIVE}
)

install(
    DIRECTORY ${CMAKE_BINARY_DIR}/include/QtCore
    DESTINATION ${KATIE_HEADERS_RELATIVE}
    COMPONENT Devel
)

katie_optimize_headers(${KATIE_HEADERS_FULL}/QtCore)
