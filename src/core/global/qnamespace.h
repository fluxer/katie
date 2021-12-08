/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the QtCore module of the Katie Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
**
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QNAMESPACE_H
#define QNAMESPACE_H

#include <QtCore/qglobal.h>


QT_BEGIN_NAMESPACE


#ifndef Q_MOC_RUN
namespace
#else
class Q_CORE_EXPORT
#endif
Qt {

#if defined(Q_MOC_RUN)
    Q_OBJECT

    // NOTE: Generally, do not add Q_ENUMS if a corresponding Q_FLAGS exists.
    Q_ENUMS(ScrollBarPolicy FocusPolicy ContextMenuPolicy)
    Q_ENUMS(ArrowType ToolButtonStyle PenStyle PenCapStyle PenJoinStyle BrushStyle)
    Q_ENUMS(FillRule MaskMode BGMode ClipOperation SizeMode)
    Q_ENUMS(Axis Corner LayoutDirection SizeHint Orientation DropAction)
    Q_FLAGS(Alignment Orientations DropActions)
    Q_FLAGS(DockWidgetAreas ToolBarAreas)
    Q_ENUMS(DockWidgetArea ToolBarArea)
    Q_ENUMS(TextFormat)
    Q_ENUMS(TextElideMode)
    Q_ENUMS(DateFormat TimeSpec DayOfWeek)
    Q_ENUMS(CursorShape GlobalColor)
    Q_ENUMS(AspectRatioMode TransformationMode)
    Q_FLAGS(ImageConversionFlags)
    Q_ENUMS(Key ShortcutContext)
    Q_ENUMS(TextInteractionFlag)
    Q_FLAGS(TextInteractionFlags)
    Q_ENUMS(ItemSelectionMode)
    Q_FLAGS(ItemFlags)
    Q_ENUMS(CheckState)
    Q_ENUMS(SortOrder CaseSensitivity)
    Q_FLAGS(MatchFlags)
    Q_FLAGS(KeyboardModifiers MouseButtons)
    Q_ENUMS(WindowType WindowState WindowModality WidgetAttribute ApplicationAttribute)
    Q_FLAGS(WindowFlags WindowStates)
    Q_ENUMS(ConnectionType)
    Q_ENUMS(CursorMoveStyle)

public:
#endif // defined(Q_MOC_RUN)

    enum GlobalColor {
        color0,
        color1,
        black,
        white,
        darkGray,
        gray,
        lightGray,
        red,
        green,
        blue,
        cyan,
        magenta,
        yellow,
        darkRed,
        darkGreen,
        darkBlue,
        darkCyan,
        darkMagenta,
        darkYellow,
        transparent
    };

    enum Key {
        // misc
        Key_Backspace                     = 0xff08,
        Key_Tab                           = 0xff09,
        Key_Linefeed                      = 0xff0a,
        Key_Clear                         = 0xff0b,
        Key_Return                        = 0xff0d,
        Key_Pause                         = 0xff13,
        Key_Scroll_Lock                   = 0xff14,
        Key_Sys_Req                       = 0xff15,
        Key_Escape                        = 0xff1b,
        Key_Delete                        = 0xffff,
        Key_Multi_key                     = 0xff20,
        Key_Codeinput                     = 0xff37,
        Key_SingleCandidate               = 0xff3c,
        Key_MultipleCandidate             = 0xff3d,
        Key_PreviousCandidate             = 0xff3e,
        Key_Kanji                         = 0xff21,
        Key_Muhenkan                      = 0xff22,
        Key_Henkan                        = 0xff23,
        Key_Romaji                        = 0xff24,
        Key_Hiragana                      = 0xff25,
        Key_Katakana                      = 0xff26,
        Key_Hiragana_Katakana             = 0xff27,
        Key_Zenkaku                       = 0xff28 ,
        Key_Hankaku                       = 0xff29,
        Key_Zenkaku_Hankaku               = 0xff2a,
        Key_Touroku                       = 0xff2b,
        Key_Massyo                        = 0xff2c,
        Key_Kana_Lock                     = 0xff2d,
        Key_Kana_Shift                    = 0xff2e,
        Key_Eisu_Shift                    = 0xff2f,
        Key_Eisu_toggle                   = 0xff30,
        Key_Home                          = 0xff50,
        Key_Left                          = 0xff51,
        Key_Up                            = 0xff52,
        Key_Right                         = 0xff53,
        Key_Down                          = 0xff54,
        Key_Page_Up                       = 0xff55,
        Key_Page_Down                     = 0xff56,
        Key_End                           = 0xff57,
        Key_Begin                         = 0xff58,
        Key_Select                        = 0xff60,
        Key_Print                         = 0xff61,
        Key_Execute                       = 0xff62,
        Key_Insert                        = 0xff63,
        Key_Undo                          = 0xff65,
        Key_Redo                          = 0xff66,
        Key_Menu                          = 0xff67,
        Key_Find                          = 0xff68,
        Key_Cancel                        = 0xff69,
        Key_Help                          = 0xff6a,
        Key_Break                         = 0xff6b,
        Key_Mode_switch                   = 0xff7e,
        Key_Num_Lock                      = 0xff7f,
        Key_F1                            = 0xffbe,
        Key_F2                            = 0xffbf,
        Key_F3                            = 0xffc0,
        Key_F4                            = 0xffc1,
        Key_F5                            = 0xffc2,
        Key_F6                            = 0xffc3,
        Key_F7                            = 0xffc4,
        Key_F8                            = 0xffc5,
        Key_F9                            = 0xffc6,
        Key_F10                           = 0xffc7,
        Key_F11                           = 0xffc8,
        Key_F12                           = 0xffc9,
        Key_F13                           = 0xffca,
        Key_F14                           = 0xffcb,
        Key_F15                           = 0xffcc,
        Key_F16                           = 0xffcd,
        Key_F17                           = 0xffce,
        Key_F18                           = 0xffcf,
        Key_F19                           = 0xffd0,
        Key_F20                           = 0xffd1,
        Key_F21                           = 0xffd2,
        Key_F22                           = 0xffd3,
        Key_F23                           = 0xffd4,
        Key_F24                           = 0xffd5,
        Key_F25                           = 0xffd6,
        Key_F26                           = 0xffd7,
        Key_F27                           = 0xffd8,
        Key_F28                           = 0xffd9,
        Key_F29                           = 0xffda,
        Key_F30                           = 0xffdb,
        Key_F31                           = 0xffdc,
        Key_F32                           = 0xffdd,
        Key_F33                           = 0xffde,
        Key_F34                           = 0xffdf,
        Key_F35                           = 0xffe0,
        Key_Shift_L                       = 0xffe1,
        Key_Shift_R                       = 0xffe2,
        Key_Control_L                     = 0xffe3,
        Key_Control_R                     = 0xffe4,
        Key_Caps_Lock                     = 0xffe5,
        Key_Shift_Lock                    = 0xffe6,
        Key_Meta_L                        = 0xffe7,
        Key_Meta_R                        = 0xffe8,
        Key_Alt_L                         = 0xffe9,
        Key_Alt_R                         = 0xffea,
        Key_Super_L                       = 0xffeb,
        Key_Super_R                       = 0xffec,
        Key_Hyper_L                       = 0xffed,
        Key_Hyper_R                       = 0xffee,

        // latin
        Key_Space                         = 0x0020,
        Key_Exclam                        = 0x0021,
        Key_QuoteDbl                      = 0x0022,
        Key_NumberSign                    = 0x0023,
        Key_Dollar                        = 0x0024,
        Key_Percent                       = 0x0025,
        Key_Ampersand                     = 0x0026,
        Key_Apostrophe                    = 0x0027,
        Key_ParenLeft                     = 0x0028,
        Key_ParenRight                    = 0x0029,
        Key_Asterisk                      = 0x002a,
        Key_Plus                          = 0x002b,
        Key_Comma                         = 0x002c,
        Key_Minus                         = 0x002d,
        Key_Period                        = 0x002e,
        Key_Slash                         = 0x002f,
        Key_0                             = 0x0030,
        Key_1                             = 0x0031,
        Key_2                             = 0x0032,
        Key_3                             = 0x0033,
        Key_4                             = 0x0034,
        Key_5                             = 0x0035,
        Key_6                             = 0x0036,
        Key_7                             = 0x0037,
        Key_8                             = 0x0038,
        Key_9                             = 0x0039,
        Key_Colon                         = 0x003a,
        Key_Semicolon                     = 0x003b,
        Key_Less                          = 0x003c,
        Key_Equal                         = 0x003d,
        Key_Greater                       = 0x003e,
        Key_Question                      = 0x003f,
        Key_At                            = 0x0040,
        Key_A                             = 0x0041,
        Key_B                             = 0x0042,
        Key_C                             = 0x0043,
        Key_D                             = 0x0044,
        Key_E                             = 0x0045,
        Key_F                             = 0x0046,
        Key_G                             = 0x0047,
        Key_H                             = 0x0048,
        Key_I                             = 0x0049,
        Key_J                             = 0x004a,
        Key_K                             = 0x004b,
        Key_L                             = 0x004c,
        Key_M                             = 0x004d,
        Key_N                             = 0x004e,
        Key_O                             = 0x004f,
        Key_P                             = 0x0050,
        Key_Q                             = 0x0051,
        Key_R                             = 0x0052,
        Key_S                             = 0x0053,
        Key_T                             = 0x0054,
        Key_U                             = 0x0055,
        Key_V                             = 0x0056,
        Key_W                             = 0x0057,
        Key_X                             = 0x0058,
        Key_Y                             = 0x0059,
        Key_Z                             = 0x005a,
        Key_BracketLeft                   = 0x005b,
        Key_Backslash                     = 0x005c,
        Key_BracketRight                  = 0x005d,
        Key_AsciiCircum                   = 0x005e,
        Key_Underscore                    = 0x005f,
        Key_Grave                         = 0x0060,
        Key_BraceLeft                     = 0x007b,
        Key_Bar                           = 0x007c,
        Key_BraceRight                    = 0x007d,
        Key_AsciiTilde                    = 0x007e,
        Key_nobreakspace                  = 0x00a0,
        Key_exclamdown                    = 0x00a1,
        Key_cent                          = 0x00a2,
        Key_sterling                      = 0x00a3,
        Key_currency                      = 0x00a4,
        Key_yen                           = 0x00a5,
        Key_brokenbar                     = 0x00a6,
        Key_section                       = 0x00a7,
        Key_diaeresis                     = 0x00a8,
        Key_copyright                     = 0x00a9,
        Key_ordfeminine                   = 0x00aa,
        Key_guillemotleft                 = 0x00ab,
        Key_notsign                       = 0x00ac,
        Key_hyphen                        = 0x00ad,
        Key_registered                    = 0x00ae,
        Key_macron                        = 0x00af,
        Key_degree                        = 0x00b0,
        Key_plusminus                     = 0x00b1,
        Key_twosuperior                   = 0x00b2,
        Key_threesuperior                 = 0x00b3,
        Key_acute                         = 0x00b4,
        Key_mu                            = 0x00b5,
        Key_paragraph                     = 0x00b6,
        Key_periodcentered                = 0x00b7,
        Key_cedilla                       = 0x00b8,
        Key_onesuperior                   = 0x00b9,
        Key_masculine                     = 0x00ba,
        Key_guillemotright                = 0x00bb,
        Key_onequarter                    = 0x00bc,
        Key_onehalf                       = 0x00bd,
        Key_threequarters                 = 0x00be,
        Key_questiondown                  = 0x00bf,
        Key_Agrave                        = 0x00c0,
        Key_Aacute                        = 0x00c1,
        Key_Acircumflex                   = 0x00c2,
        Key_Atilde                        = 0x00c3,
        Key_Adiaeresis                    = 0x00c4,
        Key_Aring                         = 0x00c5,
        Key_AE                            = 0x00c6,
        Key_Ccedilla                      = 0x00c7,
        Key_Egrave                        = 0x00c8,
        Key_Eacute                        = 0x00c9,
        Key_Ecircumflex                   = 0x00ca,
        Key_Ediaeresis                    = 0x00cb,
        Key_Igrave                        = 0x00cc,
        Key_Iacute                        = 0x00cd,
        Key_Icircumflex                   = 0x00ce,
        Key_Idiaeresis                    = 0x00cf,
        Key_ETH                           = 0x00d0,
        Key_Ntilde                        = 0x00d1,
        Key_Ograve                        = 0x00d2,
        Key_Oacute                        = 0x00d3,
        Key_Ocircumflex                   = 0x00d4,
        Key_Otilde                        = 0x00d5,
        Key_Odiaeresis                    = 0x00d6,
        Key_multiply                      = 0x00d7,
        Key_Oslash                        = 0x00d8,
        Key_Ugrave                        = 0x00d9,
        Key_Uacute                        = 0x00da,
        Key_Ucircumflex                   = 0x00db,
        Key_Udiaeresis                    = 0x00dc,
        Key_Yacute                        = 0x00dd,
        Key_THORN                         = 0x00de,
        Key_ssharp                        = 0x00df,
        Key_agrave                        = 0x00e0,
        Key_aacute                        = 0x00e1,
        Key_acircumflex                   = 0x00e2,
        Key_atilde                        = 0x00e3,
        Key_adiaeresis                    = 0x00e4,
        Key_aring                         = 0x00e5,
        Key_ae                            = 0x00e6,
        Key_ccedilla                      = 0x00e7,
        Key_egrave                        = 0x00e8,
        Key_eacute                        = 0x00e9,
        Key_ecircumflex                   = 0x00ea,
        Key_ediaeresis                    = 0x00eb,
        Key_igrave                        = 0x00ec,
        Key_iacute                        = 0x00ed,
        Key_icircumflex                   = 0x00ee,
        Key_idiaeresis                    = 0x00ef,
        Key_eth                           = 0x00f0,
        Key_ntilde                        = 0x00f1,
        Key_ograve                        = 0x00f2,
        Key_oacute                        = 0x00f3,
        Key_ocircumflex                   = 0x00f4,
        Key_otilde                        = 0x00f5,
        Key_odiaeresis                    = 0x00f6,
        Key_division                      = 0x00f7,
        Key_oslash                        = 0x00f8,
        Key_ugrave                        = 0x00f9,
        Key_uacute                        = 0x00fa,
        Key_ucircumflex                   = 0x00fb,
        Key_udiaeresis                    = 0x00fc,
        Key_yacute                        = 0x00fd,
        Key_thorn                         = 0x00fe,
        Key_ydiaeresis                    = 0x00ff,

        // multimedia
        Key_ModeLock                      = 0x1008FF01,
        Key_MonBrightnessUp               = 0x1008FF02,
        Key_MonBrightnessDown             = 0x1008FF03,
        Key_KbdLightOnOff                 = 0x1008FF04,
        Key_KbdBrightnessUp               = 0x1008FF05,
        Key_KbdBrightnessDown             = 0x1008FF06,
        Key_MonBrightnessCycle            = 0x1008FF07,
        Key_Standby                       = 0x1008FF10,
        Key_AudioLowerVolume              = 0x1008FF11,
        Key_AudioMute                     = 0x1008FF12,
        Key_AudioRaiseVolume              = 0x1008FF13,
        Key_AudioPlay                     = 0x1008FF14,
        Key_AudioStop                     = 0x1008FF15,
        Key_AudioPrev                     = 0x1008FF16,
        Key_AudioNext                     = 0x1008FF17,
        Key_HomePage                      = 0x1008FF18,
        Key_Mail                          = 0x1008FF19,
        Key_Start                         = 0x1008FF1A,
        Key_Search                        = 0x1008FF1B,
        Key_AudioRecord                   = 0x1008FF1C,
        Key_Calculator                    = 0x1008FF1D,
        Key_Memo                          = 0x1008FF1E,
        Key_ToDoList                      = 0x1008FF1F,
        Key_Calendar                      = 0x1008FF20,
        Key_PowerDown                     = 0x1008FF21,
        Key_ContrastAdjust                = 0x1008FF22,
        Key_RockerUp                      = 0x1008FF23,
        Key_RockerDown                    = 0x1008FF24,
        Key_RockerEnter                   = 0x1008FF25,
        Key_Back                          = 0x1008FF26,
        Key_Forward                       = 0x1008FF27,
        Key_Stop                          = 0x1008FF28,
        Key_Refresh                       = 0x1008FF29,
        Key_PowerOff                      = 0x1008FF2A,
        Key_WakeUp                        = 0x1008FF2B,
        Key_Eject                         = 0x1008FF2C,
        Key_ScreenSaver                   = 0x1008FF2D,
        Key_WWW                           = 0x1008FF2E,
        Key_Sleep                         = 0x1008FF2F,
        Key_Favorites                     = 0x1008FF30,
        Key_AudioPause                    = 0x1008FF31,
        Key_AudioMedia                    = 0x1008FF32,
        Key_MyComputer                    = 0x1008FF33,
        Key_VendorHome                    = 0x1008FF34,
        Key_LightBulb                     = 0x1008FF35,
        Key_Shop                          = 0x1008FF36,
        Key_History                       = 0x1008FF37,
        Key_OpenURL                       = 0x1008FF38,
        Key_AddFavorite                   = 0x1008FF39,
        Key_HotLinks                      = 0x1008FF3A,
        Key_BrightnessAdjust              = 0x1008FF3B,
        Key_Finance                       = 0x1008FF3C,
        Key_Community                     = 0x1008FF3D,
        Key_AudioRewind                   = 0x1008FF3E,
        Key_BackForward                   = 0x1008FF3F,
        Key_Launch0                       = 0x1008FF40,
        Key_Launch1                       = 0x1008FF41,
        Key_Launch2                       = 0x1008FF42,
        Key_Launch3                       = 0x1008FF43,
        Key_Launch4                       = 0x1008FF44,
        Key_Launch5                       = 0x1008FF45,
        Key_Launch6                       = 0x1008FF46,
        Key_Launch7                       = 0x1008FF47,
        Key_Launch8                       = 0x1008FF48,
        Key_Launch9                       = 0x1008FF49,
        Key_LaunchA                       = 0x1008FF4A,
        Key_LaunchB                       = 0x1008FF4B,
        Key_LaunchC                       = 0x1008FF4C,
        Key_LaunchD                       = 0x1008FF4D,
        Key_LaunchE                       = 0x1008FF4E,
        Key_LaunchF                       = 0x1008FF4F,
        Key_ApplicationLeft               = 0x1008FF50,
        Key_ApplicationRight              = 0x1008FF51,
        Key_Book                          = 0x1008FF52,
        Key_CD                            = 0x1008FF53,
        Key_Calculater                    = 0x1008FF54,
        // Key_Clear                         = 0x1008FF55,
        Key_Close                         = 0x1008FF56,
        Key_Copy                          = 0x1008FF57,
        Key_Cut                           = 0x1008FF58,
        Key_Display                       = 0x1008FF59,
        Key_DOS                           = 0x1008FF5A,
        Key_Documents                     = 0x1008FF5B,
        Key_Excel                         = 0x1008FF5C,
        Key_Explorer                      = 0x1008FF5D,
        Key_Game                          = 0x1008FF5E,
        Key_Go                            = 0x1008FF5F,
        Key_iTouch                        = 0x1008FF60,
        Key_LogOff                        = 0x1008FF61,
        Key_Market                        = 0x1008FF62,
        Key_Meeting                       = 0x1008FF63,
        Key_MenuKB                        = 0x1008FF65,
        Key_MenuPB                        = 0x1008FF66,
        Key_MySites                       = 0x1008FF67,
        Key_New                           = 0x1008FF68,
        Key_News                          = 0x1008FF69,
        Key_OfficeHome                    = 0x1008FF6A,
        Key_Open                          = 0x1008FF6B,
        Key_Option                        = 0x1008FF6C,
        Key_Paste                         = 0x1008FF6D,
        Key_Phone                         = 0x1008FF6E,
        // Key_Q                             = 0x1008FF70,
        Key_Reply                         = 0x1008FF72,
        Key_Reload                        = 0x1008FF73,
        Key_RotateWindows                 = 0x1008FF74,
        Key_RotationPB                    = 0x1008FF75,
        Key_RotationKB                    = 0x1008FF76,
        Key_Save                          = 0x1008FF77,
        Key_ScrollUp                      = 0x1008FF78,
        Key_ScrollDown                    = 0x1008FF79,
        Key_ScrollClick                   = 0x1008FF7A,
        Key_Send                          = 0x1008FF7B,
        Key_Spell                         = 0x1008FF7C,
        Key_SplitScreen                   = 0x1008FF7D,
        Key_Support                       = 0x1008FF7E,
        Key_TaskPane                      = 0x1008FF7F,
        Key_Terminal                      = 0x1008FF80,
        Key_Tools                         = 0x1008FF81,
        Key_Travel                        = 0x1008FF82,
        Key_UserPB                        = 0x1008FF84,
        Key_User1KB                       = 0x1008FF85,
        Key_User2KB                       = 0x1008FF86,
        Key_Video                         = 0x1008FF87,
        Key_WheelButton                   = 0x1008FF88,
        Key_Word                          = 0x1008FF89,
        Key_Xfer                          = 0x1008FF8A,
        Key_ZoomIn                        = 0x1008FF8B,
        Key_ZoomOut                       = 0x1008FF8C,
        Key_Away                          = 0x1008FF8D,
        Key_Messenger                     = 0x1008FF8E,
        Key_WebCam                        = 0x1008FF8F,
        Key_MailForward                   = 0x1008FF90,
        Key_Pictures                      = 0x1008FF91,
        Key_Music                         = 0x1008FF92,
        Key_Battery                       = 0x1008FF93,
        Key_Bluetooth                     = 0x1008FF94,
        Key_WLAN                          = 0x1008FF95,
        Key_UWB                           = 0x1008FF96,
        Key_AudioForward                  = 0x1008FF97,
        Key_AudioRepeat                   = 0x1008FF98,
        Key_AudioRandomPlay               = 0x1008FF99,
        Key_Subtitle                      = 0x1008FF9A,
        Key_AudioCycleTrack               = 0x1008FF9B,
        Key_CycleAngle                    = 0x1008FF9C,
        Key_FrameBack                     = 0x1008FF9D,
        Key_FrameForward                  = 0x1008FF9E,
        Key_Time                          = 0x1008FF9F,
        // Key_Select                       = 0x1008FFA0,
        Key_View                          = 0x1008FFA1,
        Key_TopMenu                       = 0x1008FFA2,
        Key_Red                           = 0x1008FFA3,
        Key_Green                         = 0x1008FFA4,
        Key_Yellow                        = 0x1008FFA5,
        Key_Blue                          = 0x1008FFA6,
        Key_Suspend                       = 0x1008FFA7,
        Key_Hibernate                     = 0x1008FFA8,
        Key_TouchpadToggle                = 0x1008FFA9,
        Key_TouchpadOn                    = 0x1008FFB0,
        Key_TouchpadOff                   = 0x1008FFB1,
        Key_AudioMicMute                  = 0x1008FFB2,
        Key_Keyboard                      = 0x1008FFB3,
        Key_WWAN                          = 0x1008FFB4,
        Key_RFKill                        = 0x1008FFB5,
        Key_AudioPreset                   = 0x1008FFB6,
        Key_RotationLockToggle            = 0x1008FFB7,
        Key_FullScreen                    = 0x1008FFB8,
        Key_Switch_VT_1                   = 0x1008FE01,
        Key_Switch_VT_2                   = 0x1008FE02,
        Key_Switch_VT_3                   = 0x1008FE03,
        Key_Switch_VT_4                   = 0x1008FE04,
        Key_Switch_VT_5                   = 0x1008FE05,
        Key_Switch_VT_6                   = 0x1008FE06,
        Key_Switch_VT_7                   = 0x1008FE07,
        Key_Switch_VT_8                   = 0x1008FE08,
        Key_Switch_VT_9                   = 0x1008FE09,
        Key_Switch_VT_10                  = 0x1008FE0A,
        Key_Switch_VT_11                  = 0x1008FE0B,
        Key_Switch_VT_12                  = 0x1008FE0C,
        Key_Ungrab                        = 0x1008FE20,
        Key_ClearGrab                     = 0x1008FE21,
        Key_Next_VMode                    = 0x1008FE22,
        Key_Prev_VMode                    = 0x1008FE23,
        Key_LogWindowTree                 = 0x1008FE24,
        Key_LogGrabInfo                   = 0x1008FE25,

        // pseudo
        Key_Shift                         = 0xffffff8,
        Key_Control                       = 0xffffff9,
        Key_Meta                          = 0xffffffa,
        Key_Alt                           = 0xffffffb,
        Key_Direction_L                   = 0xffffffc,
        Key_Direction_R                   = 0xffffffd,

        // compatibility
        Key_PageUp = Key_Page_Up,
        Key_PageDown = Key_Page_Down,
        Key_NumLock = Key_Num_Lock,
        Key_CapsLock = Key_Caps_Lock,
        Key_ScrollLock = Key_Scroll_Lock,
        Key_SysReq = Key_Sys_Req,
        Key_VolumeDown = Key_AudioLowerVolume,
        Key_VolumeUp = Key_AudioRaiseVolume,
        Key_VolumeMute = Key_AudioMute,
        Key_OpenUrl = Key_OpenURL,
        Key_MediaRecord = Key_AudioRecord,
        Key_LaunchMail = Key_Mail,
        Key_LaunchMedia = Key_AudioMedia,
        Key_MediaNext = Key_AudioNext,
        Key_MediaPrevious = Key_AudioPrev,
        Key_MediaStop = Key_AudioStop,
        Key_MediaPlay = Key_AudioPlay,
        Key_MediaPause = Key_AudioPause,
        Key_KeyboardLightOnOff = Key_KbdLightOnOff,
        Key_KeyboardBrightnessUp = Key_KbdBrightnessUp,
        Key_KeyboardBrightnessDown = Key_KbdBrightnessDown,
        Key_Camera = Key_WebCam,
        Key_Call = Key_Phone,

        // deprecated and aliases
        Key_Next = Key_Page_Down,
        Key_Prior = Key_Page_Up,
        Key_ooblique = Key_oslash,
        Key_Ooblique = Key_Oslash,
        Key_Thorn = Key_THORN,
        Key_QuoteLeft = Key_Grave,
        Key_QuoteRight = Key_Apostrophe,
        Key_Eth = Key_ETH,
        Key_Henkan_Mode = Key_Henkan,
        Key_Kanji_Bangou = Key_Codeinput,
        Key_Zen_Koho = Key_MultipleCandidate,
        Key_Mae_Koho = Key_PreviousCandidate,
        Key_script_switch = Key_Mode_switch,

        Key_Any = Key_Space,
        Key_Enter = 0xff8d,
        Key_AltGr = 0xfe03,
        Key_Backtab = 0xfe20,
        Key_unknown = 0xffffff
    };

    enum KeyboardModifier {
        NoModifier           = 0x00000000,
        ShiftModifier        = 0x02000000,
        ControlModifier      = 0x04000000,
        AltModifier          = 0x08000000,
        MetaModifier         = 0x10000000,
        KeypadModifier       = 0x20000000,
        GroupSwitchModifier  = 0x40000000,
        // Do not extend the mask to include 0x01000000
        KeyboardModifierMask = 0xfe000000
    };
    Q_DECLARE_FLAGS(KeyboardModifiers, KeyboardModifier)

    // shorter names for shortcuts
    enum Modifier {
        META          = Qt::MetaModifier,
        SHIFT         = Qt::ShiftModifier,
        CTRL          = Qt::ControlModifier,
        ALT           = Qt::AltModifier,
        MODIFIER_MASK = KeyboardModifierMask
    };

    enum MouseButton {
        NoButton         = 0x00000000,
        LeftButton       = 0x00000001,
        RightButton      = 0x00000002,
        MiddleButton     = 0x00000004,
        MouseButtonMask  = 0x000000ff
    };
    Q_DECLARE_FLAGS(MouseButtons, MouseButton)


    enum Orientation {
        Horizontal = 0x1,
        Vertical = 0x2
    };
    Q_DECLARE_FLAGS(Orientations, Orientation)

    enum FocusPolicy {
        NoFocus = 0,
        TabFocus = 0x1,
        ClickFocus = 0x2,
        StrongFocus = TabFocus | ClickFocus | 0x8,
        WheelFocus = StrongFocus | 0x4
    };

    enum SortOrder {
        AscendingOrder,
        DescendingOrder
    };

    enum TileRule {
        StretchTile,
        RepeatTile,
        RoundTile
    };

    // Text formatting flags for QPainter::drawText and QLabel.
    // The following two enums can be combined to one integer which
    // is passed as 'flags' to drawText and qt_format_text.
    enum AlignmentFlag {
        AlignLeft = 0x0001,
        AlignLeading = AlignLeft,
        AlignRight = 0x0002,
        AlignTrailing = AlignRight,
        AlignHCenter = 0x0004,
        AlignJustify = 0x0008,
        AlignAbsolute = 0x0010,
        AlignHorizontal_Mask = AlignLeft | AlignRight | AlignHCenter | AlignJustify | AlignAbsolute,

        AlignTop = 0x0020,
        AlignBottom = 0x0040,
        AlignVCenter = 0x0080,
        AlignVertical_Mask = AlignTop | AlignBottom | AlignVCenter,

        AlignCenter = AlignVCenter | AlignHCenter
    };
    Q_DECLARE_FLAGS(Alignment, AlignmentFlag)

    enum TextFlag {
        TextSingleLine = 0x0100,
        TextDontClip = 0x0200,
        TextExpandTabs = 0x0400,
        TextShowMnemonic = 0x0800,
        TextWordWrap = 0x1000,
        TextWrapAnywhere = 0x2000,
        TextDontPrint = 0x4000,
        TextIncludeTrailingSpaces = 0x08000000,
        TextHideMnemonic = 0x8000,
        TextJustificationForced = 0x10000,
        TextForceLeftToRight = 0x20000,
        TextForceRightToLeft = 0x40000,
        TextLongestVariant = 0x80000
    };

    enum TextElideMode {
        ElideLeft,
        ElideRight,
        ElideMiddle,
        ElideNone
    };

    enum WindowType {
        Widget = 0x00000000,
        Window = 0x00000001,
        Dialog = 0x00000002 | Window,
        Sheet = 0x00000004 | Window,
        Drawer = 0x00000006 | Window,
        Popup = 0x00000008 | Window,
        Tool = 0x0000000a | Window,
        ToolTip = 0x0000000c | Window,
        SplashScreen = 0x0000000e | Window,
        Desktop = 0x00000010 | Window,
        SubWindow =  0x00000012,

        WindowType_Mask = 0x000000ff,
        X11BypassWindowManagerHint = 0x00000100,
        FramelessWindowHint = 0x00000200,
        WindowTitleHint = 0x00000400,
        WindowSystemMenuHint = 0x00000800,
        WindowMinimizeButtonHint = 0x00001000,
        WindowMaximizeButtonHint = 0x00002000,
        WindowMinMaxButtonsHint = WindowMinimizeButtonHint | WindowMaximizeButtonHint,
        WindowContextHelpButtonHint = 0x00004000,
        WindowShadeButtonHint = 0x00008000,
        WindowStaysOnTopHint = 0x00010000,
        CustomizeWindowHint = 0x00020000,
        WindowStaysOnBottomHint = 0x00040000,
        WindowCloseButtonHint = 0x00080000,
        BypassGraphicsProxyWidget = 0x00100000
    };
    Q_DECLARE_FLAGS(WindowFlags, WindowType)

    enum WindowState {
        WindowNoState    = 0x00000000,
        WindowMinimized  = 0x00000001,
        WindowMaximized  = 0x00000002,
        WindowFullScreen = 0x00000004,
        WindowActive     = 0x00000008
    };
    Q_DECLARE_FLAGS(WindowStates, WindowState)

    enum WidgetAttribute {
        WA_Disabled = 0,
        WA_UnderMouse = 1,
        WA_MouseTracking = 2,
        WA_OpaquePaintEvent = 3,
        WA_NoBackground = WA_OpaquePaintEvent, // ## deprecated
        WA_StaticContents = 4,
        WA_PaintOnScreen = 5,
        WA_NoSystemBackground = 6,
        WA_UpdatesDisabled = 7,
        WA_Mapped = 8,
        WA_PaintOutsidePaintEvent = 9,
        WA_ForceDisabled = 10,
        WA_PendingMoveEvent = 11,
        WA_PendingResizeEvent = 12,
        WA_SetPalette = 13,
        WA_SetFont = 14,
        WA_SetCursor = 15,
        WA_NoChildEventsFromChildren = 16,
        WA_WindowModified = 17,
        WA_Resized = 18,
        WA_Moved = 19,
        WA_CustomWhatsThis = 20,
        WA_LayoutOnEntireRect = 21,
        WA_OutsideWSRange = 22,
        WA_TransparentForMouseEvents = 23,
        WA_PaintUnclipped = 24,
        WA_NoMouseReplay = 25,
        WA_DeleteOnClose = 26,
        WA_RightToLeft = 27,
        WA_NoChildEventsForParent = 28,
        WA_ShowModal = 29, // ## deprecated
        WA_MouseNoMask = 30,
        WA_GroupLeader = 31, // ## deprecated
        WA_NoMousePropagation = 32, // ## for now, might go away.
        WA_Hover = 33,
        WA_QuitOnClose = 34,
        WA_KeyboardFocusChange = 35,
        WA_AcceptDrops = 36,
        WA_WindowPropagation = 37,
        WA_AlwaysShowToolTips = 38,
        WA_SetStyle = 39,
        WA_SetLocale = 40,
        WA_LayoutUsesWidgetRect = 41,
        WA_ShowWithoutActivating = 42,
        WA_NativeWindow = 43,
        WA_DontCreateNativeAncestors = 44,
        WA_TranslucentBackground = 45,
        WA_X11DoNotAcceptFocus = 46,
        WA_X11BypassTransientForHint = 47,

        // window types from http://standards.freedesktop.org/wm-spec/
        WA_X11NetWmWindowTypeDesktop = 48,
        WA_X11NetWmWindowTypeDock = 49,
        WA_X11NetWmWindowTypeToolBar = 50,
        WA_X11NetWmWindowTypeMenu = 51,
        WA_X11NetWmWindowTypeUtility = 52,
        WA_X11NetWmWindowTypeSplash = 53,
        WA_X11NetWmWindowTypeDialog = 54,
        WA_X11NetWmWindowTypeDropDownMenu = 55,
        WA_X11NetWmWindowTypePopupMenu = 56,
        WA_X11NetWmWindowTypeToolTip = 57,
        WA_X11NetWmWindowTypeNotification = 58,
        WA_X11NetWmWindowTypeCombo = 59,
        WA_X11NetWmWindowTypeDND = 60,

        // internal
        WA_LaidOut = 61,
        WA_GrabbedShortcut = 62,
        WA_DontShowOnScreen = 63,
        WA_ForceUpdatesDisabled = 64,
        WA_StyledBackground = 65,
        WA_StyleSheet = 66,
        WA_DropSiteRegistered = 67,
        WA_WState_Visible = 68,
        WA_WState_Hidden = 69,
        WA_WState_Created = 70,
        WA_WState_InPaintEvent = 71,
        WA_WState_Reparented = 72,
        WA_WState_Polished = 73,
        WA_WState_OwnSizePolicy = 74,
        WA_WState_ExplicitShowHide = 75,
        WA_WState_ConfigPending = 76,
        WA_SetWindowIcon = 77,
        WA_SetLayoutDirection = 78,
        WA_SetWindowModality = 79,
        WA_NoX11EventCompression = 80,

        // Add new attributes before this line
        WA_AttributeCount
    };

    enum ApplicationAttribute {
        AA_ImmediateWidgetCreation = 0,
        AA_DontShowIconsInMenus = 1,
        AA_NativeWindows = 2,
        AA_DontCreateNativeWidgetSiblings = 3,
        AA_X11InitThreads = 4,

        // Add new attributes before this line
        AA_AttributeCount
    };

    // Image conversion flags.  The unusual ordering is caused by
    // compatibility and default requirements.
    enum ImageConversionFlag {
        ColorMode_Mask          = 0x00000003,
        AutoColor               = 0x00000000,
        ColorOnly               = 0x00000003,
        MonoOnly                = 0x00000002,
        // Reserved             = 0x00000001,

        AlphaDither_Mask        = 0x0000000c,
        ThresholdAlphaDither    = 0x00000000,
        OrderedAlphaDither      = 0x00000004,
        DiffuseAlphaDither      = 0x00000008,

        Dither_Mask             = 0x00000030,
        DiffuseDither           = 0x00000000,
        OrderedDither           = 0x00000010,
        ThresholdDither         = 0x00000020,
        // ReservedDither       = 0x00000030,

        DitherMode_Mask         = 0x000000c0,
        AutoDither              = 0x00000000,
        PreferDither            = 0x00000040,
        AvoidDither             = 0x00000080,

        NoOpaqueDetection       = 0x00000100
    };
    Q_DECLARE_FLAGS(ImageConversionFlags, ImageConversionFlag)

    enum BGMode {
        TransparentMode,
        OpaqueMode
    };

    enum ArrowType {
        NoArrow,
        UpArrow,
        DownArrow,
        LeftArrow,
        RightArrow
    };

    enum PenStyle {
        NoPen,
        SolidLine,
        DashLine,
        DotLine,
        DashDotLine,
        DashDotDotLine,
        CustomDashLine
    };

    // line endcap style
    enum PenCapStyle {
        FlatCap = 0x00,
        SquareCap = 0x10,
        RoundCap = 0x20,
    };

    // line join style
    enum PenJoinStyle {
        MiterJoin = 0x00,
        BevelJoin = 0x40,
        RoundJoin = 0x80,
        SvgMiterJoin = 0x100,
    };

    enum BrushStyle {
        NoBrush,
        SolidPattern,
        Dense1Pattern,
        Dense2Pattern,
        Dense3Pattern,
        Dense4Pattern,
        Dense5Pattern,
        Dense6Pattern,
        Dense7Pattern,
        HorPattern,
        VerPattern,
        CrossPattern,
        BDiagPattern,
        FDiagPattern,
        DiagCrossPattern,
        LinearGradientPattern,
        RadialGradientPattern,
        ConicalGradientPattern,
        TexturePattern
    };

    enum SizeMode {
        AbsoluteSize,
        RelativeSize
    };

    enum UIEffect {
        UI_General,
        UI_AnimateMenu,
        UI_FadeMenu,
        UI_AnimateCombo,
        UI_AnimateTooltip,
        UI_FadeTooltip,
        UI_AnimateToolBox
    };

    enum CursorShape {
        ArrowCursor,
        UpArrowCursor,
        CrossCursor,
        WaitCursor,
        IBeamCursor,
        SizeVerCursor,
        SizeHorCursor,
        SizeBDiagCursor,
        SizeFDiagCursor,
        SizeAllCursor,
        BlankCursor,
        SplitVCursor,
        SplitHCursor,
        PointingHandCursor,
        ForbiddenCursor,
        WhatsThisCursor,
        BusyCursor,
        OpenHandCursor,
        ClosedHandCursor,
        DragCopyCursor,
        DragMoveCursor,
        DragLinkCursor,
        LastCursor = DragLinkCursor,
        BitmapCursor = 24,
        CustomCursor = 25
    };

    enum TextFormat {
        PlainText,
        RichText,
        AutoText
    };

    enum AspectRatioMode {
        IgnoreAspectRatio,
        KeepAspectRatio,
        KeepAspectRatioByExpanding
    };

    enum DockWidgetArea {
        LeftDockWidgetArea = 0x1,
        RightDockWidgetArea = 0x2,
        TopDockWidgetArea = 0x4,
        BottomDockWidgetArea = 0x8,

        DockWidgetArea_Mask = 0xf,
        AllDockWidgetAreas = DockWidgetArea_Mask,
        NoDockWidgetArea = 0
    };
    enum DockWidgetAreaSizes {
        NDockWidgetAreas = 4
    };
    Q_DECLARE_FLAGS(DockWidgetAreas, DockWidgetArea)

    enum ToolBarArea {
        LeftToolBarArea = 0x1,
        RightToolBarArea = 0x2,
        TopToolBarArea = 0x4,
        BottomToolBarArea = 0x8,

        ToolBarArea_Mask = 0xf,
        AllToolBarAreas = ToolBarArea_Mask,
        NoToolBarArea = 0
    };

    enum ToolBarAreaSizes {
        NToolBarAreas = 4
    };
    Q_DECLARE_FLAGS(ToolBarAreas, ToolBarArea)

    enum DateFormat {
        TextDate,      // default Qt
        ISODate,       // ISO 8601
        SystemLocaleShortDate,
        SystemLocaleLongDate,
        DefaultLocaleShortDate,
        DefaultLocaleLongDate
    };

    enum TimeSpec {
        LocalTime,
        UTC,
        OffsetFromUTC
    };

    enum DayOfWeek {
        Monday = 1,
        Tuesday = 2,
        Wednesday = 3,
        Thursday = 4,
        Friday = 5,
        Saturday = 6,
        Sunday = 7
    };

    enum ScrollBarPolicy {
        ScrollBarAsNeeded,
        ScrollBarAlwaysOff,
        ScrollBarAlwaysOn
    };

    enum CaseSensitivity {
        CaseInsensitive,
        CaseSensitive
    };

    enum Corner {
        TopLeftCorner = 0x00000,
        TopRightCorner = 0x00001,
        BottomLeftCorner = 0x00002,
        BottomRightCorner = 0x00003
    };

    enum ConnectionType {
        AutoConnection,
        DirectConnection,
        QueuedConnection,
        BlockingQueuedConnection,
        UniqueConnection =  0x80
    };

    enum ShortcutContext {
        WidgetShortcut,
        WindowShortcut,
        ApplicationShortcut,
        WidgetWithChildrenShortcut
    };

    enum FillRule {
        OddEvenFill,
        WindingFill
    };

    enum MaskMode {
        MaskInColor,
        MaskOutColor
    };

    enum ClipOperation {
        NoClip,
        ReplaceClip,
        IntersectClip,
        UniteClip
    };

    // Shape = 0x1, BoundingRect = 0x2
    enum ItemSelectionMode {
        ContainsItemShape = 0x0,
        IntersectsItemShape = 0x1,
        ContainsItemBoundingRect = 0x2,
        IntersectsItemBoundingRect = 0x3
    };

    enum TransformationMode {
        FastTransformation,
        SmoothTransformation
    };

    enum Axis {
        XAxis,
        YAxis,
        ZAxis
    };

    enum FocusReason {
        MouseFocusReason,
        TabFocusReason,
        BacktabFocusReason,
        ActiveWindowFocusReason,
        PopupFocusReason,
        ShortcutFocusReason,
        MenuBarFocusReason,
        OtherFocusReason,
        NoFocusReason
    };

    enum ContextMenuPolicy {
        NoContextMenu,
        DefaultContextMenu,
        ActionsContextMenu,
        CustomContextMenu,
        PreventContextMenu
    };

    enum ToolButtonStyle {
        ToolButtonIconOnly,
        ToolButtonTextOnly,
        ToolButtonTextBesideIcon,
        ToolButtonTextUnderIcon,
        ToolButtonFollowStyle
    };

    enum LayoutDirection {
        LayoutDirectionAuto = 0,
        LeftToRight = 1,
        RightToLeft = 2
    };

    enum AnchorPoint {
        AnchorLeft = 0,
        AnchorHorizontalCenter,
        AnchorRight,
        AnchorTop,
        AnchorVerticalCenter,
        AnchorBottom
    };

    enum DropAction {
        CopyAction = 0x1,
        MoveAction = 0x2,
        LinkAction = 0x4,
        ActionMask = 0xff,
        IgnoreAction = 0x0
    };
    Q_DECLARE_FLAGS(DropActions, DropAction)

    enum CheckState {
        Unchecked,
        PartiallyChecked,
        Checked
    };

    enum ItemDataRole {
        DisplayRole = 0,
        DecorationRole = 1,
        EditRole = 2,
        ToolTipRole = 3,
        StatusTipRole = 4,
        WhatsThisRole = 5,
        // Metadata
        FontRole = 6,
        TextAlignmentRole = 7,
        BackgroundRole = 8,
        ForegroundRole = 9,
        CheckStateRole = 10,
        // Accessibility
        AccessibleTextRole = 11,
        AccessibleDescriptionRole = 12,
        // More general purpose
        SizeHintRole = 13,
        InitialSortOrderRole = 14,
        // Internal UiLib roles
        DisplayPropertyRole = 15,
        DecorationPropertyRole = 16,
        ToolTipPropertyRole = 17,
        StatusTipPropertyRole = 18,
        WhatsThisPropertyRole = 19,
        // Reserved
        UserRole = 32
    };

    enum ItemFlag {
        NoItemFlags = 0,
        ItemIsSelectable = 1,
        ItemIsEditable = 2,
        ItemIsDragEnabled = 4,
        ItemIsDropEnabled = 8,
        ItemIsUserCheckable = 16,
        ItemIsEnabled = 32,
        ItemIsTristate = 64
    };
    Q_DECLARE_FLAGS(ItemFlags, ItemFlag)

    enum MatchFlag {
        MatchExactly = 0,
        MatchContains = 1,
        MatchStartsWith = 2,
        MatchEndsWith = 4,
        MatchRegExp = 8,
        MatchWildcard = 16,
        MatchFixedString = 32,
        MatchCaseSensitive = 64,
        MatchWrap = 128,
        MatchRecursive = 256
    };
    Q_DECLARE_FLAGS(MatchFlags, MatchFlag)

#if defined(Q_WS_X11)
    typedef unsigned long HANDLE;
#endif
    typedef WindowFlags WFlags;

    enum WindowModality {
        NonModal,
        WindowModal,
        ApplicationModal
    };

    enum TextInteractionFlag {
        NoTextInteraction         = 0,
        TextSelectableByMouse     = 1,
        TextSelectableByKeyboard  = 2,
        LinksAccessibleByMouse    = 4,
        LinksAccessibleByKeyboard = 8,
        TextEditable              = 16,

        TextEditorInteraction     = TextSelectableByMouse | TextSelectableByKeyboard | TextEditable,
        TextBrowserInteraction    = TextSelectableByMouse | LinksAccessibleByMouse | LinksAccessibleByKeyboard
    };
    Q_DECLARE_FLAGS(TextInteractionFlags, TextInteractionFlag)

    enum EventPriority {
        HighEventPriority = 1,
        NormalEventPriority = 0,
        LowEventPriority = -1
    };

    enum SizeHint {
        MinimumSize,
        PreferredSize,
        MaximumSize,
        MinimumDescent,
        NSizeHints
    };

    enum WindowFrameSection {
        NoSection,
        LeftSection,           // For resize
        TopLeftSection,
        TopSection,
        TopRightSection,
        RightSection,
        BottomRightSection,
        BottomSection,
        BottomLeftSection,
        TitleBarArea    // For move
    };

    enum Initialization {
        Uninitialized
    };

    enum CoordinateSystem {
        DeviceCoordinates,
        LogicalCoordinates
    };

    enum CursorMoveStyle {
        LogicalMoveStyle,
        VisualMoveStyle
    };

    enum HitTestAccuracy {
        ExactHit,
        FuzzyHit
    };

    enum WhiteSpaceMode {
        WhiteSpaceNormal,
        WhiteSpacePre,
        WhiteSpaceNoWrap,
        WhiteSpaceModeUndefined = -1
    };

    Q_CORE_EXPORT bool mightBeRichText(const QString&);
    Q_CORE_EXPORT QString escape(const QString& plain);
    Q_CORE_EXPORT QString convertFromPlainText(const QString &plain, WhiteSpaceMode mode = WhiteSpacePre);
}
#ifdef Q_MOC_RUN
 ;
