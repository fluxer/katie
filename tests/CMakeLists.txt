# just to make catching binaries other than tests easier in case maintainer forgot to specify
# output directory or something strange happens, no files should be placed there
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/tests")

include_directories(
    ${CMAKE_BINARY_DIR}/include
    ${CMAKE_BINARY_DIR}/privateinclude
    ${CMAKE_BINARY_DIR}/include/QtCore
    ${CMAKE_BINARY_DIR}/include/QtGui
    ${CMAKE_BINARY_DIR}/include/QtDBus
    ${CMAKE_BINARY_DIR}/include/QtDeclarative
    ${CMAKE_BINARY_DIR}/include/QtDesigner
    ${CMAKE_BINARY_DIR}/include/QtNetwork
    ${CMAKE_BINARY_DIR}/include/QtSql
    ${CMAKE_BINARY_DIR}/include/QtSvg
    ${CMAKE_BINARY_DIR}/include/QtXml
    ${CMAKE_BINARY_DIR}/include/QtScript
    ${CMAKE_BINARY_DIR}/include/QtScriptTools
    ${CMAKE_BINARY_DIR}/include/QtTest
    ${CMAKE_BINARY_DIR}/include/QtUiTools
)

# FIXME: most tests are not namespaces aware
add_definitions(-DQT_NAMESPACE_COMPAT)

set(AUTOCMAKEFILES)
set(BENCHMARKSCMAKEFILES)
if(KATIE_TESTS)
    file(GLOB_RECURSE AUTOCMAKEFILES "${CMAKE_CURRENT_SOURCE_DIR}/auto/*/CMakeLists.txt")
endif()
if(KATIE_BENCHMARKS)
    file(GLOB_RECURSE BENCHMARKSCMAKEFILES "${CMAKE_CURRENT_SOURCE_DIR}/benchmarks/*/CMakeLists.txt")
endif()
foreach(cmakefile ${AUTOCMAKEFILES} ${BENCHMARKSCMAKEFILES})
    get_filename_component(dirname ${cmakefile} PATH)
    # message(STATUS "Adding sub-directory: ${dirname}")
    add_subdirectory(${dirname})
endforeach()
