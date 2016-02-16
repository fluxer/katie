# Copyright (c) 2015, Ivailo Monev, <xakepa10@gmail.com>
# Redistribution and use is allowed according to the terms of the BSD license.

if(CMAKE_CROSSCOMPILING)
    # TODO: switch to release tarball
    include(ExternalProject)
    ExternalProject_Add(
        external_katie
        GIT_REPOSITORY https://github.com/fluxer/katie
        SOURCE_DIR ${CMAKE_BINARY_DIR}/external_katie
        INSTALL_DIR ${CMAKE_BINARY_DIR}/external_katie_install
        BUILD_IN_SOURCE 1
        UPDATE_DISCONNECTED 1 # it always fails
        CMAKE_ARGS -DKATIE_BOOTSTRAP=TRUE -Wno-dev
        CMAKE_CACHE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_BINARY_DIR}/external_katie_install
    )
    # TODO: native executable suffix
    set(KATIE_UIC "${CMAKE_BINARY_DIR}/external_katie/bin/uic")
    set(KATIE_RCC "${CMAKE_BINARY_DIR}/external_katie/bin/rcc")
    set(KATIE_MOC "${CMAKE_BINARY_DIR}/external_katie/bin/moc")
    set(KATIE_LRELEASE "${CMAKE_BINARY_DIR}/external_katie/bin/lrelease")
else()
    set(KATIE_UIC "uic")
    set(KATIE_RCC "rcc")
    set(KATIE_MOC "bootstrap_moc")
    set(KATIE_LRELEASE "lrelease")
endif()
set(KATIE_QDBUSXML2CPP "qdbusxml2cpp")

macro(KATIE_GENERATE_MAP SUBDIR KEYWORD)
    execute_process(
        COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/scripts/genmap.py ${SUBDIR} ${KEYWORD}
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        RESULT_VARIABLE genmap_result
        ERROR_VARIABLE genmap_error
    )
    if(NOT genmap_result EQUAL 0)
        message(SEND_ERROR "${genmap_error}")
    endif()
endmacro()

set(CLASSMAPOUT ${CMAKE_BINARY_DIR}/include/qclass_lib_map.h)
file(WRITE ${CLASSMAPOUT} "")
macro(KATIE_GENERATE_PUBLIC PUBLIC_INCLUDES SUBDIR)
    set(metaout ${CMAKE_BINARY_DIR}/include/${SUBDIR}/${SUBDIR})
    set(metadata "#ifndef Qt${SUBDIR}_META_H\n#define Qt${SUBDIR}_META_H\n\n")
    foreach(pubheader ${PUBLIC_INCLUDES})
        string(TOLOWER ${pubheader} pubname)
        get_filename_component(pubname ${pubname} NAME_WE)
        set(pubout ${CMAKE_BINARY_DIR}/include/${SUBDIR}/${pubheader})
        set(pubcheck ${CMAKE_BINARY_DIR}/include/${SUBDIR}/${pubname}.h)
        if(NOT EXISTS ${pubout})
            file(WRITE ${pubout} "#include <${pubname}.h>")
        endif()
        if(NOT EXISTS ${pubcheck})
            message(AUTHOR_WARNING "Bogus public header: ${pubheader} (${pubcheck})")
        else()
            set(metadata "${metadata}#include <${SUBDIR}/${pubname}.h>\n")
        endif()
    endforeach(pubheader)
    set(metadata "${metadata}\n#endif\n")
    file(WRITE ${metaout} "${metadata}")
    file(APPEND ${CLASSMAPOUT} "#include <${SUBDIR}_map.h>\n")
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
        file(WRITE "${pubout}" "#include <${SUBDIR}/${REDIRECT}>")
    endif()
endmacro()

macro(KATIE_GENERATE_PACKAGE FORTARGET CXXFLAGS LIBRARIES REQUIRES)
    if(UNIX)
        # the list must be adjusted
        string(REPLACE ";" " -l" modlibs "${LIBRARIES}")
        if(NOT "${modlibs}" STREQUAL "")
            set(modlibs "-l${modlibs}")
        endif()
        string(REPLACE "Kt" "Qt" PACKAGE_FAKE "${FORTARGET}")
        set(PACKAGE_NAME ${FORTARGET})
        set(PACKAGE_CFLAGS ${CXXFLAGS})
        set(PACKAGE_LIBRARIES ${modlibs})
        set(PACKAGE_REQUIRES ${REQUIRES})
        configure_file(
            ${CMAKE_SOURCE_DIR}/cmake/pkgconfig.cmake
            ${CMAKE_BINARY_DIR}/pkgconfig/${FORTARGET}.pc
        )
        install(
            FILES ${CMAKE_BINARY_DIR}/pkgconfig/${FORTARGET}.pc
            DESTINATION ${QT_LIBRARIES_PATH_INST}/pkgconfig
        )
    endif()
