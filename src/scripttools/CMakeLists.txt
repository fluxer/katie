# add_definitions()
set(EXTRA_SCRIPTTOOLS_LIBS
    KtCore KtXml KtGui KtScript
)

set(SCRIPTTOOLS_PUBLIC_HEADERS
    QScriptEngineDebugger
)

include_directories(
    ${CMAKE_BINARY_DIR}/include
    ${CMAKE_BINARY_DIR}/privateinclude
    ${CMAKE_BINARY_DIR}/include/QtCore
    ${CMAKE_BINARY_DIR}/include/QtGui
    ${CMAKE_BINARY_DIR}/include/QtScript
    ${CMAKE_BINARY_DIR}/include/QtScriptTools
)

set(SCRIPTTOOLS_HEADERS
    ${SCRIPTTOOLS_HEADERS}
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptdebuggervalue_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptdebuggervalueproperty_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptdebuggerobjectsnapshotdelta_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptdebuggercommand_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptdebuggercommandschedulerinterface_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptdebuggercommandschedulerfrontend_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptdebuggerresponse_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptdebuggerresponsehandlerinterface_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptdebuggerevent_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptdebuggercommandexecutor_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptdebuggerbackend_p_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptdebuggerbackend_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptdebuggeragent_p_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptdebuggeragent_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptdebuggerfrontend_p_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptdebuggerfrontend_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptmessagehandlerinterface_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptstdmessagehandler_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptenginedebuggerfrontend_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptdebuggerjob_p_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptdebuggerjob_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptdebuggercommandschedulerjob_p_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptdebuggercommandschedulerjob_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptdebuggerjobschedulerinterface_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptdebuggereventhandlerinterface_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptvalueproperty_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptobjectsnapshot_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptscriptdata_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptxmlparser_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptbreakpointdata_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptdebugger_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptdebuggerconsole_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptdebuggerconsolecommand_p_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptdebuggerconsolecommand_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptdebuggerconsolecommandjob_p_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptdebuggerconsolecommandjob_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptdebuggerconsolecommandgroupdata_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptdebuggerconsolecommandmanager_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptdebuggerconsolehistorianinterface_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptcompletionproviderinterface_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptcompletiontaskinterface_p_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptcompletiontaskinterface_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptcompletiontask_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscripttooltipproviderinterface_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptdebuggerconsoleglobalobject_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptdebuggerscriptedconsolecommand_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptdebuggerconsolewidgetinterface_p_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptdebuggerconsolewidgetinterface_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptdebuggerconsolewidget_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptdebuggerstackwidgetinterface_p_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptdebuggerstackwidgetinterface_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptdebuggerstackwidget_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptdebuggerstackmodel_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptdebuggerscriptswidgetinterface_p_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptdebuggerscriptswidgetinterface_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptdebuggerscriptswidget_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptdebuggerscriptsmodel_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptdebuggerlocalswidgetinterface_p_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptdebuggerlocalswidgetinterface_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptdebuggerlocalswidget_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptdebuggerlocalsmodel_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptdebuggercodewidgetinterface_p_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptdebuggercodewidgetinterface_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptdebuggercodewidget_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptdebuggercodeviewinterface_p_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptdebuggercodeviewinterface_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptdebuggercodeview_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptdebuggercodefinderwidgetinterface_p_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptdebuggercodefinderwidgetinterface_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptdebuggercodefinderwidget_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptdebugoutputwidgetinterface_p_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptdebugoutputwidgetinterface_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptdebugoutputwidget_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptbreakpointswidgetinterface_p_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptbreakpointswidgetinterface_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptbreakpointswidget_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptbreakpointsmodel_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscripterrorlogwidgetinterface_p_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscripterrorlogwidgetinterface_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscripterrorlogwidget_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptdebuggerwidgetfactoryinterface_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptdebuggerstandardwidgetfactory_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptedit_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptsyntaxhighlighter_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptenginedebugger.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qscripttoolscommon_p.h
)

set(SCRIPTTOOLS_SOURCES
    ${SCRIPTTOOLS_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptdebuggervalue.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptdebuggervalueproperty.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptdebuggercommand.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptdebuggerresponse.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptdebuggerevent.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptdebuggercommandexecutor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptdebuggercommandschedulerfrontend.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptdebuggerbackend.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptdebuggeragent.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptdebuggerfrontend.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptstdmessagehandler.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptenginedebuggerfrontend.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptdebuggerjob.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptdebuggercommandschedulerjob.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptvalueproperty.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptobjectsnapshot.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptscriptdata.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptxmlparser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptbreakpointdata.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptdebugger.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptdebuggerconsole.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptdebuggerconsolecommand.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptdebuggerconsolecommandjob.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptdebuggerconsolecommandgroupdata.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptdebuggerconsolecommandmanager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptcompletiontaskinterface.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptcompletiontask.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptdebuggerconsoleglobalobject.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptdebuggerscriptedconsolecommand.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptdebuggerconsolewidgetinterface.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptdebuggerconsolewidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptdebuggerstackwidgetinterface.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptdebuggerstackwidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptdebuggerstackmodel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptdebuggerscriptswidgetinterface.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptdebuggerscriptswidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptdebuggerscriptsmodel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptdebuggerlocalswidgetinterface.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptdebuggerlocalswidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptdebuggerlocalsmodel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptdebuggercodewidgetinterface.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptdebuggercodewidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptdebuggercodeviewinterface.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptdebuggercodeview.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptdebuggercodefinderwidgetinterface.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptdebuggercodefinderwidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptdebugoutputwidgetinterface.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptdebugoutputwidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptbreakpointswidgetinterface.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptbreakpointswidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptbreakpointsmodel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscripterrorlogwidgetinterface.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscripterrorlogwidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptdebuggerstandardwidgetfactory.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptedit.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptsyntaxhighlighter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/qscriptenginedebugger.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging/scripttools_debugging.qrc
)

katie_generate_misc("${SCRIPTTOOLS_HEADERS}" QtScriptTools)
katie_generate_public("${SCRIPTTOOLS_PUBLIC_HEADERS}" QtScriptTools)
katie_generate_package(KtScriptTools "KtCore KtXml KtGui KtScript")
katie_setup_target(KtScriptTools ${SCRIPTTOOLS_SOURCES} ${SCRIPTTOOLS_HEADERS})

add_library(KtScriptTools ${KATIE_TYPE} ${KtScriptTools_SOURCES})
target_link_libraries(KtScriptTools PRIVATE ${EXTRA_SCRIPTTOOLS_LIBS})
set_target_properties(KtScriptTools PROPERTIES
    VERSION ${KATIE_MAJOR}.${KATIE_MINOR}
    SOVERSION ${KATIE_VERSION}
    EXPORT_NAME ScriptTools
)

install(
    TARGETS KtScriptTools
    EXPORT KatieTargets ${INSTALL_TARGETS_DEFAULT_ARGS}
    DESTINATION ${KATIE_LIBRARIES_PATH}
    COMPONENT Runtime
)

install(
    DIRECTORY ${CMAKE_BINARY_DIR}/include/QtScriptTools
    DESTINATION ${KATIE_HEADERS_PATH}
    COMPONENT Devel
)

katie_optimize_headers(${KATIE_HEADERS_PATH}/QtScriptTools)
