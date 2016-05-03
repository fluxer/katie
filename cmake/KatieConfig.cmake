# - Config file for the Katie package
#
# It defines the following variables:
#
#  KATIE_DEFINITIONS            - all preprocessor definitions
#  KATIE_INCLUDES               - all headers directories
#  KATIE_LIBRARIES              - all component libraries
#  KATIE_<COMPONENT>_INCLUDES   - component headers directories for e.g. QtCore
#  KATIE_<COMPONENT>_LIBRARIES  - component libraries to link against e.g. QtCore
#  KATIE_<TOOL>                 - path to tool e.g. moc
#

set(KATIE_FOUND TRUE)

# Compute paths
get_filename_component(KATIE_CMAKE_DIR "${CMAKE_CURRENT_LIST_FILE}" DIRECTORY)
get_filename_component(KATIE_PREFIX ${KATIE_CMAKE_DIR}/ ABSOLUTE)

# Include definitions for IMPORTED targets
include("${KATIE_CMAKE_DIR}/KatieLibraryTargets.cmake")
include("${KATIE_CMAKE_DIR}/KatieBinaryTargets.cmake")

# These are pre-set and dynamically set variables, some use IMPORTED targets
set(KATIE_DEFINITIONS @KATIE_DEFINITIONS@)
set(KATIE_INCLUDES "@QT_HEADERS_PATH@")
set(KATIE_LIBRARIES)
set(KATIE_COMPONENTS @KATIE_COMPONENTS@)
set(KATIE_TOOLS @KATIE_TOOLS@)
set(KATIE_MKSPECS_DIR "${KATIE_CMAKE_DIR}/mkspecs")
set(KATIE_MKSPECS "${KATIE_MKSPECS_DIR}/mkspecs.cmake")

foreach(component ${KATIE_COMPONENTS})
    string(TOUPPER ${component} uppercomp)
    if(uppercomp STREQUAL "MAIN")
        set(KATIE_INCLUDES ${KATIE_INCLUDES} "@QT_HEADERS_PATH@")
        set(KATIE_${uppercomp}_INCLUDES "@QT_HEADERS_PATH@")
    else()
        set(KATIE_INCLUDES ${KATIE_INCLUDES} "@QT_HEADERS_PATH@/Qt${component}")
        set(KATIE_${uppercomp}_INCLUDES "@QT_HEADERS_PATH@/Qt${component}")
    endif()
    set(KATIE_LIBRARIES ${KATIE_LIBRARIES} Katie::${component})

    set(KATIE_${uppercomp}_LIBRARIES Katie::${component})
    set(KATIE_${uppercomp}_FOUND TRUE)
endforeach()

foreach(tool ${KATIE_TOOLS})
    string(TOUPPER ${tool} uppertool)
    set(KATIE_${uppertool} Katie::${tool})
endforeach()

# Include macros, tools variables must be set before that
include("${KATIE_CMAKE_DIR}/KatieMacros.cmake")

# Platform specifiec stuff and some tests
if(EXISTS ${KATIE_MKSPECS})
    include(${KATIE_MKSPECS})
else()
    message(SEND_ERROR "mkspecs file does not exists, it is set to: ${KATIE_MKSPECS}")
endif()

# The stored configuration paths that vendors usually change
set(LDCONF_INSTALL_PATH "@LDCONF_INSTALL_PATH@")
set(PROFILE_INSTALL_PATH "@PROFILE_INSTALL_PATH@")
set(MAN_INSTALL_PATH "@MAN_INSTALL_PATH@")
set(CMAKE_INSTALL_PATH "@CMAKE_INSTALL_PATH@")
set(APPLICATIONS_INSTALL_PATH "@APPLICATIONS_INSTALL_PATH@")
set(PIXMAPS_INSTALL_PATH "@PIXMAPS_INSTALL_PATH@")

set(QT_PREFIX_PATH "@QT_PREFIX_PATH@")
set(QT_HEADERS_PATH "@QT_HEADERS_PATH@")
set(QT_LIBRARIES_PATH "@QT_LIBRARIES_PATH@")
set(QT_BINARIES_PATH "@QT_BINARIES_PATH@")
set(QT_PLUGINS_PATH "@QT_PLUGINS_PATH@")
set(QT_IMPORTS_PATH "@QT_IMPORTS_PATH@")
set(QT_DATA_PATH "@QT_DATA_PATH@")
set(QT_TRANSLATIONS_PATH "@QT_TRANSLATIONS_PATH@")
set(QT_DOCUMENTATION_PATH "@QT_DOCUMENTATION_PATH@")
set(QT_EXAMPLES_PATH "@QT_EXAMPLES_PATH@")
set(QT_DEMOS_PATH "@QT_DEMOS_PATH@")
set(QT_SETTINGS_PATH "@QT_SETTINGS_PATH@")

