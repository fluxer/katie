set(CORE_HEADERS
    ${CORE_HEADERS}
    ${CMAKE_CURRENT_SOURCE_DIR}/thread/qmutex.h
    ${CMAKE_CURRENT_SOURCE_DIR}/thread/qreadwritelock.h
    ${CMAKE_CURRENT_SOURCE_DIR}/thread/qsemaphore.h
    ${CMAKE_CURRENT_SOURCE_DIR}/thread/qthread.h
    ${CMAKE_CURRENT_SOURCE_DIR}/thread/qwaitcondition.h
    ${CMAKE_CURRENT_SOURCE_DIR}/thread/qatomic.h

    ${CMAKE_CURRENT_SOURCE_DIR}/thread/qmutex_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/thread/qmutexpool_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/thread/qorderedmutexlocker_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/thread/qreadwritelock_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/thread/qthread_p.h
)

set(CORE_SOURCES
    ${CORE_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/thread/qatomic.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/thread/qmutex.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/thread/qreadwritelock.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/thread/qmutexpool.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/thread/qsemaphore.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/thread/qthread.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/thread/qmutex_unix.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/thread/qthread_unix.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/thread/qwaitcondition_unix.cpp
)
