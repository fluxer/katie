add_definitions(-DQT_BUILD_CORE_LIB)
set(EXTRA_CORE_LIBS)

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
    QXmlStreamStringRef
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
    QXmlStreamNotationDeclaration
    QLinkedListNode
    QFutureInterfaceBase
    QLibrary
    QEventTransition
    QStringListIterator
    QMutableLinkedListIterator
    QMultiHash
    QObjectCleanupHandler
    QAbstractFileEngine
    QHistoryState
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
    QNoImplicitBoolCast
    QFutureInterface
    QAbstractFileEngineHandler
    QMetaObjectExtraData
    QSharedDataPointer
    QMultiMap
    QDateTime
    QThreadPool
    QSettings
    QStandardPaths
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
    QVariantHash
    QXmlStreamEntityDeclarations
    QMetaMethod
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
    QMetaEnum
    QHashNode
    QMutexLocker
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
    QLineF
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
    QXmlStreamNamespaceDeclaration
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
    QXmlStreamEntityResolver
    QObjectList
    QXmlStreamNotationDeclarations
    QContiguousCacheTypedData
    QXmlStreamReader
    QForeachContainer
    QtCleanUpFunction
    QScopedArrayPointer
    QSharedPointer
    QInternal
    QSize
    QRect
    QFileInfoList
    QCoreApplication
    QRegExp
    QStateMachine
    QVariantComparisonHelper
    QMetaObject
    QBasicAtomicPointer
    QPauseAnimation
    QMapData
    QXmlStreamWriter
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
    QLatin1Literal
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
    QBasicAtomicInt
    QConcatenable
    QLocale
    QMetaType
    QVariant
    QFile
    QXmlStreamEntityDeclaration
    QtConcurrentFilter
    QCache
    QLatin1String
    QAbstractState
    QAbstractEventDispatcher
    QResource
    QModelIndexList
    QRunnable
    QtEndian
    QArgument
    QGenericReturnArgument
    QAtomicInt
    QtContainerFwd
    QTimeLine
    QForeachContainerBase
    QFinalState
    QMutableFutureIterator
    QtPluginInstanceFunction
    QTextCodec
    QStringList
    QGlobalStatic
    QEasingCurve
    QMutexData
    QSystemLocale
    QVectorTypedData
    QXmlStreamNamespaceDeclarations
    QMapPayloadNode
    QBasicTimer
    QThread
    QState
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
    QAbstractListModel
    QProcessEnvironment
    QModelIndex
    QSizeF
    QRectF
    QWeakPointer
    QDate
    QTime
    # obsolete
    QFlags
)

include(animation/animation.cmake)
include(arch/arch.cmake)
include(codecs/codecs.cmake)
include(concurrent/concurrent.cmake)
include(global/global.cmake)
include(io/io.cmake)
include(json/json.cmake)
include(kernel/kernel.cmake)
#include(mimetypes/mimetypes.cmake)
include(plugin/plugin.cmake)
include(statemachine/statemachine.cmake)
include(thread/thread.cmake)
include(tools/tools.cmake)
include(xml/xml.cmake)

