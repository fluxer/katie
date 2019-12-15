# Copyright (c) 2015-2019, Ivailo Monev, <xakepa10@gmail.com>
# Redistribution and use is allowed according to the terms of the BSD license.

set(KATIE_UIC "${CMAKE_BINARY_DIR}/bin/uic")
set(KATIE_RCC "${CMAKE_BINARY_DIR}/bin/rcc")
set(KATIE_MOC "${CMAKE_BINARY_DIR}/bin/bootstrap_moc")
set(KATIE_LRELEASE "${CMAKE_BINARY_DIR}/bin/lrelease")
set(KATIE_QDBUSXML2CPP "${CMAKE_BINARY_DIR}/bin/qdbusxml2cpp")

# a macro to print a dev warning but only when the build type is Debug
macro(KATIE_WARNING MESSAGESTR)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        message(AUTHOR_WARNING "${MESSAGESTR} ${ARGN}")
    endif()
endmacro()

# a macro to create camel-case headers pointing to their lower-case alternative
# with a dev warning for possibly non-existent headers
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

# a macro to copy headers into specific directory based on their base names
# ultimately suitable for copy operation of their destination
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
    configure_file(
        ${CMAKE_SOURCE_DIR}/cmake/pkgconfig.cmake
        ${CMAKE_BINARY_DIR}/pkgconfig/${FORTARGET}.pc
    )
    katie_setup_paths()
    install(
        FILES ${CMAKE_BINARY_DIR}/pkgconfig/${FORTARGET}.pc
        DESTINATION ${KATIE_PKGCONFIG_RELATIVE}
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

# a function to get the Git checkout hash and store it in a variable
function(KATIE_GIT_CHECKOUT GITEXE OUTSTR)
    if(NOT EXISTS "${CMAKE_SOURCE_DIR}/.git")
       set(${OUTSTR} "unknown" PARENT_SCOPE)
    elseif(NOT GITEXE)
        message(WARNING "Git was not found, unable to obtain checkout.\n")
        set(${OUTSTR} "unknown" PARENT_SCOPE)
    else()
        execute_process(
            COMMAND ${GITEXE} rev-parse HEAD
            WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
            RESULT_VARIABLE git_result
            ERROR_VARIABLE git_output
            OUTPUT_VARIABLE git_output
            OUTPUT_STRIP_TRAILING_WHITESPACE
        )

        if(NOT git_result STREQUAL 0)
            message(WARNING "Git command failed, unable to obtain checkout:\n${git_output}")
            set(${OUTSTR} "unknown" PARENT_SCOPE)
        else()
            set(${OUTSTR} "${git_output}" PARENT_SCOPE)
        endif()
    endif()
endfunction()

# a macro to instruct katie_setup_target() which sources to exclude from the
# all-in-one source file
macro(KATIE_ALLINONE_EXCLUDE ARG1)
    set_source_files_properties(${ARG1} ${ARGN} PROPERTIES ALLINONE_EXCLUDE TRUE)
endmacro()

# a function to create an array of source files for a target while taking into
# account all-in-one target build setting up proper dependency for the
# moc/uic/rcc generated resources
function(KATIE_SETUP_TARGET FORTARGET)
    # this can be simpler if continue() was supported by old CMake versions
    set(resourcesdep "${CMAKE_CURRENT_BINARY_DIR}/${FORTARGET}_resources.cpp")
    if(NOT EXISTS "${resourcesdep}")
        file(WRITE "${resourcesdep}" "enum { CompilersWorkaroundAlaAutomoc = 1 };\n")
    endif()
    set(targetresources)
    set(rscpath "${CMAKE_CURRENT_BINARY_DIR}/${FORTARGET}_resources")
    include_directories(${rscpath})
    foreach(tmpres ${ARGN})
        get_filename_component(resource ${tmpres} ABSOLUTE)
        get_filename_component(rscext ${resource} EXT)
        get_filename_component(rscname ${resource} NAME_WE)
        if("${rscext}" STREQUAL ".ui")
            set(rscout "${rscpath}/ui_${rscname}.h")
            set(targetresources ${targetresources} ${rscout})
            make_directory(${rscpath})
            add_custom_command(
                COMMAND ${CMAKE_BINARY_DIR}/exec.sh "${KATIE_UIC}" "${resource}" -o "${rscout}"
                DEPENDS "${KATIE_UIC}"
                OUTPUT "${rscout}"
            )
        elseif("${rscext}" STREQUAL ".qrc")
            set(rscout "${rscpath}/qrc_${rscname}.cpp")
            set(targetresources ${targetresources} ${rscout})
            make_directory(${rscpath})
            add_custom_command(
                COMMAND ${CMAKE_BINARY_DIR}/exec.sh "${KATIE_RCC}" "${resource}" -o "${rscout}" -name "${rscname}"
                DEPENDS "${KATIE_RCC}"
                OUTPUT "${rscout}"
            )
        elseif("${rscext}" MATCHES "(.h|.hpp|.cc|.cpp)")
            file(READ "${resource}" rsccontent)
            if("${rsccontent}" MATCHES "(Q_OBJECT|Q_OBJECT_FAKE|Q_GADGET)")
                set(rscout "${rscpath}/moc_${rscname}${rscext}")
                set(targetresources ${targetresources} ${rscout})
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
                make_directory(${rscpath})
                add_custom_command(
                    COMMAND ${CMAKE_BINARY_DIR}/exec.sh "${KATIE_MOC}" -nw "${resource}" -o "${rscout}" ${mocargs}
                    DEPENDS "${KATIE_MOC}"
                    OUTPUT "${rscout}"
                )
            endif()
        elseif("${rscext}" MATCHES ".ts")
            get_filename_component(trname ${resource} NAME_WE)
            get_filename_component(trdir ${resource} DIRECTORY)
            string(REPLACE "${CMAKE_SOURCE_DIR}" "${CMAKE_BINARY_DIR}" trdir ${trdir})
            make_directory(${trdir})
            set(trout "${trdir}/${trname}.qm")
            add_custom_target(
                ${FORTARGET}_${trname} ALL
                COMMAND ${CMAKE_BINARY_DIR}/exec.sh "${KATIE_LRELEASE}" "${resource}" -qm "${trout}"
                DEPENDS "${KATIE_LRELEASE}"
            )
            set_source_files_properties(${trout} PROPERTIES GENERATED TRUE)
            install(FILES ${trout} DESTINATION ${KATIE_TRANSLATIONS_RELATIVE})

        endif()
    endforeach()
    set_source_files_properties(${resourcesdep} PROPERTIES OBJECT_DEPENDS "${targetresources}")

    if(NOT KATIE_ALLINONE)
        set(filteredsources)
        foreach(srcstring ${ARGN})
            get_filename_component(srcext ${srcstring} EXT)
            if(NOT "${srcext}" MATCHES "(.qrc|.ui)")
                set(filteredsources ${filteredsources} ${srcstring})
            endif()
        endforeach()
        set(${FORTARGET}_SOURCES ${resourcesdep} ${filteredsources} PARENT_SCOPE)
    else()
        # use temporary file, then configure_file() to avoid all-in-one target rebuilds when the
        # project is reconfigured
        set(allinonetemp "${CMAKE_CURRENT_BINARY_DIR}/allinone_temporary.cpp")
        set(allinonesource "${CMAKE_CURRENT_BINARY_DIR}/${FORTARGET}_allinone.cpp")
        set(allinonedata)
        set(excludesources)
        foreach(srcstring ${ARGN})
            get_filename_component(srcext ${srcstring} EXT)
            get_source_file_property(skip ${srcstring} ALLINONE_EXCLUDE)
            if(skip OR "${srcext}" STREQUAL ".c")
                katie_warning("Source is excluded: ${srcstring}")
                set(excludesources ${excludesources} ${srcstring})
            elseif(NOT "${srcext}" MATCHES "(.h|.qrc|.ui)")
                set(allinonedata "${allinonedata}#include \"${srcstring}\"\n")
            endif()
        endforeach()
        file(WRITE "${allinonetemp}" "${allinonedata}")
        configure_file("${allinonetemp}" "${allinonesource}")
        set(${FORTARGET}_SOURCES ${resourcesdep} ${allinonesource} ${excludesources} PARENT_SCOPE)
    endif()
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

# a function to change full installation paths to relative so that CPack
# generators do not choke, still paths must contain a string of some sort that
# is not just CMAKE_INSTALL_PREFIX - if they are null after they have been made
# relative even quoting them will not help and CMake will complain that not
# enought arguments have been passed to install() for DESTINATION
function(KATIE_SETUP_PATHS)
    set(instpaths
        _PREFIX _HEADERS _LIBRARIES _BINARIES _PLUGINS _IMPORTS _DATA
        _TRANSLATIONS _SETTINGS _CMAKE _LDCONF _PROFILE _MAN
        _APPLICATIONS _PIXMAPS _PKGCONFIG
    )
    foreach(instpath ${instpaths})
        string(REGEX REPLACE ".*${CMAKE_INSTALL_PREFIX}/" "" modpath "${KATIE${instpath}_FULL}")
        string(REGEX REPLACE ".*${CMAKE_INSTALL_PREFIX}" "" modpath "${modpath}")
        set(KATIE${instpath}_RELATIVE "${modpath}" PARENT_SCOPE)
        # message(STATUS "KATIE${instpath}_RELATIVE: ${modpath}")
    endforeach()
endfunction()

# a macro to remove conditional code from headers which is only relevant to the
# process of building Katie itself
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

# a macro to add tests easily by setting them up with the assumptions they make
macro(KATIE_TEST TESTNAME TESTSOURCES)
    katie_resources(${TESTSOURCES} ${ARGN})

    add_executable(${TESTNAME} ${TESTSOURCES} ${ARGN})

    target_link_libraries(${TESTNAME} KtCore KtTest)
    target_compile_definitions(
        ${TESTNAME} PRIVATE
        -DSRCDIR="${CMAKE_CURRENT_SOURCE_DIR}/"
    )
    set_target_properties(
        ${TESTNAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )

    add_test(
        NAME ${TESTNAME}
        COMMAND ${CMAKE_BINARY_DIR}/exec.sh ${CMAKE_CURRENT_BINARY_DIR}/${TESTNAME} -tickcounter
    )
endmacro()

# a macro to add tests that require GUI easily by setting them up with the assumptions they make
macro(KATIE_GUI_TEST TESTNAME TESTSOURCES)
    katie_test(${TESTNAME} ${TESTSOURCES} ${ARGN})

    target_link_libraries(${TESTNAME} KtGui)
    target_compile_definitions(
        ${TESTNAME} PRIVATE
        -DSRCDIR="${CMAKE_CURRENT_SOURCE_DIR}/"
        -DQT_GUI_LIB
    )
endmacro()
