add_definitions(-DQT_PLUGIN)
set(EXTRA_PLUGINS_LIBS KtCore)

katie_setup_flags()

include_directories(
    ${CMAKE_BINARY_DIR}/include
    ${CMAKE_BINARY_DIR}/privateinclude
    ${CMAKE_BINARY_DIR}/include/QtCore
    ${CMAKE_BINARY_DIR}/privateinclude/QtCore
    ${CMAKE_BINARY_DIR}/include/QtGui
    ${CMAKE_BINARY_DIR}/privateinclude/QtGui
    ${CMAKE_BINARY_DIR}/include/QtDBus
    ${CMAKE_BINARY_DIR}/privateinclude/QtDBus
    ${CMAKE_BINARY_DIR}/include/QtDeclarative
    ${CMAKE_BINARY_DIR}/privateinclude/QtDeclarative
    ${CMAKE_BINARY_DIR}/include/QtDesigner
    ${CMAKE_BINARY_DIR}/privateinclude/QtDesigner
    ${CMAKE_BINARY_DIR}/include/QtNetwork
    ${CMAKE_BINARY_DIR}/privateinclude/QtNetwork
    ${CMAKE_BINARY_DIR}/include/QtScript
    ${CMAKE_BINARY_DIR}/privateinclude/QtScript
    ${CMAKE_BINARY_DIR}/include/QtSql
    ${CMAKE_BINARY_DIR}/privateinclude/QtSql
    ${CMAKE_BINARY_DIR}/include/QtOpenGL
    ${CMAKE_BINARY_DIR}/privateinclude/QtOpenGL
    ${CMAKE_BINARY_DIR}/include/QtSvg
    ${CMAKE_BINARY_DIR}/privateinclude/QtSvg
    ${CMAKE_CURRENT_SOURCE_DIR}/script
    ${CMAKE_CURRENT_SOURCE_DIR}/codecs
    ${CMAKE_CURRENT_SOURCE_DIR}/imageformats
    ${CMAKE_CURRENT_SOURCE_DIR}/iconengines
    ${CMAKE_CURRENT_SOURCE_DIR}/inputmethods
    ${CMAKE_CURRENT_SOURCE_DIR}/accessible
    ${CMAKE_CURRENT_SOURCE_DIR}/qmltooling
    ${CMAKE_SOURCE_DIR}/src/3rdparty/harfbuzz/src
)

if(NOT ${KATIE_TYPE} STREQUAL SHARED)
    add_definitions(-DQT_STATICPLUGIN)
endif()


if(WITH_ACCESSIBILITY)
    include(accessible/widgets/widgets.cmake)
endif()


if(${KATIE_PLATFORM} STREQUAL "linux")
    include(bearer/generic/generic.cmake)
    if(WITH_DBUS AND DBUS_FOUND)
        include(bearer/connman/connman.cmake)
        include(bearer/networkmanager/networkmanager.cmake)
    endif()
elseif(${KATIE_PLATFORM} STREQUAL "win32")
    if(WITH_ICD AND WITH_DBUS AND DBUS_FOUND)
        include(bearer/icd/icd.cmake)
    endif()
    # include(nla/nla.cmake)
    include(bearer/generic/generic.cmake)
    if(NOT ${KATIE_PLATFORM} STREQUAL "wince")
        include(bearer/nativewifi/nativewifi.cmake)
    endif()
elseif(${KATIE_PLATFORM} STREQUAL "mac")
    include(bearer/generic/generic.cmake)
    if(WITH_COREWLAN)
        include(bearer/corewlan/corewlan.cmake)
    endif()
elseif(${KATIE_PLATFORM} STREQUAL "blackberry")
    include(bearer/blackberry/blackberry.cmake)
else()
    include(bearer/generic/generic.cmake)
endif()


if(WITH_ICONV AND ICONV_FOUND)
    include(codecs/cn/cn.cmake)
    include(codecs/jp/jp.cmake)
    include(codecs/kr/kr.cmake)
    include(codecs/tw/tw.cmake)
endif()


if(${KATIE_PLATFORM} STREQUAL "meego" AND WITH_OPENGLES1 AND WITH_EGL AND EGL_FOUND)
    include(graphicssystems/meego/meego.cmake)
endif()
if(WITH_OPENGL AND OPENGL_FOUND)
    include(graphicssystems/opengl/opengl.cmake)
endif()


include(iconengines/svgiconengine/svgiconengine.cmake)


include(imageformats/gif/gif.cmake)
include(imageformats/ico/ico.cmake)
if(WITH_JPEG AND JPEG_FOUND)
    include(imageformats/jpeg/jpeg.cmake)
endif()
if(WITH_MNG AND MNG_FOUND)
    include(imageformats/mng/mng.cmake)
endif()
include(imageformats/svg/svg.cmake)
include(imageformats/tga/tga.cmake)
if(WITH_TIFF AND TIFF_FOUND)
    include(imageformats/tiff/tiff.cmake)
endif()

if(NOT ${KATIE_PLATFORM} STREQUAL "mac")
    include(inputmethods/imsw-multi/imsw-multi.cmake)
endif()


if(WITH_DBUS AND DBUS_FOUND)
    include(script/qtdbus/qtdbus.cmake)
endif()


include(designer/qdeclarativeview/qdeclarativeview.cmake)
