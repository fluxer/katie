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
# not handle.
function(KATIE_FIXUP_STRING INSTR OUTSTR)
    string(STRIP "${INSTR}" instrtrimmed)
    if("${instrtrimmed}" STREQUAL "")
        set(${OUTSTR} " " PARENT_SCOPE)
    else()
        string(REPLACE ";" " " modstring "${INSTR}")
        set(${OUTSTR} "${modstring}" PARENT_SCOPE)
    endif()
endfunction()

function(KATIE_SETUP_TARGET FORTARGET)
    set(resourcesdep "${CMAKE_CURRENT_BINARY_DIR}/${FORTARGET}_resources.cpp")
    if(NOT EXISTS "${resourcesdep}")
        file(WRITE "${resourcesdep}" "enum { CompilersWorkaroundAlaAutomoc = 1 };\n")
    endif()
    set(targetresources)
    foreach(tmpres ${ARGN})
        get_filename_component(resource ${tmpres} ABSOLUTE)
        get_source_file_property(skip ${resource} SKIP_RESOURCE)
        if(NOT skip)
            get_filename_component(rscext ${resource} EXT)
            get_filename_component(rscname ${resource} NAME_WE)
            get_filename_component(rscpath ${resource} PATH)
            string(REPLACE "${CMAKE_SOURCE_DIR}" "${CMAKE_BINARY_DIR}" rscpath "${rscpath}")
            make_directory(${rscpath})
            if("${rscext}" STREQUAL ".ui")
                set(rscout "${rscpath}/ui_${rscname}.h")
                add_custom_command(
                    OUTPUT "${rscout}"
                    COMMAND "${KATIE_UIC}" "${resource}" -o "${rscout}"
                )
                set(targetresources ${targetresources} ${rscout})
            elseif("${rscext}" STREQUAL ".qrc")
                set(rscout "${rscpath}/qrc_${rscname}.cpp")
                add_custom_command(
                    OUTPUT "${rscout}"
                    COMMAND "${KATIE_RCC}" "${resource}" -o "${rscout}" -name "${rscname}"
                )
                set(targetresources ${targetresources} ${rscout})
            elseif("${rscext}" MATCHES "(.h|.cpp|.mm)")
                file(READ "${resource}" rsccontent)
                # this can be simpler if continue() was supported by old CMake versions
                if(NOT "${rsccontent}" MATCHES "(Q_OBJECT|Q_OBJECT_FAKE|Q_GADGET)")
                    continue()
                endif()

                set(rscout "${rscpath}/moc_${rscname}${rscext}")
                get_directory_property(dirdefs COMPILE_DEFINITIONS)
                get_directory_property(dirincs INCLUDE_DIRECTORIES)
                set(mocargs)
                foreach(ddef ${dirdefs})
                    # TODO: filter non -D, support -U too
                    set(mocargs ${mocargs} -D${ddef})
                endforeach()
                foreach(incdir ${dirincs})
                    set(mocargs ${mocargs} -I${incdir})
                endforeach()
                add_custom_command(
                    OUTPUT "${rscout}"
                    COMMAND "${KATIE_MOC}" -nw "${resource}" -o "${rscout}" ${mocargs}
                )
                set(targetresources ${targetresources} ${rscout})
                # this can be troublesome but common sources can cause multiple
                # rules on the same file
                set_source_files_properties(${resource} PROPERTIES SKIP_RESOURCE TRUE)
            endif()
        endif()
    endforeach()
    set_source_files_properties(${resourcesdep} PROPERTIES OBJECT_DEPENDS "${targetresources}")

    if(NOT KATIE_ALLINONE)
        set(${FORTARGET}_SOURCES ${resourcesdep} ${ARGN} PARENT_SCOPE)
    # blacklisted targets are failing and too important for other components
    elseif("${FORTARGET}" MATCHES "(KtCore|KtGui)")
        katie_warning("All-in-one build not yet support for: ${FORTARGET}")
        set(${FORTARGET}_SOURCES ${resourcesdep} ${ARGN} PARENT_SCOPE)
    else()
        set(allinonesource "${CMAKE_CURRENT_BINARY_DIR}/${FORTARGET}_allinone.cpp")
        set(allinonedata)
        set(targetobjects)
        foreach(srcstring ${ARGN})
            get_filename_component(srcname ${srcstring} EXT)
            if(NOT "${srcname}" MATCHES "(.h|.qrc|.ui)")
                set(allinonedata "${allinonedata}#include \"${srcstring}\"\n")
            endif()
        endforeach()
        file(WRITE ${allinonesource} "${allinonedata}")
        set(${FORTARGET}_SOURCES ${resourcesdep} ${allinonesource} PARENT_SCOPE)
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
