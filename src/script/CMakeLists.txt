add_definitions(
    -DQT_BUILD_SCRIPT_LIB
    -DJSC=QTJSC
    -Djscyyparse=qtjscyyparse
    -Djscyylex=qtjscyylex
    -Djscyyerror=qtjscyyerror
    -DWTF=QTWTF
    -DQT_NO_USING_NAMESPACE
    -DQLALR_NO_QSCRIPTGRAMMAR_DEBUG_INFO
    -DWTF_USE_JAVASCRIPTCORE_BINDINGS=1
    -DWTF_CHANGES=1
    # Avoid JSC C API functions being exported.
    -DJS_NO_EXPORT
    -DBUILDING_QT__
    -DBUILDING_JavaScriptCore
    -DBUILDING_WTF
)
set(EXTRA_SCRIPT_LIBS KtCore)
# TODO: fix std::auto_ptr warnings
set(KATIE_CXXFLAGS "${KATIE_CXXFLAGS} -Wno-deprecated-declarations")

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

set(SCRIPT_SOURCES
    ${SCRIPT_SOURCES}
    # Generated files, simply list them for JavaScriptCore
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/generated/Grammar.cpp
    # JSCore
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/API/JSBase.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/API/JSCallbackConstructor.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/API/JSCallbackFunction.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/API/JSCallbackObject.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/API/JSClassRef.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/API/JSContextRef.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/API/JSObjectRef.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/API/JSStringRef.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/API/JSValueRef.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/API/OpaqueJSString.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/assembler/ARMAssembler.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/assembler/MacroAssemblerARM.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/bytecode/CodeBlock.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/bytecode/JumpTable.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/bytecode/Opcode.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/bytecode/SamplingTool.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/bytecode/StructureStubInfo.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/bytecompiler/BytecodeGenerator.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/bytecompiler/NodesCodegen.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/debugger/DebuggerActivation.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/debugger/DebuggerCallFrame.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/debugger/Debugger.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/interpreter/CallFrame.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/interpreter/Interpreter.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/interpreter/RegisterFile.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/jit/ExecutableAllocatorPosix.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/jit/ExecutableAllocatorWin.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/jit/ExecutableAllocator.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/jit/JITArithmetic.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/jit/JITCall.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/jit/JIT.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/jit/JITOpcodes.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/jit/JITPropertyAccess.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/jit/JITStubs.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/parser/Lexer.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/parser/Nodes.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/parser/ParserArena.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/parser/Parser.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/runtime/ArgList.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/runtime/Arguments.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/runtime/ArrayConstructor.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/runtime/ArrayPrototype.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/runtime/BooleanConstructor.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/runtime/BooleanObject.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/runtime/BooleanPrototype.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/runtime/CallData.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/runtime/Collector.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/runtime/CommonIdentifiers.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/runtime/Completion.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/runtime/ConstructData.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/runtime/DateConstructor.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/runtime/DateConversion.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/runtime/DateInstance.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/runtime/DatePrototype.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/runtime/ErrorConstructor.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/runtime/Error.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/runtime/ErrorInstance.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/runtime/ErrorPrototype.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/runtime/ExceptionHelpers.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/runtime/Executable.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/runtime/FunctionConstructor.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/runtime/FunctionPrototype.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/runtime/GetterSetter.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/runtime/GlobalEvalFunction.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/runtime/Identifier.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/runtime/InitializeThreading.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/runtime/InternalFunction.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/runtime/JSActivation.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/runtime/JSAPIValueWrapper.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/runtime/JSArray.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/runtime/JSByteArray.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/runtime/JSCell.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/runtime/JSFunction.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/runtime/JSGlobalData.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/runtime/JSGlobalObject.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/runtime/JSGlobalObjectFunctions.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/runtime/JSImmediate.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/runtime/JSNotAnObject.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/runtime/JSNumberCell.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/runtime/JSObject.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/runtime/JSONObject.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/runtime/JSPropertyNameIterator.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/runtime/JSStaticScopeObject.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/runtime/JSString.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/runtime/JSValue.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/runtime/JSVariableObject.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/runtime/JSWrapperObject.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/runtime/LiteralParser.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/runtime/Lookup.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/runtime/MarkStackPosix.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/runtime/MarkStackWin.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/runtime/MarkStack.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/runtime/MathObject.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/runtime/NativeErrorConstructor.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/runtime/NativeErrorPrototype.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/runtime/NumberConstructor.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/runtime/NumberObject.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/runtime/NumberPrototype.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/runtime/ObjectConstructor.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/runtime/ObjectPrototype.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/runtime/Operations.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/runtime/PropertyDescriptor.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/runtime/PropertyNameArray.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/runtime/PropertySlot.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/runtime/PrototypeFunction.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/runtime/RegExpConstructor.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/runtime/RegExp.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/runtime/RegExpObject.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/runtime/RegExpPrototype.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/runtime/ScopeChain.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/runtime/SmallStrings.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/runtime/StringConstructor.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/runtime/StringObject.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/runtime/StringPrototype.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/runtime/StructureChain.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/runtime/Structure.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/runtime/TimeoutChecker.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/runtime/UString.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/runtime/UStringImpl.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/wtf/Assertions.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/wtf/ByteArray.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/wtf/CurrentTime.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/wtf/DateMath.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/wtf/dtoa.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/wtf/FastMalloc.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/wtf/HashTable.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/wtf/MainThread.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/wtf/qt/MainThreadQt.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/wtf/qt/ThreadingQt.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/wtf/RandomNumber.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/wtf/RefCountedLeakCounter.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/wtf/Threading.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/wtf/TypeTraits.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/wtf/unicode/CollatorDefault.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/wtf/unicode/UTF8.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/yarr/RegexCompiler.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/yarr/RegexInterpreter.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/yarr/RegexJIT.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/pcre/pcre_compile.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/pcre/pcre_exec.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/pcre/pcre_tables.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/pcre/pcre_ucp_searchfuncs.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/pcre/pcre_xclass.cpp
)

