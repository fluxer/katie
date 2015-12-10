set(CORE_HEADERS
    ${CORE_HEADERS}
    ${CMAKE_CURRENT_SOURCE_DIR}/statemachine/qstatemachine.h
    ${CMAKE_CURRENT_SOURCE_DIR}/statemachine/qstatemachine_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/statemachine/qsignaleventgenerator_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/statemachine/qabstractstate.h
    ${CMAKE_CURRENT_SOURCE_DIR}/statemachine/qabstractstate_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/statemachine/qstate.h
    ${CMAKE_CURRENT_SOURCE_DIR}/statemachine/qstate_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/statemachine/qfinalstate.h
    ${CMAKE_CURRENT_SOURCE_DIR}/statemachine/qhistorystate.h
    ${CMAKE_CURRENT_SOURCE_DIR}/statemachine/qhistorystate_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/statemachine/qabstracttransition.h
    ${CMAKE_CURRENT_SOURCE_DIR}/statemachine/qabstracttransition_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/statemachine/qsignaltransition.h
    ${CMAKE_CURRENT_SOURCE_DIR}/statemachine/qsignaltransition_p.h
    # FIXME: only when !QT_NO_STATEMACHINE_EVENTFILTER
    ${CMAKE_CURRENT_SOURCE_DIR}/statemachine/qeventtransition.h
    ${CMAKE_CURRENT_SOURCE_DIR}/statemachine/qeventtransition_p.h
)

set(CORE_SOURCES
    ${CORE_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/statemachine/qstatemachine.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/statemachine/qabstractstate.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/statemachine/qstate.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/statemachine/qfinalstate.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/statemachine/qhistorystate.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/statemachine/qabstracttransition.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/statemachine/qsignaltransition.cpp
    # FIXME: only when !QT_NO_STATEMACHINE_EVENTFILTER
    ${CMAKE_CURRENT_SOURCE_DIR}/statemachine/qeventtransition.cpp
)

