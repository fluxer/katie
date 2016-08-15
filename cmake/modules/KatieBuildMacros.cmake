# Copyright (c) 2015, Ivailo Monev, <xakepa10@gmail.com>
# Redistribution and use is allowed according to the terms of the BSD license.

if(CMAKE_CROSSCOMPILING)
    # TODO: switch to release tarball
    include(ExternalProject)
    ExternalProject_Add(
        external_katie
        GIT_REPOSITORY https://github.com/fluxer/katie
        SOURCE_DIR ${CMAKE_BINARY_DIR}/external_katie
        BUILD_IN_SOURCE 1
        INSTALL_COMMAND "" # disable installation
        UPDATE_DISCONNECTED 1 # it always fails
        CMAKE_ARGS -DKATIE_BOOTSTRAP=TRUE -Wno-dev
    )
    # TODO: native executable suffix
    set(KATIE_UIC "${CMAKE_BINARY_DIR}/external_katie/bin/uic")
    set(KATIE_RCC "${CMAKE_BINARY_DIR}/external_katie/bin/rcc")
    set(KATIE_MOC "${CMAKE_BINARY_DIR}/external_katie/bin/moc")
    set(KATIE_LRELEASE "${CMAKE_BINARY_DIR}/external_katie/bin/lrelease")
    # NOTE: ExternalProject_Add_StepDependencies
else()
    set(KATIE_UIC "uic")
    set(KATIE_RCC "rcc")
    set(KATIE_MOC "bootstrap_moc")
    set(KATIE_LRELEASE "lrelease")
endif()
set(KATIE_QDBUSXML2CPP "qdbusxml2cpp")

macro(KATIE_WARNING MESSAGESTR)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        message(AUTHOR_WARNING "${MESSAGESTR} ${ARGN}")
    endif()
endmacro()

macro(KATIE_GENERATE_PUBLIC PUBLIC_INCLUDES SUBDIR)
    set(metaout ${CMAKE_BINARY_DIR}/include/${SUBDIR}/${SUBDIR})
    set(metadata "#ifndef Qt${SUBDIR}_META_H\n#define Qt${SUBDIR}_META_H\n\n")
    foreach(pubheader ${PUBLIC_INCLUDES})
        string(TOLOWER ${pubheader} pubname)
        get_filename_component(pubname ${pubname} NAME_WE)
        set(pubout ${CMAKE_BINARY_DIR}/include/${SUBDIR}/${pubheader})
        set(pubcheck ${CMAKE_BINARY_DIR}/include/${SUBDIR}/${pubname}.h)
        if(NOT EXISTS ${pubout})
            file(WRITE ${pubout} "#include <${pubname}.h>\n")
        endif()
        if(NOT EXISTS ${pubcheck})
            katie_warning("Bogus public header: ${pubheader} (${pubcheck})")
        # qttestgui is exception because it requires explicit gui component linkage
        elseif(NOT "${pubname}" STREQUAL "qttestgui")
            set(metadata "${metadata}#include <${SUBDIR}/${pubname}.h>\n")
        endif()
    endforeach(pubheader)
    set(metadata "${metadata}\n#endif\n")
    file(WRITE ${metaout} "${metadata}")
endmacro()

macro(KATIE_GENERATE_MISC MISC_INCLUDES SUBDIR)
    foreach(mischeader ${MISC_INCLUDES})
        get_filename_component(headername ${mischeader} NAME)
        if("${headername}" MATCHES "(_p.h)")
            set(headout ${CMAKE_BINARY_DIR}/privateinclude/${SUBDIR}/${headername})
        else()
            set(headout ${CMAKE_BINARY_DIR}/include/${SUBDIR}/${headername})
        endif()
        configure_file(${mischeader} ${headout} COPYONLY)
    endforeach(mischeader)
endmacro()

macro(KATIE_GENERATE_OBSOLETE OBSOLETE_INCLUDE SUBDIR REDIRECT)
    set(pubout ${CMAKE_BINARY_DIR}/include/${SUBDIR}/${OBSOLETE_INCLUDE})
    if(NOT EXISTS "${pubout}")
        file(WRITE "${pubout}" "#include <${SUBDIR}/${REDIRECT}>\n")
    endif()
endmacro()

macro(KATIE_GENERATE_PACKAGE FORTARGET REQUIRES)
    if(UNIX)
        string(REPLACE "Kt" "Qt" PACKAGE_FAKE "${FORTARGET}")
        set(PACKAGE_NAME ${FORTARGET})
        set(PACKAGE_REQUIRES ${REQUIRES})
        configure_file(
            ${CMAKE_SOURCE_DIR}/cmake/pkgconfig.cmake
            ${CMAKE_BINARY_DIR}/pkgconfig/${FORTARGET}.pc
        )
        install(
            FILES ${CMAKE_BINARY_DIR}/pkgconfig/${FORTARGET}.pc
            DESTINATION ${QT_LIBRARIES_PATH}/pkgconfig
        )
    endif()
endmacro()

# the purpose of this function is to ensure that (1) the output string is not
# null so that when it is passed to another function/macro it does not complain
# about inproper number of arguments and (2) it joins the input which if quoted
# has semicolons in it (if it is a list) that the sub-command (e.g. gcc) can
# not handle. that's a dirty hack to support gcc and clang at the same time
# along with custom target COMPILE_FLAGS/LINK_FLAGS without doing compiler
# checks all over the place.
function(KATIE_FIXUP_STRING INSTR OUTSTR)
    string(STRIP "${INSTR}" instrtrimmed)
    if("${instrtrimmed}" STREQUAL "")
        set(${OUTSTR} " " PARENT_SCOPE)
    else()
        string(REPLACE ";" " " modstring "${INSTR}")
        set(${OUTSTR} "${modstring}" PARENT_SCOPE)
    endif()
