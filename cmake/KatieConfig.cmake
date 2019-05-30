# - Config file for the Katie package
#
# It defines relevant to other projects variables:
#
#  KATIE_FOUND                  - wheather the project is found
#  KATIE_DEFINITIONS            - all preprocessor definitions
#  KATIE_INCLUDES               - all headers directories
#  KATIE_LIBRARIES              - all component libraries
#  KATIE_<COMPONENT>_FOUND      - wheather component is found
#  KATIE_<COMPONENT>_INCLUDES   - component headers directories for e.g. QtCore
#  KATIE_<COMPONENT>_LIBRARIES  - component libraries to link against e.g. QtCore
#  KATIE_<TOOL>                 - path to tool e.g. moc
#  KATIE_<DATA>_PATH            - path to different data type files e.g. translations
#  KATIE_MKSPECS_DIR            - directory to mkspecs
#  KATIE_TYPE                   - build type, either SHARED or STATIC
#
# As well as some that are unilkely to be needed outside the project:
#
#  KATIE_TOOLS_SUFFIX           - tools suffix used when the project was build
#  KATIE_COMPONENTS             - components that were build
#  KATIE_TOOLS                  - tools that were build

# avoiding use of return()
if(NOT KATIE_FOUND)
    set(KATIE_FOUND TRUE)

    # Stored configuration paths that vendors usually change
    set(KATIE_PREFIX_PATH "@KATIE_PREFIX_FULL@")
    set(KATIE_HEADERS_PATH "@KATIE_HEADERS_FULL@")
    set(KATIE_LIBRARIES_PATH "@KATIE_LIBRARIES_FULL@")
    set(KATIE_BINARIES_PATH "@KATIE_BINARIES_FULL@")
    set(KATIE_PLUGINS_PATH "@KATIE_PLUGINS_FULL@")
    set(KATIE_IMPORTS_PATH "@KATIE_IMPORTS_FULL@")
    set(KATIE_DATA_PATH "@KATIE_DATA_FULL@")
    set(KATIE_TRANSLATIONS_PATH "@KATIE_TRANSLATIONS_FULL@")
    set(KATIE_SETTINGS_PATH "@KATIE_SETTINGS_FULL@")
    set(KATIE_LDCONF_PATH "@KATIE_LDCONF_FULL@")
    set(KATIE_PROFILE_PATH "@KATIE_PROFILE_FULL@")
    set(KATIE_MAN_PATH "@KATIE_MAN_FULL@")
    set(KATIE_CMAKE_PATH "@KATIE_CMAKE_FULL@")
    set(KATIE_APPLICATIONS_PATH "@KATIE_APPLICATIONS_FULL@")
    set(KATIE_PIXMAPS_PATH "@KATIE_PIXMAPS_FULL@")
    set(KATIE_TOOLS_SUFFIX "@KATIE_TOOLS_SUFFIX@")

    # Compute paths
    get_filename_component(KATIE_CMAKE_DIR "${CMAKE_CURRENT_LIST_FILE}" DIRECTORY)
    get_filename_component(KATIE_PREFIX ${KATIE_CMAKE_DIR}/ ABSOLUTE)

    # Include definitions for IMPORTED targets
    include("${KATIE_CMAKE_DIR}/KatieLibraryTargets.cmake")
    include("${KATIE_CMAKE_DIR}/KatieBinaryTargets.cmake")
    # Include version info, required for the Qt4 compatibility setup
    include("${KATIE_CMAKE_DIR}/KatieConfigVersion.cmake")

    # These are pre-set and dynamically set variables, some use IMPORTED targets
    set(KATIE_DEFINITIONS @KATIE_DEFINITIONS@)
    set(KATIE_INCLUDES "${KATIE_HEADERS_PATH}")
    set(KATIE_LIBRARIES)
    set(KATIE_COMPONENTS @KATIE_COMPONENTS@)
    set(KATIE_TOOLS @KATIE_TOOLS@)
    set(KATIE_MKSPECS_DIR "${KATIE_CMAKE_DIR}/mkspecs")
    set(KATIE_TYPE @KATIE_TYPE@)

    foreach(component ${KATIE_COMPONENTS})
        string(TOUPPER ${component} uppercomp)
        set(KATIE_INCLUDES ${KATIE_INCLUDES} "${KATIE_HEADERS_PATH}/Qt${component}")
        set(KATIE_${uppercomp}_INCLUDES "${KATIE_HEADERS_PATH}/Qt${component}")
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

    # Platform specific stuff and some tests
    include("${KATIE_MKSPECS_DIR}/mkspecs.cmake")

    if(NOT "${KATIE_FIND_QUIETLY}")
        message(STATUS "Found Katie version: ${KATIE_VERSION}")
    endif()

    # Qt4 compatibility by default, covers most cases
    if(NOT KATIE_COMPAT EQUAL FALSE AND NOT KATIE_COMPAT EQUAL OFF AND NOT KATIE_COMPAT_IS_SET)
        # precaution against an attempt to redefine imported targets which will fail
        set(KATIE_COMPAT_IS_SET CACHE BOOL TRUE "")
        message(STATUS "Setting up Qt4 compatibility via Katie")

        set(Qt4_FOUND TRUE)
        set(QT_FOUND TRUE)
        set(QT4_FOUND TRUE)
        set(QTVERSION ${KATIE_VERSION})
        set(QT_VERSION ${KATIE_VERSION})
        set(QT_VERSION_MAJOR ${KATIE_VERSION_MAJOR})
        set(QT_VERSION_MINOR ${KATIE_VERSION_MINOR})
        set(QT_VERSION_PATCH ${KATIE_VERSION_VERSION_MICRO})
        set(QT_INCLUDES ${KATIE_INCLUDES})
        set(QT_INCLUDE_DIR ${KATIE_INCLUDES})
        set(QT4_INCLUDE_DIR ${KATIE_INCLUDES})
        set(QT_LIBRARIES ${KATIE_LIBRARIES})
        set(QT_USE_FILE "${KATIE_CMAKE_DIR}/Qt4UseFile.cmake")
        set(QT_MKSPECS_DIR "${KATIE_MKSPECS_DIR}")

        # those are exceptions because they have "q" prefix which the macros from Qt4Macros do not expect
        set(QT_DBUSXML2CPP_EXECUTABLE "${KATIE_BINARIES_PATH}/qdbusxml2cpp${KATIE_TOOLS_SUFFIX}${CMAKE_EXECUTABLE_SUFFIX}")
        set(QT_DBUSCPP2XML_EXECUTABLE "${KATIE_BINARIES_PATH}/qdbuscpp2xml${KATIE_TOOLS_SUFFIX}${CMAKE_EXECUTABLE_SUFFIX}")

        if(KATIE_TYPE STREQUAL SHARED)
            set(_libsuffix "${CMAKE_SHARED_LIBRARY_SUFFIX}")
        elseif(KATIE_TYPE STREQUAL MODULE)
            set(_libsuffix "${CMAKE_SHARED_MODULE_SUFFIX}")
        else()
            set(_libsuffix "${CMAKE_STATIC_LIBRARY_SUFFIX}")
        endif()
        foreach(tool ${KATIE_TOOLS})
            add_executable(Qt4::${tool} IMPORTED)
            set_property(
                TARGET Qt4::${tool}
                PROPERTY
                IMPORTED_LOCATION "${KATIE_BINARIES_PATH}/${tool}${KATIE_TOOLS_SUFFIX}${CMAKE_EXECUTABLE_SUFFIX}"
            )
            string(TOUPPER ${tool} uppertool)
            set(QT_${uppertool}_EXECUTABLE "${KATIE_BINARIES_PATH}/${tool}${KATIE_TOOLS_SUFFIX}${CMAKE_EXECUTABLE_SUFFIX}")
        endforeach()

        foreach(component ${KATIE_COMPONENTS})
            add_library(Qt4::Qt${component} ${KATIE_TYPE} IMPORTED)
            set_property(
                TARGET Qt4::Qt${component}
                PROPERTY
                IMPORTED_LOCATION "${KATIE_LIBRARIES_PATH}/libKt${component}${_libsuffix}"
            )
        endforeach()

        # bad assumption, Qt4 module does it
        if(UNIX)
            set(Q_WS_X11 TRUE)
            find_package(X11 REQUIRED)
        endif()

        set(KATIE_DEFINITIONS ${KATIE_DEFINITIONS} -DQT_NAMESPACE_COMPAT -DQT_FOREACH_COMPAT)
        set(QT_DEFINITIONS ${KATIE_DEFINITIONS})
        set(QT4_DEFINITIONS ${KATIE_DEFINITIONS})

        foreach(component ${KATIE_COMPONENTS})
            string(TOUPPER ${component} uppercomp)
            set(QT_QT${uppercomp}_FOUND ${KATIE_${uppercomp}_FOUND})
            set(QT4_QT${uppercomp}_FOUND ${KATIE_${uppercomp}_FOUND})
            set(QT_QT${uppercomp}_LIBRARY "${KATIE_${uppercomp}_LIBRARIES}")
            set(QT4_QT${uppercomp}_LIBRARY "${KATIE_${uppercomp}_LIBRARIES}")
            set(QT_QT${uppercomp}_LIBRARIES "${KATIE_${uppercomp}_LIBRARIES}")
            set(QT4_QT${uppercomp}_LIBRARIES "${KATIE_${uppercomp}_LIBRARIES}")
            set(QT_QT${uppercomp}_INCLUDE_DIR "${KATIE_HEADERS_PATH}" "${KATIE_${uppercomp}_INCLUDES}")
            set(QT4_QT${uppercomp}_INCLUDE_DIR "${KATIE_HEADERS_PATH}" "${KATIE_${uppercomp}_INCLUDES}")
            set(QT_QT${uppercomp}_DEFINITIONS "-DQT_${uppercomp}_LIB ${KATIE_DEFINITIONS}")
            set(QT4_QT${uppercomp}_DEFINITIONS "-DQT_${uppercomp}_LIB ${KATIE_DEFINITIONS}")
        endforeach()

        # not much to be done about those since they are complex and many projects rely on them
        include(Qt4Macros)
        include(MacroAddFileDependencies)
    endif()
endif()
