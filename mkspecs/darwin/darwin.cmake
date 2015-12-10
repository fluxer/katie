set(KATIE_LDFLAGS
    ${KATIE_LDFLAGS}
    -headerpad_max_install_names
    -current_version" "
    -compatibility_version" "
)

add_definitions(-D__USE_WS_X11__)
