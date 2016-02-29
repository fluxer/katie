set(GUI_HEADERS
    ${GUI_HEADERS}
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qaction.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qaction_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qactiongroup.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qapplication.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qapplication_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qboxlayout.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qclipboard.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qcursor.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qdesktopwidget.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qdrag.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qdnd_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qevent.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qevent_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qformlayout.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qgridlayout.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qkeysequence.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qlayout.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qlayout_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qlayoutengine_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qlayoutitem.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qmime.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qsessionmanager.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qshortcut.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qshortcutmap_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qsizepolicy.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qpalette.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qstackedlayout.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qtooltip.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qwhatsthis.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qwidget.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qwidget_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qwidgetaction.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qwidgetaction_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qwindowdefs.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qkeymapper_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qgesture.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qgesture_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qstandardgestures_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qgesturerecognizer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qgesturemanager_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qsoftkeymanager_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qsoftkeymanager_common_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qguiplatformplugin_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qt_x11_p.h
)

set(GUI_SOURCES
    ${GUI_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qaction.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qactiongroup.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qapplication.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qboxlayout.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qclipboard.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qcursor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qdrag.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qdnd.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qevent.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qformlayout.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qgridlayout.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qkeysequence.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qlayout.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qlayoutengine.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qlayoutitem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qmime.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qpalette.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qshortcut.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qshortcutmap.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qstackedlayout.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qtooltip.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qguivariant.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qwhatsthis.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qwidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qwidgetaction.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qkeymapper.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qgesture.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qstandardgestures.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qgesturerecognizer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qgesturemanager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qsoftkeymanager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qdesktopwidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qguiplatformplugin.cpp
)

if(UNIX AND WITH_X11 AND X11_FOUND)
    set(GUI_HEADERS
        ${GUI_HEADERS}
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qx11embed_x11.h
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qx11info_x11.h
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qkde_p.h
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qeventdispatcher_x11_p.h
    )
    set(GUI_SOURCES
        ${GUI_SOURCES}
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qapplication_x11.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qclipboard_x11.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qcursor_x11.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qdnd_x11.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qdesktopwidget_x11.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qmotifdnd_x11.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qsound_x11.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qwidget_x11.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qwidgetcreate_x11.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qx11embed_x11.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qx11info_x11.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qkeymapper_x11.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qkde.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qeventdispatcher_x11.cpp
    )
    if(WITH_GLIB2 AND GLIB2_FOUND)
        set(GUI_HEADERS
            ${GUI_HEADERS}
            ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qguieventdispatcher_glib_p.h
        )
        set(GUI_SOURCES
            ${GUI_SOURCES}
            ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qguieventdispatcher_glib.cpp
        )
        set(EXTRA_GUI_LIBS
            ${EXTRA_GUI_LIBS}
            ${GLIB2_LIBRARIES}
        )
        include_directories(${GLIB2_INCLUDES})
    endif()
    if(WITH_NAS AND NAS_FOUND)
        set(EXTRA_GUI_LIBS
            ${EXTRA_GUI_LIBS}
            ${NAS_LIBRARIES}
        )
        include_directories(${NAS_INCLUDES})
    endif()
    include_directories(${CMAKE_SOURCE_DIR}/src/3rdparty/xorg)
elseif(KATIE_PLATFORM STREQUAL "win32")
    set(GUI_HEADERS
        ${GUI_HEADERS}
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qwindowdefs_win.h
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qwinnativepangesturerecognizer_win_p.h
    )
    set(GUI_SOURCES
        ${GUI_SOURCES}
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qapplication_win.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qclipboard_win.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qcursor_win.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qdesktopwidget_win.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qdnd_win.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qmime_win.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qsound_win.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qwidget_win.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qole_win.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qkeymapper_win.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qwinnativepangesturerecognizer_win.cpp
    )
    add_definitions(-DQT_NO_DIRECTDRAW)
    include_directories(${CMAKE_SOURCE_DIR}/src/3rdparty/wintab)
    set(EXTRA_GUI_LIBS
        ${EXTRA_GUI_LIBS}
        winmm
    )
elseif(KATIE_PLATFORM STREQUAL "wince")
    set(GUI_HEADERS
        ${GUI_HEADERS}
        ${CMAKE_SOURCE_DIR}/core/kernel/qfunctions_wince.h
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qguifunctions_wince.h
    )
    set(GUI_SOURCES
        ${GUI_SOURCES}
        ${CMAKE_SOURCE_DIR}/core/kernel/qfunctions_wince.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qguifunctions_wince.cpp
    )
elseif(KATIE_PLATFORM STREQUAL "qws")
    set(GUI_HEADERS
        ${GUI_HEADERS}
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qeventdispatcher_qws_p.h
    )
    set(GUI_SOURCES
        ${GUI_SOURCES}
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qapplication_qws.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qclipboard_qws.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qcursor_qws.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qdesktopwidget_qws.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qdnd_qws.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qeventdispatcher_qws.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qsound_qws.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qwidget_qws.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qkeymapper_qws.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qsessionmanager_qws.cpp
    )
    if(WITH_GLIB2 AND GLIB2_FOUND)
        set(GUI_HEADERS
            ${GUI_HEADERS}
            ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qeventdispatcher_glib_qws_p.h
        )
        set(GUI_SOURCES
            ${GUI_SOURCES}
            ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qeventdispatcher_glib_qws.cpp
        )
        set(EXTRA_GUI_LIBS
            ${EXTRA_GUI_LIBS}
            ${GLIB2_LIBRARIES}
        )
        include_directories(${GLIB2_INCLUDES})
    endif()
elseif(KATIE_PLATFORM STREQUAL "qpa")
    set(GUI_HEADERS
        ${GUI_HEADERS}
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qgenericpluginfactory_qpa.h
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qgenericplugin_qpa.h
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qeventdispatcher_qpa_p.h
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qwindowsysteminterface_qpa.h
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qwindowsysteminterface_qpa_p.h
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qplatformintegration_qpa.h
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qplatformscreen_qpa.h
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qplatformintegrationfactory_qpa_p.h
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qplatformintegrationplugin_qpa.h
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qplatformwindow_qpa.h
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qplatformwindowformat_qpa.h
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qplatformglcontext_qpa.h
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qdesktopwidget_qpa_p.h
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qplatformeventloopintegration_qpa.h
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qplatformcursor_qpa.h
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qplatformclipboard_qpa.h
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qplatformnativeinterface_qpa.h
    )
    set(GUI_SOURCES
        ${GUI_SOURCES}
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qapplication_qpa.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qclipboard_qpa.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qcursor_qpa.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qdnd_qws.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qdesktopwidget_qpa.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qgenericpluginfactory_qpa.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qgenericplugin_qpa.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qkeymapper_qws.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qwidget_qpa.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qeventdispatcher_qpa.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qwindowsysteminterface_qpa.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qplatformintegration_qpa.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qplatformscreen_qpa.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qplatformintegrationfactory_qpa.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qplatformintegrationplugin_qpa.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qplatformwindow_qpa.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qplatformwindowformat_qpa.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qplatformeventloopintegration_qpa.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qplatformglcontext_qpa.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qplatformcursor_qpa.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qplatformclipboard_qpa.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qplatformnativeinterface_qpa.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qsessionmanager_qpa.cpp
    )
    if(WITH_GLIB2 AND GLIB2_FOUND)
        set(GUI_HEADERS
            ${GUI_HEADERS}
            ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qeventdispatcher_glib_qpa_p.h
        )
        set(GUI_SOURCES
            ${GUI_SOURCES}
            ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qeventdispatcher_glib_qpa.cpp
        )
        set(EXTRA_GUI_LIBS
            ${EXTRA_GUI_LIBS}
            ${GLIB2_LIBRARIES}
        )
        include_directories(${GLIB2_INCLUDES})
        if(KATIE_PLATFORM STREQUAL "blackberry")
            set(GUI_HEADERS
                ${GUI_HEADERS}
                ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qeventdispatcher_blackberry_qpa_p.h
            )
            set(GUI_SOURCES
                ${GUI_SOURCES}
                ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qeventdispatcher_blackberry_qpa.cpp
            )
        endif()
    endif()
elseif(KATIE_PLATFORM STREQUAL "mac")
    set(GUI_HEADERS
        ${GUI_HEADERS}
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qt_cocoa_helpers_mac_p.h
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qcocoaapplication_mac_p.h
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qcocoaapplicationdelegate_mac_p.h
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qeventdispatcher_mac_p.h
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qcocoawindow_mac_p.h
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qcocoapanel_mac_p.h
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qcocoawindowdelegate_mac_p.h
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qcocoaview_mac_p.h
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qcocoaapplication_mac_p.h
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qcocoaapplicationdelegate_mac_p.h
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qmacgesturerecognizer_mac_p.h
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qmultitouch_mac_p.h
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qcocoasharedwindowmethods_mac_p.h
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qcocoaintrospection_p.h
    )
    set(GUI_SOURCES
        ${GUI_SOURCES}
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qclipboard_mac.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qmime_mac.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qt_mac.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qkeymapper_mac.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qcursor_mac.mm
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qdnd_mac.mm
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qsound_mac.mm 
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qapplication_mac.mm
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qwidget_mac.mm
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qcocoapanel_mac.mm
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qcocoaview_mac.mm
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qcocoawindow_mac.mm
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qcocoawindowdelegate_mac.mm
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qcocoamenuloader_mac.mm
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qcocoaapplication_mac.mm
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qcocoaapplicationdelegate_mac.mm
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qt_cocoa_helpers_mac.mm
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qdesktopwidget_mac.mm
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qeventdispatcher_mac.mm
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qcocoawindowcustomthemeframe_mac.mm
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qmacgesturerecognizer_mac.mm
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qmultitouch_mac.mm
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qcocoaintrospection_mac.mm
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/mac/macresources.qrc
    )

    # TODO: mac MENU_NIB
#     MENU_NIB.files = mac/qt_menu.nib
#     MENU_NIB.path = Resources
#     MENU_NIB.version = Versions
#     QMAKE_BUNDLE_DATA += MENU_NIB

    set(KATIE_LDFLAGS
        ${KATIE_LDFLAGS}
        -framework AppKit
        -framework Carbon
        -lz
    )
endif()

if(NOT KATIE_PLATFORM STREQUAL "qpa")
    set(GUI_HEADERS
        ${GUI_HEADERS}
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qsound.h
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qsound_p.h
    )
    set(GUI_SOURCES
        ${GUI_SOURCES}
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qsound.cpp
    )
endif()
