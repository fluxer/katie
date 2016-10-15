# semi-bootstraped
add_subdirectory(moc)

add_subdirectory(rcc)
add_subdirectory(uic)
add_subdirectory(lrelease)
add_subdirectory(lupdate)
add_subdirectory(lconvert)

if(NOT KATIE_BOOTSTRAP)
    if(WITH_DBUS AND DBUS_FOUND)
        add_subdirectory(qdbus)
        add_subdirectory(qdbusviewer)
    endif()
    add_subdirectory(qscript)
    add_subdirectory(qtconfig)
    add_subdirectory(designer)
endif()
