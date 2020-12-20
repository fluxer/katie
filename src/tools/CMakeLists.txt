add_definitions(
    -DKATIE_TOOLS_SUFFIX="${KATIE_TOOLS_SUFFIX}"
)

# semi-bootstraped
add_subdirectory(moc)

# essential tools
add_subdirectory(rcc)
add_subdirectory(uic)

# not so much
if(WITH_DBUS AND DBUS_FOUND)
    add_subdirectory(qdbus)
    add_subdirectory(qdbusviewer)
endif()
add_subdirectory(qscript)
add_subdirectory(qtconfig)
add_subdirectory(designer)
