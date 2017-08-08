set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/tests")
set(CTEST_ENVIRONMENT "QTDIR=${CMAKE_SOURCE_DIR}")

include_directories(
    ${CMAKE_BINARY_DIR}/include
    ${CMAKE_BINARY_DIR}/privateinclude
    ${CMAKE_BINARY_DIR}/include/QtCore
    ${CMAKE_BINARY_DIR}/privateinclude/QtCore
    ${CMAKE_BINARY_DIR}/include/QtGui
    ${CMAKE_BINARY_DIR}/privateinclude/QtGui
    ${CMAKE_BINARY_DIR}/include/QtDBus
    ${CMAKE_BINARY_DIR}/privateinclude/QtDBus
    ${CMAKE_BINARY_DIR}/include/QtDeclarative
    ${CMAKE_BINARY_DIR}/privateinclude/QtDeclarative
    ${CMAKE_BINARY_DIR}/include/QtDesigner
    ${CMAKE_BINARY_DIR}/privateinclude/QtDesigner
    ${CMAKE_BINARY_DIR}/include/QtNetwork
    ${CMAKE_BINARY_DIR}/privateinclude/QtNetwork
    ${CMAKE_BINARY_DIR}/include/QtOpenGL
    ${CMAKE_BINARY_DIR}/privateinclude/QtOpenGL
    ${CMAKE_BINARY_DIR}/include/QtSql
    ${CMAKE_BINARY_DIR}/privateinclude/QtSql
    ${CMAKE_BINARY_DIR}/include/QtSvg
    ${CMAKE_BINARY_DIR}/privateinclude/QtSvg
    ${CMAKE_BINARY_DIR}/include/QtXml
    ${CMAKE_BINARY_DIR}/privateinclude/QtXml
    ${CMAKE_BINARY_DIR}/include/QtXml
    ${CMAKE_BINARY_DIR}/privateinclude/QtXml
    ${CMAKE_BINARY_DIR}/include/QtScript
    ${CMAKE_BINARY_DIR}/privateinclude/QtScript
    ${CMAKE_BINARY_DIR}/include/QtScriptTools
    ${CMAKE_BINARY_DIR}/privateinclude/QtScriptTools
    ${CMAKE_BINARY_DIR}/include/QtTest
    ${CMAKE_BINARY_DIR}/privateinclude/QtTest
    ${CMAKE_BINARY_DIR}/include/QtUiTools
    ${CMAKE_BINARY_DIR}/privateinclude/QtUiTools
)

file(GLOB_RECURSE CMAKEFILES "${CMAKE_CURRENT_SOURCE_DIR}/*/CMakeLists.txt")
foreach(cmakefile ${CMAKEFILES})
    get_filename_component(dirname ${cmakefile} PATH)
    # message(STATUS "Adding sub-directory: ${dirname}")
    add_subdirectory(${dirname})
endforeach()