include_directories(
    ${CMAKE_BINARY_DIR}/include
    ${CMAKE_BINARY_DIR}/privateinclude
    ${CMAKE_BINARY_DIR}/include/QtCore
    ${CMAKE_BINARY_DIR}/privateinclude/QtCore
    ${CMAKE_BINARY_DIR}/include/QtGui
    ${CMAKE_BINARY_DIR}/privateinclude/QtGui
    ${CMAKE_BINARY_DIR}/include/QtScript
    ${CMAKE_BINARY_DIR}/privateinclude/QtScript
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/api
    ${CMAKE_CURRENT_SOURCE_DIR}/bridge
    ${CMAKE_CURRENT_SOURCE_DIR}/parser
    ${CMAKE_CURRENT_BINARY_DIR}/api
    ${CMAKE_CURRENT_BINARY_DIR}/bridge
    ${CMAKE_CURRENT_BINARY_DIR}/parser
    ${CMAKE_BINARY_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/wtf/qt
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/parser
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/bytecompiler
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/debugger
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/runtime
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/wtf
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/wtf/unicode
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/unicode
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/interpreter
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/jit
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/API
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/bytecode
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/assembler
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/generated
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/pcre
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/tmp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/yarr
)

if(KATIE_COMPILER STREQUAL "msvc")
    # Disable a few warnings on Windows.
    set(KATIE_CXXFLAGS
        ${KATIE_CXXFLAGS}
        -wd4396
        -wd4099
        -wd4291
        -wd4344
        -wd4503
        -wd4800
        -wd4819
        -wd4996
    )
endif()

if(KATIE_PLATFORM STREQUAL "mac")
    set(KATIE_LDFLAGS
        ${KATIE_LDFLAGS}
        -framework AppKit
    )
elseif(KATIE_PLATFORM MATCHES "(win32|wince)" AND KATIE_COMPILER STREQUAL "msvc")
    include_directories(
        ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/os-wince
        ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/os-win32
    )
elseif(KATIE_PLATFORM STREQUAL "integrity")
    set(KATIE_CXXFLAGS
        ${KATIE_CXXFLAGS}
        --diag_remark=236,82
    )
endif()

if(KATIE_PLATFORM STREQUAL "wince")
    set(SCRIPT_SOURCES
        ${SCRIPT_SOURCES}
        ${CMAKE_SOURCE_DIR}/src/3rdparty/ce-compat/ce_time.c
    )
    include_directories(
        ${CMAKE_SOURCE_DIR}/src/3rdparty/ce-compat
    )
    add_definitions(-DWINCEBASIC)
endif()

if(KATIE_PLATFORM MATCHES "(win32|wince)")
    set(EXTRA_SCRIPT_LIBS
        ${EXTRA_SCRIPT_LIBS}
        winmm
    )
endif()

set(SCRIPT_RESOURCES
    ${SCRIPT_RESOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/api/qscriptextensionplugin.h
    ${CMAKE_CURRENT_SOURCE_DIR}/api/qscriptengine.h
    ${CMAKE_CURRENT_SOURCE_DIR}/bridge/qscriptqobject_p.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/wtf/qt/MainThreadQt.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/javascriptcore/JavaScriptCore/wtf/qt/ThreadingQt.cpp
)

katie_generate_misc("${SCRIPT_HEADERS}" QtScript)
katie_generate_public("${SCRIPT_PUBLIC_HEADERS}" QtScript)
katie_resources(${SCRIPT_RESOURCES})
katie_setup_flags()

add_library(KtScript ${KATIE_TYPE} ${SCRIPT_SOURCES} ${SCRIPT_RESOURCES})
target_link_libraries(KtScript ${EXTRA_SCRIPT_LIBS})
set_target_properties(KtScript PROPERTIES
    VERSION ${KATIE_MAJOR}.${KATIE_MINOR}
    SOVERSION ${KATIE_VERSION}
    EXPORT_NAME Script
)

katie_generate_package(KtScript "KtCore")

install(
    TARGETS KtScript
    EXPORT KatieLibraryTargets ${INSTALL_TARGETS_DEFAULT_ARGS}
    DESTINATION ${QT_LIBRARIES_PATH}
)

install(
    DIRECTORY ${CMAKE_BINARY_DIR}/include/QtScript
    DESTINATION ${QT_HEADERS_PATH}
    COMPONENT Devel
)

katie_optimize_headers(${QT_HEADERS_PATH}/QtScript)
