add_definitions(
    -DKATIE_TOOLS_SUFFIX="${KATIE_TOOLS_SUFFIX}"
    -DKATIE_PIXMAPS_PATH="${KATIE_PIXMAPS_PATH}"
)

# semi-bootstraped
add_subdirectory(moc)

# essential tools
add_subdirectory(uic)

# not so much
if(WITH_DBUS AND DBUS_FOUND)
    add_subdirectory(qdbus)
    add_subdirectory(qdbusviewer)
endif()
if(WITH_GETTEXTPO AND GETTEXTPO_FOUND)
    add_subdirectory(trc)
endif()
add_subdirectory(qscript)
