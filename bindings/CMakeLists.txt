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
    ${SHIBOKEN_INCLUDES}
    ${PYTHON_INCLUDE_DIRS}
)

remove_definitions(
    -DQT_ASCII_CAST_WARNINGS
)

set(PyKtCore_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/KtCore/katie_qbytearray_wrapper.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/KtCore/katie_qbytearray_wrapper.h
    ${CMAKE_CURRENT_SOURCE_DIR}/KtCore/katie_qchar_wrapper.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/KtCore/katie_qchar_wrapper.h
    ${CMAKE_CURRENT_SOURCE_DIR}/KtCore/katie_qcoreapplication_wrapper.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/KtCore/katie_qcoreapplication_wrapper.h
    ${CMAKE_CURRENT_SOURCE_DIR}/KtCore/katie_qobject_wrapper.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/KtCore/katie_qobject_wrapper.h
    ${CMAKE_CURRENT_SOURCE_DIR}/KtCore/katie_qstring_wrapper.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/KtCore/katie_qstring_wrapper.h
    ${CMAKE_CURRENT_SOURCE_DIR}/KtCore/katie_qtextboundaryfinder_wrapper.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/KtCore/katie_qtextboundaryfinder_wrapper.h
    ${CMAKE_CURRENT_SOURCE_DIR}/KtCore/katie_qlatin1char_wrapper.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/KtCore/katie_qlatin1char_wrapper.h
    ${CMAKE_CURRENT_SOURCE_DIR}/KtCore/katie_qlatin1string_wrapper.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/KtCore/katie_qlatin1string_wrapper.h
    ${CMAKE_CURRENT_SOURCE_DIR}/KtCore/ktcore_module_wrapper.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/KtCore/ktcore_python.h
)

katie_generate_bindings(KtCore ${PyKtCore_SOURCES})

add_library(PyKtCore MODULE ${PyKtCore_SOURCES})
target_link_libraries(PyKtCore
    KtCore
    ${SHIBOKEN_LIBRARIES}
    ${PYTHON_LIBRARY}
)
set_target_properties(PyKtCore PROPERTIES PREFIX "")
set_target_properties(PyKtCore PROPERTIES OUTPUT_NAME KtCore)

install(
    TARGETS PyKtCore
    DESTINATION ${KATIE_PYTHON_RELATIVE}
)