endmacro()

function(KATIE_GENERATE_STRING INSTR OUTSTR OUTSTR2)
    if("${INSTR}" GREATER "536")
        message(FATAL_ERROR "${OUTSTR} is greater than 536")
    endif()
    # qt used to do some magic, adding trailing \0 in a weird way via padstring script
    set(${OUTSTR} "${INSTR}\\0" PARENT_SCOPE)

    if(OUTSTR2)
        string(REGEX REPLACE ".*${CMAKE_INSTALL_PREFIX}/" "" instpath ${INSTR})
        set(${OUTSTR2} "${instpath}" PARENT_SCOPE)
    endif()
endfunction()

# the purpose of this function is to ensure that (1) the output string is not
# null so that when it is passed to another function/macro it does not complain
# about inproper number of arguments and (2) it joins the input which if
# quoted has semicolons to it (if it is a list) that the sub-command
# (e.g. gcc) can not handle. that's a dirty hack to support gcc and clang at
# the same time along with custom target COMPILE_FLAGS/LINK_FLAGS without
# doing compiler checks all over the place. if anyone has a better solution
# I'll be glad to drop the function bellow!
function(KATIE_FIXUP_STRING INSTR OUTSTR)
    string(STRIP "${INSTR}" instrtrimmed)
    if("${instrtrimmed}" STREQUAL "")
        set(${OUTSTR} " " PARENT_SCOPE)
    else()
        string(REPLACE ";" " " modstring "${INSTR}")
        set(${OUTSTR} "${modstring}" PARENT_SCOPE)
    endif()
endfunction()

macro(KATIE_CONFIG VAR VAL)
    set(${VAR} ${VAL})
    add_definitions(-D${VAR}=${VAL})
endmacro()

function(KATIE_SETUP_FLAGS)
    katie_fixup_string("${KATIE_CXXFLAGS}" KATIE_CXXFLAGS)
    katie_fixup_string("${KATIE_LDFLAGS}" KATIE_LDFLAGS)

    if(ARGN)
        foreach(target ${ARGN})
            set_target_properties(${target} PROPERTIES
                COMPILE_FLAGS "${KATIE_CXXFLAGS}"
                LINK_FLAGS "${KATIE_LDFLAGS}"
            )
        endforeach()
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${KATIE_CXXFLAGS}" PARENT_SCOPE)
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${KATIE_LDFLAGS}" PARENT_SCOPE)
        set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${KATIE_LDFLAGS}" PARENT_SCOPE)
        set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} ${KATIE_LDFLAGS}" PARENT_SCOPE)
    endif()
endfunction()

function(KATIE_SETUP_SOURCES SOURCESVAR)
    set(compilesources)
    foreach(source ${ARGN})
        get_filename_component(sourcename ${source} NAME)
        set(compileflags)
        # TODO: sse4.1 and sse4.2 support, currently not needed
        foreach(flag 3dnow avx mmx sse sse2 sse3 ssse3 iwmmxt neon)
            string(REGEX MATCH "${flag}" flagmatch ${sourcename})
            string(TOUPPER "${flag}" upperflag)
            if("${flag}" MATCHES "(iwmmxt|neno)" AND NOT "${KATIE_ARCHITECTURE}" STREQUAL "arm")
                set(flagmatch)
            endif()
            if("${flag}" STREQUAL "mmx" AND "${sourcename}" MATCHES "iwmmxt")
                # false positive
                set(flagmatch)
            endif()
            if("${flagmatch}" STREQUAL "neon" AND KATIE_${upperflag}_RESULT)
                set(compileflags "${compileflags} -mfpu=neon")
            elseif("${flagmatch}" STREQUAL "iwmmxt" AND KATIE_${upperflag}_RESULT)
                set(compileflags "${compileflags} -mcpu=iwmmxt")
            elseif(flagmatch AND KATIE_${upperflag}_RESULT)
                set(compileflags "${compileflags} -m${flag}")
            endif()
        endforeach()
        if(compileflags)
            # message(STATUS "Setting up compile flags for: ${source} to: ${compileflags}")
            set_source_files_properties(${source} PROPERTIES COMPILE_FLAGS "${compileflags}")
            set(compilesources ${compilesources} ${source})
        endif()
    endforeach()
    if(compilesources)
        set(${SOURCESVAR} ${${SOURCESVAR}} ${compilesources} PARENT_SCOPE)
    endif()
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
        message(AUTHOR_WARNING "unifdef not installed, cannot optimize headers for: ${basename}")
    endif()
endmacro()

macro(KATIE_TEST TESTNAME TESTSOURCES)
    katie_resources(${TESTSOURCES} ${ARGN})

    add_executable(${TESTNAME} ${TESTSOURCES} ${ARGN})

    # TODO: make GUI access optional
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

    add_test(NAME ${TESTNAME} COMMAND ${TESTNAME})
endmacro()
