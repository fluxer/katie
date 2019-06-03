/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016-2019 Ivailo Monev
**
** This file is part of the FOO module of the Katie Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/



#ifndef SBK_KTCORE_PYTHON_H
#define SBK_KTCORE_PYTHON_H

#include <sbkpython.h>
#include <sbkconverter.h>
// Binded library includes
#include <qchar.h>
#include <qstringlist.h>
#include <qcoreapplication.h>
#include <qstring.h>
#include <qregexp.h>
#include <qbytearray.h>
#include <qobject.h>
#include <qglobal.h>
#include <qnamespace.h>
#include <qtextboundaryfinder.h>
#include <qvariant.h>
// Conversion Includes - Primitive Types

// Conversion Includes - Container Types
#include <list>
#include <map>
#include <utility>
#include <vector>

// Type indices
enum : int {
    SBK_KATIE_QBYTEARRAY_IDX                                 = 0,
    SBK_KATIE_QCHAR_IDX                                      = 1,
    SBK_KATIE_QCHAR_SPECIALCHARACTER_IDX                     = 7,
    SBK_KATIE_QCHAR_CATEGORY_IDX                             = 2,
    SBK_KATIE_QCHAR_DIRECTION_IDX                            = 5,
    SBK_KATIE_QCHAR_DECOMPOSITION_IDX                        = 4,
    SBK_KATIE_QCHAR_JOINING_IDX                              = 6,
    SBK_KATIE_QCHAR_COMBININGCLASS_IDX                       = 3,
    SBK_KATIE_QCHAR_UNICODEVERSION_IDX                       = 8,
    SBK_KATIE_QCOREAPPLICATION_IDX                           = 9,
    SBK_KATIE_QCOREAPPLICATION_ENCODING_IDX                  = 10,
    SBK_KATIE_QLATIN1CHAR_IDX                                = 11,
    SBK_KATIE_QLATIN1STRING_IDX                              = 12,
    SBK_KATIE_QOBJECT_IDX                                    = 13,
    SBK_KATIE_QREGEXP_IDX                                    = 14,
    SBK_KATIE_QREGEXP_PATTERNSYNTAX_IDX                      = 16,
    SBK_KATIE_QREGEXP_CARETMODE_IDX                          = 15,
    SBK_KATIE_QSTRING_IDX                                    = 17,
    SBK_KATIE_QSTRING_SECTIONFLAG_IDX                        = 19,
    SBK_KATIE_QSTRING_SPLITBEHAVIOR_IDX                      = 20,
    SBK_KATIE_QSTRING_NORMALIZATIONFORM_IDX                  = 18,
    SBK_KATIE_QSTRINGLIST_IDX                                = 21,
    SBK_KATIE_QTEXTBOUNDARYFINDER_IDX                        = 22,
    SBK_KATIE_QTEXTBOUNDARYFINDER_BOUNDARYTYPE_IDX           = 24,
    SBK_KATIE_QTEXTBOUNDARYFINDER_BOUNDARYREASON_IDX         = 23,
    SBK_KATIE_QVARIANT_IDX                                   = 25,
    SBK_KATIE_QVARIANT_TYPE_IDX                              = 26,
    SBK_KATIE_QTMSGTYPE_IDX                                  = 97,
    SBK_KATIE_QT_GLOBALCOLOR_IDX                             = 57,
    SBK_KATIE_QT_KEYBOARDMODIFIER_IDX                        = 64,
    SBK_KATIE_QT_MODIFIER_IDX                                = 68,
    SBK_KATIE_QT_MOUSEBUTTON_IDX                             = 69,
    SBK_KATIE_QT_ORIENTATION_IDX                             = 71,
    SBK_KATIE_QT_FOCUSPOLICY_IDX                             = 52,
    SBK_KATIE_QT_SORTORDER_IDX                               = 79,
    SBK_KATIE_QT_TILERULE_IDX                                = 84,
    SBK_KATIE_QT_ALIGNMENTFLAG_IDX                           = 27,
    SBK_KATIE_QT_TEXTFLAG_IDX                                = 81,
    SBK_KATIE_QT_TEXTELIDEMODE_IDX                           = 80,
    SBK_KATIE_QT_WINDOWTYPE_IDX                              = 96,
    SBK_KATIE_QT_WINDOWSTATE_IDX                             = 95,
    SBK_KATIE_QT_WIDGETATTRIBUTE_IDX                         = 92,
    SBK_KATIE_QT_APPLICATIONATTRIBUTE_IDX                    = 30,
    SBK_KATIE_QT_IMAGECONVERSIONFLAG_IDX                     = 58,
    SBK_KATIE_QT_BGMODE_IDX                                  = 34,
    SBK_KATIE_QT_KEY_IDX                                     = 63,
    SBK_KATIE_QT_ARROWTYPE_IDX                               = 31,
    SBK_KATIE_QT_PENSTYLE_IDX                                = 74,
    SBK_KATIE_QT_PENCAPSTYLE_IDX                             = 72,
    SBK_KATIE_QT_PENJOINSTYLE_IDX                            = 73,
    SBK_KATIE_QT_BRUSHSTYLE_IDX                              = 35,
    SBK_KATIE_QT_SIZEMODE_IDX                                = 78,
    SBK_KATIE_QT_UIEFFECT_IDX                                = 91,
    SBK_KATIE_QT_CURSORSHAPE_IDX                             = 44,
    SBK_KATIE_QT_TEXTFORMAT_IDX                              = 82,
    SBK_KATIE_QT_ASPECTRATIOMODE_IDX                         = 32,
    SBK_KATIE_QT_ANCHORATTRIBUTE_IDX                         = 28,
    SBK_KATIE_QT_DOCKWIDGETAREA_IDX                          = 47,
    SBK_KATIE_QT_DOCKWIDGETAREASIZES_IDX                     = 48,
    SBK_KATIE_QT_TOOLBARAREA_IDX                             = 86,
    SBK_KATIE_QT_TOOLBARAREASIZES_IDX                        = 87,
    SBK_KATIE_QT_DATEFORMAT_IDX                              = 45,
    SBK_KATIE_QT_TIMESPEC_IDX                                = 85,
    SBK_KATIE_QT_DAYOFWEEK_IDX                               = 46,
    SBK_KATIE_QT_SCROLLBARPOLICY_IDX                         = 75,
    SBK_KATIE_QT_CASESENSITIVITY_IDX                         = 36,
    SBK_KATIE_QT_CORNER_IDX                                  = 42,
    SBK_KATIE_QT_CONNECTIONTYPE_IDX                          = 39,
    SBK_KATIE_QT_SHORTCUTCONTEXT_IDX                         = 76,
    SBK_KATIE_QT_FILLRULE_IDX                                = 51,
    SBK_KATIE_QT_MASKMODE_IDX                                = 66,
    SBK_KATIE_QT_CLIPOPERATION_IDX                           = 38,
    SBK_KATIE_QT_ITEMSELECTIONMODE_IDX                       = 62,
    SBK_KATIE_QT_TRANSFORMATIONMODE_IDX                      = 90,
    SBK_KATIE_QT_AXIS_IDX                                    = 33,
    SBK_KATIE_QT_FOCUSREASON_IDX                             = 53,
    SBK_KATIE_QT_CONTEXTMENUPOLICY_IDX                       = 40,
    SBK_KATIE_QT_TOOLBUTTONSTYLE_IDX                         = 88,
    SBK_KATIE_QT_LAYOUTDIRECTION_IDX                         = 65,
    SBK_KATIE_QT_ANCHORPOINT_IDX                             = 29,
    SBK_KATIE_QT_DROPACTION_IDX                              = 49,
    SBK_KATIE_QT_CHECKSTATE_IDX                              = 37,
    SBK_KATIE_QT_ITEMDATAROLE_IDX                            = 60,
    SBK_KATIE_QT_ITEMFLAG_IDX                                = 61,
    SBK_KATIE_QT_MATCHFLAG_IDX                               = 67,
    SBK_KATIE_QT_WINDOWMODALITY_IDX                          = 94,
    SBK_KATIE_QT_TEXTINTERACTIONFLAG_IDX                     = 83,
    SBK_KATIE_QT_EVENTPRIORITY_IDX                           = 50,
    SBK_KATIE_QT_SIZEHINT_IDX                                = 77,
    SBK_KATIE_QT_WINDOWFRAMESECTION_IDX                      = 93,
    SBK_KATIE_QT_INITIALIZATION_IDX                          = 59,
    SBK_KATIE_QT_COORDINATESYSTEM_IDX                        = 41,
    SBK_KATIE_QT_TOUCHPOINTSTATE_IDX                         = 89,
    SBK_KATIE_QT_GESTURESTATE_IDX                            = 55,
    SBK_KATIE_QT_GESTURETYPE_IDX                             = 56,
    SBK_KATIE_QT_GESTUREFLAG_IDX                             = 54,
    SBK_KATIE_QT_NAVIGATIONMODE_IDX                          = 70,
    SBK_KATIE_QT_CURSORMOVESTYLE_IDX                         = 43,
    SBK_KtCore_IDX_COUNT                                     = 98
};
// This variable stores all Python types exported by this module.
extern PyTypeObject** SbkKtCoreTypes;

