# Copyright (c) 2015, Ivailo Monev, <xakepa10@gmail.com>
# Redistribution and use is allowed according to the terms of the BSD license.

macro(KATIE_RESOURCES RESOURCES)
    foreach(tmpres ${RESOURCES} ${ARGN})
        get_filename_component(resource ${tmpres} ABSOLUTE)
        get_source_file_property(skip ${resource} SKIP_RESOURCE)
        if(NOT skip)
            get_filename_component(rscext ${resource} EXT)
            get_filename_component(rscname ${resource} NAME_WE)
            get_filename_component(rscpath ${resource} PATH)
            string(REPLACE "${CMAKE_SOURCE_DIR}" "${CMAKE_BINARY_DIR}" rscpath "${rscpath}")
            if("${rscext}" STREQUAL ".ui")
                set(rscout ${rscpath}/ui_${rscname}.h)
                make_directory(${rscpath})
                add_custom_command(
                    OUTPUT ${rscout}
                    COMMAND ${KATIE_UIC} "${resource}" -o "${rscout}"
                    MAIN_DEPENDENCY ${resource}
                )
            elseif("${rscext}" STREQUAL ".qrc")
                set(rscout ${rscpath}/qrc_${rscname}.cpp)
                make_directory(${rscpath})
                add_custom_command(
                    OUTPUT ${rscout}
                    COMMAND ${KATIE_RCC} "${resource}" -o "${rscout}" -name "${rscname}"
                    MAIN_DEPENDENCY ${resource}
                )
                set_property(SOURCE ${resource} APPEND PROPERTY OBJECT_DEPENDS ${rscout})
            elseif("${rscext}" MATCHES "(.h|.cpp|.mm)")
                file(READ "${resource}" rsccontent)
                # this can be simpler if continue() was supported by old CMake versions
                if("${rsccontent}" MATCHES "(Q_OBJECT|Q_OBJECT_FAKE|Q_GADGET)")
                    set(rscout ${rscpath}/moc_${rscname}${rscext})
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
                    make_directory(${rscpath})
                    add_custom_command(
                        OUTPUT ${rscout}
                        COMMAND ${KATIE_MOC} -nw "${resource}" -o "${rscout}" ${mocargs}
                    )
                    set_property(SOURCE ${resource} APPEND PROPERTY OBJECT_DEPENDS ${rscout})
                    # XXX: this can be troublesome but common sources can cause multiple rules on the same file
                    set_source_files_properties(${resource} PROPERTIES SKIP_RESOURCE TRUE)
                endif()
            endif()
        endif()
    endforeach()
endmacro()

macro(KATIE_DBUS_ADAPTOR SRCDEP SRCIN OUTNAME)
    get_filename_component(resource ${SRCIN} ABSOLUTE)
    set(rscout ${CMAKE_CURRENT_BINARY_DIR}/${OUTNAME}.h)
    set(mocout ${CMAKE_CURRENT_BINARY_DIR}/${OUTNAME}.moc)
    add_custom_command(
        OUTPUT "${rscout}"
        COMMAND "${KATIE_QDBUSXML2CPP}" -m "${resource}" -a "${rscout}" -p "${OUTNAME}" ${ARGN}
        COMMAND "${KATIE_MOC}" -nw "${rscout}" -o "${mocout}" -i
    )
    set_property(SOURCE ${SRCDEP} APPEND PROPERTY OBJECT_DEPENDS ${rscout})
endmacro()

macro(KATIE_DBUS_INTERFACE SRCIN)
    string(REGEX MATCH ".*\\.(.*)\\.xml" ${SRCIN} OUTNAME)
    string(TOLOWER ${SRCIN} SRCIN)
    set(rscout ${CMAKE_CURRENT_BINARY_DIR}/${OUTNAME}ineterface.h)
    add_custom_command(
        OUTPUT "${rscout}"
        COMMAND "${KATIE_QDBUSXML2CPP}" -m "${SRCIN}" -a "${rscout}" -p "${OUTNAME}ineterface" ${ARGN}
    )
    set_property(SOURCE ${SRCIN} APPEND PROPERTY OBJECT_DEPENDS ${rscout})
endmacro()

macro(KATIE_TRANSLATIONS TRANSLATIONS)
    if(NOT KATIE_TRANSLATIONS_RELATIVE)
        message(SEND_ERROR "Directory where the translation should be installed is not set")
    endif()
    foreach(translation ${TRANSLATIONS} ${ARGN})
        get_filename_component(trname ${translation} NAME_WE)
        get_filename_component(trdir ${translation} DIRECTORY)
        string(REPLACE "${CMAKE_SOURCE_DIR}" "${CMAKE_BINARY_DIR}" trdir ${trdir})
        make_directory(${trdir})
        set(trout ${trdir}/${trname}.qm)
        add_custom_target(
            ${trname}_translation ALL
            COMMAND "${KATIE_LRELEASE}" "${translation}" -qm "${trout}"
        )
        set_source_files_properties(${trout} PROPERTIES GENERATED TRUE)
        install(FILES ${trout} DESTINATION ${KATIE_TRANSLATIONS_RELATIVE})
    endforeach()
endmacro()

macro(KATIE_DEFINITION DEF)
    set(KATIE_DEFINITIONS ${KATIE_DEFINITIONS} ${DEF} ${ARGN})
    add_definitions(${DEF} ${ARGN})
endmacro()
