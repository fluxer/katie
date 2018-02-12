
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bindings")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bindings")

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
    ${PYTHON_INCLUDE_DIRS}
)

set_property(SOURCE KtCore.i PROPERTY CPLUSPLUS ON)
swig_add_module(KtCore python KtCore.i)
swig_link_libraries(KtCore KtCore)

set_property(SOURCE KtGui.i PROPERTY CPLUSPLUS ON)
swig_add_module(KtGui python KtGui.i)
swig_link_libraries(KtGui KtGui)


install(
    FILES
    ${CMAKE_BINARY_DIR}/bindings/KtCore.py
    ${CMAKE_BINARY_DIR}/bindings/KtGui.py
    DESTINATION ${KATIE_PYTHON_RELATIVE}
)

install(
    TARGETS _KtCore _KtGui
    DESTINATION ${KATIE_PYTHON_RELATIVE}
)