// This variable stores the Python module object exported by this module.
extern PyObject* SbkKtCoreModuleObject;

// This variable stores all type converters exported by this module.
extern SbkConverter** SbkKtCoreTypeConverters;

// Converter indices
enum : int {
    SBK_KtCore_CONVERTERS_IDX_COUNT                          = 1
};
// Macros for type check

namespace Shiboken
{

// PyType functions, to get the PyObjectType for a type T
template<> inline PyTypeObject* SbkType< ::Katie::QtMsgType >() { return SbkKtCoreTypes[SBK_KATIE_QTMSGTYPE_IDX]; }
template<> inline PyTypeObject* SbkType< ::Katie::Qt::GlobalColor >() { return SbkKtCoreTypes[SBK_KATIE_QT_GLOBALCOLOR_IDX]; }
template<> inline PyTypeObject* SbkType< ::Katie::Qt::KeyboardModifier >() { return SbkKtCoreTypes[SBK_KATIE_QT_KEYBOARDMODIFIER_IDX]; }
template<> inline PyTypeObject* SbkType< ::Katie::Qt::Modifier >() { return SbkKtCoreTypes[SBK_KATIE_QT_MODIFIER_IDX]; }
template<> inline PyTypeObject* SbkType< ::Katie::Qt::MouseButton >() { return SbkKtCoreTypes[SBK_KATIE_QT_MOUSEBUTTON_IDX]; }
template<> inline PyTypeObject* SbkType< ::Katie::Qt::Orientation >() { return SbkKtCoreTypes[SBK_KATIE_QT_ORIENTATION_IDX]; }
template<> inline PyTypeObject* SbkType< ::Katie::Qt::FocusPolicy >() { return SbkKtCoreTypes[SBK_KATIE_QT_FOCUSPOLICY_IDX]; }
template<> inline PyTypeObject* SbkType< ::Katie::Qt::SortOrder >() { return SbkKtCoreTypes[SBK_KATIE_QT_SORTORDER_IDX]; }
template<> inline PyTypeObject* SbkType< ::Katie::Qt::TileRule >() { return SbkKtCoreTypes[SBK_KATIE_QT_TILERULE_IDX]; }
template<> inline PyTypeObject* SbkType< ::Katie::Qt::AlignmentFlag >() { return SbkKtCoreTypes[SBK_KATIE_QT_ALIGNMENTFLAG_IDX]; }
template<> inline PyTypeObject* SbkType< ::Katie::Qt::TextFlag >() { return SbkKtCoreTypes[SBK_KATIE_QT_TEXTFLAG_IDX]; }
template<> inline PyTypeObject* SbkType< ::Katie::Qt::TextElideMode >() { return SbkKtCoreTypes[SBK_KATIE_QT_TEXTELIDEMODE_IDX]; }
template<> inline PyTypeObject* SbkType< ::Katie::Qt::WindowType >() { return SbkKtCoreTypes[SBK_KATIE_QT_WINDOWTYPE_IDX]; }
template<> inline PyTypeObject* SbkType< ::Katie::Qt::WindowState >() { return SbkKtCoreTypes[SBK_KATIE_QT_WINDOWSTATE_IDX]; }
template<> inline PyTypeObject* SbkType< ::Katie::Qt::WidgetAttribute >() { return SbkKtCoreTypes[SBK_KATIE_QT_WIDGETATTRIBUTE_IDX]; }
template<> inline PyTypeObject* SbkType< ::Katie::Qt::ApplicationAttribute >() { return SbkKtCoreTypes[SBK_KATIE_QT_APPLICATIONATTRIBUTE_IDX]; }
template<> inline PyTypeObject* SbkType< ::Katie::Qt::ImageConversionFlag >() { return SbkKtCoreTypes[SBK_KATIE_QT_IMAGECONVERSIONFLAG_IDX]; }
template<> inline PyTypeObject* SbkType< ::Katie::Qt::BGMode >() { return SbkKtCoreTypes[SBK_KATIE_QT_BGMODE_IDX]; }
template<> inline PyTypeObject* SbkType< ::Katie::Qt::Key >() { return SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX]; }
template<> inline PyTypeObject* SbkType< ::Katie::Qt::ArrowType >() { return SbkKtCoreTypes[SBK_KATIE_QT_ARROWTYPE_IDX]; }
template<> inline PyTypeObject* SbkType< ::Katie::Qt::PenStyle >() { return SbkKtCoreTypes[SBK_KATIE_QT_PENSTYLE_IDX]; }
template<> inline PyTypeObject* SbkType< ::Katie::Qt::PenCapStyle >() { return SbkKtCoreTypes[SBK_KATIE_QT_PENCAPSTYLE_IDX]; }
template<> inline PyTypeObject* SbkType< ::Katie::Qt::PenJoinStyle >() { return SbkKtCoreTypes[SBK_KATIE_QT_PENJOINSTYLE_IDX]; }
template<> inline PyTypeObject* SbkType< ::Katie::Qt::BrushStyle >() { return SbkKtCoreTypes[SBK_KATIE_QT_BRUSHSTYLE_IDX]; }
template<> inline PyTypeObject* SbkType< ::Katie::Qt::SizeMode >() { return SbkKtCoreTypes[SBK_KATIE_QT_SIZEMODE_IDX]; }
template<> inline PyTypeObject* SbkType< ::Katie::Qt::UIEffect >() { return SbkKtCoreTypes[SBK_KATIE_QT_UIEFFECT_IDX]; }
template<> inline PyTypeObject* SbkType< ::Katie::Qt::CursorShape >() { return SbkKtCoreTypes[SBK_KATIE_QT_CURSORSHAPE_IDX]; }
template<> inline PyTypeObject* SbkType< ::Katie::Qt::TextFormat >() { return SbkKtCoreTypes[SBK_KATIE_QT_TEXTFORMAT_IDX]; }
template<> inline PyTypeObject* SbkType< ::Katie::Qt::AspectRatioMode >() { return SbkKtCoreTypes[SBK_KATIE_QT_ASPECTRATIOMODE_IDX]; }
template<> inline PyTypeObject* SbkType< ::Katie::Qt::AnchorAttribute >() { return SbkKtCoreTypes[SBK_KATIE_QT_ANCHORATTRIBUTE_IDX]; }
template<> inline PyTypeObject* SbkType< ::Katie::Qt::DockWidgetArea >() { return SbkKtCoreTypes[SBK_KATIE_QT_DOCKWIDGETAREA_IDX]; }
template<> inline PyTypeObject* SbkType< ::Katie::Qt::DockWidgetAreaSizes >() { return SbkKtCoreTypes[SBK_KATIE_QT_DOCKWIDGETAREASIZES_IDX]; }
template<> inline PyTypeObject* SbkType< ::Katie::Qt::ToolBarArea >() { return SbkKtCoreTypes[SBK_KATIE_QT_TOOLBARAREA_IDX]; }
template<> inline PyTypeObject* SbkType< ::Katie::Qt::ToolBarAreaSizes >() { return SbkKtCoreTypes[SBK_KATIE_QT_TOOLBARAREASIZES_IDX]; }
template<> inline PyTypeObject* SbkType< ::Katie::Qt::DateFormat >() { return SbkKtCoreTypes[SBK_KATIE_QT_DATEFORMAT_IDX]; }
template<> inline PyTypeObject* SbkType< ::Katie::Qt::TimeSpec >() { return SbkKtCoreTypes[SBK_KATIE_QT_TIMESPEC_IDX]; }
template<> inline PyTypeObject* SbkType< ::Katie::Qt::DayOfWeek >() { return SbkKtCoreTypes[SBK_KATIE_QT_DAYOFWEEK_IDX]; }
template<> inline PyTypeObject* SbkType< ::Katie::Qt::ScrollBarPolicy >() { return SbkKtCoreTypes[SBK_KATIE_QT_SCROLLBARPOLICY_IDX]; }
template<> inline PyTypeObject* SbkType< ::Katie::Qt::CaseSensitivity >() { return SbkKtCoreTypes[SBK_KATIE_QT_CASESENSITIVITY_IDX]; }
template<> inline PyTypeObject* SbkType< ::Katie::Qt::Corner >() { return SbkKtCoreTypes[SBK_KATIE_QT_CORNER_IDX]; }
template<> inline PyTypeObject* SbkType< ::Katie::Qt::ConnectionType >() { return SbkKtCoreTypes[SBK_KATIE_QT_CONNECTIONTYPE_IDX]; }
template<> inline PyTypeObject* SbkType< ::Katie::Qt::ShortcutContext >() { return SbkKtCoreTypes[SBK_KATIE_QT_SHORTCUTCONTEXT_IDX]; }
template<> inline PyTypeObject* SbkType< ::Katie::Qt::FillRule >() { return SbkKtCoreTypes[SBK_KATIE_QT_FILLRULE_IDX]; }
template<> inline PyTypeObject* SbkType< ::Katie::Qt::MaskMode >() { return SbkKtCoreTypes[SBK_KATIE_QT_MASKMODE_IDX]; }
template<> inline PyTypeObject* SbkType< ::Katie::Qt::ClipOperation >() { return SbkKtCoreTypes[SBK_KATIE_QT_CLIPOPERATION_IDX]; }
template<> inline PyTypeObject* SbkType< ::Katie::Qt::ItemSelectionMode >() { return SbkKtCoreTypes[SBK_KATIE_QT_ITEMSELECTIONMODE_IDX]; }
template<> inline PyTypeObject* SbkType< ::Katie::Qt::TransformationMode >() { return SbkKtCoreTypes[SBK_KATIE_QT_TRANSFORMATIONMODE_IDX]; }
template<> inline PyTypeObject* SbkType< ::Katie::Qt::Axis >() { return SbkKtCoreTypes[SBK_KATIE_QT_AXIS_IDX]; }
template<> inline PyTypeObject* SbkType< ::Katie::Qt::FocusReason >() { return SbkKtCoreTypes[SBK_KATIE_QT_FOCUSREASON_IDX]; }
template<> inline PyTypeObject* SbkType< ::Katie::Qt::ContextMenuPolicy >() { return SbkKtCoreTypes[SBK_KATIE_QT_CONTEXTMENUPOLICY_IDX]; }
template<> inline PyTypeObject* SbkType< ::Katie::Qt::ToolButtonStyle >() { return SbkKtCoreTypes[SBK_KATIE_QT_TOOLBUTTONSTYLE_IDX]; }
template<> inline PyTypeObject* SbkType< ::Katie::Qt::LayoutDirection >() { return SbkKtCoreTypes[SBK_KATIE_QT_LAYOUTDIRECTION_IDX]; }
template<> inline PyTypeObject* SbkType< ::Katie::Qt::AnchorPoint >() { return SbkKtCoreTypes[SBK_KATIE_QT_ANCHORPOINT_IDX]; }
template<> inline PyTypeObject* SbkType< ::Katie::Qt::DropAction >() { return SbkKtCoreTypes[SBK_KATIE_QT_DROPACTION_IDX]; }
template<> inline PyTypeObject* SbkType< ::Katie::Qt::CheckState >() { return SbkKtCoreTypes[SBK_KATIE_QT_CHECKSTATE_IDX]; }
template<> inline PyTypeObject* SbkType< ::Katie::Qt::ItemDataRole >() { return SbkKtCoreTypes[SBK_KATIE_QT_ITEMDATAROLE_IDX]; }
template<> inline PyTypeObject* SbkType< ::Katie::Qt::ItemFlag >() { return SbkKtCoreTypes[SBK_KATIE_QT_ITEMFLAG_IDX]; }
template<> inline PyTypeObject* SbkType< ::Katie::Qt::MatchFlag >() { return SbkKtCoreTypes[SBK_KATIE_QT_MATCHFLAG_IDX]; }
template<> inline PyTypeObject* SbkType< ::Katie::Qt::WindowModality >() { return SbkKtCoreTypes[SBK_KATIE_QT_WINDOWMODALITY_IDX]; }
template<> inline PyTypeObject* SbkType< ::Katie::Qt::TextInteractionFlag >() { return SbkKtCoreTypes[SBK_KATIE_QT_TEXTINTERACTIONFLAG_IDX]; }
template<> inline PyTypeObject* SbkType< ::Katie::Qt::EventPriority >() { return SbkKtCoreTypes[SBK_KATIE_QT_EVENTPRIORITY_IDX]; }
template<> inline PyTypeObject* SbkType< ::Katie::Qt::SizeHint >() { return SbkKtCoreTypes[SBK_KATIE_QT_SIZEHINT_IDX]; }
template<> inline PyTypeObject* SbkType< ::Katie::Qt::WindowFrameSection >() { return SbkKtCoreTypes[SBK_KATIE_QT_WINDOWFRAMESECTION_IDX]; }
template<> inline PyTypeObject* SbkType< ::Katie::Qt::Initialization >() { return SbkKtCoreTypes[SBK_KATIE_QT_INITIALIZATION_IDX]; }
template<> inline PyTypeObject* SbkType< ::Katie::Qt::CoordinateSystem >() { return SbkKtCoreTypes[SBK_KATIE_QT_COORDINATESYSTEM_IDX]; }
template<> inline PyTypeObject* SbkType< ::Katie::Qt::TouchPointState >() { return SbkKtCoreTypes[SBK_KATIE_QT_TOUCHPOINTSTATE_IDX]; }
template<> inline PyTypeObject* SbkType< ::Katie::Qt::GestureState >() { return SbkKtCoreTypes[SBK_KATIE_QT_GESTURESTATE_IDX]; }
template<> inline PyTypeObject* SbkType< ::Katie::Qt::GestureType >() { return SbkKtCoreTypes[SBK_KATIE_QT_GESTURETYPE_IDX]; }
template<> inline PyTypeObject* SbkType< ::Katie::Qt::GestureFlag >() { return SbkKtCoreTypes[SBK_KATIE_QT_GESTUREFLAG_IDX]; }
template<> inline PyTypeObject* SbkType< ::Katie::Qt::NavigationMode >() { return SbkKtCoreTypes[SBK_KATIE_QT_NAVIGATIONMODE_IDX]; }
template<> inline PyTypeObject* SbkType< ::Katie::Qt::CursorMoveStyle >() { return SbkKtCoreTypes[SBK_KATIE_QT_CURSORMOVESTYLE_IDX]; }
template<> inline PyTypeObject* SbkType< ::Katie::QByteArray >() { return reinterpret_cast<PyTypeObject*>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]); }
template<> inline PyTypeObject* SbkType< ::Katie::QChar::SpecialCharacter >() { return SbkKtCoreTypes[SBK_KATIE_QCHAR_SPECIALCHARACTER_IDX]; }
template<> inline PyTypeObject* SbkType< ::Katie::QChar::Category >() { return SbkKtCoreTypes[SBK_KATIE_QCHAR_CATEGORY_IDX]; }
template<> inline PyTypeObject* SbkType< ::Katie::QChar::Direction >() { return SbkKtCoreTypes[SBK_KATIE_QCHAR_DIRECTION_IDX]; }
template<> inline PyTypeObject* SbkType< ::Katie::QChar::Decomposition >() { return SbkKtCoreTypes[SBK_KATIE_QCHAR_DECOMPOSITION_IDX]; }
template<> inline PyTypeObject* SbkType< ::Katie::QChar::Joining >() { return SbkKtCoreTypes[SBK_KATIE_QCHAR_JOINING_IDX]; }
template<> inline PyTypeObject* SbkType< ::Katie::QChar::CombiningClass >() { return SbkKtCoreTypes[SBK_KATIE_QCHAR_COMBININGCLASS_IDX]; }
template<> inline PyTypeObject* SbkType< ::Katie::QChar::UnicodeVersion >() { return SbkKtCoreTypes[SBK_KATIE_QCHAR_UNICODEVERSION_IDX]; }
template<> inline PyTypeObject* SbkType< ::Katie::QChar >() { return reinterpret_cast<PyTypeObject*>(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX]); }
template<> inline PyTypeObject* SbkType< ::Katie::QCoreApplication::Encoding >() { return SbkKtCoreTypes[SBK_KATIE_QCOREAPPLICATION_ENCODING_IDX]; }
template<> inline PyTypeObject* SbkType< ::Katie::QCoreApplication >() { return reinterpret_cast<PyTypeObject*>(SbkKtCoreTypes[SBK_KATIE_QCOREAPPLICATION_IDX]); }
template<> inline PyTypeObject* SbkType< ::Katie::QLatin1Char >() { return reinterpret_cast<PyTypeObject*>(SbkKtCoreTypes[SBK_KATIE_QLATIN1CHAR_IDX]); }
template<> inline PyTypeObject* SbkType< ::Katie::QLatin1String >() { return reinterpret_cast<PyTypeObject*>(SbkKtCoreTypes[SBK_KATIE_QLATIN1STRING_IDX]); }
template<> inline PyTypeObject* SbkType< ::Katie::QObject >() { return reinterpret_cast<PyTypeObject*>(SbkKtCoreTypes[SBK_KATIE_QOBJECT_IDX]); }
template<> inline PyTypeObject* SbkType< ::Katie::QRegExp::PatternSyntax >() { return SbkKtCoreTypes[SBK_KATIE_QREGEXP_PATTERNSYNTAX_IDX]; }
template<> inline PyTypeObject* SbkType< ::Katie::QRegExp::CaretMode >() { return SbkKtCoreTypes[SBK_KATIE_QREGEXP_CARETMODE_IDX]; }
template<> inline PyTypeObject* SbkType< ::Katie::QRegExp >() { return reinterpret_cast<PyTypeObject*>(SbkKtCoreTypes[SBK_KATIE_QREGEXP_IDX]); }
template<> inline PyTypeObject* SbkType< ::Katie::QString::SectionFlag >() { return SbkKtCoreTypes[SBK_KATIE_QSTRING_SECTIONFLAG_IDX]; }
template<> inline PyTypeObject* SbkType< ::Katie::QString::SplitBehavior >() { return SbkKtCoreTypes[SBK_KATIE_QSTRING_SPLITBEHAVIOR_IDX]; }
template<> inline PyTypeObject* SbkType< ::Katie::QString::NormalizationForm >() { return SbkKtCoreTypes[SBK_KATIE_QSTRING_NORMALIZATIONFORM_IDX]; }
template<> inline PyTypeObject* SbkType< ::Katie::QString >() { return reinterpret_cast<PyTypeObject*>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]); }
template<> inline PyTypeObject* SbkType< ::Katie::QStringList >() { return reinterpret_cast<PyTypeObject*>(SbkKtCoreTypes[SBK_KATIE_QSTRINGLIST_IDX]); }
template<> inline PyTypeObject* SbkType< ::Katie::QTextBoundaryFinder::BoundaryType >() { return SbkKtCoreTypes[SBK_KATIE_QTEXTBOUNDARYFINDER_BOUNDARYTYPE_IDX]; }
template<> inline PyTypeObject* SbkType< ::Katie::QTextBoundaryFinder::BoundaryReason >() { return SbkKtCoreTypes[SBK_KATIE_QTEXTBOUNDARYFINDER_BOUNDARYREASON_IDX]; }
template<> inline PyTypeObject* SbkType< ::Katie::QTextBoundaryFinder >() { return reinterpret_cast<PyTypeObject*>(SbkKtCoreTypes[SBK_KATIE_QTEXTBOUNDARYFINDER_IDX]); }
template<> inline PyTypeObject* SbkType< ::Katie::QVariant::Type >() { return SbkKtCoreTypes[SBK_KATIE_QVARIANT_TYPE_IDX]; }
template<> inline PyTypeObject* SbkType< ::Katie::QVariant >() { return reinterpret_cast<PyTypeObject*>(SbkKtCoreTypes[SBK_KATIE_QVARIANT_IDX]); }

} // namespace Shiboken

#endif // SBK_KTCORE_PYTHON_H

