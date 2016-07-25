if("${KATIE_COMPILER}" STREQUAL "gcc")
    add_dependencies(-D__APPLE__ -D__GNUC__)
endif()

if("${KATIE_ARCHITECTURE}" STREQUAL "i386")
    set(KATIE_CXXFLAGS
        ${KATIE_CXXFLAGS}
        -arch i386
    )
elseif("${KATIE_ARCHITECTURE}" STREQUAL "x86_64")
    set(KATIE_CXXFLAGS
        ${KATIE_CXXFLAGS}
        -arch i386
        -Xarch_x86_64
        -mmacosx-version-min=10.5
    )
elseif("${KATIE_ARCHITECTURE}" STREQUAL "ppc")
    set(KATIE_CXXFLAGS
        ${KATIE_CXXFLAGS}
        -arch ppc
    )
elseif("${KATIE_ARCHITECTURE}" STREQUAL "ppc64")
    set(KATIE_CXXFLAGS
        ${KATIE_CXXFLAGS}
        -arch ppc64
        -Xarch_ppc64
        -mmacosx-version-min=10.5
    )
endif()

if("${KATIE_COMPILER}" MATCHES "(gcc|clang)")
    set(KATIE_CXXFLAGS
        ${KATIE_CXXFLAGS}
        -fvisibility=hidden -fvisibility-inlines-hidden
        -Wformat -Werror=format-security
        -Wall -Wundef -Wunused
        # -fno-strict-aliasing -Wcast-align
        # -Wmissing-noreturn -Winit-self -Winline -Wcast-qual
    )
    set(KATIE_LDFLAGS
        ${KATIE_LDFLAGS}
        -Wl,--no-undefined
        -Wl,-Bsymbolic-functions
        # -Wl,--dynamic-list
    )
    set(QT_VISIBILITY_AVAILABLE TRUE)
endif()

set(QT_PREFIX_PATH "${CMAKE_INSTALL_PREFIX}")
set(QT_HEADERS_PATH "${QT_PREFIX_PATH}/include")
set(QT_LIBRARIES_PATH "${QT_PREFIX_PATH}/lib")
set(QT_BINARIES_PATH "${QT_LIBRARIES_PATH}/bin")
set(QT_PLUGINS_PATH "${QT_LIBRARIES_PATH}/plugins")
set(QT_IMPORTS_PATH "${QT_LIBRARIES_PATH}/imports")
set(QT_DATA_PATH "${QT_PREFIX_PATH}")
set(QT_TRANSLATIONS_PATH "${QT_DATA_PATH}/translations")
set(QT_DOCUMENTATION_PATH "${QT_DATA_PATH}/doc")
set(QT_EXAMPLES_PATH "${QT_DATA_PATH}/examples")
set(QT_DEMOS_PATH "${QT_DATA_PATH}/demos")
# FIXME: that's incorrect, probably irrelevant once QStandardPaths are used
set(QT_SETTINGS_PATH "${QT_DATA_PATH}/settings")