if(NOT "${KATIE_FIND_QUIETLY}")
    message(STATUS "Found Katie version: ${KATIE_VERSION}")
endif()

# Qt4 compatibility by default, not a silver bullet
if(NOT KATIE_COMPAT EQUAL FALSE AND NOT KATIE_COMPAT EQUAL OFF)
    message(STATUS "Setting up Qt4 compatibility via Katie")

    # FIXME: once used lists turn into strings, great..
    string(REPLACE " " ";" _TOOLS ${KATIE_TOOLS})
    string(REPLACE " " ";" _COMPONENTS ${KATIE_COMPONENTS})

    set(Qt4_FOUND TRUE)
    set(QT_FOUND TRUE)
    set(QT4_FOUND TRUE)
    set(QT_VERSION_MAJOR ${KATIE_MAJOR})
    set(QT_VERSION_MINOR ${KATIE_MINOR})
    set(QT_VERSION_PATCH ${KATIE_MICRO})
    set(QT_VERSION ${KATIE_VERSION})
    set(QT_INCLUDES ${KATIE_INCLUDES})
    set(QT_INCLUDE_DIR ${KATIE_INCLUDES})
    set(QT4_INCLUDE_DIR ${KATIE_INCLUDES})
    set(QT_LIBRARIES ${KATIE_LIBRARIES})
    set(QT_USE_FILE ${KATIE_MKSPECS})
    set(QT_MKSPECS_DIR ${KATIE_MKSPECS_DIR})

    set(_binsuffix "${CMAKE_EXECUTABLE_SUFFIX}")
    # those are exceptions because they have "q" prefix
    set(QT_DBUSXML2CPP_EXECUTABLE "@QT_BINARIES_PATH@/qdbusxml2cpp${_binsuffix}")
    set(QT_DBUSCPP2XML_EXECUTABLE "@QT_BINARIES_PATH@/qdbuscpp2xml${_binsuffix}")

    if(NOT KATIE_COMPAT_ISSET)
        set(KATIE_COMPAT_ISSET CACHE BOOL TRUE "")
        if(KATIE_TYPE STREQUAL SHARED)
            set(_libsuffix "${CMAKE_SHARED_LIBRARY_SUFFIX}")
        elseif(KATIE_TYPE STREQUAL MODULE)
            set(_libsuffix "${CMAKE_SHARED_MODULE_SUFFIX}")
        else()
            set(_libsuffix "${CMAKE_STATIC_LIBRARY_SUFFIX}")
        endif()
        foreach(tool ${_TOOLS})
            add_executable(Qt4::${tool} IMPORTED)
            set_property(
                TARGET Qt4::${tool}
                PROPERTY
                IMPORTED_LOCATION "@QT_BINARIES_PATH@/${tool}${_binsuffix}"
            )
            string(TOUPPER ${tool} uppertool)
            set(QT_${uppertool}_EXECUTABLE "@QT_BINARIES_PATH@/${tool}${_binsuffix}")
        endforeach()

        foreach(component ${_COMPONENTS})
            add_library(Qt4::Qt${component} ${KATIE_TYPE} IMPORTED)
            set_property(
                TARGET Qt4::Qt${component}
                PROPERTY
                IMPORTED_LOCATION "@QT_LIBRARIES_PATH@/libKt${component}${_libsuffix}"
            )
        endforeach()
    endif()

    # bad assumption, Qt4 module does it
    if(UNIX)
        set(Q_WS_X11 TRUE)
        find_package(X11 REQUIRED)
    endif()

    foreach(component ${_COMPONENTS})
        string(TOUPPER ${component} uppercomp)
        set(QT_QT${uppercomp}_FOUND ${KATIE_${uppercomp}_FOUND})
        set(QT4_QT${uppercomp}_FOUND ${KATIE_${uppercomp}_FOUND})
        set(QT_QT${uppercomp}_LIBRARY "${KATIE_${uppercomp}_LIBRARIES}")
        set(QT4_QT${uppercomp}_LIBRARY "${KATIE_${uppercomp}_LIBRARIES}")
        set(QT_QT${uppercomp}_LIBRARIES "${KATIE_${uppercomp}_LIBRARIES}")
        set(QT4_QT${uppercomp}_LIBRARIES "${KATIE_${uppercomp}_LIBRARIES}")
        set(QT_QT${uppercomp}_INCLUDE_DIR "@QT_HEADERS_PATH@" "${KATIE_${uppercomp}_INCLUDES}")
        set(QT4_QT${uppercomp}_INCLUDE_DIR "@QT_HEADERS_PATH@" "${KATIE_${uppercomp}_INCLUDES}")
        # TODO: component definitions
    endforeach()

    set(QT_DEFINITIONS ${KATIE_DEFINITIONS} -DQT_INCLUDE_COMPAT)

    # not much to be done about those..
    include(Qt4Macros)
    include(MacroAddFileDependencies)
endif()
