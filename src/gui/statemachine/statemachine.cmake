set(GUI_SOURCES
    ${GUI_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/statemachine/qguistatemachine.cpp
)

# TODO: !contains(DEFINES, QT_NO_STATEMACHINE_EVENTFILTER) {
set(GUI_HEADERS
    ${GUI_HEADERS}
    ${CMAKE_CURRENT_SOURCE_DIR}/statemachine/qkeyeventtransition.h
    ${CMAKE_CURRENT_SOURCE_DIR}/statemachine/qmouseeventtransition.h
    ${CMAKE_CURRENT_SOURCE_DIR}/statemachine/qbasickeyeventtransition_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/statemachine/qbasicmouseeventtransition_p.h
)

set(GUI_SOURCES
    ${GUI_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/statemachine/qkeyeventtransition.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/statemachine/qmouseeventtransition.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/statemachine/qbasickeyeventtransition.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/statemachine/qbasicmouseeventtransition.cpp
)
