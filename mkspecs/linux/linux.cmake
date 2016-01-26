if("${KATIE_COMPILER}" MATCHES "(gcc|clang)")
    set(KATIE_CXXFLAGS
        ${KATIE_CXXFLAGS}
        -fPIC
        -Wno-unused
        -Wno-parentheses
        -Wformat
        -Werror=format-security
        -fvisibility=hidden
        -fvisibility-inlines-hidden
        -Wreturn-type -Wunused-function -Wundef
        # -Wall -fno-strict-aliasing -Wcast-align -Wchar-subscripts
        # -Wundef -Wmissing-noreturn -Winit-self -Wunused-comparison
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