include_directories(
    ${CMAKE_BINARY_DIR}/include
    ${CMAKE_BINARY_DIR}/privateinclude
    ${CMAKE_BINARY_DIR}/include/QtCore
    ${CMAKE_BINARY_DIR}/privateinclude/QtCore
    ${CMAKE_CURRENT_SOURCE_DIR}/animation
    ${CMAKE_CURRENT_SOURCE_DIR}/arch
    ${CMAKE_CURRENT_SOURCE_DIR}/codecs
    ${CMAKE_CURRENT_SOURCE_DIR}/concurrent
    ${CMAKE_CURRENT_SOURCE_DIR}/global
    ${CMAKE_CURRENT_SOURCE_DIR}/io
    ${CMAKE_CURRENT_SOURCE_DIR}/json
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel
    ${CMAKE_CURRENT_SOURCE_DIR}/plugin
    ${CMAKE_CURRENT_SOURCE_DIR}/statemachine
    ${CMAKE_CURRENT_SOURCE_DIR}/thread
    ${CMAKE_CURRENT_SOURCE_DIR}/tools
    ${CMAKE_CURRENT_SOURCE_DIR}/xml
    ${CMAKE_CURRENT_BINARY_DIR}/animation
    ${CMAKE_CURRENT_BINARY_DIR}/arch
    ${CMAKE_CURRENT_BINARY_DIR}/codecs
    ${CMAKE_CURRENT_BINARY_DIR}/concurrent
    ${CMAKE_CURRENT_BINARY_DIR}/global
    ${CMAKE_CURRENT_BINARY_DIR}/io
    ${CMAKE_CURRENT_BINARY_DIR}/json
    ${CMAKE_CURRENT_BINARY_DIR}/kernel
    ${CMAKE_CURRENT_BINARY_DIR}/plugin
    ${CMAKE_CURRENT_BINARY_DIR}/statemachine
    ${CMAKE_CURRENT_BINARY_DIR}/thread
    ${CMAKE_CURRENT_BINARY_DIR}/tools
    ${CMAKE_CURRENT_BINARY_DIR}/xml
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

if(WITH_ZLIB AND ZLIB_FOUND)
    set(EXTRA_CORE_LIBS
        ${EXTRA_CORE_LIBS}
        ${ZLIB_LIBRARIES}
    )
    include_directories(${ZLIB_INCLUDE_DIRS})
else()
    set(CORE_SOURCES
        ${CORE_SOURCES}
        ${CMAKE_SOURCE_DIR}/src/3rdparty/miniz/miniz.c
    )
    include_directories(${CMAKE_SOURCE_DIR}/src/3rdparty/miniz)
endif()

if(${KATIE_PLATFORM} STREQUAL "linux")
    set(EXTRA_CORE_LIBS
        ${EXTRA_CORE_LIBS}
        dl
    )
elseif(${KATIE_PLATFORM} STREQUAL "win32")
    set(EXTRA_CORE_LIBS
        ${EXTRA_CORE_LIBS}
        ole32
        ws2_32
        uuid
    )
    add_definitions(-DQT_MAKEDLL)
elseif(${KATIE_PLATFORM} STREQUAL "mac")
    set(KATIE_LDFLAGS
        ${KATIE_LDFLAGS}
        -framework CoreFoundation
        -framework Security
        -framework ApplicationServices
        -lObjC
    )
endif()

if(WITH_THREADS AND THREADS_FOUND)
    set(EXTRA_CORE_LIBS
        ${EXTRA_CORE_LIBS}
        ${CMAKE_THREAD_LIBS_INIT}
    )
endif()

katie_generate_misc("${CORE_HEADERS}" QtCore)
katie_generate_public("${CORE_PUBLIC_HEADERS}" QtCore)
katie_resources(${CORE_SOURCES} ${CORE_HEADERS})
katie_setup_flags()

add_library(KtCore ${KATIE_TYPE} ${CORE_SOURCES} ${CORE_HEADERS})
target_link_libraries(KtCore ${EXTRA_CORE_LIBS})
set_target_properties(KtCore PROPERTIES
    VERSION ${KATIE_MAJOR}.${KATIE_MINOR}
    SOVERSION ${KATIE_VERSION}
    EXPORT_NAME Core
)

katie_generate_package(KtCore "")

install(
    TARGETS KtCore
    EXPORT KatieLibraryTargets ${INSTALL_TARGETS_DEFAULT_ARGS}
    DESTINATION ${QT_LIBRARIES_PATH}
)

install(
    DIRECTORY ${CMAKE_BINARY_DIR}/include/QtCore
    DESTINATION ${QT_HEADERS_PATH}
    COMPONENT Devel
)

katie_optimize_headers(${QT_HEADERS_PATH}/QtCore)
