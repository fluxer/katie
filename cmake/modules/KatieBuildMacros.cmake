# Copyright (C) 2015, Ivailo Monev, <xakepa10@gmail.com>
# Redistribution and use is allowed according to the terms of the BSD license.

# a function to append definitions to KATIE_DEFINITIONS which is stored in
# KatieConfig.cmake and pkg-config files as interface definitions and add
# definitions to the current directory scope
function(KATIE_DEFINITION DEF)
    set(KATIE_DEFINITIONS ${KATIE_DEFINITIONS} ${DEF} ${ARGN} PARENT_SCOPE)
    add_definitions(${DEF} ${ARGN})
endfunction()

# a function that sets config option variable to ON so that it is stored as
# disabled in qconfig.h
function(KATIE_CONFIG CONF)
    set(${CONF} TRUE PARENT_SCOPE)
endfunction()

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
    set(CMAKE_REQUIRED_INCLUDES /usr/X11R6/include /usr/X11R7/include /usr/pkg/include /usr/local/include /usr/include)
    set(CMAKE_REQUIRED_LINK_OPTIONS -L/usr/X11R6/lib -L/usr/X11R7/lib -L/usr/pkg/lib -L/usr/local/lib -L/usr/lib -L/lib)
    check_cxx_source_compiles(
        "
#include <stdio.h>
#include <${FROMHEADER}>

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

# a function to check for C struct member presence in header, if member is
# found a definition is added
function(KATIE_CHECK_STRUCT FORSTRUCT FORMEMBER FROMHEADER)
    check_struct_has_member("struct ${FORSTRUCT}" "${FORMEMBER}" "sys/types.h;${FROMHEADER}" HAVE_${FORSTRUCT}_${FORMEMBER})

    if(HAVE_${FORSTRUCT}_${FORMEMBER})
        string(TOUPPER "${FORSTRUCT}_${FORMEMBER}" upperstructmember)
        add_definitions(-DQT_HAVE_${upperstructmember})
    endif()
endfunction()

# a function to check for file existence in /proc, if file exists a definition
# is added, this function is over-engineered for two reasons - to be able to
# override the test on the command-line and to show test status messages
# similar to other tests
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
        katie_generate_obsolete("${pubheader}" "${SUBDIR}" "${pubname}.h")
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

# a macro to create alias headers for the sake of compatibility
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

    set(rscpath "${CMAKE_CURRENT_BINARY_DIR}/${FORTARGET}_resources")
    foreach(tmparg ${ARGN})
        get_filename_component(fileabs "${tmparg}" ABSOLUTE)
        get_filename_component(fileext "${fileabs}" EXT)
        get_filename_component(filename "${fileabs}" NAME_WE)
        if("${fileext}" STREQUAL ".ui")
            set(rscout "${rscpath}/ui_${filename}.h")
            make_directory("${rscpath}")
            include_directories("${rscpath}")
            add_custom_command(
                COMMAND "${CMAKE_BINARY_DIR}/exec.sh" "${CMAKE_BINARY_DIR}/bin/uic${KATIE_TOOLS_SUFFIX}" "${fileabs}" -o "${rscout}"
                DEPENDS "uic"
                OUTPUT "${rscout}"
            )
            set_property(SOURCE "${fileabs}" APPEND PROPERTY OBJECT_DEPENDS "${rscout}")
        elseif("${fileext}" STREQUAL ".qrc")
            set(rscout "${rscpath}/qrc_${filename}.cpp")
            make_directory("${rscpath}")
            include_directories("${rscpath}")
            add_custom_command(
                COMMAND "${CMAKE_BINARY_DIR}/exec.sh" "${CMAKE_BINARY_DIR}/bin/rcc${KATIE_TOOLS_SUFFIX}" "${fileabs}" -o "${rscout}" -name "${filename}"
                DEPENDS "rcc"
                OUTPUT "${rscout}"
            )
            set_property(SOURCE "${fileabs}" APPEND PROPERTY OBJECT_DEPENDS "${rscout}")
        elseif("${fileext}" MATCHES "(.c|.h|.hpp|.cc|.cpp)")
            file(READ "${fileabs}" rsccontent)
            if("${rsccontent}" MATCHES "(Q_OBJECT|Q_OBJECT_FAKE|Q_GADGET)")
                set(rscout "${rscpath}/moc_${filename}${fileext}")
                make_directory("${rscpath}")
                include_directories("${rscpath}")
                add_custom_command(
                    COMMAND "${CMAKE_BINARY_DIR}/exec.sh" "${CMAKE_BINARY_DIR}/bin/bootstrap_moc" -nw "${fileabs}" -o "${rscout}" ${mocargs}
                    DEPENDS "bootstrap_moc"
                    OUTPUT "${rscout}"
                )
                set_property(SOURCE "${fileabs}" APPEND PROPERTY OBJECT_DEPENDS "${rscout}")
            endif()
        else()
            message(SEND_ERROR "Unkown source type in sources list: ${fileabs}")
        endif()
    endforeach()

    set(${FORTARGET}_SOURCES ${ARGN} PARENT_SCOPE)
endfunction()

# a function to make a meta target depend on all plugins, the meta target itself is used in the
# tests setup macros to build all plugins before any test so that plugins from the host are not
# used.
# TODO: this is cheap and sub-optimal way of forcing tests depend on all plugins, perhaps with
# plugin and test targets dependencies introspection it can be optimized, e.g. make tests that
# depend on KtNetwork depend on plugins that depend on it too
add_custom_target(plugins_dependant_tests)
function(KATIE_SETUP_PLUGIN FORPLUGIN)
    add_dependencies(plugins_dependant_tests ${FORPLUGIN})
endfunction()

# a macro to ensure that object targets are build with PIC if the target they
# are going to be used in (like $<TARGET_OBJECTS:foo>) is build with PIC or
# PIC has been enabled for all module/library/executable targets. in addition
# the macro will add the object include directories and definitions to the
# target properties
macro(KATIE_SETUP_OBJECT FORTARGET)
    get_target_property(target_pic ${FORTARGET} POSITION_INDEPENDENT_CODE)

    foreach(objtarget ${ARGN})
        if(CMAKE_POSITION_INDEPENDENT_CODE OR target_pic)
            set_target_properties(${objtarget} PROPERTIES
                POSITION_INDEPENDENT_CODE TRUE
            )
        endif()

        get_target_property(object_definitions ${objtarget} COMPILE_DEFINITIONS)
        get_target_property(object_includes ${objtarget} INCLUDE_DIRECTORIES)
        if(object_definitions)
            target_compile_definitions(${FORTARGET} PRIVATE ${object_definitions})
        endif()
        target_include_directories(${FORTARGET} PRIVATE ${object_includes})
    endforeach()
endmacro()

# a macro to remove conditional code from headers which is only relevant to the
# process of building Katie itself
macro(KATIE_OPTIMIZE_HEADERS DIR)
    if(KATIE_UNIFDEF)
        install(
            CODE "set(UNIFDEF_EXECUTABLE \"${KATIE_UNIFDEF}\")"
            CODE "set(HEADERS_DIRECTORY \"${DIR}\")"
            SCRIPT "${CMAKE_SOURCE_DIR}/cmake/modules/OptimizeHeaders.cmake"
        )
    endif()
endmacro()

# a macro to add tests easily by setting them up with the assumptions they make
macro(KATIE_TEST TESTNAME TESTSOURCES)
    katie_setup_target(${TESTNAME} ${TESTSOURCES} ${ARGN})

    add_executable(${TESTNAME} ${${TESTNAME}_SOURCES})
    add_dependencies(${TESTNAME} plugins_dependant_tests)

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
    add_dependencies(${TESTNAME} plugins_dependant_tests)

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
    add_dependencies(${TESTNAME} plugins_dependant_tests)

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
