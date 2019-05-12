# - Try to find Shiboken
# Once done this will define
#
#  SHIBOKEN_FOUND - system has Shiboken
#  SHIBOKEN_INCLUDES - the Shiboken include directory
#  SHIBOKEN_LIBRARIES - the libraries needed to use Shiboken
#  SHIBOKEN_BINARY - the binary needed to use Shiboken
#  SHIBOKEN_VERSION - the Shiboken version

if(SHIBOKEN_INCLUDES AND SHIBOKEN_LIBRARIES AND SHIBOKEN_BINARY)
    set(SHIBOKEN_FIND_QUIETLY TRUE)
endif()

include(FindPkgConfig)
pkg_check_modules(PC_SHIBOKEN QUIET shiboken2)

execute_process(
    COMMAND ${PKG_CONFIG_EXECUTABLE} --variable=includedir shiboken2
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    RESULT_VARIABLE pkg_result
    ERROR_VARIABLE pkg_error
    OUTPUT_VARIABLE shiboken_includedir
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

execute_process(
    COMMAND ${PKG_CONFIG_EXECUTABLE} --variable=python_include_dir shiboken2
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    RESULT_VARIABLE pkg_result
    ERROR_VARIABLE pkg_error
    OUTPUT_VARIABLE python_shiboken_include_dir
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

set(SHIBOKEN_INCLUDES
    ${shiboken_includedir}
    ${python_shiboken_include_dir}
)

execute_process(
    COMMAND ${PKG_CONFIG_EXECUTABLE} --libs shiboken2
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    RESULT_VARIABLE pkg_result
    ERROR_VARIABLE pkg_error
    OUTPUT_VARIABLE shiboken_libraries
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

set(SHIBOKEN_LIBRARIES)
foreach(lib ${shiboken_libraries})
    # message(STATUS "shiboken_libraries: ${lib}")
    set(SHIBOKEN_LIBRARIES
        ${SHIBOKEN_LIBRARIES}
        ${lib}
    )
endforeach()

execute_process(
    COMMAND ${PKG_CONFIG_EXECUTABLE} --variable=generator_location shiboken2
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    RESULT_VARIABLE pkg_result
    ERROR_VARIABLE pkg_error
    OUTPUT_VARIABLE SHIBOKEN_BINARY
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

execute_process(
    COMMAND ${PKG_CONFIG_EXECUTABLE} --modversion shiboken2
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    RESULT_VARIABLE pkg_result
    ERROR_VARIABLE pkg_error
    OUTPUT_VARIABLE SHIBOKEN_VERSION
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Shiboken
    VERSION_VAR SHIBOKEN_VERSION
    REQUIRED_VARS SHIBOKEN_INCLUDES SHIBOKEN_LIBRARIES SHIBOKEN_BINARY
)

mark_as_advanced(SHIBOKEN_INCLUDES SHIBOKEN_LIBRARIES SHIBOKEN_BINARY)
