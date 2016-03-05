set(DESIGNERCOMPONENTS_HEADERS
    ${DESIGNERCOMPONENTS_HEADERS}
    ${CMAKE_CURRENT_SOURCE_DIR}/signalsloteditor/signalslot_utils_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/signalsloteditor/connectdialog_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/signalsloteditor/signalsloteditor.h
    ${CMAKE_CURRENT_SOURCE_DIR}/signalsloteditor/signalsloteditor_tool.h
    ${CMAKE_CURRENT_SOURCE_DIR}/signalsloteditor/signalsloteditor_plugin.h
    ${CMAKE_CURRENT_SOURCE_DIR}/signalsloteditor/signalsloteditor_global.h
    ${CMAKE_CURRENT_SOURCE_DIR}/signalsloteditor/signalsloteditor_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/signalsloteditor/signalsloteditorwindow.h
)

set(DESIGNERCOMPONENTS_SOURCES
    ${DESIGNERCOMPONENTS_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/signalsloteditor/signalslot_utils.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/signalsloteditor/connectdialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/signalsloteditor/signalsloteditor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/signalsloteditor/signalsloteditor_tool.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/signalsloteditor/signalsloteditor_plugin.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/signalsloteditor/signalsloteditor_instance.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/signalsloteditor/signalsloteditorwindow.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/signalsloteditor/connectdialog.ui
)

