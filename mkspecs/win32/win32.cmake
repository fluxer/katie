add_definitions(-D_USE_MATH_DEFINES -DUNICODE -DWIN32)
set(KATIE_TOOLS "${KATIE_TOOLS} idc")

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
# FIXME: that's incorrect
set(QT_SETTINGS_PATH "${QT_DATA_PATH}/settings")

set(WITH_FREETYPE OFF)
set(WITH_FONTCONFIG OFF)
# set(WITH_ICD ON)
# TODO: enable directwrite on Vista+
# set(WITH_DIRECTWRITE ON)