#endif

Q_DECLARE_OPERATORS_FOR_FLAGS(Qt::MouseButtons)
Q_DECLARE_OPERATORS_FOR_FLAGS(Qt::Orientations)
Q_DECLARE_OPERATORS_FOR_FLAGS(Qt::KeyboardModifiers)
Q_DECLARE_OPERATORS_FOR_FLAGS(Qt::WindowFlags)
Q_DECLARE_OPERATORS_FOR_FLAGS(Qt::Alignment)
Q_DECLARE_OPERATORS_FOR_FLAGS(Qt::ImageConversionFlags)
Q_DECLARE_OPERATORS_FOR_FLAGS(Qt::DockWidgetAreas)
Q_DECLARE_OPERATORS_FOR_FLAGS(Qt::ToolBarAreas)
Q_DECLARE_OPERATORS_FOR_FLAGS(Qt::WindowStates)
Q_DECLARE_OPERATORS_FOR_FLAGS(Qt::DropActions)
Q_DECLARE_OPERATORS_FOR_FLAGS(Qt::ItemFlags)
Q_DECLARE_OPERATORS_FOR_FLAGS(Qt::MatchFlags)
Q_DECLARE_OPERATORS_FOR_FLAGS(Qt::TextInteractionFlags)

typedef bool (*qInternalCallback)(void **);

class Q_CORE_EXPORT QInternal {
public:
    enum PaintDeviceFlags {
        UnknownDevice     = 0,
        Widget            = 1,
        Pixmap            = 2,
        Image             = 3,
        Printer           = 4
    };

    enum DockPosition {
        LeftDock,
        RightDock,
        TopDock,
        BottomDock,
        DockCount
    };

    static void registerCallback(qInternalCallback);
    static void unregisterCallback(qInternalCallback);

    static bool activateCallbacks(void **);
};

QT_END_NAMESPACE


#endif // QNAMESPACE_H
