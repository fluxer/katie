# Copyright (c) 2015-2020, Ivailo Monev, <xakepa10@gmail.com>
# Redistribution and use is allowed according to the terms of the BSD license.

set(KATIE_UIC "uic")
set(KATIE_RCC "rcc")
set(KATIE_MOC "bootstrap_moc")

# a function to check for header presence, if header is found a definition is
# added
function(KATIE_CHECK_HEADER FORHEADER)
    string(REPLACE "." "_" underscoreheader "${FORHEADER}")
    check_include_file_cxx("${FORHEADER}" HAVE_${underscoreheader})

    if(HAVE_${underscoreheader})
        string(TOUPPER "${underscoreheader}" upperheader)
        add_definitions(-DQT_HAVE_${upperheader})
    endif()
endfunction()

# a function to check for C function/definition, works for external functions
function(KATIE_CHECK_DEFINED FORDEFINITION FROMHEADER)
    # see comment in top-level CMake file
    set(CMAKE_REQUIRED_INCLUDES /usr/X11R7/include /usr/pkg/include /usr/local/include /usr/include)
    set(CMAKE_REQUIRED_LINK_OPTIONS -L/usr/X11R7/lib -L/usr/pkg/lib -L/usr/local/lib -L/usr/lib -L/lib)
    set(includedata)
    foreach(inc ${FROMHEADER})
        set(includedata "${includedata}#include <${inc}>\n")
    endforeach()
    check_cxx_source_compiles(
        "
#include <stdio.h>
${includedata}

int main() {
    printf(\"%p\", &${FORDEFINITION});
    return 0;
}
"
        HAVE_${FORDEFINITION}
    )
endfunction()

# a macro to check for C function presence in header, if function is found a
# definition is added
macro(KATIE_CHECK_FUNCTION FORFUNCTION FROMHEADER)
    katie_check_defined("${FORFUNCTION}" "${FROMHEADER}")

    if(HAVE_${FORFUNCTION})
        string(TOUPPER "${FORFUNCTION}" upperfunction)
        add_definitions(-DQT_HAVE_${upperfunction})
    endif()
endmacro()

# a function to check for C function with 64-bit offset alternative, sets
# QT_LARGEFILE_SUPPORT to FALSE if not available and does not perform
# additional checks if one fails
function(KATIE_CHECK_FUNCTION64 FORFUNCTION FROMHEADER)
    if(QT_LARGEFILE_SUPPORT)
        cmake_reset_check_state()
        set(CMAKE_REQUIRED_DEFINITIONS -D_LARGEFILE64_SOURCE -D_LARGEFILE_SOURCE)
        katie_check_defined("${FORFUNCTION}" "${FROMHEADER}")
        cmake_reset_check_state()

        if(NOT HAVE_${FORFUNCTION})
            set(QT_LARGEFILE_SUPPORT FALSE PARENT_SCOPE)
        endif()
    endif()
endfunction()

# a function to check for C struct member presence in header, if member is found a
# definition is added
function(KATIE_CHECK_STRUCT FORSTRUCT FORMEMBER FROMHEADER)
    check_struct_has_member("struct ${FORSTRUCT}" "${FORMEMBER}" "${FROMHEADER}" HAVE_${FORSTRUCT}_${FORMEMBER})

    if(HAVE_${FORSTRUCT}_${FORMEMBER})
        string(TOUPPER "${FORSTRUCT}_${FORMEMBER}" upperstructmember)
        add_definitions(-DQT_HAVE_${upperstructmember})
    endif()
endfunction()

# a function to check for file existence in /proc, if file exists a definition
# is added
function(KATIE_CHECK_PROC FORFILE)
    check_cxx_source_runs(
        "
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>

int main() {
    char snprintfbuf[1024];
    snprintf(snprintfbuf, sizeof(snprintfbuf), \"/proc/%d/${FORFILE}\", getpid());

    struct stat statbuf;
    if (lstat(snprintfbuf, &statbuf) == -1) {
        return 1;
    }
    return 0;
}
"
        HAVE_proc_${FORFILE}
    )
    if(HAVE_proc_${FORFILE})
        string(TOUPPER "${FORFILE}" upperfile)
        add_definitions(-DQT_HAVE_PROC_${upperfile})
    endif()
endfunction()

# a macro to write data to file, does nothing if the file exists and its
# content is the same as the data to be written
macro(KATIE_WRITE_FILE OUTFILE DATA)
    if(NOT EXISTS "${OUTFILE}")
        file(WRITE "${OUTFILE}" "${DATA}")
    else()
        file(READ "${OUTFILE}" OUTDATA)
        if(NOT "${OUTDATA}" STREQUAL "${DATA}")
            file(WRITE "${OUTFILE}" "${DATA}")
        endif()
    endif()
endmacro()

# a macro to create camel-case headers pointing to their lower-case alternative
# as well as meta header that includes all component headers
macro(KATIE_GENERATE_PUBLIC PUBLIC_INCLUDES SUBDIR)
    foreach(pubheader ${PUBLIC_INCLUDES})
        string(TOLOWER ${pubheader} pubname)
        set(pubout "${CMAKE_BINARY_DIR}/include/${SUBDIR}/${pubheader}")
        katie_write_file("${pubout}" "#include <${pubname}.h>\n")
    endforeach()

    file(GLOB PUBLIC_LIST "${CMAKE_BINARY_DIR}/include/${SUBDIR}/*.h")
    set(metaout "${CMAKE_BINARY_DIR}/include/${SUBDIR}/${SUBDIR}")
    set(metadata "#ifndef ${SUBDIR}_META_H\n#define ${SUBDIR}_META_H\n\n")
    foreach(pubheader ${PUBLIC_LIST})
        get_filename_component(pubname ${pubheader} NAME)
        # qtest_gui.h is exception because it requires explicit gui component linkage
        if(NOT "${pubname}" STREQUAL "qtest_gui.h")
            set(metadata "${metadata}#include <${SUBDIR}/${pubname}>\n")
        endif()
    endforeach()
    set(metadata "${metadata}\n#endif\n")
    katie_write_file("${metaout}" "${metadata}")
endmacro()

# a macro to copy headers into specific directory based on their base names
# ultimately suitable for copy operation of their destination
macro(KATIE_GENERATE_MISC MISC_INCLUDES SUBDIR)
    foreach(mischeader ${MISC_INCLUDES})
        get_filename_component(headername ${mischeader} NAME)
        if("${headername}" MATCHES "(_p.h)")
            set(headout "${CMAKE_BINARY_DIR}/privateinclude/${headername}")
        else()
            set(headout "${CMAKE_BINARY_DIR}/include/${SUBDIR}/${headername}")
        endif()
        configure_file("${mischeader}" "${headout}" COPYONLY)
    endforeach(mischeader)
endmacro()

macro(KATIE_GENERATE_OBSOLETE OBSOLETE_INCLUDE SUBDIR REDIRECT)
    set(pubout "${CMAKE_BINARY_DIR}/include/${SUBDIR}/${OBSOLETE_INCLUDE}")
    katie_write_file("${pubout}" "#include <${SUBDIR}/${REDIRECT}>\n")
endmacro()

# a macro for creating pkgconfig files for major components
macro(KATIE_GENERATE_PACKAGE FORTARGET REQUIRES)
    string(REPLACE "Kt" "Qt" PACKAGE_FAKE "${FORTARGET}")
    set(PACKAGE_NAME ${FORTARGET})
    set(PACKAGE_REQUIRES ${REQUIRES})
    string(REPLACE "Kt" "" compname "${FORTARGET}")
    string(TOUPPER ${compname} compname)
    set(PACKAGE_FLAGS "-DQT_${compname}_LIB")
    # adding the definitions to other components is simply redundant since
    # all components require the core component
    if("${FORTARGET}" STREQUAL "KtCore")
        katie_string_wrap("${KATIE_DEFINITIONS}" KATIE_DEFINITIONS)
        set(PACKAGE_FLAGS "${PACKAGE_FLAGS} ${KATIE_DEFINITIONS}")
    endif()
    set(PACKAGE_PREFIXDIR "${CMAKE_INSTALL_PREFIX}")
    string(REPLACE "${PACKAGE_PREFIXDIR}" "\${prefix}" PACKAGE_INCLUDEDIR "${KATIE_HEADERS_PATH}")
    string(REPLACE "${PACKAGE_PREFIXDIR}" "\${exec_prefix}" PACKAGE_LIBDIR "${KATIE_LIBRARIES_PATH}")
    configure_file(
        "${CMAKE_SOURCE_DIR}/cmake/pkgconfig.cmake"
        "${CMAKE_BINARY_DIR}/pkgconfig/${FORTARGET}.pc"
        @ONLY
    )
    install(
        FILES "${CMAKE_BINARY_DIR}/pkgconfig/${FORTARGET}.pc"
        DESTINATION "${KATIE_PKGCONFIG_PATH}"
        COMPONENT Devel
    )
endmacro()

# a function to ensure that (1) the output string is not null so that when it
# is passed to another function/macro it does not complain about inproper
# number of arguments and (2) it joins the input which if quoted has semicolons
# in it (if it is a list) that the sub-command (e.g. gcc) can not handle
function(KATIE_STRING_WRAP INVAR OUTSTR)
    string(STRIP "${INVAR}" instrtrimmed)
    if("${instrtrimmed}" STREQUAL "")
        set(${OUTSTR} " " PARENT_SCOPE)
    else()
        string(REPLACE ";" " " modstring "${INVAR}")
        set(${OUTSTR} "${modstring}" PARENT_SCOPE)
    endif()
endfunction()

# a function to convert string to list, opposite of katie_string_wrap()
function(KATIE_STRING_UNWRAP INSTR OUTLST)
    string(STRIP "${INSTR}" instrtrimmed)
    if("${instrtrimmed}" STREQUAL "")
        set(${OUTLST} " " PARENT_SCOPE)
    else()
        string(REPLACE " " ";${ARGN}" modstring "${ARGN}${INSTR}")
        set(${OUTLST} ${modstring} PARENT_SCOPE)
    endif()
endfunction()

# a macro to instruct CMake which sources to exclude from the unity source file
macro(KATIE_UNITY_EXCLUDE ARG1)
    set_source_files_properties(${ARG1} ${ARGN} PROPERTIES SKIP_UNITY_BUILD_INCLUSION TRUE)
endmacro()

# a function to create an array of source files for a target setting up proper
# dependency for the moc/uic/rcc generated resources
function(KATIE_SETUP_TARGET FORTARGET)
    get_directory_property(dirdefs COMPILE_DEFINITIONS)
    get_directory_property(dirincs INCLUDE_DIRECTORIES)
    set(mocargs)
    # COMPILE_DEFINITIONS does not include undefine definitions
    foreach(ddef ${dirdefs})
        set(mocargs ${mocargs} -D${ddef})
    endforeach()
    foreach(incdir ${dirincs})
        set(mocargs ${mocargs} -I${incdir})
    endforeach()

    # this can be simpler if continue() was supported by old CMake versions
    set(resourcesdep "${CMAKE_CURRENT_BINARY_DIR}/${FORTARGET}_resources.cpp")
    katie_write_file("${resourcesdep}" "enum { CompilersWorkaroundAlaAutomoc = 1 };\n")
    set(filteredsources)
    set(targetresources)
    set(rscpath "${CMAKE_CURRENT_BINARY_DIR}/${FORTARGET}_resources")
    include_directories("${rscpath}")
    foreach(tmpres ${ARGN})
        get_filename_component(resource "${tmpres}" ABSOLUTE)
        get_filename_component(rscext "${resource}" EXT)
        get_filename_component(rscname "${resource}" NAME_WE)
        if("${rscext}" STREQUAL ".ui")
            set(rscout "${rscpath}/ui_${rscname}.h")
            set(targetresources ${targetresources} "${rscout}")
            make_directory("${rscpath}")
            add_custom_command(
                COMMAND "${CMAKE_BINARY_DIR}/exec.sh" "${CMAKE_BINARY_DIR}/bin/${KATIE_UIC}${KATIE_TOOLS_SUFFIX}" "${resource}" -o "${rscout}"
                DEPENDS "${KATIE_UIC}"
                OUTPUT "${rscout}"
            )
        elseif("${rscext}" STREQUAL ".qrc")
            set(rscout "${rscpath}/qrc_${rscname}.cpp")
            set(targetresources ${targetresources} "${rscout}")
            make_directory("${rscpath}")
            add_custom_command(
                COMMAND "${CMAKE_BINARY_DIR}/exec.sh" "${CMAKE_BINARY_DIR}/bin/${KATIE_RCC}${KATIE_TOOLS_SUFFIX}" "${resource}" -o "${rscout}" -name "${rscname}"
                DEPENDS "${KATIE_RCC}"
                OUTPUT "${rscout}"
            )
        elseif("${rscext}" MATCHES "(.c|.h|.hpp|.cc|.cpp)")
            set(filteredsources ${filteredsources} "${resource}")
            file(READ "${resource}" rsccontent)
            if("${rsccontent}" MATCHES "(Q_OBJECT|Q_OBJECT_FAKE|Q_GADGET)")
                set(rscout "${rscpath}/moc_${rscname}${rscext}")
                set(targetresources ${targetresources} "${rscout}")
                make_directory("${rscpath}")
                add_custom_command(
                    COMMAND "${CMAKE_BINARY_DIR}/exec.sh" "${CMAKE_BINARY_DIR}/bin/${KATIE_MOC}" -nw "${resource}" -o "${rscout}" ${mocargs}
                    DEPENDS "${KATIE_MOC}"
                    OUTPUT "${rscout}"
                )
            endif()
        endif()
    endforeach()
    set_property(SOURCE "${resourcesdep}" APPEND PROPERTY OBJECT_DEPENDS "${targetresources}")

    set(${FORTARGET}_SOURCES "${resourcesdep}" ${filteredsources} PARENT_SCOPE)
endfunction()

# a macro to ensure that object targets are build with PIC if the target they
# are going to be used in (like $<TARGET_OBJECTS:foo>) is build with PIC or
# PIC has been enabled for all module/library/executable targets. in addition
# the macro will add the object include directories and definitions to the
# target properties
macro(KATIE_SETUP_OBJECT FORTARGET)
    get_target_property(target_pic ${FORTARGET} POSITION_INDEPENDENT_CODE)
    if(CMAKE_POSITION_INDEPENDENT_CODE OR target_pic)
        foreach(objtarget ${ARGN})
            set_target_properties(${objtarget} PROPERTIES
                POSITION_INDEPENDENT_CODE TRUE
            )
        endforeach()
    endif()

    foreach(objtarget ${ARGN})
        get_target_property(object_definitions ${objtarget} COMPILE_DEFINITIONS)
        get_target_property(object_includes ${objtarget} INCLUDE_DIRECTORIES)
        if(object_definitions)
            target_compile_definitions(${FORTARGET} PRIVATE ${object_definitions})
        endif()
        target_include_directories(${FORTARGET} PRIVATE ${object_includes})
    endforeach()
endmacro()

# a macro to setup pre-compiled header for target
macro(KATIE_SETUP_PCH FORTARGET)
    if(KATIE_PCH)
        if (NOT CMAKE_VERSION VERSION_LESS "3.16.0")
            target_precompile_headers(${FORTARGET} PRIVATE "${CMAKE_SOURCE_DIR}/src/core/qt_pch.h")
        else()
            message(FATAL_ERROR "Pre-compiled headers option requires CMake v3.16+")
        endif()
    endif()
endmacro()

# a macro to remove conditional code from headers which is only relevant to the
# process of building Katie itself
macro(KATIE_OPTIMIZE_HEADERS DIR)
    if(KATIE_UNIFDEF)
        install(
            CODE "set(UNIFDEF_EXECUTABLE \"${unifdef}\")"
            CODE "set(HEADERS_DIRECTORY \"${DIR}\")"
            CODE "set(HEADERS_DEFINITIONS \"${ARGN}\")"
            SCRIPT "${CMAKE_SOURCE_DIR}/cmake/modules/OptimizeHeaders.cmake"
        )
    endif()
endmacro()

# a macro to add tests easily by setting them up with the assumptions they make
macro(KATIE_TEST TESTNAME TESTSOURCES)
    katie_setup_target(${TESTNAME} ${TESTSOURCES} ${ARGN})

    add_executable(${TESTNAME} ${${TESTNAME}_SOURCES})

    target_link_libraries(${TESTNAME} KtCore KtTest)
    target_compile_definitions(
        ${TESTNAME} PRIVATE
        -DSRCDIR="${CMAKE_CURRENT_SOURCE_DIR}/"
    )
    set_target_properties(
        ${TESTNAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
    )

    add_test(
        NAME ${TESTNAME}
        COMMAND "${CMAKE_BINARY_DIR}/exec.sh" "${CMAKE_CURRENT_BINARY_DIR}/${TESTNAME}"
    )
endmacro()

# a macro to add D-Bus tests easily by setting them up with the assumptions they make
macro(KATIE_DBUS_TEST TESTNAME TESTSOURCES)
    katie_setup_target(${TESTNAME} ${TESTSOURCES} ${ARGN})

    add_executable(${TESTNAME} ${${TESTNAME}_SOURCES})

    target_link_libraries(${TESTNAME} KtCore KtDBus KtTest)
    target_compile_definitions(
        ${TESTNAME} PRIVATE
        -DSRCDIR="${CMAKE_CURRENT_SOURCE_DIR}/"
    )
    set_target_properties(
        ${TESTNAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
    )

    add_test(
        NAME ${TESTNAME}
        COMMAND "${CMAKE_BINARY_DIR}/dbus.sh" "${CMAKE_CURRENT_BINARY_DIR}/${TESTNAME}"
    )
endmacro()

# a macro to add tests that require GUI easily by setting them up with the assumptions they make
macro(KATIE_GUI_TEST TESTNAME TESTSOURCES)
    katie_setup_target(${TESTNAME} ${TESTSOURCES} ${ARGN})

    add_executable(${TESTNAME} ${${TESTNAME}_SOURCES})

    target_link_libraries(${TESTNAME} KtCore KtGui KtTest)
    target_compile_definitions(
        ${TESTNAME} PRIVATE
        -DSRCDIR="${CMAKE_CURRENT_SOURCE_DIR}/"
        -DQT_GUI_LIB
    )
    set_target_properties(
        ${TESTNAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
    )

    add_test(
        NAME ${TESTNAME}
        COMMAND "${CMAKE_BINARY_DIR}/xvfb.sh" "${CMAKE_CURRENT_BINARY_DIR}/${TESTNAME}"
    )
endmacro()
