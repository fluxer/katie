file(GLOB TRANSLATIONS "*.po")

foreach(tmpres ${TRANSLATIONS})
    get_filename_component(resource "${tmpres}" ABSOLUTE)
    get_filename_component(rscname "${resource}" NAME_WE)
    make_directory("${CMAKE_CURRENT_BINARY_DIR}")
    set(rscout "${CMAKE_CURRENT_BINARY_DIR}/${rscname}.mo")
    if("${rscname}" MATCHES "qt_tools")
        set(rscbase "qt_tools")
        string(REPLACE "qt_tools_" "" rsclocale "${rscname}")
    else()
        set(rscbase "qt")
        string(REPLACE "qt_" "" rsclocale "${rscname}")
    endif()
    add_custom_target(
        translations_${rscname} ALL
        COMMAND ${INTL_MSGFMT} -v "${resource}" -o "${rscout}"
        COMMENT "Generating ${rscname}.mo"
    )
    set_source_files_properties("${rscout}" PROPERTIES GENERATED TRUE)
    install(
        FILES "${rscout}"
        DESTINATION "${KATIE_TRANSLATIONS_PATH}/${rsclocale}/LC_MESSAGES"
        RENAME "${rscbase}.mo"
        COMPONENT Runtime
    )
endforeach()
