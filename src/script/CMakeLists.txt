add_definitions(
    -DQT_BUILD_SCRIPT_LIB
    -DJSC=QTJSC
    -Djscyyparse=qtjscyyparse
    -Djscyylex=qtjscyylex
    -Djscyyerror=qtjscyyerror
    -DWTF=QTWTF
    -DQLALR_NO_QSCRIPTGRAMMAR_DEBUG_INFO
    -DBUILDING_QT__
)
set(EXTRA_SCRIPT_LIBS KtCore ${PCRE_LIBRARIES})

include(api/api.cmake)
include(bridge/bridge.cmake)
include(parser/parser.cmake)

set(SCRIPT_PUBLIC_HEADERS
    ${SCRIPT_PUBLIC_HEADERS}
    QScriptable
    QScriptClass
    QScriptClassPropertyIterator
    QScriptContext
    QScriptContextInfo
    QScriptContextInfoList
    QScriptEngine
    QScriptEngineAgent
    QScriptExtensionInterface
    QScriptExtensionPlugin
    QScriptProgram
    QScriptString
    QScriptSyntaxCheckResult
    QScriptValue
    QScriptValueIterator
    QScriptValueList
)

include_directories(
    ${CMAKE_BINARY_DIR}/include
    ${CMAKE_BINARY_DIR}/privateinclude
    ${CMAKE_BINARY_DIR}/include/QtCore
    ${CMAKE_BINARY_DIR}/privateinclude/QtCore
    ${CMAKE_BINARY_DIR}/include/QtScript
    ${CMAKE_BINARY_DIR}/privateinclude/QtScript
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/parser
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/bytecompiler
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/debugger
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/runtime
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/wtf
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/wtf/unicode
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/interpreter
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/API
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/bytecode
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/generated
    ${PCRE_INCLUDES}
)

set(SCRIPT_SOURCES
    ${SCRIPT_SOURCES}
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/generated/Grammar.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/API/JSCallbackConstructor.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/API/JSCallbackFunction.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/API/JSCallbackObject.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/API/JSClassRef.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/API/JSContextRef.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/API/JSObjectRef.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/API/JSStringRef.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/API/JSValueRef.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/API/OpaqueJSString.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/bytecode/CodeBlock.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/bytecode/JumpTable.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/bytecode/Opcode.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/bytecode/SamplingTool.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/bytecompiler/BytecodeGenerator.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/bytecompiler/NodesCodegen.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/debugger/DebuggerCallFrame.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/debugger/Debugger.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/interpreter/CallFrame.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/interpreter/Interpreter.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/interpreter/RegisterFile.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/parser/Lexer.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/parser/Nodes.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/parser/ParserArena.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/parser/Parser.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/runtime/ArgList.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/runtime/Arguments.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/runtime/ArrayConstructor.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/runtime/ArrayPrototype.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/runtime/BooleanConstructor.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/runtime/BooleanObject.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/runtime/BooleanPrototype.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/runtime/CallData.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/runtime/Collector.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/runtime/CommonIdentifiers.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/runtime/ConstructData.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/runtime/DateConstructor.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/runtime/DateConversion.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/runtime/DateInstance.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/runtime/DatePrototype.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/runtime/ErrorConstructor.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/runtime/Error.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/runtime/ErrorInstance.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/runtime/ErrorPrototype.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/runtime/ExceptionHelpers.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/runtime/Executable.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/runtime/FunctionConstructor.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/runtime/FunctionPrototype.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/runtime/GetterSetter.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/runtime/GlobalEvalFunction.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/runtime/Identifier.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/runtime/InitializeThreading.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/runtime/InternalFunction.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/runtime/JSActivation.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/runtime/JSAPIValueWrapper.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/runtime/JSArray.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/runtime/JSByteArray.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/runtime/JSCell.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/runtime/JSFunction.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/runtime/JSGlobalData.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/runtime/JSGlobalObject.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/runtime/JSGlobalObjectFunctions.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/runtime/JSImmediate.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/runtime/JSNotAnObject.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/runtime/JSNumberCell.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/runtime/JSObject.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/runtime/JSONObject.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/runtime/JSPropertyNameIterator.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/runtime/JSStaticScopeObject.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/runtime/JSString.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/runtime/JSValue.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/runtime/JSVariableObject.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/runtime/JSWrapperObject.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/runtime/LiteralParser.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/runtime/Lookup.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/runtime/MarkStack.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/runtime/MathObject.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/runtime/NativeErrorConstructor.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/runtime/NativeErrorPrototype.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/runtime/NumberConstructor.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/runtime/NumberObject.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/runtime/NumberPrototype.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/runtime/ObjectConstructor.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/runtime/ObjectPrototype.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/runtime/Operations.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/runtime/PropertyDescriptor.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/runtime/PropertyNameArray.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/runtime/PropertySlot.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/runtime/PrototypeFunction.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/runtime/RegExpConstructor.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/runtime/RegExp.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/runtime/RegExpObject.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/runtime/RegExpPrototype.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/runtime/ScopeChain.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/runtime/SmallStrings.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/runtime/StringConstructor.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/runtime/StringObject.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/runtime/StringPrototype.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/runtime/StructureChain.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/runtime/Structure.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/runtime/TimeoutChecker.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/runtime/UString.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/runtime/UStringImpl.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/wtf/ByteArray.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/wtf/DateMath.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/wtf/dtoa.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/wtf/FastMalloc.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/wtf/HashTable.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/wtf/qt/ThreadingQt.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/wtf/Threading.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/wtf/TypeTraits.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/wtf/unicode/UTF8.cpp
)

katie_generate_misc("${SCRIPT_HEADERS}" QtScript)
katie_generate_public("${SCRIPT_PUBLIC_HEADERS}" QtScript)
katie_generate_package(KtScript "KtCore")
katie_setup_target(KtScript ${SCRIPT_SOURCES} ${SCRIPT_HEADERS})

add_library(KtScript ${KATIE_TYPE} ${KtScript_SOURCES})
target_link_libraries(KtScript ${EXTRA_SCRIPT_LIBS})
set_target_properties(KtScript PROPERTIES
    VERSION ${KATIE_MAJOR}.${KATIE_MINOR}
    SOVERSION ${KATIE_VERSION}
    EXPORT_NAME Script
)

install(
    TARGETS KtScript
    EXPORT KatieLibraryTargets ${INSTALL_TARGETS_DEFAULT_ARGS}
    DESTINATION ${KATIE_LIBRARIES_RELATIVE}
    COMPONENT Runtime
)

install(
    DIRECTORY ${CMAKE_BINARY_DIR}/include/QtScript
    DESTINATION ${KATIE_HEADERS_RELATIVE}
    COMPONENT Devel
)

katie_optimize_headers(${KATIE_HEADERS_FULL}/QtScript)
