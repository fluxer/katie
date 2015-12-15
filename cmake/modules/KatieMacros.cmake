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
            make_directory(${rscpath})
            if("${rscext}" STREQUAL ".ui")
                set(rscout ${rscpath}/ui_${rscname}.h)
                add_custom_command(
                    OUTPUT ${rscout}
                    COMMAND ${KATIE_UIC} "${resource}" -o "${rscout}"
                    MAIN_DEPENDENCY ${resource}
                )
            elseif("${rscext}" STREQUAL ".qrc")
                set(rscout ${rscpath}/qrc_${rscname}.cpp)
                add_custom_command(
                    OUTPUT ${rscout}
                    COMMAND ${KATIE_RCC} "${resource}" -o "${rscout}" -name "${rscname}"
                    MAIN_DEPENDENCY ${resource}
                )
                set_property(SOURCE ${resource} APPEND PROPERTY OBJECT_DEPENDS ${rscout})
            elseif("${rscext}" MATCHES "(.h|.cpp)")
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
                add_custom_command(
                    OUTPUT "${rscout}"
                    COMMAND "${KATIE_MOC}" -nw "${resource}" -o "${rscout}" ${mocargs}
                )
                set_property(SOURCE ${resource} APPEND PROPERTY OBJECT_DEPENDS ${rscout})
                # XXX: this can be troublesome but common sources can cause multiple rules on the same file
                set_source_files_properties(${resource} PROPERTIES SKIP_RESOURCE TRUE)
            endif()
        endif()
    endforeach()
endmacro()

macro(KATIE_RESOURCE SRCDEP RESOURCES OUTNAME)
    set(rscout ${CMAKE_CURRENT_BINARY_DIR}/qrc_${OUTNAME}.cpp)
    add_custom_command(
        OUTPUT "${rscout}"
        COMMAND "${KATIE_RCC}" ${RESOURCES} -o "${rscout}" -name "${OUTNAME}"
        # MAIN_DEPENDENCY ${resource}
    )
    set_property(SOURCE ${SRCDEP} APPEND PROPERTY OBJECT_DEPENDS ${rscout})
endmacro()

macro(KATIE_DBUS_ADAPTOR SRCDEP SRCIN SRCOUT)
    get_filename_component(resource ${SRCIN} ABSOLUTE)
    set(rscout ${CMAKE_CURRENT_BINARY_DIR}/${SRCOUT}.h)
    set(mocout ${CMAKE_CURRENT_BINARY_DIR}/${SRCOUT}.moc)
    add_custom_command(
        OUTPUT "${rscout}"
        COMMAND "${KATIE_QDBUSXML2CPP}" -m "${resource}" -a "${rscout}" -p "${SRCOUT}" ${ARGN}
        COMMAND "${KATIE_MOC}" -nw "${rscout}" -o "${mocout}" -i
    )
    set_property(SOURCE ${SRCDEP} APPEND PROPERTY OBJECT_DEPENDS ${rscout})
endmacro()

macro(KATIE_DBUS_INTERFACE SRCIN)
    string(REGEX MATCH ".*\\.(.*)\\.xml" ${SRCIN} SRCOUT)
    string(TOLOWER ${SRCIN} SRCIN)
    set(rscout ${CMAKE_CURRENT_BINARY_DIR}/${SRCOUT}ineterface.h)
    add_custom_command(
        OUTPUT "${rscout}"
        COMMAND "${KATIE_QDBUSXML2CPP}" -m "${SRCIN}" -a "${rscout}" -p "${SRCOUT}ineterface" ${ARGN}
    )
    set_property(SOURCE ${SRCIN} APPEND PROPERTY OBJECT_DEPENDS ${rscout})
endmacro()

macro(KATIE_TRANSLATIONS TRANSLATIONS)
    if(NOT QT_TRANSLATIONS_PATH_INST)
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
        file(WRITE ${trout} "This file exists only to keep the CMake logic straight")
        install(FILES ${trout} DESTINATION ${QT_TRANSLATIONS_PATH_INST})
    endforeach()
endmacro()

macro(KATIE_DEFINITION DEF)
    set(KATIE_DEFINITIONS ${KATIE_DEFINITIONS} ${DEF} ${ARGN})
    add_definitions(${DEF} ${ARGN})
endmacro()