endfunction()

# since KATIE_SETUP_FLAGS() applies KATIE_CXXFLAGS and some compilers choke
# on them just filter them instead of using KATIE_CFLAGS
function(KATIE_FILTER_FLAGS INSTR OUTSTR)
    set(KATIE_CFILTER "-fvisibility-inlines-hidden" CACHE STRING "C flags to filter from the C++ flags")
    set(modstring "${INSTR}")
    foreach(cxxflag ${KATIE_CFILTER})
        string(REPLACE "${cxxflag}" " " modstring "${modstring}")
    endforeach()
    set(${OUTSTR} "${modstring}" PARENT_SCOPE)
endfunction()

function(KATIE_SETUP_FLAGS)
    katie_fixup_string("${KATIE_CXXFLAGS}" KATIE_CXXFLAGS)
    katie_fixup_string("${KATIE_LDFLAGS}" KATIE_LDFLAGS)
    katie_filter_flags("${KATIE_CXXFLAGS}" KATIE_CFLAGS)

    if(ARGN)
        foreach(target ${ARGN})
            set_target_properties(${target} PROPERTIES
                COMPILE_FLAGS "${KATIE_CXXFLAGS}"
                LINK_FLAGS "${KATIE_LDFLAGS}"
            )
        endforeach()
    else()
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${KATIE_CFLAGS}" PARENT_SCOPE)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${KATIE_CXXFLAGS}" PARENT_SCOPE)
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${KATIE_LDFLAGS}" PARENT_SCOPE)
        set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${KATIE_LDFLAGS}" PARENT_SCOPE)
        set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} ${KATIE_LDFLAGS}" PARENT_SCOPE)
    endif()
endfunction()

macro(KATIE_SETUP_OBJECT FORTARGET)
    get_target_property(targets_pic ${FORTARGET} POSITION_INDEPENDENT_CODE)
    if(CMAKE_POSITION_INDEPENDENT_CODE OR targets_pic)
        foreach(objtarget ${ARGN})
            set_target_properties(${objtarget} PROPERTIES
                POSITION_INDEPENDENT_CODE TRUE
            )
        endforeach()
    endif()
endmacro()

# a function to change full installation paths to relative so that CPack
# generators do not choke, still paths must contain a string of some sort - if
# they are null even quoting them will not help and CMake will complain that
# not enought arguments have been passed to install() for DESTINATION
function(KATIE_SETUP_PATHS)
    set(instpaths
        _PREFIX _HEADERS _LIBRARIES _BINARIES _PLUGINS _IMPORTS _DATA
        _TRANSLATIONS _DOCUMENTATION _EXAMPLES _DEMOS _SETTINGS
    )
    set(miscpaths
        CMAKE_ LDCONF_ PROFILE_ MAN_ APPLICATIONS_ PIXMAPS_
    )
    foreach(instpath ${instpaths})
        string(REGEX REPLACE ".*${CMAKE_INSTALL_PREFIX}/" "" modpath "${QT${instpath}_PATH}")
        string(REGEX REPLACE ".*${CMAKE_INSTALL_PREFIX}" "" modpath "${modpath}")
        set(QT${instpath}_PATH "${modpath}" PARENT_SCOPE)
        # message(STATUS "QT${instpath}_PATH: ${modpath}")
    endforeach()

    foreach(instpath ${miscpaths})
        string(REGEX REPLACE ".*${CMAKE_INSTALL_PREFIX}/" "" modpath "${${instpath}INSTALL_PATH}")
        string(REGEX REPLACE ".*${CMAKE_INSTALL_PREFIX}" "" modpath "${modpath}")
        set(${instpath}INSTALL_PATH "${modpath}" PARENT_SCOPE)
        # message(STATUS "${instpath}INSTALL_PATH: ${modpath}")
    endforeach()
endfunction()

macro(KATIE_OPTIMIZE_HEADERS DIR)
    find_program(unifdef NAMES unifdef)
    if(unifdef)
        install(
            CODE "set(UNIFDEF_EXECUTABLE \"${unifdef}\")"
            CODE "set(HEADERS_DIRECTORY \"${DIR}\")"
            CODE "set(HEADERS_DEFINITIONS \"${ARGN}\")"
            SCRIPT "${CMAKE_SOURCE_DIR}/cmake/modules/OptimizeHeaders.cmake"
        )
    else()
        get_filename_component(basename ${DIR} NAME)
        katie_warning("unifdef not installed, cannot optimize headers for: ${basename}")
    endif()
endmacro()

macro(KATIE_TEST TESTNAME TESTSOURCES)
    katie_resources(${TESTSOURCES} ${ARGN})

    add_executable(${TESTNAME} ${TESTSOURCES} ${ARGN})

    # TODO: make GUI access optional, it is required by many tests so it should still be default
    target_link_libraries(${TESTNAME} KtCore KtGui KtTest)
    if(KATIE_PLATFORM MATCHES "(win32|wince)")
        target_link_libraries(${TESTNAME} KtMain)
    endif()
    target_compile_definitions(
        ${TESTNAME} PRIVATE
        -DSRCDIR="${CMAKE_CURRENT_SOURCE_DIR}/"
        -DQT_GUI_LIB
    )
    set_target_properties(
        ${TESTNAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )

    add_test(
        NAME ${TESTNAME}
        COMMAND ${TESTNAME}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )
endmacro()
