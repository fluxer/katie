if("${KATIE_COMPILER}" MATCHES "(gcc|clang)")
    set(KATIE_CXXFLAGS
        ${KATIE_CXXFLAGS}
        -fPIC
        -fvisibility=hidden -fvisibility-inlines-hidden
        -Wformat -Werror=format-security
        -Wreturn-type -Wundef -Wunused -Wparentheses
        # -Wall -fno-strict-aliasing -Wcast-align -Wchar-subscripts
        # -Wmissing-noreturn -Winit-self -Winline -Wcast-qual
    )
    set(KATIE_LDFLAGS
        ${KATIE_LDFLAGS}
        -Wl,-O1
        -Wl,--no-undefined
        -Wl,-Bsymbolic-functions
        # -Wl,--dynamic-list
    )
    set(QT_VISIBILITY_AVAILABLE TRUE)
endif()

set(LDCONF_INSTALL_PATH "${SYSCONF_INSTALL_DIR}/ld.so.conf.d")
set(PROFILE_INSTALL_PATH "${SYSCONF_INSTALL_DIR}/profile.d")
set(MAN_INSTALL_PATH "${CMAKE_INSTALL_PREFIX}/share/man")
set(CMAKE_INSTALL_PATH "${CMAKE_INSTALL_PREFIX}/share/cmake")
set(APPLICATIONS_INSTALL_PATH "${CMAKE_INSTALL_PREFIX}/share/applications")
set(PIXMAPS_INSTALL_PATH "${CMAKE_INSTALL_PREFIX}/share/pixmaps")

set(QT_PREFIX_PATH "${CMAKE_INSTALL_PREFIX}")
set(QT_HEADERS_PATH "${QT_PREFIX_PATH}/include/katie")
set(QT_LIBRARIES_PATH "${QT_PREFIX_PATH}/lib/katie")
set(QT_BINARIES_PATH "${QT_LIBRARIES_PATH}/bin")
set(QT_PLUGINS_PATH "${QT_LIBRARIES_PATH}/plugins")
set(QT_IMPORTS_PATH "${QT_LIBRARIES_PATH}/imports")
set(QT_DATA_PATH "${QT_PREFIX_PATH}/share")
set(QT_TRANSLATIONS_PATH "${QT_DATA_PATH}/translations")
set(QT_DOCUMENTATION_PATH "${QT_DATA_PATH}/doc")
set(QT_EXAMPLES_PATH "${QT_DATA_PATH}/examples")
set(QT_DEMOS_PATH "${QT_DATA_PATH}/demos")
set(QT_SETTINGS_PATH "/etc/xdg")

