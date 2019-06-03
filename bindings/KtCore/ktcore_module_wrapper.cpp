/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
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


#include <sbkpython.h>
#include <shiboken.h>
#include <algorithm>
#include <signature.h>
#include "ktcore_python.h"



// Extra includes

// Current module's type array.
PyTypeObject** SbkKtCoreTypes = nullptr;
// Current module's PyObject pointer.
PyObject* SbkKtCoreModuleObject = nullptr;
// Current module's converter array.
SbkConverter** SbkKtCoreTypeConverters = nullptr;
// Global functions ------------------------------------------------------------

static PyMethodDef KtCore_methods[] = {
    {0} // Sentinel
};

// Classes initialization functions ------------------------------------------------------------
void init_Katie_QVariant(PyObject* module);
void init_Katie_QTextBoundaryFinder(PyObject* module);
void init_Katie_QStringList(PyObject* module);
void init_Katie_QRegExp(PyObject* module);
void init_Katie_QObject(PyObject* module);
void init_Katie_QLatin1String(PyObject* module);
void init_Katie_QLatin1Char(PyObject* module);
void init_Katie_QCoreApplication(PyObject* module);
void init_Katie_QChar(PyObject* module);
void init_Katie_QString(PyObject* module);
void init_Katie_QByteArray(PyObject* module);

// Enum definitions ------------------------------------------------------------
static void Katie_QtMsgType_PythonToCpp_Katie_QtMsgType(PyObject* pyIn, void* cppOut) {
    *reinterpret_cast<::Katie::QtMsgType*>(cppOut) =
        static_cast<::Katie::QtMsgType>(Shiboken::Enum::getValue(pyIn));

}
static PythonToCppFunc is_Katie_QtMsgType_PythonToCpp_Katie_QtMsgType_Convertible(PyObject* pyIn) {
    if (PyObject_TypeCheck(pyIn, SbkKtCoreTypes[SBK_KATIE_QTMSGTYPE_IDX]))
        return Katie_QtMsgType_PythonToCpp_Katie_QtMsgType;
    return {};
}
static PyObject* Katie_QtMsgType_CppToPython_Katie_QtMsgType(const void* cppIn) {
    const int castCppIn = int(*reinterpret_cast<const ::Katie::QtMsgType *>(cppIn));
    return Shiboken::Enum::newItem(SbkKtCoreTypes[SBK_KATIE_QTMSGTYPE_IDX], castCppIn);

}


static void Katie_Qt_GlobalColor_PythonToCpp_Katie_Qt_GlobalColor(PyObject* pyIn, void* cppOut) {
    *reinterpret_cast<::Katie::Qt::GlobalColor*>(cppOut) =
        static_cast<::Katie::Qt::GlobalColor>(Shiboken::Enum::getValue(pyIn));

}
static PythonToCppFunc is_Katie_Qt_GlobalColor_PythonToCpp_Katie_Qt_GlobalColor_Convertible(PyObject* pyIn) {
    if (PyObject_TypeCheck(pyIn, SbkKtCoreTypes[SBK_KATIE_QT_GLOBALCOLOR_IDX]))
        return Katie_Qt_GlobalColor_PythonToCpp_Katie_Qt_GlobalColor;
    return {};
}
static PyObject* Katie_Qt_GlobalColor_CppToPython_Katie_Qt_GlobalColor(const void* cppIn) {
    const int castCppIn = int(*reinterpret_cast<const ::Katie::Qt::GlobalColor *>(cppIn));
    return Shiboken::Enum::newItem(SbkKtCoreTypes[SBK_KATIE_QT_GLOBALCOLOR_IDX], castCppIn);

}


static void Katie_Qt_KeyboardModifier_PythonToCpp_Katie_Qt_KeyboardModifier(PyObject* pyIn, void* cppOut) {
    *reinterpret_cast<::Katie::Qt::KeyboardModifier*>(cppOut) =
        static_cast<::Katie::Qt::KeyboardModifier>(Shiboken::Enum::getValue(pyIn));

}
static PythonToCppFunc is_Katie_Qt_KeyboardModifier_PythonToCpp_Katie_Qt_KeyboardModifier_Convertible(PyObject* pyIn) {
    if (PyObject_TypeCheck(pyIn, SbkKtCoreTypes[SBK_KATIE_QT_KEYBOARDMODIFIER_IDX]))
        return Katie_Qt_KeyboardModifier_PythonToCpp_Katie_Qt_KeyboardModifier;
    return {};
}
static PyObject* Katie_Qt_KeyboardModifier_CppToPython_Katie_Qt_KeyboardModifier(const void* cppIn) {
    const int castCppIn = int(*reinterpret_cast<const ::Katie::Qt::KeyboardModifier *>(cppIn));
    return Shiboken::Enum::newItem(SbkKtCoreTypes[SBK_KATIE_QT_KEYBOARDMODIFIER_IDX], castCppIn);

}


static void Katie_Qt_Modifier_PythonToCpp_Katie_Qt_Modifier(PyObject* pyIn, void* cppOut) {
    *reinterpret_cast<::Katie::Qt::Modifier*>(cppOut) =
        static_cast<::Katie::Qt::Modifier>(Shiboken::Enum::getValue(pyIn));

}
static PythonToCppFunc is_Katie_Qt_Modifier_PythonToCpp_Katie_Qt_Modifier_Convertible(PyObject* pyIn) {
    if (PyObject_TypeCheck(pyIn, SbkKtCoreTypes[SBK_KATIE_QT_MODIFIER_IDX]))
        return Katie_Qt_Modifier_PythonToCpp_Katie_Qt_Modifier;
    return {};
}
static PyObject* Katie_Qt_Modifier_CppToPython_Katie_Qt_Modifier(const void* cppIn) {
    const int castCppIn = int(*reinterpret_cast<const ::Katie::Qt::Modifier *>(cppIn));
    return Shiboken::Enum::newItem(SbkKtCoreTypes[SBK_KATIE_QT_MODIFIER_IDX], castCppIn);

}


static void Katie_Qt_MouseButton_PythonToCpp_Katie_Qt_MouseButton(PyObject* pyIn, void* cppOut) {
    *reinterpret_cast<::Katie::Qt::MouseButton*>(cppOut) =
        static_cast<::Katie::Qt::MouseButton>(Shiboken::Enum::getValue(pyIn));

}
static PythonToCppFunc is_Katie_Qt_MouseButton_PythonToCpp_Katie_Qt_MouseButton_Convertible(PyObject* pyIn) {
    if (PyObject_TypeCheck(pyIn, SbkKtCoreTypes[SBK_KATIE_QT_MOUSEBUTTON_IDX]))
        return Katie_Qt_MouseButton_PythonToCpp_Katie_Qt_MouseButton;
    return {};
}
static PyObject* Katie_Qt_MouseButton_CppToPython_Katie_Qt_MouseButton(const void* cppIn) {
    const int castCppIn = int(*reinterpret_cast<const ::Katie::Qt::MouseButton *>(cppIn));
    return Shiboken::Enum::newItem(SbkKtCoreTypes[SBK_KATIE_QT_MOUSEBUTTON_IDX], castCppIn);

}


static void Katie_Qt_Orientation_PythonToCpp_Katie_Qt_Orientation(PyObject* pyIn, void* cppOut) {
    *reinterpret_cast<::Katie::Qt::Orientation*>(cppOut) =
        static_cast<::Katie::Qt::Orientation>(Shiboken::Enum::getValue(pyIn));

}
static PythonToCppFunc is_Katie_Qt_Orientation_PythonToCpp_Katie_Qt_Orientation_Convertible(PyObject* pyIn) {
    if (PyObject_TypeCheck(pyIn, SbkKtCoreTypes[SBK_KATIE_QT_ORIENTATION_IDX]))
        return Katie_Qt_Orientation_PythonToCpp_Katie_Qt_Orientation;
    return {};
}
static PyObject* Katie_Qt_Orientation_CppToPython_Katie_Qt_Orientation(const void* cppIn) {
    const int castCppIn = int(*reinterpret_cast<const ::Katie::Qt::Orientation *>(cppIn));
    return Shiboken::Enum::newItem(SbkKtCoreTypes[SBK_KATIE_QT_ORIENTATION_IDX], castCppIn);

}


static void Katie_Qt_FocusPolicy_PythonToCpp_Katie_Qt_FocusPolicy(PyObject* pyIn, void* cppOut) {
    *reinterpret_cast<::Katie::Qt::FocusPolicy*>(cppOut) =
        static_cast<::Katie::Qt::FocusPolicy>(Shiboken::Enum::getValue(pyIn));

}
static PythonToCppFunc is_Katie_Qt_FocusPolicy_PythonToCpp_Katie_Qt_FocusPolicy_Convertible(PyObject* pyIn) {
    if (PyObject_TypeCheck(pyIn, SbkKtCoreTypes[SBK_KATIE_QT_FOCUSPOLICY_IDX]))
        return Katie_Qt_FocusPolicy_PythonToCpp_Katie_Qt_FocusPolicy;
    return {};
}
static PyObject* Katie_Qt_FocusPolicy_CppToPython_Katie_Qt_FocusPolicy(const void* cppIn) {
    const int castCppIn = int(*reinterpret_cast<const ::Katie::Qt::FocusPolicy *>(cppIn));
    return Shiboken::Enum::newItem(SbkKtCoreTypes[SBK_KATIE_QT_FOCUSPOLICY_IDX], castCppIn);

}


static void Katie_Qt_SortOrder_PythonToCpp_Katie_Qt_SortOrder(PyObject* pyIn, void* cppOut) {
    *reinterpret_cast<::Katie::Qt::SortOrder*>(cppOut) =
        static_cast<::Katie::Qt::SortOrder>(Shiboken::Enum::getValue(pyIn));

}
static PythonToCppFunc is_Katie_Qt_SortOrder_PythonToCpp_Katie_Qt_SortOrder_Convertible(PyObject* pyIn) {
    if (PyObject_TypeCheck(pyIn, SbkKtCoreTypes[SBK_KATIE_QT_SORTORDER_IDX]))
        return Katie_Qt_SortOrder_PythonToCpp_Katie_Qt_SortOrder;
    return {};
}
static PyObject* Katie_Qt_SortOrder_CppToPython_Katie_Qt_SortOrder(const void* cppIn) {
    const int castCppIn = int(*reinterpret_cast<const ::Katie::Qt::SortOrder *>(cppIn));
    return Shiboken::Enum::newItem(SbkKtCoreTypes[SBK_KATIE_QT_SORTORDER_IDX], castCppIn);

}


static void Katie_Qt_TileRule_PythonToCpp_Katie_Qt_TileRule(PyObject* pyIn, void* cppOut) {
    *reinterpret_cast<::Katie::Qt::TileRule*>(cppOut) =
        static_cast<::Katie::Qt::TileRule>(Shiboken::Enum::getValue(pyIn));

}
static PythonToCppFunc is_Katie_Qt_TileRule_PythonToCpp_Katie_Qt_TileRule_Convertible(PyObject* pyIn) {
    if (PyObject_TypeCheck(pyIn, SbkKtCoreTypes[SBK_KATIE_QT_TILERULE_IDX]))
        return Katie_Qt_TileRule_PythonToCpp_Katie_Qt_TileRule;
    return {};
}
static PyObject* Katie_Qt_TileRule_CppToPython_Katie_Qt_TileRule(const void* cppIn) {
    const int castCppIn = int(*reinterpret_cast<const ::Katie::Qt::TileRule *>(cppIn));
    return Shiboken::Enum::newItem(SbkKtCoreTypes[SBK_KATIE_QT_TILERULE_IDX], castCppIn);

}


static void Katie_Qt_AlignmentFlag_PythonToCpp_Katie_Qt_AlignmentFlag(PyObject* pyIn, void* cppOut) {
    *reinterpret_cast<::Katie::Qt::AlignmentFlag*>(cppOut) =
        static_cast<::Katie::Qt::AlignmentFlag>(Shiboken::Enum::getValue(pyIn));

}
static PythonToCppFunc is_Katie_Qt_AlignmentFlag_PythonToCpp_Katie_Qt_AlignmentFlag_Convertible(PyObject* pyIn) {
    if (PyObject_TypeCheck(pyIn, SbkKtCoreTypes[SBK_KATIE_QT_ALIGNMENTFLAG_IDX]))
        return Katie_Qt_AlignmentFlag_PythonToCpp_Katie_Qt_AlignmentFlag;
    return {};
}
static PyObject* Katie_Qt_AlignmentFlag_CppToPython_Katie_Qt_AlignmentFlag(const void* cppIn) {
    const int castCppIn = int(*reinterpret_cast<const ::Katie::Qt::AlignmentFlag *>(cppIn));
    return Shiboken::Enum::newItem(SbkKtCoreTypes[SBK_KATIE_QT_ALIGNMENTFLAG_IDX], castCppIn);

}


static void Katie_Qt_TextFlag_PythonToCpp_Katie_Qt_TextFlag(PyObject* pyIn, void* cppOut) {
    *reinterpret_cast<::Katie::Qt::TextFlag*>(cppOut) =
        static_cast<::Katie::Qt::TextFlag>(Shiboken::Enum::getValue(pyIn));

}
static PythonToCppFunc is_Katie_Qt_TextFlag_PythonToCpp_Katie_Qt_TextFlag_Convertible(PyObject* pyIn) {
    if (PyObject_TypeCheck(pyIn, SbkKtCoreTypes[SBK_KATIE_QT_TEXTFLAG_IDX]))
        return Katie_Qt_TextFlag_PythonToCpp_Katie_Qt_TextFlag;
    return {};
}
static PyObject* Katie_Qt_TextFlag_CppToPython_Katie_Qt_TextFlag(const void* cppIn) {
    const int castCppIn = int(*reinterpret_cast<const ::Katie::Qt::TextFlag *>(cppIn));
    return Shiboken::Enum::newItem(SbkKtCoreTypes[SBK_KATIE_QT_TEXTFLAG_IDX], castCppIn);

}


static void Katie_Qt_TextElideMode_PythonToCpp_Katie_Qt_TextElideMode(PyObject* pyIn, void* cppOut) {
    *reinterpret_cast<::Katie::Qt::TextElideMode*>(cppOut) =
        static_cast<::Katie::Qt::TextElideMode>(Shiboken::Enum::getValue(pyIn));

}
static PythonToCppFunc is_Katie_Qt_TextElideMode_PythonToCpp_Katie_Qt_TextElideMode_Convertible(PyObject* pyIn) {
    if (PyObject_TypeCheck(pyIn, SbkKtCoreTypes[SBK_KATIE_QT_TEXTELIDEMODE_IDX]))
        return Katie_Qt_TextElideMode_PythonToCpp_Katie_Qt_TextElideMode;
    return {};
}
static PyObject* Katie_Qt_TextElideMode_CppToPython_Katie_Qt_TextElideMode(const void* cppIn) {
    const int castCppIn = int(*reinterpret_cast<const ::Katie::Qt::TextElideMode *>(cppIn));
    return Shiboken::Enum::newItem(SbkKtCoreTypes[SBK_KATIE_QT_TEXTELIDEMODE_IDX], castCppIn);

}


static void Katie_Qt_WindowType_PythonToCpp_Katie_Qt_WindowType(PyObject* pyIn, void* cppOut) {
    *reinterpret_cast<::Katie::Qt::WindowType*>(cppOut) =
        static_cast<::Katie::Qt::WindowType>(Shiboken::Enum::getValue(pyIn));

}
static PythonToCppFunc is_Katie_Qt_WindowType_PythonToCpp_Katie_Qt_WindowType_Convertible(PyObject* pyIn) {
    if (PyObject_TypeCheck(pyIn, SbkKtCoreTypes[SBK_KATIE_QT_WINDOWTYPE_IDX]))
        return Katie_Qt_WindowType_PythonToCpp_Katie_Qt_WindowType;
    return {};
}
static PyObject* Katie_Qt_WindowType_CppToPython_Katie_Qt_WindowType(const void* cppIn) {
    const int castCppIn = int(*reinterpret_cast<const ::Katie::Qt::WindowType *>(cppIn));
    return Shiboken::Enum::newItem(SbkKtCoreTypes[SBK_KATIE_QT_WINDOWTYPE_IDX], castCppIn);

}


static void Katie_Qt_WindowState_PythonToCpp_Katie_Qt_WindowState(PyObject* pyIn, void* cppOut) {
    *reinterpret_cast<::Katie::Qt::WindowState*>(cppOut) =
        static_cast<::Katie::Qt::WindowState>(Shiboken::Enum::getValue(pyIn));

}
static PythonToCppFunc is_Katie_Qt_WindowState_PythonToCpp_Katie_Qt_WindowState_Convertible(PyObject* pyIn) {
    if (PyObject_TypeCheck(pyIn, SbkKtCoreTypes[SBK_KATIE_QT_WINDOWSTATE_IDX]))
        return Katie_Qt_WindowState_PythonToCpp_Katie_Qt_WindowState;
    return {};
}
static PyObject* Katie_Qt_WindowState_CppToPython_Katie_Qt_WindowState(const void* cppIn) {
    const int castCppIn = int(*reinterpret_cast<const ::Katie::Qt::WindowState *>(cppIn));
    return Shiboken::Enum::newItem(SbkKtCoreTypes[SBK_KATIE_QT_WINDOWSTATE_IDX], castCppIn);

}


static void Katie_Qt_WidgetAttribute_PythonToCpp_Katie_Qt_WidgetAttribute(PyObject* pyIn, void* cppOut) {
    *reinterpret_cast<::Katie::Qt::WidgetAttribute*>(cppOut) =
        static_cast<::Katie::Qt::WidgetAttribute>(Shiboken::Enum::getValue(pyIn));

}
static PythonToCppFunc is_Katie_Qt_WidgetAttribute_PythonToCpp_Katie_Qt_WidgetAttribute_Convertible(PyObject* pyIn) {
    if (PyObject_TypeCheck(pyIn, SbkKtCoreTypes[SBK_KATIE_QT_WIDGETATTRIBUTE_IDX]))
        return Katie_Qt_WidgetAttribute_PythonToCpp_Katie_Qt_WidgetAttribute;
    return {};
}
static PyObject* Katie_Qt_WidgetAttribute_CppToPython_Katie_Qt_WidgetAttribute(const void* cppIn) {
    const int castCppIn = int(*reinterpret_cast<const ::Katie::Qt::WidgetAttribute *>(cppIn));
    return Shiboken::Enum::newItem(SbkKtCoreTypes[SBK_KATIE_QT_WIDGETATTRIBUTE_IDX], castCppIn);

}


static void Katie_Qt_ApplicationAttribute_PythonToCpp_Katie_Qt_ApplicationAttribute(PyObject* pyIn, void* cppOut) {
    *reinterpret_cast<::Katie::Qt::ApplicationAttribute*>(cppOut) =
        static_cast<::Katie::Qt::ApplicationAttribute>(Shiboken::Enum::getValue(pyIn));

}
static PythonToCppFunc is_Katie_Qt_ApplicationAttribute_PythonToCpp_Katie_Qt_ApplicationAttribute_Convertible(PyObject* pyIn) {
    if (PyObject_TypeCheck(pyIn, SbkKtCoreTypes[SBK_KATIE_QT_APPLICATIONATTRIBUTE_IDX]))
        return Katie_Qt_ApplicationAttribute_PythonToCpp_Katie_Qt_ApplicationAttribute;
    return {};
}
static PyObject* Katie_Qt_ApplicationAttribute_CppToPython_Katie_Qt_ApplicationAttribute(const void* cppIn) {
    const int castCppIn = int(*reinterpret_cast<const ::Katie::Qt::ApplicationAttribute *>(cppIn));
    return Shiboken::Enum::newItem(SbkKtCoreTypes[SBK_KATIE_QT_APPLICATIONATTRIBUTE_IDX], castCppIn);

}


static void Katie_Qt_ImageConversionFlag_PythonToCpp_Katie_Qt_ImageConversionFlag(PyObject* pyIn, void* cppOut) {
    *reinterpret_cast<::Katie::Qt::ImageConversionFlag*>(cppOut) =
        static_cast<::Katie::Qt::ImageConversionFlag>(Shiboken::Enum::getValue(pyIn));

}
static PythonToCppFunc is_Katie_Qt_ImageConversionFlag_PythonToCpp_Katie_Qt_ImageConversionFlag_Convertible(PyObject* pyIn) {
    if (PyObject_TypeCheck(pyIn, SbkKtCoreTypes[SBK_KATIE_QT_IMAGECONVERSIONFLAG_IDX]))
        return Katie_Qt_ImageConversionFlag_PythonToCpp_Katie_Qt_ImageConversionFlag;
    return {};
}
static PyObject* Katie_Qt_ImageConversionFlag_CppToPython_Katie_Qt_ImageConversionFlag(const void* cppIn) {
    const int castCppIn = int(*reinterpret_cast<const ::Katie::Qt::ImageConversionFlag *>(cppIn));
    return Shiboken::Enum::newItem(SbkKtCoreTypes[SBK_KATIE_QT_IMAGECONVERSIONFLAG_IDX], castCppIn);

}


static void Katie_Qt_BGMode_PythonToCpp_Katie_Qt_BGMode(PyObject* pyIn, void* cppOut) {
    *reinterpret_cast<::Katie::Qt::BGMode*>(cppOut) =
        static_cast<::Katie::Qt::BGMode>(Shiboken::Enum::getValue(pyIn));

}
static PythonToCppFunc is_Katie_Qt_BGMode_PythonToCpp_Katie_Qt_BGMode_Convertible(PyObject* pyIn) {
    if (PyObject_TypeCheck(pyIn, SbkKtCoreTypes[SBK_KATIE_QT_BGMODE_IDX]))
        return Katie_Qt_BGMode_PythonToCpp_Katie_Qt_BGMode;
    return {};
}
static PyObject* Katie_Qt_BGMode_CppToPython_Katie_Qt_BGMode(const void* cppIn) {
    const int castCppIn = int(*reinterpret_cast<const ::Katie::Qt::BGMode *>(cppIn));
    return Shiboken::Enum::newItem(SbkKtCoreTypes[SBK_KATIE_QT_BGMODE_IDX], castCppIn);

}


static void Katie_Qt_Key_PythonToCpp_Katie_Qt_Key(PyObject* pyIn, void* cppOut) {
    *reinterpret_cast<::Katie::Qt::Key*>(cppOut) =
        static_cast<::Katie::Qt::Key>(Shiboken::Enum::getValue(pyIn));

}
static PythonToCppFunc is_Katie_Qt_Key_PythonToCpp_Katie_Qt_Key_Convertible(PyObject* pyIn) {
    if (PyObject_TypeCheck(pyIn, SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX]))
        return Katie_Qt_Key_PythonToCpp_Katie_Qt_Key;
    return {};
}
static PyObject* Katie_Qt_Key_CppToPython_Katie_Qt_Key(const void* cppIn) {
    const int castCppIn = int(*reinterpret_cast<const ::Katie::Qt::Key *>(cppIn));
    return Shiboken::Enum::newItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX], castCppIn);

}


static void Katie_Qt_ArrowType_PythonToCpp_Katie_Qt_ArrowType(PyObject* pyIn, void* cppOut) {
    *reinterpret_cast<::Katie::Qt::ArrowType*>(cppOut) =
        static_cast<::Katie::Qt::ArrowType>(Shiboken::Enum::getValue(pyIn));

}
static PythonToCppFunc is_Katie_Qt_ArrowType_PythonToCpp_Katie_Qt_ArrowType_Convertible(PyObject* pyIn) {
    if (PyObject_TypeCheck(pyIn, SbkKtCoreTypes[SBK_KATIE_QT_ARROWTYPE_IDX]))
        return Katie_Qt_ArrowType_PythonToCpp_Katie_Qt_ArrowType;
    return {};
}
static PyObject* Katie_Qt_ArrowType_CppToPython_Katie_Qt_ArrowType(const void* cppIn) {
    const int castCppIn = int(*reinterpret_cast<const ::Katie::Qt::ArrowType *>(cppIn));
    return Shiboken::Enum::newItem(SbkKtCoreTypes[SBK_KATIE_QT_ARROWTYPE_IDX], castCppIn);

}


static void Katie_Qt_PenStyle_PythonToCpp_Katie_Qt_PenStyle(PyObject* pyIn, void* cppOut) {
    *reinterpret_cast<::Katie::Qt::PenStyle*>(cppOut) =
        static_cast<::Katie::Qt::PenStyle>(Shiboken::Enum::getValue(pyIn));

}
static PythonToCppFunc is_Katie_Qt_PenStyle_PythonToCpp_Katie_Qt_PenStyle_Convertible(PyObject* pyIn) {
    if (PyObject_TypeCheck(pyIn, SbkKtCoreTypes[SBK_KATIE_QT_PENSTYLE_IDX]))
        return Katie_Qt_PenStyle_PythonToCpp_Katie_Qt_PenStyle;
    return {};
}
static PyObject* Katie_Qt_PenStyle_CppToPython_Katie_Qt_PenStyle(const void* cppIn) {
    const int castCppIn = int(*reinterpret_cast<const ::Katie::Qt::PenStyle *>(cppIn));
    return Shiboken::Enum::newItem(SbkKtCoreTypes[SBK_KATIE_QT_PENSTYLE_IDX], castCppIn);

}


static void Katie_Qt_PenCapStyle_PythonToCpp_Katie_Qt_PenCapStyle(PyObject* pyIn, void* cppOut) {
    *reinterpret_cast<::Katie::Qt::PenCapStyle*>(cppOut) =
        static_cast<::Katie::Qt::PenCapStyle>(Shiboken::Enum::getValue(pyIn));

}
static PythonToCppFunc is_Katie_Qt_PenCapStyle_PythonToCpp_Katie_Qt_PenCapStyle_Convertible(PyObject* pyIn) {
    if (PyObject_TypeCheck(pyIn, SbkKtCoreTypes[SBK_KATIE_QT_PENCAPSTYLE_IDX]))
        return Katie_Qt_PenCapStyle_PythonToCpp_Katie_Qt_PenCapStyle;
    return {};
}
static PyObject* Katie_Qt_PenCapStyle_CppToPython_Katie_Qt_PenCapStyle(const void* cppIn) {
    const int castCppIn = int(*reinterpret_cast<const ::Katie::Qt::PenCapStyle *>(cppIn));
    return Shiboken::Enum::newItem(SbkKtCoreTypes[SBK_KATIE_QT_PENCAPSTYLE_IDX], castCppIn);

}


static void Katie_Qt_PenJoinStyle_PythonToCpp_Katie_Qt_PenJoinStyle(PyObject* pyIn, void* cppOut) {
    *reinterpret_cast<::Katie::Qt::PenJoinStyle*>(cppOut) =
        static_cast<::Katie::Qt::PenJoinStyle>(Shiboken::Enum::getValue(pyIn));

}
static PythonToCppFunc is_Katie_Qt_PenJoinStyle_PythonToCpp_Katie_Qt_PenJoinStyle_Convertible(PyObject* pyIn) {
    if (PyObject_TypeCheck(pyIn, SbkKtCoreTypes[SBK_KATIE_QT_PENJOINSTYLE_IDX]))
        return Katie_Qt_PenJoinStyle_PythonToCpp_Katie_Qt_PenJoinStyle;
    return {};
}
static PyObject* Katie_Qt_PenJoinStyle_CppToPython_Katie_Qt_PenJoinStyle(const void* cppIn) {
    const int castCppIn = int(*reinterpret_cast<const ::Katie::Qt::PenJoinStyle *>(cppIn));
    return Shiboken::Enum::newItem(SbkKtCoreTypes[SBK_KATIE_QT_PENJOINSTYLE_IDX], castCppIn);

}


static void Katie_Qt_BrushStyle_PythonToCpp_Katie_Qt_BrushStyle(PyObject* pyIn, void* cppOut) {
    *reinterpret_cast<::Katie::Qt::BrushStyle*>(cppOut) =
        static_cast<::Katie::Qt::BrushStyle>(Shiboken::Enum::getValue(pyIn));

}
static PythonToCppFunc is_Katie_Qt_BrushStyle_PythonToCpp_Katie_Qt_BrushStyle_Convertible(PyObject* pyIn) {
    if (PyObject_TypeCheck(pyIn, SbkKtCoreTypes[SBK_KATIE_QT_BRUSHSTYLE_IDX]))
        return Katie_Qt_BrushStyle_PythonToCpp_Katie_Qt_BrushStyle;
    return {};
}
static PyObject* Katie_Qt_BrushStyle_CppToPython_Katie_Qt_BrushStyle(const void* cppIn) {
    const int castCppIn = int(*reinterpret_cast<const ::Katie::Qt::BrushStyle *>(cppIn));
    return Shiboken::Enum::newItem(SbkKtCoreTypes[SBK_KATIE_QT_BRUSHSTYLE_IDX], castCppIn);

}


static void Katie_Qt_SizeMode_PythonToCpp_Katie_Qt_SizeMode(PyObject* pyIn, void* cppOut) {
    *reinterpret_cast<::Katie::Qt::SizeMode*>(cppOut) =
        static_cast<::Katie::Qt::SizeMode>(Shiboken::Enum::getValue(pyIn));

}
static PythonToCppFunc is_Katie_Qt_SizeMode_PythonToCpp_Katie_Qt_SizeMode_Convertible(PyObject* pyIn) {
    if (PyObject_TypeCheck(pyIn, SbkKtCoreTypes[SBK_KATIE_QT_SIZEMODE_IDX]))
        return Katie_Qt_SizeMode_PythonToCpp_Katie_Qt_SizeMode;
    return {};
}
static PyObject* Katie_Qt_SizeMode_CppToPython_Katie_Qt_SizeMode(const void* cppIn) {
    const int castCppIn = int(*reinterpret_cast<const ::Katie::Qt::SizeMode *>(cppIn));
    return Shiboken::Enum::newItem(SbkKtCoreTypes[SBK_KATIE_QT_SIZEMODE_IDX], castCppIn);

}


static void Katie_Qt_UIEffect_PythonToCpp_Katie_Qt_UIEffect(PyObject* pyIn, void* cppOut) {
    *reinterpret_cast<::Katie::Qt::UIEffect*>(cppOut) =
        static_cast<::Katie::Qt::UIEffect>(Shiboken::Enum::getValue(pyIn));

}
static PythonToCppFunc is_Katie_Qt_UIEffect_PythonToCpp_Katie_Qt_UIEffect_Convertible(PyObject* pyIn) {
    if (PyObject_TypeCheck(pyIn, SbkKtCoreTypes[SBK_KATIE_QT_UIEFFECT_IDX]))
        return Katie_Qt_UIEffect_PythonToCpp_Katie_Qt_UIEffect;
    return {};
}
static PyObject* Katie_Qt_UIEffect_CppToPython_Katie_Qt_UIEffect(const void* cppIn) {
    const int castCppIn = int(*reinterpret_cast<const ::Katie::Qt::UIEffect *>(cppIn));
    return Shiboken::Enum::newItem(SbkKtCoreTypes[SBK_KATIE_QT_UIEFFECT_IDX], castCppIn);

}


static void Katie_Qt_CursorShape_PythonToCpp_Katie_Qt_CursorShape(PyObject* pyIn, void* cppOut) {
    *reinterpret_cast<::Katie::Qt::CursorShape*>(cppOut) =
        static_cast<::Katie::Qt::CursorShape>(Shiboken::Enum::getValue(pyIn));

}
static PythonToCppFunc is_Katie_Qt_CursorShape_PythonToCpp_Katie_Qt_CursorShape_Convertible(PyObject* pyIn) {
    if (PyObject_TypeCheck(pyIn, SbkKtCoreTypes[SBK_KATIE_QT_CURSORSHAPE_IDX]))
        return Katie_Qt_CursorShape_PythonToCpp_Katie_Qt_CursorShape;
    return {};
}
static PyObject* Katie_Qt_CursorShape_CppToPython_Katie_Qt_CursorShape(const void* cppIn) {
    const int castCppIn = int(*reinterpret_cast<const ::Katie::Qt::CursorShape *>(cppIn));
    return Shiboken::Enum::newItem(SbkKtCoreTypes[SBK_KATIE_QT_CURSORSHAPE_IDX], castCppIn);

}


static void Katie_Qt_TextFormat_PythonToCpp_Katie_Qt_TextFormat(PyObject* pyIn, void* cppOut) {
    *reinterpret_cast<::Katie::Qt::TextFormat*>(cppOut) =
        static_cast<::Katie::Qt::TextFormat>(Shiboken::Enum::getValue(pyIn));

}
static PythonToCppFunc is_Katie_Qt_TextFormat_PythonToCpp_Katie_Qt_TextFormat_Convertible(PyObject* pyIn) {
    if (PyObject_TypeCheck(pyIn, SbkKtCoreTypes[SBK_KATIE_QT_TEXTFORMAT_IDX]))
        return Katie_Qt_TextFormat_PythonToCpp_Katie_Qt_TextFormat;
    return {};
}
static PyObject* Katie_Qt_TextFormat_CppToPython_Katie_Qt_TextFormat(const void* cppIn) {
    const int castCppIn = int(*reinterpret_cast<const ::Katie::Qt::TextFormat *>(cppIn));
    return Shiboken::Enum::newItem(SbkKtCoreTypes[SBK_KATIE_QT_TEXTFORMAT_IDX], castCppIn);

}


static void Katie_Qt_AspectRatioMode_PythonToCpp_Katie_Qt_AspectRatioMode(PyObject* pyIn, void* cppOut) {
    *reinterpret_cast<::Katie::Qt::AspectRatioMode*>(cppOut) =
        static_cast<::Katie::Qt::AspectRatioMode>(Shiboken::Enum::getValue(pyIn));

}
static PythonToCppFunc is_Katie_Qt_AspectRatioMode_PythonToCpp_Katie_Qt_AspectRatioMode_Convertible(PyObject* pyIn) {
    if (PyObject_TypeCheck(pyIn, SbkKtCoreTypes[SBK_KATIE_QT_ASPECTRATIOMODE_IDX]))
        return Katie_Qt_AspectRatioMode_PythonToCpp_Katie_Qt_AspectRatioMode;
    return {};
}
static PyObject* Katie_Qt_AspectRatioMode_CppToPython_Katie_Qt_AspectRatioMode(const void* cppIn) {
    const int castCppIn = int(*reinterpret_cast<const ::Katie::Qt::AspectRatioMode *>(cppIn));
    return Shiboken::Enum::newItem(SbkKtCoreTypes[SBK_KATIE_QT_ASPECTRATIOMODE_IDX], castCppIn);

}


static void Katie_Qt_AnchorAttribute_PythonToCpp_Katie_Qt_AnchorAttribute(PyObject* pyIn, void* cppOut) {
    *reinterpret_cast<::Katie::Qt::AnchorAttribute*>(cppOut) =
        static_cast<::Katie::Qt::AnchorAttribute>(Shiboken::Enum::getValue(pyIn));

}
static PythonToCppFunc is_Katie_Qt_AnchorAttribute_PythonToCpp_Katie_Qt_AnchorAttribute_Convertible(PyObject* pyIn) {
    if (PyObject_TypeCheck(pyIn, SbkKtCoreTypes[SBK_KATIE_QT_ANCHORATTRIBUTE_IDX]))
        return Katie_Qt_AnchorAttribute_PythonToCpp_Katie_Qt_AnchorAttribute;
    return {};
}
static PyObject* Katie_Qt_AnchorAttribute_CppToPython_Katie_Qt_AnchorAttribute(const void* cppIn) {
    const int castCppIn = int(*reinterpret_cast<const ::Katie::Qt::AnchorAttribute *>(cppIn));
    return Shiboken::Enum::newItem(SbkKtCoreTypes[SBK_KATIE_QT_ANCHORATTRIBUTE_IDX], castCppIn);

}


static void Katie_Qt_DockWidgetArea_PythonToCpp_Katie_Qt_DockWidgetArea(PyObject* pyIn, void* cppOut) {
    *reinterpret_cast<::Katie::Qt::DockWidgetArea*>(cppOut) =
        static_cast<::Katie::Qt::DockWidgetArea>(Shiboken::Enum::getValue(pyIn));

}
static PythonToCppFunc is_Katie_Qt_DockWidgetArea_PythonToCpp_Katie_Qt_DockWidgetArea_Convertible(PyObject* pyIn) {
    if (PyObject_TypeCheck(pyIn, SbkKtCoreTypes[SBK_KATIE_QT_DOCKWIDGETAREA_IDX]))
        return Katie_Qt_DockWidgetArea_PythonToCpp_Katie_Qt_DockWidgetArea;
    return {};
}
static PyObject* Katie_Qt_DockWidgetArea_CppToPython_Katie_Qt_DockWidgetArea(const void* cppIn) {
    const int castCppIn = int(*reinterpret_cast<const ::Katie::Qt::DockWidgetArea *>(cppIn));
    return Shiboken::Enum::newItem(SbkKtCoreTypes[SBK_KATIE_QT_DOCKWIDGETAREA_IDX], castCppIn);

}


static void Katie_Qt_DockWidgetAreaSizes_PythonToCpp_Katie_Qt_DockWidgetAreaSizes(PyObject* pyIn, void* cppOut) {
    *reinterpret_cast<::Katie::Qt::DockWidgetAreaSizes*>(cppOut) =
        static_cast<::Katie::Qt::DockWidgetAreaSizes>(Shiboken::Enum::getValue(pyIn));

}
static PythonToCppFunc is_Katie_Qt_DockWidgetAreaSizes_PythonToCpp_Katie_Qt_DockWidgetAreaSizes_Convertible(PyObject* pyIn) {
    if (PyObject_TypeCheck(pyIn, SbkKtCoreTypes[SBK_KATIE_QT_DOCKWIDGETAREASIZES_IDX]))
        return Katie_Qt_DockWidgetAreaSizes_PythonToCpp_Katie_Qt_DockWidgetAreaSizes;
    return {};
}
static PyObject* Katie_Qt_DockWidgetAreaSizes_CppToPython_Katie_Qt_DockWidgetAreaSizes(const void* cppIn) {
    const int castCppIn = int(*reinterpret_cast<const ::Katie::Qt::DockWidgetAreaSizes *>(cppIn));
    return Shiboken::Enum::newItem(SbkKtCoreTypes[SBK_KATIE_QT_DOCKWIDGETAREASIZES_IDX], castCppIn);

}


static void Katie_Qt_ToolBarArea_PythonToCpp_Katie_Qt_ToolBarArea(PyObject* pyIn, void* cppOut) {
    *reinterpret_cast<::Katie::Qt::ToolBarArea*>(cppOut) =
        static_cast<::Katie::Qt::ToolBarArea>(Shiboken::Enum::getValue(pyIn));

}
static PythonToCppFunc is_Katie_Qt_ToolBarArea_PythonToCpp_Katie_Qt_ToolBarArea_Convertible(PyObject* pyIn) {
    if (PyObject_TypeCheck(pyIn, SbkKtCoreTypes[SBK_KATIE_QT_TOOLBARAREA_IDX]))
        return Katie_Qt_ToolBarArea_PythonToCpp_Katie_Qt_ToolBarArea;
    return {};
}
static PyObject* Katie_Qt_ToolBarArea_CppToPython_Katie_Qt_ToolBarArea(const void* cppIn) {
    const int castCppIn = int(*reinterpret_cast<const ::Katie::Qt::ToolBarArea *>(cppIn));
    return Shiboken::Enum::newItem(SbkKtCoreTypes[SBK_KATIE_QT_TOOLBARAREA_IDX], castCppIn);

}


static void Katie_Qt_ToolBarAreaSizes_PythonToCpp_Katie_Qt_ToolBarAreaSizes(PyObject* pyIn, void* cppOut) {
    *reinterpret_cast<::Katie::Qt::ToolBarAreaSizes*>(cppOut) =
        static_cast<::Katie::Qt::ToolBarAreaSizes>(Shiboken::Enum::getValue(pyIn));

}
static PythonToCppFunc is_Katie_Qt_ToolBarAreaSizes_PythonToCpp_Katie_Qt_ToolBarAreaSizes_Convertible(PyObject* pyIn) {
    if (PyObject_TypeCheck(pyIn, SbkKtCoreTypes[SBK_KATIE_QT_TOOLBARAREASIZES_IDX]))
        return Katie_Qt_ToolBarAreaSizes_PythonToCpp_Katie_Qt_ToolBarAreaSizes;
    return {};
}
static PyObject* Katie_Qt_ToolBarAreaSizes_CppToPython_Katie_Qt_ToolBarAreaSizes(const void* cppIn) {
    const int castCppIn = int(*reinterpret_cast<const ::Katie::Qt::ToolBarAreaSizes *>(cppIn));
    return Shiboken::Enum::newItem(SbkKtCoreTypes[SBK_KATIE_QT_TOOLBARAREASIZES_IDX], castCppIn);

}


static void Katie_Qt_DateFormat_PythonToCpp_Katie_Qt_DateFormat(PyObject* pyIn, void* cppOut) {
    *reinterpret_cast<::Katie::Qt::DateFormat*>(cppOut) =
        static_cast<::Katie::Qt::DateFormat>(Shiboken::Enum::getValue(pyIn));

}
static PythonToCppFunc is_Katie_Qt_DateFormat_PythonToCpp_Katie_Qt_DateFormat_Convertible(PyObject* pyIn) {
    if (PyObject_TypeCheck(pyIn, SbkKtCoreTypes[SBK_KATIE_QT_DATEFORMAT_IDX]))
        return Katie_Qt_DateFormat_PythonToCpp_Katie_Qt_DateFormat;
    return {};
}
static PyObject* Katie_Qt_DateFormat_CppToPython_Katie_Qt_DateFormat(const void* cppIn) {
    const int castCppIn = int(*reinterpret_cast<const ::Katie::Qt::DateFormat *>(cppIn));
    return Shiboken::Enum::newItem(SbkKtCoreTypes[SBK_KATIE_QT_DATEFORMAT_IDX], castCppIn);

}


static void Katie_Qt_TimeSpec_PythonToCpp_Katie_Qt_TimeSpec(PyObject* pyIn, void* cppOut) {
    *reinterpret_cast<::Katie::Qt::TimeSpec*>(cppOut) =
        static_cast<::Katie::Qt::TimeSpec>(Shiboken::Enum::getValue(pyIn));

}
static PythonToCppFunc is_Katie_Qt_TimeSpec_PythonToCpp_Katie_Qt_TimeSpec_Convertible(PyObject* pyIn) {
    if (PyObject_TypeCheck(pyIn, SbkKtCoreTypes[SBK_KATIE_QT_TIMESPEC_IDX]))
        return Katie_Qt_TimeSpec_PythonToCpp_Katie_Qt_TimeSpec;
    return {};
}
static PyObject* Katie_Qt_TimeSpec_CppToPython_Katie_Qt_TimeSpec(const void* cppIn) {
    const int castCppIn = int(*reinterpret_cast<const ::Katie::Qt::TimeSpec *>(cppIn));
    return Shiboken::Enum::newItem(SbkKtCoreTypes[SBK_KATIE_QT_TIMESPEC_IDX], castCppIn);

}


static void Katie_Qt_DayOfWeek_PythonToCpp_Katie_Qt_DayOfWeek(PyObject* pyIn, void* cppOut) {
    *reinterpret_cast<::Katie::Qt::DayOfWeek*>(cppOut) =
        static_cast<::Katie::Qt::DayOfWeek>(Shiboken::Enum::getValue(pyIn));

}
static PythonToCppFunc is_Katie_Qt_DayOfWeek_PythonToCpp_Katie_Qt_DayOfWeek_Convertible(PyObject* pyIn) {
    if (PyObject_TypeCheck(pyIn, SbkKtCoreTypes[SBK_KATIE_QT_DAYOFWEEK_IDX]))
        return Katie_Qt_DayOfWeek_PythonToCpp_Katie_Qt_DayOfWeek;
    return {};
}
static PyObject* Katie_Qt_DayOfWeek_CppToPython_Katie_Qt_DayOfWeek(const void* cppIn) {
    const int castCppIn = int(*reinterpret_cast<const ::Katie::Qt::DayOfWeek *>(cppIn));
    return Shiboken::Enum::newItem(SbkKtCoreTypes[SBK_KATIE_QT_DAYOFWEEK_IDX], castCppIn);

}


static void Katie_Qt_ScrollBarPolicy_PythonToCpp_Katie_Qt_ScrollBarPolicy(PyObject* pyIn, void* cppOut) {
    *reinterpret_cast<::Katie::Qt::ScrollBarPolicy*>(cppOut) =
        static_cast<::Katie::Qt::ScrollBarPolicy>(Shiboken::Enum::getValue(pyIn));

}
static PythonToCppFunc is_Katie_Qt_ScrollBarPolicy_PythonToCpp_Katie_Qt_ScrollBarPolicy_Convertible(PyObject* pyIn) {
    if (PyObject_TypeCheck(pyIn, SbkKtCoreTypes[SBK_KATIE_QT_SCROLLBARPOLICY_IDX]))
        return Katie_Qt_ScrollBarPolicy_PythonToCpp_Katie_Qt_ScrollBarPolicy;
    return {};
}
static PyObject* Katie_Qt_ScrollBarPolicy_CppToPython_Katie_Qt_ScrollBarPolicy(const void* cppIn) {
    const int castCppIn = int(*reinterpret_cast<const ::Katie::Qt::ScrollBarPolicy *>(cppIn));
    return Shiboken::Enum::newItem(SbkKtCoreTypes[SBK_KATIE_QT_SCROLLBARPOLICY_IDX], castCppIn);

}


static void Katie_Qt_CaseSensitivity_PythonToCpp_Katie_Qt_CaseSensitivity(PyObject* pyIn, void* cppOut) {
    *reinterpret_cast<::Katie::Qt::CaseSensitivity*>(cppOut) =
        static_cast<::Katie::Qt::CaseSensitivity>(Shiboken::Enum::getValue(pyIn));

}
static PythonToCppFunc is_Katie_Qt_CaseSensitivity_PythonToCpp_Katie_Qt_CaseSensitivity_Convertible(PyObject* pyIn) {
    if (PyObject_TypeCheck(pyIn, SbkKtCoreTypes[SBK_KATIE_QT_CASESENSITIVITY_IDX]))
        return Katie_Qt_CaseSensitivity_PythonToCpp_Katie_Qt_CaseSensitivity;
    return {};
}
static PyObject* Katie_Qt_CaseSensitivity_CppToPython_Katie_Qt_CaseSensitivity(const void* cppIn) {
    const int castCppIn = int(*reinterpret_cast<const ::Katie::Qt::CaseSensitivity *>(cppIn));
    return Shiboken::Enum::newItem(SbkKtCoreTypes[SBK_KATIE_QT_CASESENSITIVITY_IDX], castCppIn);

}


static void Katie_Qt_Corner_PythonToCpp_Katie_Qt_Corner(PyObject* pyIn, void* cppOut) {
    *reinterpret_cast<::Katie::Qt::Corner*>(cppOut) =
        static_cast<::Katie::Qt::Corner>(Shiboken::Enum::getValue(pyIn));

}
static PythonToCppFunc is_Katie_Qt_Corner_PythonToCpp_Katie_Qt_Corner_Convertible(PyObject* pyIn) {
    if (PyObject_TypeCheck(pyIn, SbkKtCoreTypes[SBK_KATIE_QT_CORNER_IDX]))
        return Katie_Qt_Corner_PythonToCpp_Katie_Qt_Corner;
    return {};
}
static PyObject* Katie_Qt_Corner_CppToPython_Katie_Qt_Corner(const void* cppIn) {
    const int castCppIn = int(*reinterpret_cast<const ::Katie::Qt::Corner *>(cppIn));
    return Shiboken::Enum::newItem(SbkKtCoreTypes[SBK_KATIE_QT_CORNER_IDX], castCppIn);

}


static void Katie_Qt_ConnectionType_PythonToCpp_Katie_Qt_ConnectionType(PyObject* pyIn, void* cppOut) {
    *reinterpret_cast<::Katie::Qt::ConnectionType*>(cppOut) =
        static_cast<::Katie::Qt::ConnectionType>(Shiboken::Enum::getValue(pyIn));

}
static PythonToCppFunc is_Katie_Qt_ConnectionType_PythonToCpp_Katie_Qt_ConnectionType_Convertible(PyObject* pyIn) {
    if (PyObject_TypeCheck(pyIn, SbkKtCoreTypes[SBK_KATIE_QT_CONNECTIONTYPE_IDX]))
        return Katie_Qt_ConnectionType_PythonToCpp_Katie_Qt_ConnectionType;
    return {};
}
static PyObject* Katie_Qt_ConnectionType_CppToPython_Katie_Qt_ConnectionType(const void* cppIn) {
    const int castCppIn = int(*reinterpret_cast<const ::Katie::Qt::ConnectionType *>(cppIn));
    return Shiboken::Enum::newItem(SbkKtCoreTypes[SBK_KATIE_QT_CONNECTIONTYPE_IDX], castCppIn);

}


static void Katie_Qt_ShortcutContext_PythonToCpp_Katie_Qt_ShortcutContext(PyObject* pyIn, void* cppOut) {
    *reinterpret_cast<::Katie::Qt::ShortcutContext*>(cppOut) =
        static_cast<::Katie::Qt::ShortcutContext>(Shiboken::Enum::getValue(pyIn));

}
static PythonToCppFunc is_Katie_Qt_ShortcutContext_PythonToCpp_Katie_Qt_ShortcutContext_Convertible(PyObject* pyIn) {
    if (PyObject_TypeCheck(pyIn, SbkKtCoreTypes[SBK_KATIE_QT_SHORTCUTCONTEXT_IDX]))
        return Katie_Qt_ShortcutContext_PythonToCpp_Katie_Qt_ShortcutContext;
    return {};
}
static PyObject* Katie_Qt_ShortcutContext_CppToPython_Katie_Qt_ShortcutContext(const void* cppIn) {
    const int castCppIn = int(*reinterpret_cast<const ::Katie::Qt::ShortcutContext *>(cppIn));
    return Shiboken::Enum::newItem(SbkKtCoreTypes[SBK_KATIE_QT_SHORTCUTCONTEXT_IDX], castCppIn);

}


static void Katie_Qt_FillRule_PythonToCpp_Katie_Qt_FillRule(PyObject* pyIn, void* cppOut) {
    *reinterpret_cast<::Katie::Qt::FillRule*>(cppOut) =
        static_cast<::Katie::Qt::FillRule>(Shiboken::Enum::getValue(pyIn));

}
static PythonToCppFunc is_Katie_Qt_FillRule_PythonToCpp_Katie_Qt_FillRule_Convertible(PyObject* pyIn) {
    if (PyObject_TypeCheck(pyIn, SbkKtCoreTypes[SBK_KATIE_QT_FILLRULE_IDX]))
        return Katie_Qt_FillRule_PythonToCpp_Katie_Qt_FillRule;
    return {};
}
static PyObject* Katie_Qt_FillRule_CppToPython_Katie_Qt_FillRule(const void* cppIn) {
    const int castCppIn = int(*reinterpret_cast<const ::Katie::Qt::FillRule *>(cppIn));
    return Shiboken::Enum::newItem(SbkKtCoreTypes[SBK_KATIE_QT_FILLRULE_IDX], castCppIn);

}


static void Katie_Qt_MaskMode_PythonToCpp_Katie_Qt_MaskMode(PyObject* pyIn, void* cppOut) {
    *reinterpret_cast<::Katie::Qt::MaskMode*>(cppOut) =
        static_cast<::Katie::Qt::MaskMode>(Shiboken::Enum::getValue(pyIn));

}
static PythonToCppFunc is_Katie_Qt_MaskMode_PythonToCpp_Katie_Qt_MaskMode_Convertible(PyObject* pyIn) {
    if (PyObject_TypeCheck(pyIn, SbkKtCoreTypes[SBK_KATIE_QT_MASKMODE_IDX]))
        return Katie_Qt_MaskMode_PythonToCpp_Katie_Qt_MaskMode;
    return {};
}
static PyObject* Katie_Qt_MaskMode_CppToPython_Katie_Qt_MaskMode(const void* cppIn) {
    const int castCppIn = int(*reinterpret_cast<const ::Katie::Qt::MaskMode *>(cppIn));
    return Shiboken::Enum::newItem(SbkKtCoreTypes[SBK_KATIE_QT_MASKMODE_IDX], castCppIn);

}


static void Katie_Qt_ClipOperation_PythonToCpp_Katie_Qt_ClipOperation(PyObject* pyIn, void* cppOut) {
    *reinterpret_cast<::Katie::Qt::ClipOperation*>(cppOut) =
        static_cast<::Katie::Qt::ClipOperation>(Shiboken::Enum::getValue(pyIn));

}
static PythonToCppFunc is_Katie_Qt_ClipOperation_PythonToCpp_Katie_Qt_ClipOperation_Convertible(PyObject* pyIn) {
    if (PyObject_TypeCheck(pyIn, SbkKtCoreTypes[SBK_KATIE_QT_CLIPOPERATION_IDX]))
        return Katie_Qt_ClipOperation_PythonToCpp_Katie_Qt_ClipOperation;
    return {};
}
static PyObject* Katie_Qt_ClipOperation_CppToPython_Katie_Qt_ClipOperation(const void* cppIn) {
    const int castCppIn = int(*reinterpret_cast<const ::Katie::Qt::ClipOperation *>(cppIn));
    return Shiboken::Enum::newItem(SbkKtCoreTypes[SBK_KATIE_QT_CLIPOPERATION_IDX], castCppIn);

}


static void Katie_Qt_ItemSelectionMode_PythonToCpp_Katie_Qt_ItemSelectionMode(PyObject* pyIn, void* cppOut) {
    *reinterpret_cast<::Katie::Qt::ItemSelectionMode*>(cppOut) =
        static_cast<::Katie::Qt::ItemSelectionMode>(Shiboken::Enum::getValue(pyIn));

}
static PythonToCppFunc is_Katie_Qt_ItemSelectionMode_PythonToCpp_Katie_Qt_ItemSelectionMode_Convertible(PyObject* pyIn) {
    if (PyObject_TypeCheck(pyIn, SbkKtCoreTypes[SBK_KATIE_QT_ITEMSELECTIONMODE_IDX]))
        return Katie_Qt_ItemSelectionMode_PythonToCpp_Katie_Qt_ItemSelectionMode;
    return {};
}
static PyObject* Katie_Qt_ItemSelectionMode_CppToPython_Katie_Qt_ItemSelectionMode(const void* cppIn) {
    const int castCppIn = int(*reinterpret_cast<const ::Katie::Qt::ItemSelectionMode *>(cppIn));
    return Shiboken::Enum::newItem(SbkKtCoreTypes[SBK_KATIE_QT_ITEMSELECTIONMODE_IDX], castCppIn);

}


static void Katie_Qt_TransformationMode_PythonToCpp_Katie_Qt_TransformationMode(PyObject* pyIn, void* cppOut) {
    *reinterpret_cast<::Katie::Qt::TransformationMode*>(cppOut) =
        static_cast<::Katie::Qt::TransformationMode>(Shiboken::Enum::getValue(pyIn));

}
static PythonToCppFunc is_Katie_Qt_TransformationMode_PythonToCpp_Katie_Qt_TransformationMode_Convertible(PyObject* pyIn) {
    if (PyObject_TypeCheck(pyIn, SbkKtCoreTypes[SBK_KATIE_QT_TRANSFORMATIONMODE_IDX]))
        return Katie_Qt_TransformationMode_PythonToCpp_Katie_Qt_TransformationMode;
    return {};
}
static PyObject* Katie_Qt_TransformationMode_CppToPython_Katie_Qt_TransformationMode(const void* cppIn) {
    const int castCppIn = int(*reinterpret_cast<const ::Katie::Qt::TransformationMode *>(cppIn));
    return Shiboken::Enum::newItem(SbkKtCoreTypes[SBK_KATIE_QT_TRANSFORMATIONMODE_IDX], castCppIn);

}


static void Katie_Qt_Axis_PythonToCpp_Katie_Qt_Axis(PyObject* pyIn, void* cppOut) {
    *reinterpret_cast<::Katie::Qt::Axis*>(cppOut) =
        static_cast<::Katie::Qt::Axis>(Shiboken::Enum::getValue(pyIn));

}
static PythonToCppFunc is_Katie_Qt_Axis_PythonToCpp_Katie_Qt_Axis_Convertible(PyObject* pyIn) {
    if (PyObject_TypeCheck(pyIn, SbkKtCoreTypes[SBK_KATIE_QT_AXIS_IDX]))
        return Katie_Qt_Axis_PythonToCpp_Katie_Qt_Axis;
    return {};
}
static PyObject* Katie_Qt_Axis_CppToPython_Katie_Qt_Axis(const void* cppIn) {
    const int castCppIn = int(*reinterpret_cast<const ::Katie::Qt::Axis *>(cppIn));
    return Shiboken::Enum::newItem(SbkKtCoreTypes[SBK_KATIE_QT_AXIS_IDX], castCppIn);

}


static void Katie_Qt_FocusReason_PythonToCpp_Katie_Qt_FocusReason(PyObject* pyIn, void* cppOut) {
    *reinterpret_cast<::Katie::Qt::FocusReason*>(cppOut) =
        static_cast<::Katie::Qt::FocusReason>(Shiboken::Enum::getValue(pyIn));

}
static PythonToCppFunc is_Katie_Qt_FocusReason_PythonToCpp_Katie_Qt_FocusReason_Convertible(PyObject* pyIn) {
    if (PyObject_TypeCheck(pyIn, SbkKtCoreTypes[SBK_KATIE_QT_FOCUSREASON_IDX]))
        return Katie_Qt_FocusReason_PythonToCpp_Katie_Qt_FocusReason;
    return {};
}
static PyObject* Katie_Qt_FocusReason_CppToPython_Katie_Qt_FocusReason(const void* cppIn) {
    const int castCppIn = int(*reinterpret_cast<const ::Katie::Qt::FocusReason *>(cppIn));
    return Shiboken::Enum::newItem(SbkKtCoreTypes[SBK_KATIE_QT_FOCUSREASON_IDX], castCppIn);

}


static void Katie_Qt_ContextMenuPolicy_PythonToCpp_Katie_Qt_ContextMenuPolicy(PyObject* pyIn, void* cppOut) {
    *reinterpret_cast<::Katie::Qt::ContextMenuPolicy*>(cppOut) =
        static_cast<::Katie::Qt::ContextMenuPolicy>(Shiboken::Enum::getValue(pyIn));

}
static PythonToCppFunc is_Katie_Qt_ContextMenuPolicy_PythonToCpp_Katie_Qt_ContextMenuPolicy_Convertible(PyObject* pyIn) {
    if (PyObject_TypeCheck(pyIn, SbkKtCoreTypes[SBK_KATIE_QT_CONTEXTMENUPOLICY_IDX]))
        return Katie_Qt_ContextMenuPolicy_PythonToCpp_Katie_Qt_ContextMenuPolicy;
    return {};
}
static PyObject* Katie_Qt_ContextMenuPolicy_CppToPython_Katie_Qt_ContextMenuPolicy(const void* cppIn) {
    const int castCppIn = int(*reinterpret_cast<const ::Katie::Qt::ContextMenuPolicy *>(cppIn));
    return Shiboken::Enum::newItem(SbkKtCoreTypes[SBK_KATIE_QT_CONTEXTMENUPOLICY_IDX], castCppIn);

}


static void Katie_Qt_ToolButtonStyle_PythonToCpp_Katie_Qt_ToolButtonStyle(PyObject* pyIn, void* cppOut) {
    *reinterpret_cast<::Katie::Qt::ToolButtonStyle*>(cppOut) =
        static_cast<::Katie::Qt::ToolButtonStyle>(Shiboken::Enum::getValue(pyIn));

}
static PythonToCppFunc is_Katie_Qt_ToolButtonStyle_PythonToCpp_Katie_Qt_ToolButtonStyle_Convertible(PyObject* pyIn) {
    if (PyObject_TypeCheck(pyIn, SbkKtCoreTypes[SBK_KATIE_QT_TOOLBUTTONSTYLE_IDX]))
        return Katie_Qt_ToolButtonStyle_PythonToCpp_Katie_Qt_ToolButtonStyle;
    return {};
}
static PyObject* Katie_Qt_ToolButtonStyle_CppToPython_Katie_Qt_ToolButtonStyle(const void* cppIn) {
    const int castCppIn = int(*reinterpret_cast<const ::Katie::Qt::ToolButtonStyle *>(cppIn));
    return Shiboken::Enum::newItem(SbkKtCoreTypes[SBK_KATIE_QT_TOOLBUTTONSTYLE_IDX], castCppIn);

}


static void Katie_Qt_LayoutDirection_PythonToCpp_Katie_Qt_LayoutDirection(PyObject* pyIn, void* cppOut) {
    *reinterpret_cast<::Katie::Qt::LayoutDirection*>(cppOut) =
        static_cast<::Katie::Qt::LayoutDirection>(Shiboken::Enum::getValue(pyIn));

}
static PythonToCppFunc is_Katie_Qt_LayoutDirection_PythonToCpp_Katie_Qt_LayoutDirection_Convertible(PyObject* pyIn) {
    if (PyObject_TypeCheck(pyIn, SbkKtCoreTypes[SBK_KATIE_QT_LAYOUTDIRECTION_IDX]))
        return Katie_Qt_LayoutDirection_PythonToCpp_Katie_Qt_LayoutDirection;
    return {};
}
static PyObject* Katie_Qt_LayoutDirection_CppToPython_Katie_Qt_LayoutDirection(const void* cppIn) {
    const int castCppIn = int(*reinterpret_cast<const ::Katie::Qt::LayoutDirection *>(cppIn));
    return Shiboken::Enum::newItem(SbkKtCoreTypes[SBK_KATIE_QT_LAYOUTDIRECTION_IDX], castCppIn);

}


static void Katie_Qt_AnchorPoint_PythonToCpp_Katie_Qt_AnchorPoint(PyObject* pyIn, void* cppOut) {
    *reinterpret_cast<::Katie::Qt::AnchorPoint*>(cppOut) =
        static_cast<::Katie::Qt::AnchorPoint>(Shiboken::Enum::getValue(pyIn));

}
static PythonToCppFunc is_Katie_Qt_AnchorPoint_PythonToCpp_Katie_Qt_AnchorPoint_Convertible(PyObject* pyIn) {
    if (PyObject_TypeCheck(pyIn, SbkKtCoreTypes[SBK_KATIE_QT_ANCHORPOINT_IDX]))
        return Katie_Qt_AnchorPoint_PythonToCpp_Katie_Qt_AnchorPoint;
    return {};
}
static PyObject* Katie_Qt_AnchorPoint_CppToPython_Katie_Qt_AnchorPoint(const void* cppIn) {
    const int castCppIn = int(*reinterpret_cast<const ::Katie::Qt::AnchorPoint *>(cppIn));
    return Shiboken::Enum::newItem(SbkKtCoreTypes[SBK_KATIE_QT_ANCHORPOINT_IDX], castCppIn);

}


static void Katie_Qt_DropAction_PythonToCpp_Katie_Qt_DropAction(PyObject* pyIn, void* cppOut) {
    *reinterpret_cast<::Katie::Qt::DropAction*>(cppOut) =
        static_cast<::Katie::Qt::DropAction>(Shiboken::Enum::getValue(pyIn));

}
static PythonToCppFunc is_Katie_Qt_DropAction_PythonToCpp_Katie_Qt_DropAction_Convertible(PyObject* pyIn) {
    if (PyObject_TypeCheck(pyIn, SbkKtCoreTypes[SBK_KATIE_QT_DROPACTION_IDX]))
        return Katie_Qt_DropAction_PythonToCpp_Katie_Qt_DropAction;
    return {};
}
static PyObject* Katie_Qt_DropAction_CppToPython_Katie_Qt_DropAction(const void* cppIn) {
    const int castCppIn = int(*reinterpret_cast<const ::Katie::Qt::DropAction *>(cppIn));
    return Shiboken::Enum::newItem(SbkKtCoreTypes[SBK_KATIE_QT_DROPACTION_IDX], castCppIn);

}


static void Katie_Qt_CheckState_PythonToCpp_Katie_Qt_CheckState(PyObject* pyIn, void* cppOut) {
    *reinterpret_cast<::Katie::Qt::CheckState*>(cppOut) =
        static_cast<::Katie::Qt::CheckState>(Shiboken::Enum::getValue(pyIn));

}
static PythonToCppFunc is_Katie_Qt_CheckState_PythonToCpp_Katie_Qt_CheckState_Convertible(PyObject* pyIn) {
    if (PyObject_TypeCheck(pyIn, SbkKtCoreTypes[SBK_KATIE_QT_CHECKSTATE_IDX]))
        return Katie_Qt_CheckState_PythonToCpp_Katie_Qt_CheckState;
    return {};
}
static PyObject* Katie_Qt_CheckState_CppToPython_Katie_Qt_CheckState(const void* cppIn) {
    const int castCppIn = int(*reinterpret_cast<const ::Katie::Qt::CheckState *>(cppIn));
    return Shiboken::Enum::newItem(SbkKtCoreTypes[SBK_KATIE_QT_CHECKSTATE_IDX], castCppIn);

}


static void Katie_Qt_ItemDataRole_PythonToCpp_Katie_Qt_ItemDataRole(PyObject* pyIn, void* cppOut) {
    *reinterpret_cast<::Katie::Qt::ItemDataRole*>(cppOut) =
        static_cast<::Katie::Qt::ItemDataRole>(Shiboken::Enum::getValue(pyIn));

}
static PythonToCppFunc is_Katie_Qt_ItemDataRole_PythonToCpp_Katie_Qt_ItemDataRole_Convertible(PyObject* pyIn) {
    if (PyObject_TypeCheck(pyIn, SbkKtCoreTypes[SBK_KATIE_QT_ITEMDATAROLE_IDX]))
        return Katie_Qt_ItemDataRole_PythonToCpp_Katie_Qt_ItemDataRole;
    return {};
}
static PyObject* Katie_Qt_ItemDataRole_CppToPython_Katie_Qt_ItemDataRole(const void* cppIn) {
    const int castCppIn = int(*reinterpret_cast<const ::Katie::Qt::ItemDataRole *>(cppIn));
    return Shiboken::Enum::newItem(SbkKtCoreTypes[SBK_KATIE_QT_ITEMDATAROLE_IDX], castCppIn);

}


static void Katie_Qt_ItemFlag_PythonToCpp_Katie_Qt_ItemFlag(PyObject* pyIn, void* cppOut) {
    *reinterpret_cast<::Katie::Qt::ItemFlag*>(cppOut) =
        static_cast<::Katie::Qt::ItemFlag>(Shiboken::Enum::getValue(pyIn));

}
static PythonToCppFunc is_Katie_Qt_ItemFlag_PythonToCpp_Katie_Qt_ItemFlag_Convertible(PyObject* pyIn) {
    if (PyObject_TypeCheck(pyIn, SbkKtCoreTypes[SBK_KATIE_QT_ITEMFLAG_IDX]))
        return Katie_Qt_ItemFlag_PythonToCpp_Katie_Qt_ItemFlag;
    return {};
}
static PyObject* Katie_Qt_ItemFlag_CppToPython_Katie_Qt_ItemFlag(const void* cppIn) {
    const int castCppIn = int(*reinterpret_cast<const ::Katie::Qt::ItemFlag *>(cppIn));
    return Shiboken::Enum::newItem(SbkKtCoreTypes[SBK_KATIE_QT_ITEMFLAG_IDX], castCppIn);

}


static void Katie_Qt_MatchFlag_PythonToCpp_Katie_Qt_MatchFlag(PyObject* pyIn, void* cppOut) {
    *reinterpret_cast<::Katie::Qt::MatchFlag*>(cppOut) =
        static_cast<::Katie::Qt::MatchFlag>(Shiboken::Enum::getValue(pyIn));

}
static PythonToCppFunc is_Katie_Qt_MatchFlag_PythonToCpp_Katie_Qt_MatchFlag_Convertible(PyObject* pyIn) {
    if (PyObject_TypeCheck(pyIn, SbkKtCoreTypes[SBK_KATIE_QT_MATCHFLAG_IDX]))
        return Katie_Qt_MatchFlag_PythonToCpp_Katie_Qt_MatchFlag;
    return {};
}
static PyObject* Katie_Qt_MatchFlag_CppToPython_Katie_Qt_MatchFlag(const void* cppIn) {
    const int castCppIn = int(*reinterpret_cast<const ::Katie::Qt::MatchFlag *>(cppIn));
    return Shiboken::Enum::newItem(SbkKtCoreTypes[SBK_KATIE_QT_MATCHFLAG_IDX], castCppIn);

}


static void Katie_Qt_WindowModality_PythonToCpp_Katie_Qt_WindowModality(PyObject* pyIn, void* cppOut) {
    *reinterpret_cast<::Katie::Qt::WindowModality*>(cppOut) =
        static_cast<::Katie::Qt::WindowModality>(Shiboken::Enum::getValue(pyIn));

}
static PythonToCppFunc is_Katie_Qt_WindowModality_PythonToCpp_Katie_Qt_WindowModality_Convertible(PyObject* pyIn) {
    if (PyObject_TypeCheck(pyIn, SbkKtCoreTypes[SBK_KATIE_QT_WINDOWMODALITY_IDX]))
        return Katie_Qt_WindowModality_PythonToCpp_Katie_Qt_WindowModality;
    return {};
}
static PyObject* Katie_Qt_WindowModality_CppToPython_Katie_Qt_WindowModality(const void* cppIn) {
    const int castCppIn = int(*reinterpret_cast<const ::Katie::Qt::WindowModality *>(cppIn));
    return Shiboken::Enum::newItem(SbkKtCoreTypes[SBK_KATIE_QT_WINDOWMODALITY_IDX], castCppIn);

}


static void Katie_Qt_TextInteractionFlag_PythonToCpp_Katie_Qt_TextInteractionFlag(PyObject* pyIn, void* cppOut) {
    *reinterpret_cast<::Katie::Qt::TextInteractionFlag*>(cppOut) =
        static_cast<::Katie::Qt::TextInteractionFlag>(Shiboken::Enum::getValue(pyIn));

}
static PythonToCppFunc is_Katie_Qt_TextInteractionFlag_PythonToCpp_Katie_Qt_TextInteractionFlag_Convertible(PyObject* pyIn) {
    if (PyObject_TypeCheck(pyIn, SbkKtCoreTypes[SBK_KATIE_QT_TEXTINTERACTIONFLAG_IDX]))
        return Katie_Qt_TextInteractionFlag_PythonToCpp_Katie_Qt_TextInteractionFlag;
    return {};
}
static PyObject* Katie_Qt_TextInteractionFlag_CppToPython_Katie_Qt_TextInteractionFlag(const void* cppIn) {
    const int castCppIn = int(*reinterpret_cast<const ::Katie::Qt::TextInteractionFlag *>(cppIn));
    return Shiboken::Enum::newItem(SbkKtCoreTypes[SBK_KATIE_QT_TEXTINTERACTIONFLAG_IDX], castCppIn);

}


static void Katie_Qt_EventPriority_PythonToCpp_Katie_Qt_EventPriority(PyObject* pyIn, void* cppOut) {
    *reinterpret_cast<::Katie::Qt::EventPriority*>(cppOut) =
        static_cast<::Katie::Qt::EventPriority>(Shiboken::Enum::getValue(pyIn));

}
static PythonToCppFunc is_Katie_Qt_EventPriority_PythonToCpp_Katie_Qt_EventPriority_Convertible(PyObject* pyIn) {
    if (PyObject_TypeCheck(pyIn, SbkKtCoreTypes[SBK_KATIE_QT_EVENTPRIORITY_IDX]))
        return Katie_Qt_EventPriority_PythonToCpp_Katie_Qt_EventPriority;
    return {};
}
static PyObject* Katie_Qt_EventPriority_CppToPython_Katie_Qt_EventPriority(const void* cppIn) {
    const int castCppIn = int(*reinterpret_cast<const ::Katie::Qt::EventPriority *>(cppIn));
    return Shiboken::Enum::newItem(SbkKtCoreTypes[SBK_KATIE_QT_EVENTPRIORITY_IDX], castCppIn);

}


static void Katie_Qt_SizeHint_PythonToCpp_Katie_Qt_SizeHint(PyObject* pyIn, void* cppOut) {
    *reinterpret_cast<::Katie::Qt::SizeHint*>(cppOut) =
        static_cast<::Katie::Qt::SizeHint>(Shiboken::Enum::getValue(pyIn));

}
static PythonToCppFunc is_Katie_Qt_SizeHint_PythonToCpp_Katie_Qt_SizeHint_Convertible(PyObject* pyIn) {
    if (PyObject_TypeCheck(pyIn, SbkKtCoreTypes[SBK_KATIE_QT_SIZEHINT_IDX]))
        return Katie_Qt_SizeHint_PythonToCpp_Katie_Qt_SizeHint;
    return {};
}
static PyObject* Katie_Qt_SizeHint_CppToPython_Katie_Qt_SizeHint(const void* cppIn) {
    const int castCppIn = int(*reinterpret_cast<const ::Katie::Qt::SizeHint *>(cppIn));
    return Shiboken::Enum::newItem(SbkKtCoreTypes[SBK_KATIE_QT_SIZEHINT_IDX], castCppIn);

}


static void Katie_Qt_WindowFrameSection_PythonToCpp_Katie_Qt_WindowFrameSection(PyObject* pyIn, void* cppOut) {
    *reinterpret_cast<::Katie::Qt::WindowFrameSection*>(cppOut) =
        static_cast<::Katie::Qt::WindowFrameSection>(Shiboken::Enum::getValue(pyIn));

}
static PythonToCppFunc is_Katie_Qt_WindowFrameSection_PythonToCpp_Katie_Qt_WindowFrameSection_Convertible(PyObject* pyIn) {
    if (PyObject_TypeCheck(pyIn, SbkKtCoreTypes[SBK_KATIE_QT_WINDOWFRAMESECTION_IDX]))
        return Katie_Qt_WindowFrameSection_PythonToCpp_Katie_Qt_WindowFrameSection;
    return {};
}
static PyObject* Katie_Qt_WindowFrameSection_CppToPython_Katie_Qt_WindowFrameSection(const void* cppIn) {
    const int castCppIn = int(*reinterpret_cast<const ::Katie::Qt::WindowFrameSection *>(cppIn));
    return Shiboken::Enum::newItem(SbkKtCoreTypes[SBK_KATIE_QT_WINDOWFRAMESECTION_IDX], castCppIn);

}


static void Katie_Qt_Initialization_PythonToCpp_Katie_Qt_Initialization(PyObject* pyIn, void* cppOut) {
    *reinterpret_cast<::Katie::Qt::Initialization*>(cppOut) =
        static_cast<::Katie::Qt::Initialization>(Shiboken::Enum::getValue(pyIn));

}
static PythonToCppFunc is_Katie_Qt_Initialization_PythonToCpp_Katie_Qt_Initialization_Convertible(PyObject* pyIn) {
    if (PyObject_TypeCheck(pyIn, SbkKtCoreTypes[SBK_KATIE_QT_INITIALIZATION_IDX]))
        return Katie_Qt_Initialization_PythonToCpp_Katie_Qt_Initialization;
    return {};
}
static PyObject* Katie_Qt_Initialization_CppToPython_Katie_Qt_Initialization(const void* cppIn) {
    const int castCppIn = int(*reinterpret_cast<const ::Katie::Qt::Initialization *>(cppIn));
    return Shiboken::Enum::newItem(SbkKtCoreTypes[SBK_KATIE_QT_INITIALIZATION_IDX], castCppIn);

}


static void Katie_Qt_CoordinateSystem_PythonToCpp_Katie_Qt_CoordinateSystem(PyObject* pyIn, void* cppOut) {
    *reinterpret_cast<::Katie::Qt::CoordinateSystem*>(cppOut) =
        static_cast<::Katie::Qt::CoordinateSystem>(Shiboken::Enum::getValue(pyIn));

}
static PythonToCppFunc is_Katie_Qt_CoordinateSystem_PythonToCpp_Katie_Qt_CoordinateSystem_Convertible(PyObject* pyIn) {
    if (PyObject_TypeCheck(pyIn, SbkKtCoreTypes[SBK_KATIE_QT_COORDINATESYSTEM_IDX]))
        return Katie_Qt_CoordinateSystem_PythonToCpp_Katie_Qt_CoordinateSystem;
    return {};
}
static PyObject* Katie_Qt_CoordinateSystem_CppToPython_Katie_Qt_CoordinateSystem(const void* cppIn) {
    const int castCppIn = int(*reinterpret_cast<const ::Katie::Qt::CoordinateSystem *>(cppIn));
    return Shiboken::Enum::newItem(SbkKtCoreTypes[SBK_KATIE_QT_COORDINATESYSTEM_IDX], castCppIn);

}


static void Katie_Qt_TouchPointState_PythonToCpp_Katie_Qt_TouchPointState(PyObject* pyIn, void* cppOut) {
    *reinterpret_cast<::Katie::Qt::TouchPointState*>(cppOut) =
        static_cast<::Katie::Qt::TouchPointState>(Shiboken::Enum::getValue(pyIn));

}
static PythonToCppFunc is_Katie_Qt_TouchPointState_PythonToCpp_Katie_Qt_TouchPointState_Convertible(PyObject* pyIn) {
    if (PyObject_TypeCheck(pyIn, SbkKtCoreTypes[SBK_KATIE_QT_TOUCHPOINTSTATE_IDX]))
        return Katie_Qt_TouchPointState_PythonToCpp_Katie_Qt_TouchPointState;
    return {};
}
static PyObject* Katie_Qt_TouchPointState_CppToPython_Katie_Qt_TouchPointState(const void* cppIn) {
    const int castCppIn = int(*reinterpret_cast<const ::Katie::Qt::TouchPointState *>(cppIn));
    return Shiboken::Enum::newItem(SbkKtCoreTypes[SBK_KATIE_QT_TOUCHPOINTSTATE_IDX], castCppIn);

}


static void Katie_Qt_GestureState_PythonToCpp_Katie_Qt_GestureState(PyObject* pyIn, void* cppOut) {
    *reinterpret_cast<::Katie::Qt::GestureState*>(cppOut) =
        static_cast<::Katie::Qt::GestureState>(Shiboken::Enum::getValue(pyIn));

}
static PythonToCppFunc is_Katie_Qt_GestureState_PythonToCpp_Katie_Qt_GestureState_Convertible(PyObject* pyIn) {
    if (PyObject_TypeCheck(pyIn, SbkKtCoreTypes[SBK_KATIE_QT_GESTURESTATE_IDX]))
        return Katie_Qt_GestureState_PythonToCpp_Katie_Qt_GestureState;
    return {};
}
static PyObject* Katie_Qt_GestureState_CppToPython_Katie_Qt_GestureState(const void* cppIn) {
    const int castCppIn = int(*reinterpret_cast<const ::Katie::Qt::GestureState *>(cppIn));
    return Shiboken::Enum::newItem(SbkKtCoreTypes[SBK_KATIE_QT_GESTURESTATE_IDX], castCppIn);

}


static void Katie_Qt_GestureType_PythonToCpp_Katie_Qt_GestureType(PyObject* pyIn, void* cppOut) {
    *reinterpret_cast<::Katie::Qt::GestureType*>(cppOut) =
        static_cast<::Katie::Qt::GestureType>(Shiboken::Enum::getValue(pyIn));

}
static PythonToCppFunc is_Katie_Qt_GestureType_PythonToCpp_Katie_Qt_GestureType_Convertible(PyObject* pyIn) {
    if (PyObject_TypeCheck(pyIn, SbkKtCoreTypes[SBK_KATIE_QT_GESTURETYPE_IDX]))
        return Katie_Qt_GestureType_PythonToCpp_Katie_Qt_GestureType;
    return {};
}
static PyObject* Katie_Qt_GestureType_CppToPython_Katie_Qt_GestureType(const void* cppIn) {
    const int castCppIn = int(*reinterpret_cast<const ::Katie::Qt::GestureType *>(cppIn));
    return Shiboken::Enum::newItem(SbkKtCoreTypes[SBK_KATIE_QT_GESTURETYPE_IDX], castCppIn);

}


static void Katie_Qt_GestureFlag_PythonToCpp_Katie_Qt_GestureFlag(PyObject* pyIn, void* cppOut) {
    *reinterpret_cast<::Katie::Qt::GestureFlag*>(cppOut) =
        static_cast<::Katie::Qt::GestureFlag>(Shiboken::Enum::getValue(pyIn));

}
static PythonToCppFunc is_Katie_Qt_GestureFlag_PythonToCpp_Katie_Qt_GestureFlag_Convertible(PyObject* pyIn) {
    if (PyObject_TypeCheck(pyIn, SbkKtCoreTypes[SBK_KATIE_QT_GESTUREFLAG_IDX]))
        return Katie_Qt_GestureFlag_PythonToCpp_Katie_Qt_GestureFlag;
    return {};
}
static PyObject* Katie_Qt_GestureFlag_CppToPython_Katie_Qt_GestureFlag(const void* cppIn) {
    const int castCppIn = int(*reinterpret_cast<const ::Katie::Qt::GestureFlag *>(cppIn));
    return Shiboken::Enum::newItem(SbkKtCoreTypes[SBK_KATIE_QT_GESTUREFLAG_IDX], castCppIn);

}


static void Katie_Qt_NavigationMode_PythonToCpp_Katie_Qt_NavigationMode(PyObject* pyIn, void* cppOut) {
    *reinterpret_cast<::Katie::Qt::NavigationMode*>(cppOut) =
        static_cast<::Katie::Qt::NavigationMode>(Shiboken::Enum::getValue(pyIn));

}
static PythonToCppFunc is_Katie_Qt_NavigationMode_PythonToCpp_Katie_Qt_NavigationMode_Convertible(PyObject* pyIn) {
    if (PyObject_TypeCheck(pyIn, SbkKtCoreTypes[SBK_KATIE_QT_NAVIGATIONMODE_IDX]))
        return Katie_Qt_NavigationMode_PythonToCpp_Katie_Qt_NavigationMode;
    return {};
}
static PyObject* Katie_Qt_NavigationMode_CppToPython_Katie_Qt_NavigationMode(const void* cppIn) {
    const int castCppIn = int(*reinterpret_cast<const ::Katie::Qt::NavigationMode *>(cppIn));
    return Shiboken::Enum::newItem(SbkKtCoreTypes[SBK_KATIE_QT_NAVIGATIONMODE_IDX], castCppIn);

}


static void Katie_Qt_CursorMoveStyle_PythonToCpp_Katie_Qt_CursorMoveStyle(PyObject* pyIn, void* cppOut) {
    *reinterpret_cast<::Katie::Qt::CursorMoveStyle*>(cppOut) =
        static_cast<::Katie::Qt::CursorMoveStyle>(Shiboken::Enum::getValue(pyIn));

}
static PythonToCppFunc is_Katie_Qt_CursorMoveStyle_PythonToCpp_Katie_Qt_CursorMoveStyle_Convertible(PyObject* pyIn) {
    if (PyObject_TypeCheck(pyIn, SbkKtCoreTypes[SBK_KATIE_QT_CURSORMOVESTYLE_IDX]))
        return Katie_Qt_CursorMoveStyle_PythonToCpp_Katie_Qt_CursorMoveStyle;
    return {};
}
static PyObject* Katie_Qt_CursorMoveStyle_CppToPython_Katie_Qt_CursorMoveStyle(const void* cppIn) {
    const int castCppIn = int(*reinterpret_cast<const ::Katie::Qt::CursorMoveStyle *>(cppIn));
    return Shiboken::Enum::newItem(SbkKtCoreTypes[SBK_KATIE_QT_CURSORMOVESTYLE_IDX], castCppIn);

}



// Module initialization ------------------------------------------------------------
#if defined _WIN32 || defined __CYGWIN__
    #define SBK_EXPORT_MODULE __declspec(dllexport)
#elif __GNUC__ >= 4
    #define SBK_EXPORT_MODULE __attribute__ ((visibility("default")))
#else
    #define SBK_EXPORT_MODULE
#endif

#ifdef IS_PY3K
static struct PyModuleDef moduledef = {
    /* m_base     */ PyModuleDef_HEAD_INIT,
    /* m_name     */ "KtCore",
    /* m_doc      */ nullptr,
    /* m_size     */ -1,
    /* m_methods  */ KtCore_methods,
    /* m_reload   */ nullptr,
    /* m_traverse */ nullptr,
    /* m_clear    */ nullptr,
    /* m_free     */ nullptr
};

#endif

// The signatures string for the global functions.
// Multiple signatures have their index "n:" in front.
static const char *KtCore_SignatureStrings[] = {
    nullptr}; // Sentinel

SBK_MODULE_INIT_FUNCTION_BEGIN(KtCore)
    // Create an array of wrapper types for the current module.
    static PyTypeObject* cppApi[SBK_KtCore_IDX_COUNT];
    SbkKtCoreTypes = cppApi;

    // Create an array of primitive type converters for the current module.
    static SbkConverter* sbkConverters[SBK_KtCore_CONVERTERS_IDX_COUNT];
    SbkKtCoreTypeConverters = sbkConverters;

#ifdef IS_PY3K
    PyObject* module = Shiboken::Module::create("KtCore", &moduledef);
#else
    PyObject* module = Shiboken::Module::create("KtCore", KtCore_methods);
#endif

    // Make module available from global scope
    SbkKtCoreModuleObject = module;

    // Initialize classes in the type system
    init_Katie_QVariant(module);
    init_Katie_QTextBoundaryFinder(module);
    init_Katie_QStringList(module);
    init_Katie_QRegExp(module);
    init_Katie_QObject(module);
    init_Katie_QLatin1String(module);
    init_Katie_QLatin1Char(module);
    init_Katie_QCoreApplication(module);
    init_Katie_QChar(module);
    init_Katie_QString(module);
    init_Katie_QByteArray(module);
    // Initialization of enums.

    // Initialization of enum 'QtMsgType'.
    SbkKtCoreTypes[SBK_KATIE_QTMSGTYPE_IDX] = Shiboken::Enum::createGlobalEnum(module,
        "QtMsgType",
        "KtCore.Katie.QtMsgType",
        "Katie::QtMsgType");
    if (!SbkKtCoreTypes[SBK_KATIE_QTMSGTYPE_IDX])
        return SBK_MODULE_INIT_ERROR;

    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QTMSGTYPE_IDX],
        module, "QtDebugMsg", (long) Katie::QtMsgType::QtDebugMsg))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QTMSGTYPE_IDX],
        module, "QtWarningMsg", (long) Katie::QtMsgType::QtWarningMsg))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QTMSGTYPE_IDX],
        module, "QtCriticalMsg", (long) Katie::QtMsgType::QtCriticalMsg))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QTMSGTYPE_IDX],
        module, "QtFatalMsg", (long) Katie::QtMsgType::QtFatalMsg))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QTMSGTYPE_IDX],
        module, "QtSystemMsg", (long) Katie::QtMsgType::QtSystemMsg))
        return SBK_MODULE_INIT_ERROR;
    // Register converter for enum 'Katie::QtMsgType'.
    {
        SbkConverter* converter = Shiboken::Conversions::createConverter(SbkKtCoreTypes[SBK_KATIE_QTMSGTYPE_IDX],
            Katie_QtMsgType_CppToPython_Katie_QtMsgType);
        Shiboken::Conversions::addPythonToCppValueConversion(converter,
            Katie_QtMsgType_PythonToCpp_Katie_QtMsgType,
            is_Katie_QtMsgType_PythonToCpp_Katie_QtMsgType_Convertible);
        Shiboken::Enum::setTypeConverter(SbkKtCoreTypes[SBK_KATIE_QTMSGTYPE_IDX], converter);
        Shiboken::Conversions::registerConverterName(converter, "Katie::QtMsgType");
        Shiboken::Conversions::registerConverterName(converter, "QtMsgType");
    }
    // End of 'QtMsgType' enum.

    // Initialization of enum 'GlobalColor'.
    SbkKtCoreTypes[SBK_KATIE_QT_GLOBALCOLOR_IDX] = Shiboken::Enum::createGlobalEnum(module,
        "GlobalColor",
        "KtCore.Katie.Qt.GlobalColor",
        "Katie::Qt::GlobalColor");
    if (!SbkKtCoreTypes[SBK_KATIE_QT_GLOBALCOLOR_IDX])
        return SBK_MODULE_INIT_ERROR;

    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_GLOBALCOLOR_IDX],
        module, "color0", (long) Katie::Qt::GlobalColor::color0))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_GLOBALCOLOR_IDX],
        module, "color1", (long) Katie::Qt::GlobalColor::color1))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_GLOBALCOLOR_IDX],
        module, "black", (long) Katie::Qt::GlobalColor::black))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_GLOBALCOLOR_IDX],
        module, "white", (long) Katie::Qt::GlobalColor::white))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_GLOBALCOLOR_IDX],
        module, "darkGray", (long) Katie::Qt::GlobalColor::darkGray))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_GLOBALCOLOR_IDX],
        module, "gray", (long) Katie::Qt::GlobalColor::gray))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_GLOBALCOLOR_IDX],
        module, "lightGray", (long) Katie::Qt::GlobalColor::lightGray))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_GLOBALCOLOR_IDX],
        module, "red", (long) Katie::Qt::GlobalColor::red))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_GLOBALCOLOR_IDX],
        module, "green", (long) Katie::Qt::GlobalColor::green))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_GLOBALCOLOR_IDX],
        module, "blue", (long) Katie::Qt::GlobalColor::blue))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_GLOBALCOLOR_IDX],
        module, "cyan", (long) Katie::Qt::GlobalColor::cyan))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_GLOBALCOLOR_IDX],
        module, "magenta", (long) Katie::Qt::GlobalColor::magenta))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_GLOBALCOLOR_IDX],
        module, "yellow", (long) Katie::Qt::GlobalColor::yellow))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_GLOBALCOLOR_IDX],
        module, "darkRed", (long) Katie::Qt::GlobalColor::darkRed))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_GLOBALCOLOR_IDX],
        module, "darkGreen", (long) Katie::Qt::GlobalColor::darkGreen))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_GLOBALCOLOR_IDX],
        module, "darkBlue", (long) Katie::Qt::GlobalColor::darkBlue))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_GLOBALCOLOR_IDX],
        module, "darkCyan", (long) Katie::Qt::GlobalColor::darkCyan))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_GLOBALCOLOR_IDX],
        module, "darkMagenta", (long) Katie::Qt::GlobalColor::darkMagenta))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_GLOBALCOLOR_IDX],
        module, "darkYellow", (long) Katie::Qt::GlobalColor::darkYellow))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_GLOBALCOLOR_IDX],
        module, "transparent", (long) Katie::Qt::GlobalColor::transparent))
        return SBK_MODULE_INIT_ERROR;
    // Register converter for enum 'Katie::Qt::GlobalColor'.
    {
        SbkConverter* converter = Shiboken::Conversions::createConverter(SbkKtCoreTypes[SBK_KATIE_QT_GLOBALCOLOR_IDX],
            Katie_Qt_GlobalColor_CppToPython_Katie_Qt_GlobalColor);
        Shiboken::Conversions::addPythonToCppValueConversion(converter,
            Katie_Qt_GlobalColor_PythonToCpp_Katie_Qt_GlobalColor,
            is_Katie_Qt_GlobalColor_PythonToCpp_Katie_Qt_GlobalColor_Convertible);
        Shiboken::Enum::setTypeConverter(SbkKtCoreTypes[SBK_KATIE_QT_GLOBALCOLOR_IDX], converter);
        Shiboken::Conversions::registerConverterName(converter, "Katie::Qt::GlobalColor");
        Shiboken::Conversions::registerConverterName(converter, "Qt::GlobalColor");
        Shiboken::Conversions::registerConverterName(converter, "GlobalColor");
    }
    // End of 'GlobalColor' enum.

    // Initialization of enum 'KeyboardModifier'.
    SbkKtCoreTypes[SBK_KATIE_QT_KEYBOARDMODIFIER_IDX] = Shiboken::Enum::createGlobalEnum(module,
        "KeyboardModifier",
        "KtCore.Katie.Qt.KeyboardModifier",
        "Katie::Qt::KeyboardModifier");
    if (!SbkKtCoreTypes[SBK_KATIE_QT_KEYBOARDMODIFIER_IDX])
        return SBK_MODULE_INIT_ERROR;

    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEYBOARDMODIFIER_IDX],
        module, "NoModifier", (long) Katie::Qt::KeyboardModifier::NoModifier))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEYBOARDMODIFIER_IDX],
        module, "ShiftModifier", (long) Katie::Qt::KeyboardModifier::ShiftModifier))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEYBOARDMODIFIER_IDX],
        module, "ControlModifier", (long) Katie::Qt::KeyboardModifier::ControlModifier))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEYBOARDMODIFIER_IDX],
        module, "AltModifier", (long) Katie::Qt::KeyboardModifier::AltModifier))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEYBOARDMODIFIER_IDX],
        module, "MetaModifier", (long) Katie::Qt::KeyboardModifier::MetaModifier))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEYBOARDMODIFIER_IDX],
        module, "KeypadModifier", (long) Katie::Qt::KeyboardModifier::KeypadModifier))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEYBOARDMODIFIER_IDX],
        module, "GroupSwitchModifier", (long) Katie::Qt::KeyboardModifier::GroupSwitchModifier))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEYBOARDMODIFIER_IDX],
        module, "KeyboardModifierMask", (long) Katie::Qt::KeyboardModifier::KeyboardModifierMask))
        return SBK_MODULE_INIT_ERROR;
    // Register converter for enum 'Katie::Qt::KeyboardModifier'.
    {
        SbkConverter* converter = Shiboken::Conversions::createConverter(SbkKtCoreTypes[SBK_KATIE_QT_KEYBOARDMODIFIER_IDX],
            Katie_Qt_KeyboardModifier_CppToPython_Katie_Qt_KeyboardModifier);
        Shiboken::Conversions::addPythonToCppValueConversion(converter,
            Katie_Qt_KeyboardModifier_PythonToCpp_Katie_Qt_KeyboardModifier,
            is_Katie_Qt_KeyboardModifier_PythonToCpp_Katie_Qt_KeyboardModifier_Convertible);
        Shiboken::Enum::setTypeConverter(SbkKtCoreTypes[SBK_KATIE_QT_KEYBOARDMODIFIER_IDX], converter);
        Shiboken::Conversions::registerConverterName(converter, "Katie::Qt::KeyboardModifier");
        Shiboken::Conversions::registerConverterName(converter, "Qt::KeyboardModifier");
        Shiboken::Conversions::registerConverterName(converter, "KeyboardModifier");
    }
    // End of 'KeyboardModifier' enum.

    // Initialization of enum 'Modifier'.
    SbkKtCoreTypes[SBK_KATIE_QT_MODIFIER_IDX] = Shiboken::Enum::createGlobalEnum(module,
        "Modifier",
        "KtCore.Katie.Qt.Modifier",
        "Katie::Qt::Modifier");
    if (!SbkKtCoreTypes[SBK_KATIE_QT_MODIFIER_IDX])
        return SBK_MODULE_INIT_ERROR;

    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_MODIFIER_IDX],
        module, "META", (long) Katie::Qt::Modifier::META))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_MODIFIER_IDX],
        module, "SHIFT", (long) Katie::Qt::Modifier::SHIFT))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_MODIFIER_IDX],
        module, "CTRL", (long) Katie::Qt::Modifier::CTRL))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_MODIFIER_IDX],
        module, "ALT", (long) Katie::Qt::Modifier::ALT))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_MODIFIER_IDX],
        module, "MODIFIER_MASK", (long) Katie::Qt::Modifier::MODIFIER_MASK))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_MODIFIER_IDX],
        module, "UNICODE_ACCEL", (long) Katie::Qt::Modifier::UNICODE_ACCEL))
        return SBK_MODULE_INIT_ERROR;
    // Register converter for enum 'Katie::Qt::Modifier'.
    {
        SbkConverter* converter = Shiboken::Conversions::createConverter(SbkKtCoreTypes[SBK_KATIE_QT_MODIFIER_IDX],
            Katie_Qt_Modifier_CppToPython_Katie_Qt_Modifier);
        Shiboken::Conversions::addPythonToCppValueConversion(converter,
            Katie_Qt_Modifier_PythonToCpp_Katie_Qt_Modifier,
            is_Katie_Qt_Modifier_PythonToCpp_Katie_Qt_Modifier_Convertible);
        Shiboken::Enum::setTypeConverter(SbkKtCoreTypes[SBK_KATIE_QT_MODIFIER_IDX], converter);
        Shiboken::Conversions::registerConverterName(converter, "Katie::Qt::Modifier");
        Shiboken::Conversions::registerConverterName(converter, "Qt::Modifier");
        Shiboken::Conversions::registerConverterName(converter, "Modifier");
    }
    // End of 'Modifier' enum.

    // Initialization of enum 'MouseButton'.
    SbkKtCoreTypes[SBK_KATIE_QT_MOUSEBUTTON_IDX] = Shiboken::Enum::createGlobalEnum(module,
        "MouseButton",
        "KtCore.Katie.Qt.MouseButton",
        "Katie::Qt::MouseButton");
    if (!SbkKtCoreTypes[SBK_KATIE_QT_MOUSEBUTTON_IDX])
        return SBK_MODULE_INIT_ERROR;

    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_MOUSEBUTTON_IDX],
        module, "NoButton", (long) Katie::Qt::MouseButton::NoButton))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_MOUSEBUTTON_IDX],
        module, "LeftButton", (long) Katie::Qt::MouseButton::LeftButton))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_MOUSEBUTTON_IDX],
        module, "RightButton", (long) Katie::Qt::MouseButton::RightButton))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_MOUSEBUTTON_IDX],
        module, "MiddleButton", (long) Katie::Qt::MouseButton::MiddleButton))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_MOUSEBUTTON_IDX],
        module, "XButton1", (long) Katie::Qt::MouseButton::XButton1))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_MOUSEBUTTON_IDX],
        module, "XButton2", (long) Katie::Qt::MouseButton::XButton2))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_MOUSEBUTTON_IDX],
        module, "MouseButtonMask", (long) Katie::Qt::MouseButton::MouseButtonMask))
        return SBK_MODULE_INIT_ERROR;
    // Register converter for enum 'Katie::Qt::MouseButton'.
    {
        SbkConverter* converter = Shiboken::Conversions::createConverter(SbkKtCoreTypes[SBK_KATIE_QT_MOUSEBUTTON_IDX],
            Katie_Qt_MouseButton_CppToPython_Katie_Qt_MouseButton);
        Shiboken::Conversions::addPythonToCppValueConversion(converter,
            Katie_Qt_MouseButton_PythonToCpp_Katie_Qt_MouseButton,
            is_Katie_Qt_MouseButton_PythonToCpp_Katie_Qt_MouseButton_Convertible);
        Shiboken::Enum::setTypeConverter(SbkKtCoreTypes[SBK_KATIE_QT_MOUSEBUTTON_IDX], converter);
        Shiboken::Conversions::registerConverterName(converter, "Katie::Qt::MouseButton");
        Shiboken::Conversions::registerConverterName(converter, "Qt::MouseButton");
        Shiboken::Conversions::registerConverterName(converter, "MouseButton");
    }
    // End of 'MouseButton' enum.

    // Initialization of enum 'Orientation'.
    SbkKtCoreTypes[SBK_KATIE_QT_ORIENTATION_IDX] = Shiboken::Enum::createGlobalEnum(module,
        "Orientation",
        "KtCore.Katie.Qt.Orientation",
        "Katie::Qt::Orientation");
    if (!SbkKtCoreTypes[SBK_KATIE_QT_ORIENTATION_IDX])
        return SBK_MODULE_INIT_ERROR;

    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_ORIENTATION_IDX],
        module, "Horizontal", (long) Katie::Qt::Orientation::Horizontal))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_ORIENTATION_IDX],
        module, "Vertical", (long) Katie::Qt::Orientation::Vertical))
        return SBK_MODULE_INIT_ERROR;
    // Register converter for enum 'Katie::Qt::Orientation'.
    {
        SbkConverter* converter = Shiboken::Conversions::createConverter(SbkKtCoreTypes[SBK_KATIE_QT_ORIENTATION_IDX],
            Katie_Qt_Orientation_CppToPython_Katie_Qt_Orientation);
        Shiboken::Conversions::addPythonToCppValueConversion(converter,
            Katie_Qt_Orientation_PythonToCpp_Katie_Qt_Orientation,
            is_Katie_Qt_Orientation_PythonToCpp_Katie_Qt_Orientation_Convertible);
        Shiboken::Enum::setTypeConverter(SbkKtCoreTypes[SBK_KATIE_QT_ORIENTATION_IDX], converter);
        Shiboken::Conversions::registerConverterName(converter, "Katie::Qt::Orientation");
        Shiboken::Conversions::registerConverterName(converter, "Qt::Orientation");
        Shiboken::Conversions::registerConverterName(converter, "Orientation");
    }
    // End of 'Orientation' enum.

    // Initialization of enum 'FocusPolicy'.
    SbkKtCoreTypes[SBK_KATIE_QT_FOCUSPOLICY_IDX] = Shiboken::Enum::createGlobalEnum(module,
        "FocusPolicy",
        "KtCore.Katie.Qt.FocusPolicy",
        "Katie::Qt::FocusPolicy");
    if (!SbkKtCoreTypes[SBK_KATIE_QT_FOCUSPOLICY_IDX])
        return SBK_MODULE_INIT_ERROR;

    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_FOCUSPOLICY_IDX],
        module, "NoFocus", (long) Katie::Qt::FocusPolicy::NoFocus))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_FOCUSPOLICY_IDX],
        module, "TabFocus", (long) Katie::Qt::FocusPolicy::TabFocus))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_FOCUSPOLICY_IDX],
        module, "ClickFocus", (long) Katie::Qt::FocusPolicy::ClickFocus))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_FOCUSPOLICY_IDX],
        module, "StrongFocus", (long) Katie::Qt::FocusPolicy::StrongFocus))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_FOCUSPOLICY_IDX],
        module, "WheelFocus", (long) Katie::Qt::FocusPolicy::WheelFocus))
        return SBK_MODULE_INIT_ERROR;
    // Register converter for enum 'Katie::Qt::FocusPolicy'.
    {
        SbkConverter* converter = Shiboken::Conversions::createConverter(SbkKtCoreTypes[SBK_KATIE_QT_FOCUSPOLICY_IDX],
            Katie_Qt_FocusPolicy_CppToPython_Katie_Qt_FocusPolicy);
        Shiboken::Conversions::addPythonToCppValueConversion(converter,
            Katie_Qt_FocusPolicy_PythonToCpp_Katie_Qt_FocusPolicy,
            is_Katie_Qt_FocusPolicy_PythonToCpp_Katie_Qt_FocusPolicy_Convertible);
        Shiboken::Enum::setTypeConverter(SbkKtCoreTypes[SBK_KATIE_QT_FOCUSPOLICY_IDX], converter);
        Shiboken::Conversions::registerConverterName(converter, "Katie::Qt::FocusPolicy");
        Shiboken::Conversions::registerConverterName(converter, "Qt::FocusPolicy");
        Shiboken::Conversions::registerConverterName(converter, "FocusPolicy");
    }
    // End of 'FocusPolicy' enum.

    // Initialization of enum 'SortOrder'.
    SbkKtCoreTypes[SBK_KATIE_QT_SORTORDER_IDX] = Shiboken::Enum::createGlobalEnum(module,
        "SortOrder",
        "KtCore.Katie.Qt.SortOrder",
        "Katie::Qt::SortOrder");
    if (!SbkKtCoreTypes[SBK_KATIE_QT_SORTORDER_IDX])
        return SBK_MODULE_INIT_ERROR;

    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_SORTORDER_IDX],
        module, "AscendingOrder", (long) Katie::Qt::SortOrder::AscendingOrder))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_SORTORDER_IDX],
        module, "DescendingOrder", (long) Katie::Qt::SortOrder::DescendingOrder))
        return SBK_MODULE_INIT_ERROR;
    // Register converter for enum 'Katie::Qt::SortOrder'.
    {
        SbkConverter* converter = Shiboken::Conversions::createConverter(SbkKtCoreTypes[SBK_KATIE_QT_SORTORDER_IDX],
            Katie_Qt_SortOrder_CppToPython_Katie_Qt_SortOrder);
        Shiboken::Conversions::addPythonToCppValueConversion(converter,
            Katie_Qt_SortOrder_PythonToCpp_Katie_Qt_SortOrder,
            is_Katie_Qt_SortOrder_PythonToCpp_Katie_Qt_SortOrder_Convertible);
        Shiboken::Enum::setTypeConverter(SbkKtCoreTypes[SBK_KATIE_QT_SORTORDER_IDX], converter);
        Shiboken::Conversions::registerConverterName(converter, "Katie::Qt::SortOrder");
        Shiboken::Conversions::registerConverterName(converter, "Qt::SortOrder");
        Shiboken::Conversions::registerConverterName(converter, "SortOrder");
    }
    // End of 'SortOrder' enum.

    // Initialization of enum 'TileRule'.
    SbkKtCoreTypes[SBK_KATIE_QT_TILERULE_IDX] = Shiboken::Enum::createGlobalEnum(module,
        "TileRule",
        "KtCore.Katie.Qt.TileRule",
        "Katie::Qt::TileRule");
    if (!SbkKtCoreTypes[SBK_KATIE_QT_TILERULE_IDX])
        return SBK_MODULE_INIT_ERROR;

    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_TILERULE_IDX],
        module, "StretchTile", (long) Katie::Qt::TileRule::StretchTile))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_TILERULE_IDX],
        module, "RepeatTile", (long) Katie::Qt::TileRule::RepeatTile))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_TILERULE_IDX],
        module, "RoundTile", (long) Katie::Qt::TileRule::RoundTile))
        return SBK_MODULE_INIT_ERROR;
    // Register converter for enum 'Katie::Qt::TileRule'.
    {
        SbkConverter* converter = Shiboken::Conversions::createConverter(SbkKtCoreTypes[SBK_KATIE_QT_TILERULE_IDX],
            Katie_Qt_TileRule_CppToPython_Katie_Qt_TileRule);
        Shiboken::Conversions::addPythonToCppValueConversion(converter,
            Katie_Qt_TileRule_PythonToCpp_Katie_Qt_TileRule,
            is_Katie_Qt_TileRule_PythonToCpp_Katie_Qt_TileRule_Convertible);
        Shiboken::Enum::setTypeConverter(SbkKtCoreTypes[SBK_KATIE_QT_TILERULE_IDX], converter);
        Shiboken::Conversions::registerConverterName(converter, "Katie::Qt::TileRule");
        Shiboken::Conversions::registerConverterName(converter, "Qt::TileRule");
        Shiboken::Conversions::registerConverterName(converter, "TileRule");
    }
    // End of 'TileRule' enum.

    // Initialization of enum 'AlignmentFlag'.
    SbkKtCoreTypes[SBK_KATIE_QT_ALIGNMENTFLAG_IDX] = Shiboken::Enum::createGlobalEnum(module,
        "AlignmentFlag",
        "KtCore.Katie.Qt.AlignmentFlag",
        "Katie::Qt::AlignmentFlag");
    if (!SbkKtCoreTypes[SBK_KATIE_QT_ALIGNMENTFLAG_IDX])
        return SBK_MODULE_INIT_ERROR;

    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_ALIGNMENTFLAG_IDX],
        module, "AlignLeft", (long) Katie::Qt::AlignmentFlag::AlignLeft))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_ALIGNMENTFLAG_IDX],
        module, "AlignLeading", (long) Katie::Qt::AlignmentFlag::AlignLeading))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_ALIGNMENTFLAG_IDX],
        module, "AlignRight", (long) Katie::Qt::AlignmentFlag::AlignRight))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_ALIGNMENTFLAG_IDX],
        module, "AlignTrailing", (long) Katie::Qt::AlignmentFlag::AlignTrailing))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_ALIGNMENTFLAG_IDX],
        module, "AlignHCenter", (long) Katie::Qt::AlignmentFlag::AlignHCenter))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_ALIGNMENTFLAG_IDX],
        module, "AlignJustify", (long) Katie::Qt::AlignmentFlag::AlignJustify))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_ALIGNMENTFLAG_IDX],
        module, "AlignAbsolute", (long) Katie::Qt::AlignmentFlag::AlignAbsolute))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_ALIGNMENTFLAG_IDX],
        module, "AlignHorizontal_Mask", (long) Katie::Qt::AlignmentFlag::AlignHorizontal_Mask))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_ALIGNMENTFLAG_IDX],
        module, "AlignTop", (long) Katie::Qt::AlignmentFlag::AlignTop))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_ALIGNMENTFLAG_IDX],
        module, "AlignBottom", (long) Katie::Qt::AlignmentFlag::AlignBottom))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_ALIGNMENTFLAG_IDX],
        module, "AlignVCenter", (long) Katie::Qt::AlignmentFlag::AlignVCenter))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_ALIGNMENTFLAG_IDX],
        module, "AlignVertical_Mask", (long) Katie::Qt::AlignmentFlag::AlignVertical_Mask))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_ALIGNMENTFLAG_IDX],
        module, "AlignCenter", (long) Katie::Qt::AlignmentFlag::AlignCenter))
        return SBK_MODULE_INIT_ERROR;
    // Register converter for enum 'Katie::Qt::AlignmentFlag'.
    {
        SbkConverter* converter = Shiboken::Conversions::createConverter(SbkKtCoreTypes[SBK_KATIE_QT_ALIGNMENTFLAG_IDX],
            Katie_Qt_AlignmentFlag_CppToPython_Katie_Qt_AlignmentFlag);
        Shiboken::Conversions::addPythonToCppValueConversion(converter,
            Katie_Qt_AlignmentFlag_PythonToCpp_Katie_Qt_AlignmentFlag,
            is_Katie_Qt_AlignmentFlag_PythonToCpp_Katie_Qt_AlignmentFlag_Convertible);
        Shiboken::Enum::setTypeConverter(SbkKtCoreTypes[SBK_KATIE_QT_ALIGNMENTFLAG_IDX], converter);
        Shiboken::Conversions::registerConverterName(converter, "Katie::Qt::AlignmentFlag");
        Shiboken::Conversions::registerConverterName(converter, "Qt::AlignmentFlag");
        Shiboken::Conversions::registerConverterName(converter, "AlignmentFlag");
    }
    // End of 'AlignmentFlag' enum.

    // Initialization of enum 'TextFlag'.
    SbkKtCoreTypes[SBK_KATIE_QT_TEXTFLAG_IDX] = Shiboken::Enum::createGlobalEnum(module,
        "TextFlag",
        "KtCore.Katie.Qt.TextFlag",
        "Katie::Qt::TextFlag");
    if (!SbkKtCoreTypes[SBK_KATIE_QT_TEXTFLAG_IDX])
        return SBK_MODULE_INIT_ERROR;

    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_TEXTFLAG_IDX],
        module, "TextSingleLine", (long) Katie::Qt::TextFlag::TextSingleLine))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_TEXTFLAG_IDX],
        module, "TextDontClip", (long) Katie::Qt::TextFlag::TextDontClip))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_TEXTFLAG_IDX],
        module, "TextExpandTabs", (long) Katie::Qt::TextFlag::TextExpandTabs))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_TEXTFLAG_IDX],
        module, "TextShowMnemonic", (long) Katie::Qt::TextFlag::TextShowMnemonic))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_TEXTFLAG_IDX],
        module, "TextWordWrap", (long) Katie::Qt::TextFlag::TextWordWrap))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_TEXTFLAG_IDX],
        module, "TextWrapAnywhere", (long) Katie::Qt::TextFlag::TextWrapAnywhere))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_TEXTFLAG_IDX],
        module, "TextDontPrint", (long) Katie::Qt::TextFlag::TextDontPrint))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_TEXTFLAG_IDX],
        module, "TextIncludeTrailingSpaces", (long) Katie::Qt::TextFlag::TextIncludeTrailingSpaces))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_TEXTFLAG_IDX],
        module, "TextHideMnemonic", (long) Katie::Qt::TextFlag::TextHideMnemonic))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_TEXTFLAG_IDX],
        module, "TextJustificationForced", (long) Katie::Qt::TextFlag::TextJustificationForced))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_TEXTFLAG_IDX],
        module, "TextForceLeftToRight", (long) Katie::Qt::TextFlag::TextForceLeftToRight))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_TEXTFLAG_IDX],
        module, "TextForceRightToLeft", (long) Katie::Qt::TextFlag::TextForceRightToLeft))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_TEXTFLAG_IDX],
        module, "TextLongestVariant", (long) Katie::Qt::TextFlag::TextLongestVariant))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_TEXTFLAG_IDX],
        module, "TextBypassShaping", (long) Katie::Qt::TextFlag::TextBypassShaping))
        return SBK_MODULE_INIT_ERROR;
    // Register converter for enum 'Katie::Qt::TextFlag'.
    {
        SbkConverter* converter = Shiboken::Conversions::createConverter(SbkKtCoreTypes[SBK_KATIE_QT_TEXTFLAG_IDX],
            Katie_Qt_TextFlag_CppToPython_Katie_Qt_TextFlag);
        Shiboken::Conversions::addPythonToCppValueConversion(converter,
            Katie_Qt_TextFlag_PythonToCpp_Katie_Qt_TextFlag,
            is_Katie_Qt_TextFlag_PythonToCpp_Katie_Qt_TextFlag_Convertible);
        Shiboken::Enum::setTypeConverter(SbkKtCoreTypes[SBK_KATIE_QT_TEXTFLAG_IDX], converter);
        Shiboken::Conversions::registerConverterName(converter, "Katie::Qt::TextFlag");
        Shiboken::Conversions::registerConverterName(converter, "Qt::TextFlag");
        Shiboken::Conversions::registerConverterName(converter, "TextFlag");
    }
    // End of 'TextFlag' enum.

    // Initialization of enum 'TextElideMode'.
    SbkKtCoreTypes[SBK_KATIE_QT_TEXTELIDEMODE_IDX] = Shiboken::Enum::createGlobalEnum(module,
        "TextElideMode",
        "KtCore.Katie.Qt.TextElideMode",
        "Katie::Qt::TextElideMode");
    if (!SbkKtCoreTypes[SBK_KATIE_QT_TEXTELIDEMODE_IDX])
        return SBK_MODULE_INIT_ERROR;

    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_TEXTELIDEMODE_IDX],
        module, "ElideLeft", (long) Katie::Qt::TextElideMode::ElideLeft))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_TEXTELIDEMODE_IDX],
        module, "ElideRight", (long) Katie::Qt::TextElideMode::ElideRight))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_TEXTELIDEMODE_IDX],
        module, "ElideMiddle", (long) Katie::Qt::TextElideMode::ElideMiddle))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_TEXTELIDEMODE_IDX],
        module, "ElideNone", (long) Katie::Qt::TextElideMode::ElideNone))
        return SBK_MODULE_INIT_ERROR;
    // Register converter for enum 'Katie::Qt::TextElideMode'.
    {
        SbkConverter* converter = Shiboken::Conversions::createConverter(SbkKtCoreTypes[SBK_KATIE_QT_TEXTELIDEMODE_IDX],
            Katie_Qt_TextElideMode_CppToPython_Katie_Qt_TextElideMode);
        Shiboken::Conversions::addPythonToCppValueConversion(converter,
            Katie_Qt_TextElideMode_PythonToCpp_Katie_Qt_TextElideMode,
            is_Katie_Qt_TextElideMode_PythonToCpp_Katie_Qt_TextElideMode_Convertible);
        Shiboken::Enum::setTypeConverter(SbkKtCoreTypes[SBK_KATIE_QT_TEXTELIDEMODE_IDX], converter);
        Shiboken::Conversions::registerConverterName(converter, "Katie::Qt::TextElideMode");
        Shiboken::Conversions::registerConverterName(converter, "Qt::TextElideMode");
        Shiboken::Conversions::registerConverterName(converter, "TextElideMode");
    }
    // End of 'TextElideMode' enum.

    // Initialization of enum 'WindowType'.
    SbkKtCoreTypes[SBK_KATIE_QT_WINDOWTYPE_IDX] = Shiboken::Enum::createGlobalEnum(module,
        "WindowType",
        "KtCore.Katie.Qt.WindowType",
        "Katie::Qt::WindowType");
    if (!SbkKtCoreTypes[SBK_KATIE_QT_WINDOWTYPE_IDX])
        return SBK_MODULE_INIT_ERROR;

    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WINDOWTYPE_IDX],
        module, "Widget", (long) Katie::Qt::WindowType::Widget))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WINDOWTYPE_IDX],
        module, "Window", (long) Katie::Qt::WindowType::Window))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WINDOWTYPE_IDX],
        module, "Dialog", (long) Katie::Qt::WindowType::Dialog))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WINDOWTYPE_IDX],
        module, "Sheet", (long) Katie::Qt::WindowType::Sheet))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WINDOWTYPE_IDX],
        module, "Drawer", (long) Katie::Qt::WindowType::Drawer))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WINDOWTYPE_IDX],
        module, "Popup", (long) Katie::Qt::WindowType::Popup))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WINDOWTYPE_IDX],
        module, "Tool", (long) Katie::Qt::WindowType::Tool))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WINDOWTYPE_IDX],
        module, "ToolTip", (long) Katie::Qt::WindowType::ToolTip))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WINDOWTYPE_IDX],
        module, "SplashScreen", (long) Katie::Qt::WindowType::SplashScreen))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WINDOWTYPE_IDX],
        module, "Desktop", (long) Katie::Qt::WindowType::Desktop))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WINDOWTYPE_IDX],
        module, "SubWindow", (long) Katie::Qt::WindowType::SubWindow))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WINDOWTYPE_IDX],
        module, "WindowType_Mask", (long) Katie::Qt::WindowType::WindowType_Mask))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WINDOWTYPE_IDX],
        module, "X11BypassWindowManagerHint", (long) Katie::Qt::WindowType::X11BypassWindowManagerHint))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WINDOWTYPE_IDX],
        module, "FramelessWindowHint", (long) Katie::Qt::WindowType::FramelessWindowHint))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WINDOWTYPE_IDX],
        module, "WindowTitleHint", (long) Katie::Qt::WindowType::WindowTitleHint))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WINDOWTYPE_IDX],
        module, "WindowSystemMenuHint", (long) Katie::Qt::WindowType::WindowSystemMenuHint))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WINDOWTYPE_IDX],
        module, "WindowMinimizeButtonHint", (long) Katie::Qt::WindowType::WindowMinimizeButtonHint))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WINDOWTYPE_IDX],
        module, "WindowMaximizeButtonHint", (long) Katie::Qt::WindowType::WindowMaximizeButtonHint))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WINDOWTYPE_IDX],
        module, "WindowMinMaxButtonsHint", (long) Katie::Qt::WindowType::WindowMinMaxButtonsHint))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WINDOWTYPE_IDX],
        module, "WindowContextHelpButtonHint", (long) Katie::Qt::WindowType::WindowContextHelpButtonHint))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WINDOWTYPE_IDX],
        module, "WindowShadeButtonHint", (long) Katie::Qt::WindowType::WindowShadeButtonHint))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WINDOWTYPE_IDX],
        module, "WindowStaysOnTopHint", (long) Katie::Qt::WindowType::WindowStaysOnTopHint))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WINDOWTYPE_IDX],
        module, "CustomizeWindowHint", (long) Katie::Qt::WindowType::CustomizeWindowHint))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WINDOWTYPE_IDX],
        module, "WindowStaysOnBottomHint", (long) Katie::Qt::WindowType::WindowStaysOnBottomHint))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WINDOWTYPE_IDX],
        module, "WindowCloseButtonHint", (long) Katie::Qt::WindowType::WindowCloseButtonHint))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WINDOWTYPE_IDX],
        module, "BypassGraphicsProxyWidget", (long) Katie::Qt::WindowType::BypassGraphicsProxyWidget))
        return SBK_MODULE_INIT_ERROR;
    // Register converter for enum 'Katie::Qt::WindowType'.
    {
        SbkConverter* converter = Shiboken::Conversions::createConverter(SbkKtCoreTypes[SBK_KATIE_QT_WINDOWTYPE_IDX],
            Katie_Qt_WindowType_CppToPython_Katie_Qt_WindowType);
        Shiboken::Conversions::addPythonToCppValueConversion(converter,
            Katie_Qt_WindowType_PythonToCpp_Katie_Qt_WindowType,
            is_Katie_Qt_WindowType_PythonToCpp_Katie_Qt_WindowType_Convertible);
        Shiboken::Enum::setTypeConverter(SbkKtCoreTypes[SBK_KATIE_QT_WINDOWTYPE_IDX], converter);
        Shiboken::Conversions::registerConverterName(converter, "Katie::Qt::WindowType");
        Shiboken::Conversions::registerConverterName(converter, "Qt::WindowType");
        Shiboken::Conversions::registerConverterName(converter, "WindowType");
    }
    // End of 'WindowType' enum.

    // Initialization of enum 'WindowState'.
    SbkKtCoreTypes[SBK_KATIE_QT_WINDOWSTATE_IDX] = Shiboken::Enum::createGlobalEnum(module,
        "WindowState",
        "KtCore.Katie.Qt.WindowState",
        "Katie::Qt::WindowState");
    if (!SbkKtCoreTypes[SBK_KATIE_QT_WINDOWSTATE_IDX])
        return SBK_MODULE_INIT_ERROR;

    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WINDOWSTATE_IDX],
        module, "WindowNoState", (long) Katie::Qt::WindowState::WindowNoState))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WINDOWSTATE_IDX],
        module, "WindowMinimized", (long) Katie::Qt::WindowState::WindowMinimized))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WINDOWSTATE_IDX],
        module, "WindowMaximized", (long) Katie::Qt::WindowState::WindowMaximized))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WINDOWSTATE_IDX],
        module, "WindowFullScreen", (long) Katie::Qt::WindowState::WindowFullScreen))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WINDOWSTATE_IDX],
        module, "WindowActive", (long) Katie::Qt::WindowState::WindowActive))
        return SBK_MODULE_INIT_ERROR;
    // Register converter for enum 'Katie::Qt::WindowState'.
    {
        SbkConverter* converter = Shiboken::Conversions::createConverter(SbkKtCoreTypes[SBK_KATIE_QT_WINDOWSTATE_IDX],
            Katie_Qt_WindowState_CppToPython_Katie_Qt_WindowState);
        Shiboken::Conversions::addPythonToCppValueConversion(converter,
            Katie_Qt_WindowState_PythonToCpp_Katie_Qt_WindowState,
            is_Katie_Qt_WindowState_PythonToCpp_Katie_Qt_WindowState_Convertible);
        Shiboken::Enum::setTypeConverter(SbkKtCoreTypes[SBK_KATIE_QT_WINDOWSTATE_IDX], converter);
        Shiboken::Conversions::registerConverterName(converter, "Katie::Qt::WindowState");
        Shiboken::Conversions::registerConverterName(converter, "Qt::WindowState");
        Shiboken::Conversions::registerConverterName(converter, "WindowState");
    }
    // End of 'WindowState' enum.

    // Initialization of enum 'WidgetAttribute'.
    SbkKtCoreTypes[SBK_KATIE_QT_WIDGETATTRIBUTE_IDX] = Shiboken::Enum::createGlobalEnum(module,
        "WidgetAttribute",
        "KtCore.Katie.Qt.WidgetAttribute",
        "Katie::Qt::WidgetAttribute");
    if (!SbkKtCoreTypes[SBK_KATIE_QT_WIDGETATTRIBUTE_IDX])
        return SBK_MODULE_INIT_ERROR;

    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WIDGETATTRIBUTE_IDX],
        module, "WA_Disabled", (long) Katie::Qt::WidgetAttribute::WA_Disabled))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WIDGETATTRIBUTE_IDX],
        module, "WA_UnderMouse", (long) Katie::Qt::WidgetAttribute::WA_UnderMouse))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WIDGETATTRIBUTE_IDX],
        module, "WA_MouseTracking", (long) Katie::Qt::WidgetAttribute::WA_MouseTracking))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WIDGETATTRIBUTE_IDX],
        module, "WA_ContentsPropagated", (long) Katie::Qt::WidgetAttribute::WA_ContentsPropagated))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WIDGETATTRIBUTE_IDX],
        module, "WA_OpaquePaintEvent", (long) Katie::Qt::WidgetAttribute::WA_OpaquePaintEvent))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WIDGETATTRIBUTE_IDX],
        module, "WA_NoBackground", (long) Katie::Qt::WidgetAttribute::WA_NoBackground))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WIDGETATTRIBUTE_IDX],
        module, "WA_StaticContents", (long) Katie::Qt::WidgetAttribute::WA_StaticContents))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WIDGETATTRIBUTE_IDX],
        module, "WA_LaidOut", (long) Katie::Qt::WidgetAttribute::WA_LaidOut))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WIDGETATTRIBUTE_IDX],
        module, "WA_PaintOnScreen", (long) Katie::Qt::WidgetAttribute::WA_PaintOnScreen))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WIDGETATTRIBUTE_IDX],
        module, "WA_NoSystemBackground", (long) Katie::Qt::WidgetAttribute::WA_NoSystemBackground))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WIDGETATTRIBUTE_IDX],
        module, "WA_UpdatesDisabled", (long) Katie::Qt::WidgetAttribute::WA_UpdatesDisabled))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WIDGETATTRIBUTE_IDX],
        module, "WA_Mapped", (long) Katie::Qt::WidgetAttribute::WA_Mapped))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WIDGETATTRIBUTE_IDX],
        module, "WA_PaintOutsidePaintEvent", (long) Katie::Qt::WidgetAttribute::WA_PaintOutsidePaintEvent))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WIDGETATTRIBUTE_IDX],
        module, "WA_WState_Visible", (long) Katie::Qt::WidgetAttribute::WA_WState_Visible))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WIDGETATTRIBUTE_IDX],
        module, "WA_WState_Hidden", (long) Katie::Qt::WidgetAttribute::WA_WState_Hidden))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WIDGETATTRIBUTE_IDX],
        module, "WA_ForceDisabled", (long) Katie::Qt::WidgetAttribute::WA_ForceDisabled))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WIDGETATTRIBUTE_IDX],
        module, "WA_PendingMoveEvent", (long) Katie::Qt::WidgetAttribute::WA_PendingMoveEvent))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WIDGETATTRIBUTE_IDX],
        module, "WA_PendingResizeEvent", (long) Katie::Qt::WidgetAttribute::WA_PendingResizeEvent))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WIDGETATTRIBUTE_IDX],
        module, "WA_SetPalette", (long) Katie::Qt::WidgetAttribute::WA_SetPalette))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WIDGETATTRIBUTE_IDX],
        module, "WA_SetFont", (long) Katie::Qt::WidgetAttribute::WA_SetFont))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WIDGETATTRIBUTE_IDX],
        module, "WA_SetCursor", (long) Katie::Qt::WidgetAttribute::WA_SetCursor))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WIDGETATTRIBUTE_IDX],
        module, "WA_NoChildEventsFromChildren", (long) Katie::Qt::WidgetAttribute::WA_NoChildEventsFromChildren))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WIDGETATTRIBUTE_IDX],
        module, "WA_WindowModified", (long) Katie::Qt::WidgetAttribute::WA_WindowModified))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WIDGETATTRIBUTE_IDX],
        module, "WA_Resized", (long) Katie::Qt::WidgetAttribute::WA_Resized))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WIDGETATTRIBUTE_IDX],
        module, "WA_Moved", (long) Katie::Qt::WidgetAttribute::WA_Moved))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WIDGETATTRIBUTE_IDX],
        module, "WA_PendingUpdate", (long) Katie::Qt::WidgetAttribute::WA_PendingUpdate))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WIDGETATTRIBUTE_IDX],
        module, "WA_InvalidSize", (long) Katie::Qt::WidgetAttribute::WA_InvalidSize))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WIDGETATTRIBUTE_IDX],
        module, "WA_CustomWhatsThis", (long) Katie::Qt::WidgetAttribute::WA_CustomWhatsThis))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WIDGETATTRIBUTE_IDX],
        module, "WA_LayoutOnEntireRect", (long) Katie::Qt::WidgetAttribute::WA_LayoutOnEntireRect))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WIDGETATTRIBUTE_IDX],
        module, "WA_OutsideWSRange", (long) Katie::Qt::WidgetAttribute::WA_OutsideWSRange))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WIDGETATTRIBUTE_IDX],
        module, "WA_GrabbedShortcut", (long) Katie::Qt::WidgetAttribute::WA_GrabbedShortcut))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WIDGETATTRIBUTE_IDX],
        module, "WA_TransparentForMouseEvents", (long) Katie::Qt::WidgetAttribute::WA_TransparentForMouseEvents))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WIDGETATTRIBUTE_IDX],
        module, "WA_PaintUnclipped", (long) Katie::Qt::WidgetAttribute::WA_PaintUnclipped))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WIDGETATTRIBUTE_IDX],
        module, "WA_SetWindowIcon", (long) Katie::Qt::WidgetAttribute::WA_SetWindowIcon))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WIDGETATTRIBUTE_IDX],
        module, "WA_NoMouseReplay", (long) Katie::Qt::WidgetAttribute::WA_NoMouseReplay))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WIDGETATTRIBUTE_IDX],
        module, "WA_DeleteOnClose", (long) Katie::Qt::WidgetAttribute::WA_DeleteOnClose))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WIDGETATTRIBUTE_IDX],
        module, "WA_RightToLeft", (long) Katie::Qt::WidgetAttribute::WA_RightToLeft))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WIDGETATTRIBUTE_IDX],
        module, "WA_SetLayoutDirection", (long) Katie::Qt::WidgetAttribute::WA_SetLayoutDirection))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WIDGETATTRIBUTE_IDX],
        module, "WA_NoChildEventsForParent", (long) Katie::Qt::WidgetAttribute::WA_NoChildEventsForParent))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WIDGETATTRIBUTE_IDX],
        module, "WA_ForceUpdatesDisabled", (long) Katie::Qt::WidgetAttribute::WA_ForceUpdatesDisabled))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WIDGETATTRIBUTE_IDX],
        module, "WA_WState_Created", (long) Katie::Qt::WidgetAttribute::WA_WState_Created))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WIDGETATTRIBUTE_IDX],
        module, "WA_WState_CompressKeys", (long) Katie::Qt::WidgetAttribute::WA_WState_CompressKeys))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WIDGETATTRIBUTE_IDX],
        module, "WA_WState_InPaintEvent", (long) Katie::Qt::WidgetAttribute::WA_WState_InPaintEvent))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WIDGETATTRIBUTE_IDX],
        module, "WA_WState_Reparented", (long) Katie::Qt::WidgetAttribute::WA_WState_Reparented))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WIDGETATTRIBUTE_IDX],
        module, "WA_WState_ConfigPending", (long) Katie::Qt::WidgetAttribute::WA_WState_ConfigPending))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WIDGETATTRIBUTE_IDX],
        module, "WA_WState_Polished", (long) Katie::Qt::WidgetAttribute::WA_WState_Polished))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WIDGETATTRIBUTE_IDX],
        module, "WA_WState_DND", (long) Katie::Qt::WidgetAttribute::WA_WState_DND))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WIDGETATTRIBUTE_IDX],
        module, "WA_WState_OwnSizePolicy", (long) Katie::Qt::WidgetAttribute::WA_WState_OwnSizePolicy))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WIDGETATTRIBUTE_IDX],
        module, "WA_WState_ExplicitShowHide", (long) Katie::Qt::WidgetAttribute::WA_WState_ExplicitShowHide))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WIDGETATTRIBUTE_IDX],
        module, "WA_ShowModal", (long) Katie::Qt::WidgetAttribute::WA_ShowModal))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WIDGETATTRIBUTE_IDX],
        module, "WA_MouseNoMask", (long) Katie::Qt::WidgetAttribute::WA_MouseNoMask))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WIDGETATTRIBUTE_IDX],
        module, "WA_GroupLeader", (long) Katie::Qt::WidgetAttribute::WA_GroupLeader))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WIDGETATTRIBUTE_IDX],
        module, "WA_NoMousePropagation", (long) Katie::Qt::WidgetAttribute::WA_NoMousePropagation))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WIDGETATTRIBUTE_IDX],
        module, "WA_Hover", (long) Katie::Qt::WidgetAttribute::WA_Hover))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WIDGETATTRIBUTE_IDX],
        module, "WA_QuitOnClose", (long) Katie::Qt::WidgetAttribute::WA_QuitOnClose))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WIDGETATTRIBUTE_IDX],
        module, "WA_KeyboardFocusChange", (long) Katie::Qt::WidgetAttribute::WA_KeyboardFocusChange))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WIDGETATTRIBUTE_IDX],
        module, "WA_AcceptDrops", (long) Katie::Qt::WidgetAttribute::WA_AcceptDrops))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WIDGETATTRIBUTE_IDX],
        module, "WA_DropSiteRegistered", (long) Katie::Qt::WidgetAttribute::WA_DropSiteRegistered))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WIDGETATTRIBUTE_IDX],
        module, "WA_WindowPropagation", (long) Katie::Qt::WidgetAttribute::WA_WindowPropagation))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WIDGETATTRIBUTE_IDX],
        module, "WA_TintedBackground", (long) Katie::Qt::WidgetAttribute::WA_TintedBackground))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WIDGETATTRIBUTE_IDX],
        module, "WA_AlwaysShowToolTips", (long) Katie::Qt::WidgetAttribute::WA_AlwaysShowToolTips))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WIDGETATTRIBUTE_IDX],
        module, "WA_SetStyle", (long) Katie::Qt::WidgetAttribute::WA_SetStyle))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WIDGETATTRIBUTE_IDX],
        module, "WA_SetLocale", (long) Katie::Qt::WidgetAttribute::WA_SetLocale))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WIDGETATTRIBUTE_IDX],
        module, "WA_LayoutUsesWidgetRect", (long) Katie::Qt::WidgetAttribute::WA_LayoutUsesWidgetRect))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WIDGETATTRIBUTE_IDX],
        module, "WA_StyledBackground", (long) Katie::Qt::WidgetAttribute::WA_StyledBackground))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WIDGETATTRIBUTE_IDX],
        module, "WA_CanHostQMdiSubWindowTitleBar", (long) Katie::Qt::WidgetAttribute::WA_CanHostQMdiSubWindowTitleBar))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WIDGETATTRIBUTE_IDX],
        module, "WA_StyleSheet", (long) Katie::Qt::WidgetAttribute::WA_StyleSheet))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WIDGETATTRIBUTE_IDX],
        module, "WA_ShowWithoutActivating", (long) Katie::Qt::WidgetAttribute::WA_ShowWithoutActivating))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WIDGETATTRIBUTE_IDX],
        module, "WA_X11BypassTransientForHint", (long) Katie::Qt::WidgetAttribute::WA_X11BypassTransientForHint))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WIDGETATTRIBUTE_IDX],
        module, "WA_NativeWindow", (long) Katie::Qt::WidgetAttribute::WA_NativeWindow))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WIDGETATTRIBUTE_IDX],
        module, "WA_DontCreateNativeAncestors", (long) Katie::Qt::WidgetAttribute::WA_DontCreateNativeAncestors))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WIDGETATTRIBUTE_IDX],
        module, "WA_DontShowOnScreen", (long) Katie::Qt::WidgetAttribute::WA_DontShowOnScreen))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WIDGETATTRIBUTE_IDX],
        module, "WA_X11NetWmWindowTypeDesktop", (long) Katie::Qt::WidgetAttribute::WA_X11NetWmWindowTypeDesktop))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WIDGETATTRIBUTE_IDX],
        module, "WA_X11NetWmWindowTypeDock", (long) Katie::Qt::WidgetAttribute::WA_X11NetWmWindowTypeDock))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WIDGETATTRIBUTE_IDX],
        module, "WA_X11NetWmWindowTypeToolBar", (long) Katie::Qt::WidgetAttribute::WA_X11NetWmWindowTypeToolBar))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WIDGETATTRIBUTE_IDX],
        module, "WA_X11NetWmWindowTypeMenu", (long) Katie::Qt::WidgetAttribute::WA_X11NetWmWindowTypeMenu))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WIDGETATTRIBUTE_IDX],
        module, "WA_X11NetWmWindowTypeUtility", (long) Katie::Qt::WidgetAttribute::WA_X11NetWmWindowTypeUtility))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WIDGETATTRIBUTE_IDX],
        module, "WA_X11NetWmWindowTypeSplash", (long) Katie::Qt::WidgetAttribute::WA_X11NetWmWindowTypeSplash))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WIDGETATTRIBUTE_IDX],
        module, "WA_X11NetWmWindowTypeDialog", (long) Katie::Qt::WidgetAttribute::WA_X11NetWmWindowTypeDialog))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WIDGETATTRIBUTE_IDX],
        module, "WA_X11NetWmWindowTypeDropDownMenu", (long) Katie::Qt::WidgetAttribute::WA_X11NetWmWindowTypeDropDownMenu))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WIDGETATTRIBUTE_IDX],
        module, "WA_X11NetWmWindowTypePopupMenu", (long) Katie::Qt::WidgetAttribute::WA_X11NetWmWindowTypePopupMenu))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WIDGETATTRIBUTE_IDX],
        module, "WA_X11NetWmWindowTypeToolTip", (long) Katie::Qt::WidgetAttribute::WA_X11NetWmWindowTypeToolTip))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WIDGETATTRIBUTE_IDX],
        module, "WA_X11NetWmWindowTypeNotification", (long) Katie::Qt::WidgetAttribute::WA_X11NetWmWindowTypeNotification))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WIDGETATTRIBUTE_IDX],
        module, "WA_X11NetWmWindowTypeCombo", (long) Katie::Qt::WidgetAttribute::WA_X11NetWmWindowTypeCombo))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WIDGETATTRIBUTE_IDX],
        module, "WA_X11NetWmWindowTypeDND", (long) Katie::Qt::WidgetAttribute::WA_X11NetWmWindowTypeDND))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WIDGETATTRIBUTE_IDX],
        module, "WA_SetWindowModality", (long) Katie::Qt::WidgetAttribute::WA_SetWindowModality))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WIDGETATTRIBUTE_IDX],
        module, "WA_WState_WindowOpacitySet", (long) Katie::Qt::WidgetAttribute::WA_WState_WindowOpacitySet))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WIDGETATTRIBUTE_IDX],
        module, "WA_TranslucentBackground", (long) Katie::Qt::WidgetAttribute::WA_TranslucentBackground))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WIDGETATTRIBUTE_IDX],
        module, "WA_AcceptTouchEvents", (long) Katie::Qt::WidgetAttribute::WA_AcceptTouchEvents))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WIDGETATTRIBUTE_IDX],
        module, "WA_WState_AcceptedTouchBeginEvent", (long) Katie::Qt::WidgetAttribute::WA_WState_AcceptedTouchBeginEvent))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WIDGETATTRIBUTE_IDX],
        module, "WA_TouchPadAcceptSingleTouchEvents", (long) Katie::Qt::WidgetAttribute::WA_TouchPadAcceptSingleTouchEvents))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WIDGETATTRIBUTE_IDX],
        module, "WA_LockPortraitOrientation", (long) Katie::Qt::WidgetAttribute::WA_LockPortraitOrientation))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WIDGETATTRIBUTE_IDX],
        module, "WA_LockLandscapeOrientation", (long) Katie::Qt::WidgetAttribute::WA_LockLandscapeOrientation))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WIDGETATTRIBUTE_IDX],
        module, "WA_AutoOrientation", (long) Katie::Qt::WidgetAttribute::WA_AutoOrientation))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WIDGETATTRIBUTE_IDX],
        module, "WA_X11DoNotAcceptFocus", (long) Katie::Qt::WidgetAttribute::WA_X11DoNotAcceptFocus))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WIDGETATTRIBUTE_IDX],
        module, "WA_AttributeCount", (long) Katie::Qt::WidgetAttribute::WA_AttributeCount))
        return SBK_MODULE_INIT_ERROR;
    // Register converter for enum 'Katie::Qt::WidgetAttribute'.
    {
        SbkConverter* converter = Shiboken::Conversions::createConverter(SbkKtCoreTypes[SBK_KATIE_QT_WIDGETATTRIBUTE_IDX],
            Katie_Qt_WidgetAttribute_CppToPython_Katie_Qt_WidgetAttribute);
        Shiboken::Conversions::addPythonToCppValueConversion(converter,
            Katie_Qt_WidgetAttribute_PythonToCpp_Katie_Qt_WidgetAttribute,
            is_Katie_Qt_WidgetAttribute_PythonToCpp_Katie_Qt_WidgetAttribute_Convertible);
        Shiboken::Enum::setTypeConverter(SbkKtCoreTypes[SBK_KATIE_QT_WIDGETATTRIBUTE_IDX], converter);
        Shiboken::Conversions::registerConverterName(converter, "Katie::Qt::WidgetAttribute");
        Shiboken::Conversions::registerConverterName(converter, "Qt::WidgetAttribute");
        Shiboken::Conversions::registerConverterName(converter, "WidgetAttribute");
    }
    // End of 'WidgetAttribute' enum.

    // Initialization of enum 'ApplicationAttribute'.
    SbkKtCoreTypes[SBK_KATIE_QT_APPLICATIONATTRIBUTE_IDX] = Shiboken::Enum::createGlobalEnum(module,
        "ApplicationAttribute",
        "KtCore.Katie.Qt.ApplicationAttribute",
        "Katie::Qt::ApplicationAttribute");
    if (!SbkKtCoreTypes[SBK_KATIE_QT_APPLICATIONATTRIBUTE_IDX])
        return SBK_MODULE_INIT_ERROR;

    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_APPLICATIONATTRIBUTE_IDX],
        module, "AA_ImmediateWidgetCreation", (long) Katie::Qt::ApplicationAttribute::AA_ImmediateWidgetCreation))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_APPLICATIONATTRIBUTE_IDX],
        module, "AA_DontShowIconsInMenus", (long) Katie::Qt::ApplicationAttribute::AA_DontShowIconsInMenus))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_APPLICATIONATTRIBUTE_IDX],
        module, "AA_NativeWindows", (long) Katie::Qt::ApplicationAttribute::AA_NativeWindows))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_APPLICATIONATTRIBUTE_IDX],
        module, "AA_DontCreateNativeWidgetSiblings", (long) Katie::Qt::ApplicationAttribute::AA_DontCreateNativeWidgetSiblings))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_APPLICATIONATTRIBUTE_IDX],
        module, "AA_X11InitThreads", (long) Katie::Qt::ApplicationAttribute::AA_X11InitThreads))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_APPLICATIONATTRIBUTE_IDX],
        module, "AA_CaptureMultimediaKeys", (long) Katie::Qt::ApplicationAttribute::AA_CaptureMultimediaKeys))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_APPLICATIONATTRIBUTE_IDX],
        module, "AA_AttributeCount", (long) Katie::Qt::ApplicationAttribute::AA_AttributeCount))
        return SBK_MODULE_INIT_ERROR;
    // Register converter for enum 'Katie::Qt::ApplicationAttribute'.
    {
        SbkConverter* converter = Shiboken::Conversions::createConverter(SbkKtCoreTypes[SBK_KATIE_QT_APPLICATIONATTRIBUTE_IDX],
            Katie_Qt_ApplicationAttribute_CppToPython_Katie_Qt_ApplicationAttribute);
        Shiboken::Conversions::addPythonToCppValueConversion(converter,
            Katie_Qt_ApplicationAttribute_PythonToCpp_Katie_Qt_ApplicationAttribute,
            is_Katie_Qt_ApplicationAttribute_PythonToCpp_Katie_Qt_ApplicationAttribute_Convertible);
        Shiboken::Enum::setTypeConverter(SbkKtCoreTypes[SBK_KATIE_QT_APPLICATIONATTRIBUTE_IDX], converter);
        Shiboken::Conversions::registerConverterName(converter, "Katie::Qt::ApplicationAttribute");
        Shiboken::Conversions::registerConverterName(converter, "Qt::ApplicationAttribute");
        Shiboken::Conversions::registerConverterName(converter, "ApplicationAttribute");
    }
    // End of 'ApplicationAttribute' enum.

    // Initialization of enum 'ImageConversionFlag'.
    SbkKtCoreTypes[SBK_KATIE_QT_IMAGECONVERSIONFLAG_IDX] = Shiboken::Enum::createGlobalEnum(module,
        "ImageConversionFlag",
        "KtCore.Katie.Qt.ImageConversionFlag",
        "Katie::Qt::ImageConversionFlag");
    if (!SbkKtCoreTypes[SBK_KATIE_QT_IMAGECONVERSIONFLAG_IDX])
        return SBK_MODULE_INIT_ERROR;

    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_IMAGECONVERSIONFLAG_IDX],
        module, "ColorMode_Mask", (long) Katie::Qt::ImageConversionFlag::ColorMode_Mask))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_IMAGECONVERSIONFLAG_IDX],
        module, "AutoColor", (long) Katie::Qt::ImageConversionFlag::AutoColor))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_IMAGECONVERSIONFLAG_IDX],
        module, "ColorOnly", (long) Katie::Qt::ImageConversionFlag::ColorOnly))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_IMAGECONVERSIONFLAG_IDX],
        module, "MonoOnly", (long) Katie::Qt::ImageConversionFlag::MonoOnly))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_IMAGECONVERSIONFLAG_IDX],
        module, "AlphaDither_Mask", (long) Katie::Qt::ImageConversionFlag::AlphaDither_Mask))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_IMAGECONVERSIONFLAG_IDX],
        module, "ThresholdAlphaDither", (long) Katie::Qt::ImageConversionFlag::ThresholdAlphaDither))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_IMAGECONVERSIONFLAG_IDX],
        module, "OrderedAlphaDither", (long) Katie::Qt::ImageConversionFlag::OrderedAlphaDither))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_IMAGECONVERSIONFLAG_IDX],
        module, "DiffuseAlphaDither", (long) Katie::Qt::ImageConversionFlag::DiffuseAlphaDither))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_IMAGECONVERSIONFLAG_IDX],
        module, "NoAlpha", (long) Katie::Qt::ImageConversionFlag::NoAlpha))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_IMAGECONVERSIONFLAG_IDX],
        module, "Dither_Mask", (long) Katie::Qt::ImageConversionFlag::Dither_Mask))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_IMAGECONVERSIONFLAG_IDX],
        module, "DiffuseDither", (long) Katie::Qt::ImageConversionFlag::DiffuseDither))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_IMAGECONVERSIONFLAG_IDX],
        module, "OrderedDither", (long) Katie::Qt::ImageConversionFlag::OrderedDither))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_IMAGECONVERSIONFLAG_IDX],
        module, "ThresholdDither", (long) Katie::Qt::ImageConversionFlag::ThresholdDither))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_IMAGECONVERSIONFLAG_IDX],
        module, "DitherMode_Mask", (long) Katie::Qt::ImageConversionFlag::DitherMode_Mask))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_IMAGECONVERSIONFLAG_IDX],
        module, "AutoDither", (long) Katie::Qt::ImageConversionFlag::AutoDither))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_IMAGECONVERSIONFLAG_IDX],
        module, "PreferDither", (long) Katie::Qt::ImageConversionFlag::PreferDither))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_IMAGECONVERSIONFLAG_IDX],
        module, "AvoidDither", (long) Katie::Qt::ImageConversionFlag::AvoidDither))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_IMAGECONVERSIONFLAG_IDX],
        module, "NoOpaqueDetection", (long) Katie::Qt::ImageConversionFlag::NoOpaqueDetection))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_IMAGECONVERSIONFLAG_IDX],
        module, "NoFormatConversion", (long) Katie::Qt::ImageConversionFlag::NoFormatConversion))
        return SBK_MODULE_INIT_ERROR;
    // Register converter for enum 'Katie::Qt::ImageConversionFlag'.
    {
        SbkConverter* converter = Shiboken::Conversions::createConverter(SbkKtCoreTypes[SBK_KATIE_QT_IMAGECONVERSIONFLAG_IDX],
            Katie_Qt_ImageConversionFlag_CppToPython_Katie_Qt_ImageConversionFlag);
        Shiboken::Conversions::addPythonToCppValueConversion(converter,
            Katie_Qt_ImageConversionFlag_PythonToCpp_Katie_Qt_ImageConversionFlag,
            is_Katie_Qt_ImageConversionFlag_PythonToCpp_Katie_Qt_ImageConversionFlag_Convertible);
        Shiboken::Enum::setTypeConverter(SbkKtCoreTypes[SBK_KATIE_QT_IMAGECONVERSIONFLAG_IDX], converter);
        Shiboken::Conversions::registerConverterName(converter, "Katie::Qt::ImageConversionFlag");
        Shiboken::Conversions::registerConverterName(converter, "Qt::ImageConversionFlag");
        Shiboken::Conversions::registerConverterName(converter, "ImageConversionFlag");
    }
    // End of 'ImageConversionFlag' enum.

    // Initialization of enum 'BGMode'.
    SbkKtCoreTypes[SBK_KATIE_QT_BGMODE_IDX] = Shiboken::Enum::createGlobalEnum(module,
        "BGMode",
        "KtCore.Katie.Qt.BGMode",
        "Katie::Qt::BGMode");
    if (!SbkKtCoreTypes[SBK_KATIE_QT_BGMODE_IDX])
        return SBK_MODULE_INIT_ERROR;

    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_BGMODE_IDX],
        module, "TransparentMode", (long) Katie::Qt::BGMode::TransparentMode))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_BGMODE_IDX],
        module, "OpaqueMode", (long) Katie::Qt::BGMode::OpaqueMode))
        return SBK_MODULE_INIT_ERROR;
    // Register converter for enum 'Katie::Qt::BGMode'.
    {
        SbkConverter* converter = Shiboken::Conversions::createConverter(SbkKtCoreTypes[SBK_KATIE_QT_BGMODE_IDX],
            Katie_Qt_BGMode_CppToPython_Katie_Qt_BGMode);
        Shiboken::Conversions::addPythonToCppValueConversion(converter,
            Katie_Qt_BGMode_PythonToCpp_Katie_Qt_BGMode,
            is_Katie_Qt_BGMode_PythonToCpp_Katie_Qt_BGMode_Convertible);
        Shiboken::Enum::setTypeConverter(SbkKtCoreTypes[SBK_KATIE_QT_BGMODE_IDX], converter);
        Shiboken::Conversions::registerConverterName(converter, "Katie::Qt::BGMode");
        Shiboken::Conversions::registerConverterName(converter, "Qt::BGMode");
        Shiboken::Conversions::registerConverterName(converter, "BGMode");
    }
    // End of 'BGMode' enum.

    // Initialization of enum 'Key'.
    SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX] = Shiboken::Enum::createGlobalEnum(module,
        "Key",
        "KtCore.Katie.Qt.Key",
        "Katie::Qt::Key");
    if (!SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX])
        return SBK_MODULE_INIT_ERROR;

    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Escape", (long) Katie::Qt::Key::Key_Escape))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Tab", (long) Katie::Qt::Key::Key_Tab))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Backtab", (long) Katie::Qt::Key::Key_Backtab))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Backspace", (long) Katie::Qt::Key::Key_Backspace))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Return", (long) Katie::Qt::Key::Key_Return))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Enter", (long) Katie::Qt::Key::Key_Enter))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Insert", (long) Katie::Qt::Key::Key_Insert))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Delete", (long) Katie::Qt::Key::Key_Delete))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Pause", (long) Katie::Qt::Key::Key_Pause))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Print", (long) Katie::Qt::Key::Key_Print))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_SysReq", (long) Katie::Qt::Key::Key_SysReq))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Clear", (long) Katie::Qt::Key::Key_Clear))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Home", (long) Katie::Qt::Key::Key_Home))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_End", (long) Katie::Qt::Key::Key_End))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Left", (long) Katie::Qt::Key::Key_Left))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Up", (long) Katie::Qt::Key::Key_Up))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Right", (long) Katie::Qt::Key::Key_Right))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Down", (long) Katie::Qt::Key::Key_Down))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_PageUp", (long) Katie::Qt::Key::Key_PageUp))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_PageDown", (long) Katie::Qt::Key::Key_PageDown))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Shift", (long) Katie::Qt::Key::Key_Shift))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Control", (long) Katie::Qt::Key::Key_Control))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Meta", (long) Katie::Qt::Key::Key_Meta))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Alt", (long) Katie::Qt::Key::Key_Alt))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_CapsLock", (long) Katie::Qt::Key::Key_CapsLock))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_NumLock", (long) Katie::Qt::Key::Key_NumLock))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_ScrollLock", (long) Katie::Qt::Key::Key_ScrollLock))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_F1", (long) Katie::Qt::Key::Key_F1))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_F2", (long) Katie::Qt::Key::Key_F2))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_F3", (long) Katie::Qt::Key::Key_F3))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_F4", (long) Katie::Qt::Key::Key_F4))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_F5", (long) Katie::Qt::Key::Key_F5))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_F6", (long) Katie::Qt::Key::Key_F6))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_F7", (long) Katie::Qt::Key::Key_F7))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_F8", (long) Katie::Qt::Key::Key_F8))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_F9", (long) Katie::Qt::Key::Key_F9))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_F10", (long) Katie::Qt::Key::Key_F10))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_F11", (long) Katie::Qt::Key::Key_F11))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_F12", (long) Katie::Qt::Key::Key_F12))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_F13", (long) Katie::Qt::Key::Key_F13))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_F14", (long) Katie::Qt::Key::Key_F14))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_F15", (long) Katie::Qt::Key::Key_F15))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_F16", (long) Katie::Qt::Key::Key_F16))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_F17", (long) Katie::Qt::Key::Key_F17))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_F18", (long) Katie::Qt::Key::Key_F18))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_F19", (long) Katie::Qt::Key::Key_F19))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_F20", (long) Katie::Qt::Key::Key_F20))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_F21", (long) Katie::Qt::Key::Key_F21))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_F22", (long) Katie::Qt::Key::Key_F22))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_F23", (long) Katie::Qt::Key::Key_F23))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_F24", (long) Katie::Qt::Key::Key_F24))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_F25", (long) Katie::Qt::Key::Key_F25))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_F26", (long) Katie::Qt::Key::Key_F26))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_F27", (long) Katie::Qt::Key::Key_F27))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_F28", (long) Katie::Qt::Key::Key_F28))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_F29", (long) Katie::Qt::Key::Key_F29))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_F30", (long) Katie::Qt::Key::Key_F30))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_F31", (long) Katie::Qt::Key::Key_F31))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_F32", (long) Katie::Qt::Key::Key_F32))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_F33", (long) Katie::Qt::Key::Key_F33))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_F34", (long) Katie::Qt::Key::Key_F34))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_F35", (long) Katie::Qt::Key::Key_F35))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Super_L", (long) Katie::Qt::Key::Key_Super_L))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Super_R", (long) Katie::Qt::Key::Key_Super_R))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Menu", (long) Katie::Qt::Key::Key_Menu))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Hyper_L", (long) Katie::Qt::Key::Key_Hyper_L))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Hyper_R", (long) Katie::Qt::Key::Key_Hyper_R))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Help", (long) Katie::Qt::Key::Key_Help))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Direction_L", (long) Katie::Qt::Key::Key_Direction_L))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Direction_R", (long) Katie::Qt::Key::Key_Direction_R))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Space", (long) Katie::Qt::Key::Key_Space))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Any", (long) Katie::Qt::Key::Key_Any))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Exclam", (long) Katie::Qt::Key::Key_Exclam))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_QuoteDbl", (long) Katie::Qt::Key::Key_QuoteDbl))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_NumberSign", (long) Katie::Qt::Key::Key_NumberSign))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Dollar", (long) Katie::Qt::Key::Key_Dollar))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Percent", (long) Katie::Qt::Key::Key_Percent))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Ampersand", (long) Katie::Qt::Key::Key_Ampersand))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Apostrophe", (long) Katie::Qt::Key::Key_Apostrophe))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_ParenLeft", (long) Katie::Qt::Key::Key_ParenLeft))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_ParenRight", (long) Katie::Qt::Key::Key_ParenRight))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Asterisk", (long) Katie::Qt::Key::Key_Asterisk))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Plus", (long) Katie::Qt::Key::Key_Plus))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Comma", (long) Katie::Qt::Key::Key_Comma))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Minus", (long) Katie::Qt::Key::Key_Minus))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Period", (long) Katie::Qt::Key::Key_Period))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Slash", (long) Katie::Qt::Key::Key_Slash))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_0", (long) Katie::Qt::Key::Key_0))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_1", (long) Katie::Qt::Key::Key_1))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_2", (long) Katie::Qt::Key::Key_2))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_3", (long) Katie::Qt::Key::Key_3))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_4", (long) Katie::Qt::Key::Key_4))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_5", (long) Katie::Qt::Key::Key_5))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_6", (long) Katie::Qt::Key::Key_6))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_7", (long) Katie::Qt::Key::Key_7))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_8", (long) Katie::Qt::Key::Key_8))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_9", (long) Katie::Qt::Key::Key_9))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Colon", (long) Katie::Qt::Key::Key_Colon))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Semicolon", (long) Katie::Qt::Key::Key_Semicolon))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Less", (long) Katie::Qt::Key::Key_Less))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Equal", (long) Katie::Qt::Key::Key_Equal))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Greater", (long) Katie::Qt::Key::Key_Greater))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Question", (long) Katie::Qt::Key::Key_Question))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_At", (long) Katie::Qt::Key::Key_At))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_A", (long) Katie::Qt::Key::Key_A))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_B", (long) Katie::Qt::Key::Key_B))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_C", (long) Katie::Qt::Key::Key_C))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_D", (long) Katie::Qt::Key::Key_D))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_E", (long) Katie::Qt::Key::Key_E))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_F", (long) Katie::Qt::Key::Key_F))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_G", (long) Katie::Qt::Key::Key_G))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_H", (long) Katie::Qt::Key::Key_H))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_I", (long) Katie::Qt::Key::Key_I))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_J", (long) Katie::Qt::Key::Key_J))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_K", (long) Katie::Qt::Key::Key_K))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_L", (long) Katie::Qt::Key::Key_L))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_M", (long) Katie::Qt::Key::Key_M))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_N", (long) Katie::Qt::Key::Key_N))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_O", (long) Katie::Qt::Key::Key_O))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_P", (long) Katie::Qt::Key::Key_P))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Q", (long) Katie::Qt::Key::Key_Q))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_R", (long) Katie::Qt::Key::Key_R))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_S", (long) Katie::Qt::Key::Key_S))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_T", (long) Katie::Qt::Key::Key_T))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_U", (long) Katie::Qt::Key::Key_U))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_V", (long) Katie::Qt::Key::Key_V))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_W", (long) Katie::Qt::Key::Key_W))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_X", (long) Katie::Qt::Key::Key_X))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Y", (long) Katie::Qt::Key::Key_Y))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Z", (long) Katie::Qt::Key::Key_Z))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_BracketLeft", (long) Katie::Qt::Key::Key_BracketLeft))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Backslash", (long) Katie::Qt::Key::Key_Backslash))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_BracketRight", (long) Katie::Qt::Key::Key_BracketRight))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_AsciiCircum", (long) Katie::Qt::Key::Key_AsciiCircum))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Underscore", (long) Katie::Qt::Key::Key_Underscore))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_QuoteLeft", (long) Katie::Qt::Key::Key_QuoteLeft))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_BraceLeft", (long) Katie::Qt::Key::Key_BraceLeft))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Bar", (long) Katie::Qt::Key::Key_Bar))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_BraceRight", (long) Katie::Qt::Key::Key_BraceRight))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_AsciiTilde", (long) Katie::Qt::Key::Key_AsciiTilde))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_nobreakspace", (long) Katie::Qt::Key::Key_nobreakspace))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_exclamdown", (long) Katie::Qt::Key::Key_exclamdown))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_cent", (long) Katie::Qt::Key::Key_cent))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_sterling", (long) Katie::Qt::Key::Key_sterling))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_currency", (long) Katie::Qt::Key::Key_currency))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_yen", (long) Katie::Qt::Key::Key_yen))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_brokenbar", (long) Katie::Qt::Key::Key_brokenbar))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_section", (long) Katie::Qt::Key::Key_section))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_diaeresis", (long) Katie::Qt::Key::Key_diaeresis))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_copyright", (long) Katie::Qt::Key::Key_copyright))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_ordfeminine", (long) Katie::Qt::Key::Key_ordfeminine))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_guillemotleft", (long) Katie::Qt::Key::Key_guillemotleft))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_notsign", (long) Katie::Qt::Key::Key_notsign))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_hyphen", (long) Katie::Qt::Key::Key_hyphen))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_registered", (long) Katie::Qt::Key::Key_registered))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_macron", (long) Katie::Qt::Key::Key_macron))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_degree", (long) Katie::Qt::Key::Key_degree))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_plusminus", (long) Katie::Qt::Key::Key_plusminus))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_twosuperior", (long) Katie::Qt::Key::Key_twosuperior))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_threesuperior", (long) Katie::Qt::Key::Key_threesuperior))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_acute", (long) Katie::Qt::Key::Key_acute))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_mu", (long) Katie::Qt::Key::Key_mu))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_paragraph", (long) Katie::Qt::Key::Key_paragraph))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_periodcentered", (long) Katie::Qt::Key::Key_periodcentered))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_cedilla", (long) Katie::Qt::Key::Key_cedilla))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_onesuperior", (long) Katie::Qt::Key::Key_onesuperior))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_masculine", (long) Katie::Qt::Key::Key_masculine))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_guillemotright", (long) Katie::Qt::Key::Key_guillemotright))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_onequarter", (long) Katie::Qt::Key::Key_onequarter))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_onehalf", (long) Katie::Qt::Key::Key_onehalf))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_threequarters", (long) Katie::Qt::Key::Key_threequarters))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_questiondown", (long) Katie::Qt::Key::Key_questiondown))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Agrave", (long) Katie::Qt::Key::Key_Agrave))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Aacute", (long) Katie::Qt::Key::Key_Aacute))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Acircumflex", (long) Katie::Qt::Key::Key_Acircumflex))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Atilde", (long) Katie::Qt::Key::Key_Atilde))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Adiaeresis", (long) Katie::Qt::Key::Key_Adiaeresis))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Aring", (long) Katie::Qt::Key::Key_Aring))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_AE", (long) Katie::Qt::Key::Key_AE))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Ccedilla", (long) Katie::Qt::Key::Key_Ccedilla))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Egrave", (long) Katie::Qt::Key::Key_Egrave))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Eacute", (long) Katie::Qt::Key::Key_Eacute))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Ecircumflex", (long) Katie::Qt::Key::Key_Ecircumflex))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Ediaeresis", (long) Katie::Qt::Key::Key_Ediaeresis))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Igrave", (long) Katie::Qt::Key::Key_Igrave))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Iacute", (long) Katie::Qt::Key::Key_Iacute))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Icircumflex", (long) Katie::Qt::Key::Key_Icircumflex))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Idiaeresis", (long) Katie::Qt::Key::Key_Idiaeresis))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_ETH", (long) Katie::Qt::Key::Key_ETH))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Ntilde", (long) Katie::Qt::Key::Key_Ntilde))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Ograve", (long) Katie::Qt::Key::Key_Ograve))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Oacute", (long) Katie::Qt::Key::Key_Oacute))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Ocircumflex", (long) Katie::Qt::Key::Key_Ocircumflex))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Otilde", (long) Katie::Qt::Key::Key_Otilde))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Odiaeresis", (long) Katie::Qt::Key::Key_Odiaeresis))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_multiply", (long) Katie::Qt::Key::Key_multiply))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Ooblique", (long) Katie::Qt::Key::Key_Ooblique))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Ugrave", (long) Katie::Qt::Key::Key_Ugrave))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Uacute", (long) Katie::Qt::Key::Key_Uacute))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Ucircumflex", (long) Katie::Qt::Key::Key_Ucircumflex))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Udiaeresis", (long) Katie::Qt::Key::Key_Udiaeresis))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Yacute", (long) Katie::Qt::Key::Key_Yacute))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_THORN", (long) Katie::Qt::Key::Key_THORN))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_ssharp", (long) Katie::Qt::Key::Key_ssharp))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_division", (long) Katie::Qt::Key::Key_division))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_ydiaeresis", (long) Katie::Qt::Key::Key_ydiaeresis))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_AltGr", (long) Katie::Qt::Key::Key_AltGr))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Multi_key", (long) Katie::Qt::Key::Key_Multi_key))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Codeinput", (long) Katie::Qt::Key::Key_Codeinput))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_SingleCandidate", (long) Katie::Qt::Key::Key_SingleCandidate))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_MultipleCandidate", (long) Katie::Qt::Key::Key_MultipleCandidate))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_PreviousCandidate", (long) Katie::Qt::Key::Key_PreviousCandidate))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Mode_switch", (long) Katie::Qt::Key::Key_Mode_switch))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Kanji", (long) Katie::Qt::Key::Key_Kanji))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Muhenkan", (long) Katie::Qt::Key::Key_Muhenkan))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Henkan", (long) Katie::Qt::Key::Key_Henkan))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Romaji", (long) Katie::Qt::Key::Key_Romaji))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Hiragana", (long) Katie::Qt::Key::Key_Hiragana))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Katakana", (long) Katie::Qt::Key::Key_Katakana))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Hiragana_Katakana", (long) Katie::Qt::Key::Key_Hiragana_Katakana))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Zenkaku", (long) Katie::Qt::Key::Key_Zenkaku))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Hankaku", (long) Katie::Qt::Key::Key_Hankaku))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Zenkaku_Hankaku", (long) Katie::Qt::Key::Key_Zenkaku_Hankaku))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Touroku", (long) Katie::Qt::Key::Key_Touroku))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Massyo", (long) Katie::Qt::Key::Key_Massyo))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Kana_Lock", (long) Katie::Qt::Key::Key_Kana_Lock))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Kana_Shift", (long) Katie::Qt::Key::Key_Kana_Shift))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Eisu_Shift", (long) Katie::Qt::Key::Key_Eisu_Shift))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Eisu_toggle", (long) Katie::Qt::Key::Key_Eisu_toggle))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Hangul", (long) Katie::Qt::Key::Key_Hangul))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Hangul_Start", (long) Katie::Qt::Key::Key_Hangul_Start))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Hangul_End", (long) Katie::Qt::Key::Key_Hangul_End))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Hangul_Hanja", (long) Katie::Qt::Key::Key_Hangul_Hanja))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Hangul_Jamo", (long) Katie::Qt::Key::Key_Hangul_Jamo))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Hangul_Romaja", (long) Katie::Qt::Key::Key_Hangul_Romaja))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Hangul_Jeonja", (long) Katie::Qt::Key::Key_Hangul_Jeonja))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Hangul_Banja", (long) Katie::Qt::Key::Key_Hangul_Banja))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Hangul_PreHanja", (long) Katie::Qt::Key::Key_Hangul_PreHanja))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Hangul_PostHanja", (long) Katie::Qt::Key::Key_Hangul_PostHanja))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Hangul_Special", (long) Katie::Qt::Key::Key_Hangul_Special))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Dead_Grave", (long) Katie::Qt::Key::Key_Dead_Grave))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Dead_Acute", (long) Katie::Qt::Key::Key_Dead_Acute))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Dead_Circumflex", (long) Katie::Qt::Key::Key_Dead_Circumflex))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Dead_Tilde", (long) Katie::Qt::Key::Key_Dead_Tilde))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Dead_Macron", (long) Katie::Qt::Key::Key_Dead_Macron))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Dead_Breve", (long) Katie::Qt::Key::Key_Dead_Breve))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Dead_Abovedot", (long) Katie::Qt::Key::Key_Dead_Abovedot))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Dead_Diaeresis", (long) Katie::Qt::Key::Key_Dead_Diaeresis))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Dead_Abovering", (long) Katie::Qt::Key::Key_Dead_Abovering))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Dead_Doubleacute", (long) Katie::Qt::Key::Key_Dead_Doubleacute))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Dead_Caron", (long) Katie::Qt::Key::Key_Dead_Caron))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Dead_Cedilla", (long) Katie::Qt::Key::Key_Dead_Cedilla))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Dead_Ogonek", (long) Katie::Qt::Key::Key_Dead_Ogonek))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Dead_Iota", (long) Katie::Qt::Key::Key_Dead_Iota))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Dead_Voiced_Sound", (long) Katie::Qt::Key::Key_Dead_Voiced_Sound))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Dead_Semivoiced_Sound", (long) Katie::Qt::Key::Key_Dead_Semivoiced_Sound))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Dead_Belowdot", (long) Katie::Qt::Key::Key_Dead_Belowdot))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Dead_Hook", (long) Katie::Qt::Key::Key_Dead_Hook))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Dead_Horn", (long) Katie::Qt::Key::Key_Dead_Horn))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Back", (long) Katie::Qt::Key::Key_Back))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Forward", (long) Katie::Qt::Key::Key_Forward))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Stop", (long) Katie::Qt::Key::Key_Stop))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Refresh", (long) Katie::Qt::Key::Key_Refresh))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_VolumeDown", (long) Katie::Qt::Key::Key_VolumeDown))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_VolumeMute", (long) Katie::Qt::Key::Key_VolumeMute))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_VolumeUp", (long) Katie::Qt::Key::Key_VolumeUp))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_BassBoost", (long) Katie::Qt::Key::Key_BassBoost))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_BassUp", (long) Katie::Qt::Key::Key_BassUp))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_BassDown", (long) Katie::Qt::Key::Key_BassDown))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_TrebleUp", (long) Katie::Qt::Key::Key_TrebleUp))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_TrebleDown", (long) Katie::Qt::Key::Key_TrebleDown))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_MediaPlay", (long) Katie::Qt::Key::Key_MediaPlay))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_MediaStop", (long) Katie::Qt::Key::Key_MediaStop))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_MediaPrevious", (long) Katie::Qt::Key::Key_MediaPrevious))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_MediaNext", (long) Katie::Qt::Key::Key_MediaNext))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_MediaRecord", (long) Katie::Qt::Key::Key_MediaRecord))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_MediaPause", (long) Katie::Qt::Key::Key_MediaPause))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_MediaTogglePlayPause", (long) Katie::Qt::Key::Key_MediaTogglePlayPause))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_HomePage", (long) Katie::Qt::Key::Key_HomePage))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Favorites", (long) Katie::Qt::Key::Key_Favorites))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Search", (long) Katie::Qt::Key::Key_Search))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Standby", (long) Katie::Qt::Key::Key_Standby))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_OpenUrl", (long) Katie::Qt::Key::Key_OpenUrl))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_LaunchMail", (long) Katie::Qt::Key::Key_LaunchMail))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_LaunchMedia", (long) Katie::Qt::Key::Key_LaunchMedia))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Launch0", (long) Katie::Qt::Key::Key_Launch0))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Launch1", (long) Katie::Qt::Key::Key_Launch1))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Launch2", (long) Katie::Qt::Key::Key_Launch2))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Launch3", (long) Katie::Qt::Key::Key_Launch3))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Launch4", (long) Katie::Qt::Key::Key_Launch4))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Launch5", (long) Katie::Qt::Key::Key_Launch5))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Launch6", (long) Katie::Qt::Key::Key_Launch6))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Launch7", (long) Katie::Qt::Key::Key_Launch7))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Launch8", (long) Katie::Qt::Key::Key_Launch8))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Launch9", (long) Katie::Qt::Key::Key_Launch9))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_LaunchA", (long) Katie::Qt::Key::Key_LaunchA))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_LaunchB", (long) Katie::Qt::Key::Key_LaunchB))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_LaunchC", (long) Katie::Qt::Key::Key_LaunchC))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_LaunchD", (long) Katie::Qt::Key::Key_LaunchD))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_LaunchE", (long) Katie::Qt::Key::Key_LaunchE))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_LaunchF", (long) Katie::Qt::Key::Key_LaunchF))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_MonBrightnessUp", (long) Katie::Qt::Key::Key_MonBrightnessUp))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_MonBrightnessDown", (long) Katie::Qt::Key::Key_MonBrightnessDown))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_KeyboardLightOnOff", (long) Katie::Qt::Key::Key_KeyboardLightOnOff))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_KeyboardBrightnessUp", (long) Katie::Qt::Key::Key_KeyboardBrightnessUp))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_KeyboardBrightnessDown", (long) Katie::Qt::Key::Key_KeyboardBrightnessDown))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_PowerOff", (long) Katie::Qt::Key::Key_PowerOff))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_WakeUp", (long) Katie::Qt::Key::Key_WakeUp))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Eject", (long) Katie::Qt::Key::Key_Eject))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_ScreenSaver", (long) Katie::Qt::Key::Key_ScreenSaver))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_WWW", (long) Katie::Qt::Key::Key_WWW))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Memo", (long) Katie::Qt::Key::Key_Memo))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_LightBulb", (long) Katie::Qt::Key::Key_LightBulb))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Shop", (long) Katie::Qt::Key::Key_Shop))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_History", (long) Katie::Qt::Key::Key_History))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_AddFavorite", (long) Katie::Qt::Key::Key_AddFavorite))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_HotLinks", (long) Katie::Qt::Key::Key_HotLinks))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_BrightnessAdjust", (long) Katie::Qt::Key::Key_BrightnessAdjust))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Finance", (long) Katie::Qt::Key::Key_Finance))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Community", (long) Katie::Qt::Key::Key_Community))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_AudioRewind", (long) Katie::Qt::Key::Key_AudioRewind))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_BackForward", (long) Katie::Qt::Key::Key_BackForward))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_ApplicationLeft", (long) Katie::Qt::Key::Key_ApplicationLeft))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_ApplicationRight", (long) Katie::Qt::Key::Key_ApplicationRight))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Book", (long) Katie::Qt::Key::Key_Book))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_CD", (long) Katie::Qt::Key::Key_CD))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Calculator", (long) Katie::Qt::Key::Key_Calculator))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_ToDoList", (long) Katie::Qt::Key::Key_ToDoList))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_ClearGrab", (long) Katie::Qt::Key::Key_ClearGrab))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Close", (long) Katie::Qt::Key::Key_Close))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Copy", (long) Katie::Qt::Key::Key_Copy))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Cut", (long) Katie::Qt::Key::Key_Cut))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Display", (long) Katie::Qt::Key::Key_Display))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_DOS", (long) Katie::Qt::Key::Key_DOS))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Documents", (long) Katie::Qt::Key::Key_Documents))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Excel", (long) Katie::Qt::Key::Key_Excel))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Explorer", (long) Katie::Qt::Key::Key_Explorer))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Game", (long) Katie::Qt::Key::Key_Game))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Go", (long) Katie::Qt::Key::Key_Go))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_iTouch", (long) Katie::Qt::Key::Key_iTouch))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_LogOff", (long) Katie::Qt::Key::Key_LogOff))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Market", (long) Katie::Qt::Key::Key_Market))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Meeting", (long) Katie::Qt::Key::Key_Meeting))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_MenuKB", (long) Katie::Qt::Key::Key_MenuKB))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_MenuPB", (long) Katie::Qt::Key::Key_MenuPB))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_MySites", (long) Katie::Qt::Key::Key_MySites))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_News", (long) Katie::Qt::Key::Key_News))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_OfficeHome", (long) Katie::Qt::Key::Key_OfficeHome))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Option", (long) Katie::Qt::Key::Key_Option))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Paste", (long) Katie::Qt::Key::Key_Paste))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Phone", (long) Katie::Qt::Key::Key_Phone))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Calendar", (long) Katie::Qt::Key::Key_Calendar))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Reply", (long) Katie::Qt::Key::Key_Reply))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Reload", (long) Katie::Qt::Key::Key_Reload))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_RotateWindows", (long) Katie::Qt::Key::Key_RotateWindows))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_RotationPB", (long) Katie::Qt::Key::Key_RotationPB))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_RotationKB", (long) Katie::Qt::Key::Key_RotationKB))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Save", (long) Katie::Qt::Key::Key_Save))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Send", (long) Katie::Qt::Key::Key_Send))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Spell", (long) Katie::Qt::Key::Key_Spell))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_SplitScreen", (long) Katie::Qt::Key::Key_SplitScreen))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Support", (long) Katie::Qt::Key::Key_Support))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_TaskPane", (long) Katie::Qt::Key::Key_TaskPane))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Terminal", (long) Katie::Qt::Key::Key_Terminal))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Tools", (long) Katie::Qt::Key::Key_Tools))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Travel", (long) Katie::Qt::Key::Key_Travel))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Video", (long) Katie::Qt::Key::Key_Video))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Word", (long) Katie::Qt::Key::Key_Word))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Xfer", (long) Katie::Qt::Key::Key_Xfer))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_ZoomIn", (long) Katie::Qt::Key::Key_ZoomIn))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_ZoomOut", (long) Katie::Qt::Key::Key_ZoomOut))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Away", (long) Katie::Qt::Key::Key_Away))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Messenger", (long) Katie::Qt::Key::Key_Messenger))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_WebCam", (long) Katie::Qt::Key::Key_WebCam))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_MailForward", (long) Katie::Qt::Key::Key_MailForward))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Pictures", (long) Katie::Qt::Key::Key_Pictures))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Music", (long) Katie::Qt::Key::Key_Music))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Battery", (long) Katie::Qt::Key::Key_Battery))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Bluetooth", (long) Katie::Qt::Key::Key_Bluetooth))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_WLAN", (long) Katie::Qt::Key::Key_WLAN))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_UWB", (long) Katie::Qt::Key::Key_UWB))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_AudioForward", (long) Katie::Qt::Key::Key_AudioForward))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_AudioRepeat", (long) Katie::Qt::Key::Key_AudioRepeat))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_AudioRandomPlay", (long) Katie::Qt::Key::Key_AudioRandomPlay))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Subtitle", (long) Katie::Qt::Key::Key_Subtitle))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_AudioCycleTrack", (long) Katie::Qt::Key::Key_AudioCycleTrack))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Time", (long) Katie::Qt::Key::Key_Time))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Hibernate", (long) Katie::Qt::Key::Key_Hibernate))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_View", (long) Katie::Qt::Key::Key_View))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_TopMenu", (long) Katie::Qt::Key::Key_TopMenu))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_PowerDown", (long) Katie::Qt::Key::Key_PowerDown))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Suspend", (long) Katie::Qt::Key::Key_Suspend))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_ContrastAdjust", (long) Katie::Qt::Key::Key_ContrastAdjust))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_LaunchG", (long) Katie::Qt::Key::Key_LaunchG))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_LaunchH", (long) Katie::Qt::Key::Key_LaunchH))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_MediaLast", (long) Katie::Qt::Key::Key_MediaLast))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Select", (long) Katie::Qt::Key::Key_Select))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Yes", (long) Katie::Qt::Key::Key_Yes))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_No", (long) Katie::Qt::Key::Key_No))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Cancel", (long) Katie::Qt::Key::Key_Cancel))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Printer", (long) Katie::Qt::Key::Key_Printer))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Execute", (long) Katie::Qt::Key::Key_Execute))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Sleep", (long) Katie::Qt::Key::Key_Sleep))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Play", (long) Katie::Qt::Key::Key_Play))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Zoom", (long) Katie::Qt::Key::Key_Zoom))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Context1", (long) Katie::Qt::Key::Key_Context1))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Context2", (long) Katie::Qt::Key::Key_Context2))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Context3", (long) Katie::Qt::Key::Key_Context3))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Context4", (long) Katie::Qt::Key::Key_Context4))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Call", (long) Katie::Qt::Key::Key_Call))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Hangup", (long) Katie::Qt::Key::Key_Hangup))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Flip", (long) Katie::Qt::Key::Key_Flip))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_ToggleCallHangup", (long) Katie::Qt::Key::Key_ToggleCallHangup))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_VoiceDial", (long) Katie::Qt::Key::Key_VoiceDial))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_LastNumberRedial", (long) Katie::Qt::Key::Key_LastNumberRedial))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_Camera", (long) Katie::Qt::Key::Key_Camera))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_CameraFocus", (long) Katie::Qt::Key::Key_CameraFocus))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
        module, "Key_unknown", (long) Katie::Qt::Key::Key_unknown))
        return SBK_MODULE_INIT_ERROR;
    // Register converter for enum 'Katie::Qt::Key'.
    {
        SbkConverter* converter = Shiboken::Conversions::createConverter(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX],
            Katie_Qt_Key_CppToPython_Katie_Qt_Key);
        Shiboken::Conversions::addPythonToCppValueConversion(converter,
            Katie_Qt_Key_PythonToCpp_Katie_Qt_Key,
            is_Katie_Qt_Key_PythonToCpp_Katie_Qt_Key_Convertible);
        Shiboken::Enum::setTypeConverter(SbkKtCoreTypes[SBK_KATIE_QT_KEY_IDX], converter);
        Shiboken::Conversions::registerConverterName(converter, "Katie::Qt::Key");
        Shiboken::Conversions::registerConverterName(converter, "Qt::Key");
        Shiboken::Conversions::registerConverterName(converter, "Key");
    }
    // End of 'Key' enum.

    // Initialization of enum 'ArrowType'.
    SbkKtCoreTypes[SBK_KATIE_QT_ARROWTYPE_IDX] = Shiboken::Enum::createGlobalEnum(module,
        "ArrowType",
        "KtCore.Katie.Qt.ArrowType",
        "Katie::Qt::ArrowType");
    if (!SbkKtCoreTypes[SBK_KATIE_QT_ARROWTYPE_IDX])
        return SBK_MODULE_INIT_ERROR;

    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_ARROWTYPE_IDX],
        module, "NoArrow", (long) Katie::Qt::ArrowType::NoArrow))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_ARROWTYPE_IDX],
        module, "UpArrow", (long) Katie::Qt::ArrowType::UpArrow))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_ARROWTYPE_IDX],
        module, "DownArrow", (long) Katie::Qt::ArrowType::DownArrow))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_ARROWTYPE_IDX],
        module, "LeftArrow", (long) Katie::Qt::ArrowType::LeftArrow))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_ARROWTYPE_IDX],
        module, "RightArrow", (long) Katie::Qt::ArrowType::RightArrow))
        return SBK_MODULE_INIT_ERROR;
    // Register converter for enum 'Katie::Qt::ArrowType'.
    {
        SbkConverter* converter = Shiboken::Conversions::createConverter(SbkKtCoreTypes[SBK_KATIE_QT_ARROWTYPE_IDX],
            Katie_Qt_ArrowType_CppToPython_Katie_Qt_ArrowType);
        Shiboken::Conversions::addPythonToCppValueConversion(converter,
            Katie_Qt_ArrowType_PythonToCpp_Katie_Qt_ArrowType,
            is_Katie_Qt_ArrowType_PythonToCpp_Katie_Qt_ArrowType_Convertible);
        Shiboken::Enum::setTypeConverter(SbkKtCoreTypes[SBK_KATIE_QT_ARROWTYPE_IDX], converter);
        Shiboken::Conversions::registerConverterName(converter, "Katie::Qt::ArrowType");
        Shiboken::Conversions::registerConverterName(converter, "Qt::ArrowType");
        Shiboken::Conversions::registerConverterName(converter, "ArrowType");
    }
    // End of 'ArrowType' enum.

    // Initialization of enum 'PenStyle'.
    SbkKtCoreTypes[SBK_KATIE_QT_PENSTYLE_IDX] = Shiboken::Enum::createGlobalEnum(module,
        "PenStyle",
        "KtCore.Katie.Qt.PenStyle",
        "Katie::Qt::PenStyle");
    if (!SbkKtCoreTypes[SBK_KATIE_QT_PENSTYLE_IDX])
        return SBK_MODULE_INIT_ERROR;

    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_PENSTYLE_IDX],
        module, "NoPen", (long) Katie::Qt::PenStyle::NoPen))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_PENSTYLE_IDX],
        module, "SolidLine", (long) Katie::Qt::PenStyle::SolidLine))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_PENSTYLE_IDX],
        module, "DashLine", (long) Katie::Qt::PenStyle::DashLine))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_PENSTYLE_IDX],
        module, "DotLine", (long) Katie::Qt::PenStyle::DotLine))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_PENSTYLE_IDX],
        module, "DashDotLine", (long) Katie::Qt::PenStyle::DashDotLine))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_PENSTYLE_IDX],
        module, "DashDotDotLine", (long) Katie::Qt::PenStyle::DashDotDotLine))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_PENSTYLE_IDX],
        module, "CustomDashLine", (long) Katie::Qt::PenStyle::CustomDashLine))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_PENSTYLE_IDX],
        module, "MPenStyle", (long) Katie::Qt::PenStyle::MPenStyle))
        return SBK_MODULE_INIT_ERROR;
    // Register converter for enum 'Katie::Qt::PenStyle'.
    {
        SbkConverter* converter = Shiboken::Conversions::createConverter(SbkKtCoreTypes[SBK_KATIE_QT_PENSTYLE_IDX],
            Katie_Qt_PenStyle_CppToPython_Katie_Qt_PenStyle);
        Shiboken::Conversions::addPythonToCppValueConversion(converter,
            Katie_Qt_PenStyle_PythonToCpp_Katie_Qt_PenStyle,
            is_Katie_Qt_PenStyle_PythonToCpp_Katie_Qt_PenStyle_Convertible);
        Shiboken::Enum::setTypeConverter(SbkKtCoreTypes[SBK_KATIE_QT_PENSTYLE_IDX], converter);
        Shiboken::Conversions::registerConverterName(converter, "Katie::Qt::PenStyle");
        Shiboken::Conversions::registerConverterName(converter, "Qt::PenStyle");
        Shiboken::Conversions::registerConverterName(converter, "PenStyle");
    }
    // End of 'PenStyle' enum.

    // Initialization of enum 'PenCapStyle'.
    SbkKtCoreTypes[SBK_KATIE_QT_PENCAPSTYLE_IDX] = Shiboken::Enum::createGlobalEnum(module,
        "PenCapStyle",
        "KtCore.Katie.Qt.PenCapStyle",
        "Katie::Qt::PenCapStyle");
    if (!SbkKtCoreTypes[SBK_KATIE_QT_PENCAPSTYLE_IDX])
        return SBK_MODULE_INIT_ERROR;

    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_PENCAPSTYLE_IDX],
        module, "FlatCap", (long) Katie::Qt::PenCapStyle::FlatCap))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_PENCAPSTYLE_IDX],
        module, "SquareCap", (long) Katie::Qt::PenCapStyle::SquareCap))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_PENCAPSTYLE_IDX],
        module, "RoundCap", (long) Katie::Qt::PenCapStyle::RoundCap))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_PENCAPSTYLE_IDX],
        module, "MPenCapStyle", (long) Katie::Qt::PenCapStyle::MPenCapStyle))
        return SBK_MODULE_INIT_ERROR;
    // Register converter for enum 'Katie::Qt::PenCapStyle'.
    {
        SbkConverter* converter = Shiboken::Conversions::createConverter(SbkKtCoreTypes[SBK_KATIE_QT_PENCAPSTYLE_IDX],
            Katie_Qt_PenCapStyle_CppToPython_Katie_Qt_PenCapStyle);
        Shiboken::Conversions::addPythonToCppValueConversion(converter,
            Katie_Qt_PenCapStyle_PythonToCpp_Katie_Qt_PenCapStyle,
            is_Katie_Qt_PenCapStyle_PythonToCpp_Katie_Qt_PenCapStyle_Convertible);
        Shiboken::Enum::setTypeConverter(SbkKtCoreTypes[SBK_KATIE_QT_PENCAPSTYLE_IDX], converter);
        Shiboken::Conversions::registerConverterName(converter, "Katie::Qt::PenCapStyle");
        Shiboken::Conversions::registerConverterName(converter, "Qt::PenCapStyle");
        Shiboken::Conversions::registerConverterName(converter, "PenCapStyle");
    }
    // End of 'PenCapStyle' enum.

    // Initialization of enum 'PenJoinStyle'.
    SbkKtCoreTypes[SBK_KATIE_QT_PENJOINSTYLE_IDX] = Shiboken::Enum::createGlobalEnum(module,
        "PenJoinStyle",
        "KtCore.Katie.Qt.PenJoinStyle",
        "Katie::Qt::PenJoinStyle");
    if (!SbkKtCoreTypes[SBK_KATIE_QT_PENJOINSTYLE_IDX])
        return SBK_MODULE_INIT_ERROR;

    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_PENJOINSTYLE_IDX],
        module, "MiterJoin", (long) Katie::Qt::PenJoinStyle::MiterJoin))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_PENJOINSTYLE_IDX],
        module, "BevelJoin", (long) Katie::Qt::PenJoinStyle::BevelJoin))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_PENJOINSTYLE_IDX],
        module, "RoundJoin", (long) Katie::Qt::PenJoinStyle::RoundJoin))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_PENJOINSTYLE_IDX],
        module, "SvgMiterJoin", (long) Katie::Qt::PenJoinStyle::SvgMiterJoin))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_PENJOINSTYLE_IDX],
        module, "MPenJoinStyle", (long) Katie::Qt::PenJoinStyle::MPenJoinStyle))
        return SBK_MODULE_INIT_ERROR;
    // Register converter for enum 'Katie::Qt::PenJoinStyle'.
    {
        SbkConverter* converter = Shiboken::Conversions::createConverter(SbkKtCoreTypes[SBK_KATIE_QT_PENJOINSTYLE_IDX],
            Katie_Qt_PenJoinStyle_CppToPython_Katie_Qt_PenJoinStyle);
        Shiboken::Conversions::addPythonToCppValueConversion(converter,
            Katie_Qt_PenJoinStyle_PythonToCpp_Katie_Qt_PenJoinStyle,
            is_Katie_Qt_PenJoinStyle_PythonToCpp_Katie_Qt_PenJoinStyle_Convertible);
        Shiboken::Enum::setTypeConverter(SbkKtCoreTypes[SBK_KATIE_QT_PENJOINSTYLE_IDX], converter);
        Shiboken::Conversions::registerConverterName(converter, "Katie::Qt::PenJoinStyle");
        Shiboken::Conversions::registerConverterName(converter, "Qt::PenJoinStyle");
        Shiboken::Conversions::registerConverterName(converter, "PenJoinStyle");
    }
    // End of 'PenJoinStyle' enum.

    // Initialization of enum 'BrushStyle'.
    SbkKtCoreTypes[SBK_KATIE_QT_BRUSHSTYLE_IDX] = Shiboken::Enum::createGlobalEnum(module,
        "BrushStyle",
        "KtCore.Katie.Qt.BrushStyle",
        "Katie::Qt::BrushStyle");
    if (!SbkKtCoreTypes[SBK_KATIE_QT_BRUSHSTYLE_IDX])
        return SBK_MODULE_INIT_ERROR;

    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_BRUSHSTYLE_IDX],
        module, "NoBrush", (long) Katie::Qt::BrushStyle::NoBrush))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_BRUSHSTYLE_IDX],
        module, "SolidPattern", (long) Katie::Qt::BrushStyle::SolidPattern))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_BRUSHSTYLE_IDX],
        module, "Dense1Pattern", (long) Katie::Qt::BrushStyle::Dense1Pattern))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_BRUSHSTYLE_IDX],
        module, "Dense2Pattern", (long) Katie::Qt::BrushStyle::Dense2Pattern))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_BRUSHSTYLE_IDX],
        module, "Dense3Pattern", (long) Katie::Qt::BrushStyle::Dense3Pattern))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_BRUSHSTYLE_IDX],
        module, "Dense4Pattern", (long) Katie::Qt::BrushStyle::Dense4Pattern))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_BRUSHSTYLE_IDX],
        module, "Dense5Pattern", (long) Katie::Qt::BrushStyle::Dense5Pattern))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_BRUSHSTYLE_IDX],
        module, "Dense6Pattern", (long) Katie::Qt::BrushStyle::Dense6Pattern))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_BRUSHSTYLE_IDX],
        module, "Dense7Pattern", (long) Katie::Qt::BrushStyle::Dense7Pattern))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_BRUSHSTYLE_IDX],
        module, "HorPattern", (long) Katie::Qt::BrushStyle::HorPattern))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_BRUSHSTYLE_IDX],
        module, "VerPattern", (long) Katie::Qt::BrushStyle::VerPattern))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_BRUSHSTYLE_IDX],
        module, "CrossPattern", (long) Katie::Qt::BrushStyle::CrossPattern))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_BRUSHSTYLE_IDX],
        module, "BDiagPattern", (long) Katie::Qt::BrushStyle::BDiagPattern))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_BRUSHSTYLE_IDX],
        module, "FDiagPattern", (long) Katie::Qt::BrushStyle::FDiagPattern))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_BRUSHSTYLE_IDX],
        module, "DiagCrossPattern", (long) Katie::Qt::BrushStyle::DiagCrossPattern))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_BRUSHSTYLE_IDX],
        module, "LinearGradientPattern", (long) Katie::Qt::BrushStyle::LinearGradientPattern))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_BRUSHSTYLE_IDX],
        module, "RadialGradientPattern", (long) Katie::Qt::BrushStyle::RadialGradientPattern))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_BRUSHSTYLE_IDX],
        module, "ConicalGradientPattern", (long) Katie::Qt::BrushStyle::ConicalGradientPattern))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_BRUSHSTYLE_IDX],
        module, "TexturePattern", (long) Katie::Qt::BrushStyle::TexturePattern))
        return SBK_MODULE_INIT_ERROR;
    // Register converter for enum 'Katie::Qt::BrushStyle'.
    {
        SbkConverter* converter = Shiboken::Conversions::createConverter(SbkKtCoreTypes[SBK_KATIE_QT_BRUSHSTYLE_IDX],
            Katie_Qt_BrushStyle_CppToPython_Katie_Qt_BrushStyle);
        Shiboken::Conversions::addPythonToCppValueConversion(converter,
            Katie_Qt_BrushStyle_PythonToCpp_Katie_Qt_BrushStyle,
            is_Katie_Qt_BrushStyle_PythonToCpp_Katie_Qt_BrushStyle_Convertible);
        Shiboken::Enum::setTypeConverter(SbkKtCoreTypes[SBK_KATIE_QT_BRUSHSTYLE_IDX], converter);
        Shiboken::Conversions::registerConverterName(converter, "Katie::Qt::BrushStyle");
        Shiboken::Conversions::registerConverterName(converter, "Qt::BrushStyle");
        Shiboken::Conversions::registerConverterName(converter, "BrushStyle");
    }
    // End of 'BrushStyle' enum.

    // Initialization of enum 'SizeMode'.
    SbkKtCoreTypes[SBK_KATIE_QT_SIZEMODE_IDX] = Shiboken::Enum::createGlobalEnum(module,
        "SizeMode",
        "KtCore.Katie.Qt.SizeMode",
        "Katie::Qt::SizeMode");
    if (!SbkKtCoreTypes[SBK_KATIE_QT_SIZEMODE_IDX])
        return SBK_MODULE_INIT_ERROR;

    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_SIZEMODE_IDX],
        module, "AbsoluteSize", (long) Katie::Qt::SizeMode::AbsoluteSize))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_SIZEMODE_IDX],
        module, "RelativeSize", (long) Katie::Qt::SizeMode::RelativeSize))
        return SBK_MODULE_INIT_ERROR;
    // Register converter for enum 'Katie::Qt::SizeMode'.
    {
        SbkConverter* converter = Shiboken::Conversions::createConverter(SbkKtCoreTypes[SBK_KATIE_QT_SIZEMODE_IDX],
            Katie_Qt_SizeMode_CppToPython_Katie_Qt_SizeMode);
        Shiboken::Conversions::addPythonToCppValueConversion(converter,
            Katie_Qt_SizeMode_PythonToCpp_Katie_Qt_SizeMode,
            is_Katie_Qt_SizeMode_PythonToCpp_Katie_Qt_SizeMode_Convertible);
        Shiboken::Enum::setTypeConverter(SbkKtCoreTypes[SBK_KATIE_QT_SIZEMODE_IDX], converter);
        Shiboken::Conversions::registerConverterName(converter, "Katie::Qt::SizeMode");
        Shiboken::Conversions::registerConverterName(converter, "Qt::SizeMode");
        Shiboken::Conversions::registerConverterName(converter, "SizeMode");
    }
    // End of 'SizeMode' enum.

    // Initialization of enum 'UIEffect'.
    SbkKtCoreTypes[SBK_KATIE_QT_UIEFFECT_IDX] = Shiboken::Enum::createGlobalEnum(module,
        "UIEffect",
        "KtCore.Katie.Qt.UIEffect",
        "Katie::Qt::UIEffect");
    if (!SbkKtCoreTypes[SBK_KATIE_QT_UIEFFECT_IDX])
        return SBK_MODULE_INIT_ERROR;

    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_UIEFFECT_IDX],
        module, "UI_General", (long) Katie::Qt::UIEffect::UI_General))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_UIEFFECT_IDX],
        module, "UI_AnimateMenu", (long) Katie::Qt::UIEffect::UI_AnimateMenu))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_UIEFFECT_IDX],
        module, "UI_FadeMenu", (long) Katie::Qt::UIEffect::UI_FadeMenu))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_UIEFFECT_IDX],
        module, "UI_AnimateCombo", (long) Katie::Qt::UIEffect::UI_AnimateCombo))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_UIEFFECT_IDX],
        module, "UI_AnimateTooltip", (long) Katie::Qt::UIEffect::UI_AnimateTooltip))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_UIEFFECT_IDX],
        module, "UI_FadeTooltip", (long) Katie::Qt::UIEffect::UI_FadeTooltip))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_UIEFFECT_IDX],
        module, "UI_AnimateToolBox", (long) Katie::Qt::UIEffect::UI_AnimateToolBox))
        return SBK_MODULE_INIT_ERROR;
    // Register converter for enum 'Katie::Qt::UIEffect'.
    {
        SbkConverter* converter = Shiboken::Conversions::createConverter(SbkKtCoreTypes[SBK_KATIE_QT_UIEFFECT_IDX],
            Katie_Qt_UIEffect_CppToPython_Katie_Qt_UIEffect);
        Shiboken::Conversions::addPythonToCppValueConversion(converter,
            Katie_Qt_UIEffect_PythonToCpp_Katie_Qt_UIEffect,
            is_Katie_Qt_UIEffect_PythonToCpp_Katie_Qt_UIEffect_Convertible);
        Shiboken::Enum::setTypeConverter(SbkKtCoreTypes[SBK_KATIE_QT_UIEFFECT_IDX], converter);
        Shiboken::Conversions::registerConverterName(converter, "Katie::Qt::UIEffect");
        Shiboken::Conversions::registerConverterName(converter, "Qt::UIEffect");
        Shiboken::Conversions::registerConverterName(converter, "UIEffect");
    }
    // End of 'UIEffect' enum.

    // Initialization of enum 'CursorShape'.
    SbkKtCoreTypes[SBK_KATIE_QT_CURSORSHAPE_IDX] = Shiboken::Enum::createGlobalEnum(module,
        "CursorShape",
        "KtCore.Katie.Qt.CursorShape",
        "Katie::Qt::CursorShape");
    if (!SbkKtCoreTypes[SBK_KATIE_QT_CURSORSHAPE_IDX])
        return SBK_MODULE_INIT_ERROR;

    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_CURSORSHAPE_IDX],
        module, "ArrowCursor", (long) Katie::Qt::CursorShape::ArrowCursor))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_CURSORSHAPE_IDX],
        module, "UpArrowCursor", (long) Katie::Qt::CursorShape::UpArrowCursor))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_CURSORSHAPE_IDX],
        module, "CrossCursor", (long) Katie::Qt::CursorShape::CrossCursor))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_CURSORSHAPE_IDX],
        module, "WaitCursor", (long) Katie::Qt::CursorShape::WaitCursor))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_CURSORSHAPE_IDX],
        module, "IBeamCursor", (long) Katie::Qt::CursorShape::IBeamCursor))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_CURSORSHAPE_IDX],
        module, "SizeVerCursor", (long) Katie::Qt::CursorShape::SizeVerCursor))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_CURSORSHAPE_IDX],
        module, "SizeHorCursor", (long) Katie::Qt::CursorShape::SizeHorCursor))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_CURSORSHAPE_IDX],
        module, "SizeBDiagCursor", (long) Katie::Qt::CursorShape::SizeBDiagCursor))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_CURSORSHAPE_IDX],
        module, "SizeFDiagCursor", (long) Katie::Qt::CursorShape::SizeFDiagCursor))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_CURSORSHAPE_IDX],
        module, "SizeAllCursor", (long) Katie::Qt::CursorShape::SizeAllCursor))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_CURSORSHAPE_IDX],
        module, "BlankCursor", (long) Katie::Qt::CursorShape::BlankCursor))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_CURSORSHAPE_IDX],
        module, "SplitVCursor", (long) Katie::Qt::CursorShape::SplitVCursor))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_CURSORSHAPE_IDX],
        module, "SplitHCursor", (long) Katie::Qt::CursorShape::SplitHCursor))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_CURSORSHAPE_IDX],
        module, "PointingHandCursor", (long) Katie::Qt::CursorShape::PointingHandCursor))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_CURSORSHAPE_IDX],
        module, "ForbiddenCursor", (long) Katie::Qt::CursorShape::ForbiddenCursor))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_CURSORSHAPE_IDX],
        module, "WhatsThisCursor", (long) Katie::Qt::CursorShape::WhatsThisCursor))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_CURSORSHAPE_IDX],
        module, "BusyCursor", (long) Katie::Qt::CursorShape::BusyCursor))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_CURSORSHAPE_IDX],
        module, "OpenHandCursor", (long) Katie::Qt::CursorShape::OpenHandCursor))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_CURSORSHAPE_IDX],
        module, "ClosedHandCursor", (long) Katie::Qt::CursorShape::ClosedHandCursor))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_CURSORSHAPE_IDX],
        module, "DragCopyCursor", (long) Katie::Qt::CursorShape::DragCopyCursor))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_CURSORSHAPE_IDX],
        module, "DragMoveCursor", (long) Katie::Qt::CursorShape::DragMoveCursor))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_CURSORSHAPE_IDX],
        module, "DragLinkCursor", (long) Katie::Qt::CursorShape::DragLinkCursor))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_CURSORSHAPE_IDX],
        module, "LastCursor", (long) Katie::Qt::CursorShape::LastCursor))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_CURSORSHAPE_IDX],
        module, "BitmapCursor", (long) Katie::Qt::CursorShape::BitmapCursor))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_CURSORSHAPE_IDX],
        module, "CustomCursor", (long) Katie::Qt::CursorShape::CustomCursor))
        return SBK_MODULE_INIT_ERROR;
    // Register converter for enum 'Katie::Qt::CursorShape'.
    {
        SbkConverter* converter = Shiboken::Conversions::createConverter(SbkKtCoreTypes[SBK_KATIE_QT_CURSORSHAPE_IDX],
            Katie_Qt_CursorShape_CppToPython_Katie_Qt_CursorShape);
        Shiboken::Conversions::addPythonToCppValueConversion(converter,
            Katie_Qt_CursorShape_PythonToCpp_Katie_Qt_CursorShape,
            is_Katie_Qt_CursorShape_PythonToCpp_Katie_Qt_CursorShape_Convertible);
        Shiboken::Enum::setTypeConverter(SbkKtCoreTypes[SBK_KATIE_QT_CURSORSHAPE_IDX], converter);
        Shiboken::Conversions::registerConverterName(converter, "Katie::Qt::CursorShape");
        Shiboken::Conversions::registerConverterName(converter, "Qt::CursorShape");
        Shiboken::Conversions::registerConverterName(converter, "CursorShape");
    }
    // End of 'CursorShape' enum.

    // Initialization of enum 'TextFormat'.
    SbkKtCoreTypes[SBK_KATIE_QT_TEXTFORMAT_IDX] = Shiboken::Enum::createGlobalEnum(module,
        "TextFormat",
        "KtCore.Katie.Qt.TextFormat",
        "Katie::Qt::TextFormat");
    if (!SbkKtCoreTypes[SBK_KATIE_QT_TEXTFORMAT_IDX])
        return SBK_MODULE_INIT_ERROR;

    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_TEXTFORMAT_IDX],
        module, "PlainText", (long) Katie::Qt::TextFormat::PlainText))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_TEXTFORMAT_IDX],
        module, "RichText", (long) Katie::Qt::TextFormat::RichText))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_TEXTFORMAT_IDX],
        module, "AutoText", (long) Katie::Qt::TextFormat::AutoText))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_TEXTFORMAT_IDX],
        module, "LogText", (long) Katie::Qt::TextFormat::LogText))
        return SBK_MODULE_INIT_ERROR;
    // Register converter for enum 'Katie::Qt::TextFormat'.
    {
        SbkConverter* converter = Shiboken::Conversions::createConverter(SbkKtCoreTypes[SBK_KATIE_QT_TEXTFORMAT_IDX],
            Katie_Qt_TextFormat_CppToPython_Katie_Qt_TextFormat);
        Shiboken::Conversions::addPythonToCppValueConversion(converter,
            Katie_Qt_TextFormat_PythonToCpp_Katie_Qt_TextFormat,
            is_Katie_Qt_TextFormat_PythonToCpp_Katie_Qt_TextFormat_Convertible);
        Shiboken::Enum::setTypeConverter(SbkKtCoreTypes[SBK_KATIE_QT_TEXTFORMAT_IDX], converter);
        Shiboken::Conversions::registerConverterName(converter, "Katie::Qt::TextFormat");
        Shiboken::Conversions::registerConverterName(converter, "Qt::TextFormat");
        Shiboken::Conversions::registerConverterName(converter, "TextFormat");
    }
    // End of 'TextFormat' enum.

    // Initialization of enum 'AspectRatioMode'.
    SbkKtCoreTypes[SBK_KATIE_QT_ASPECTRATIOMODE_IDX] = Shiboken::Enum::createGlobalEnum(module,
        "AspectRatioMode",
        "KtCore.Katie.Qt.AspectRatioMode",
        "Katie::Qt::AspectRatioMode");
    if (!SbkKtCoreTypes[SBK_KATIE_QT_ASPECTRATIOMODE_IDX])
        return SBK_MODULE_INIT_ERROR;

    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_ASPECTRATIOMODE_IDX],
        module, "IgnoreAspectRatio", (long) Katie::Qt::AspectRatioMode::IgnoreAspectRatio))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_ASPECTRATIOMODE_IDX],
        module, "KeepAspectRatio", (long) Katie::Qt::AspectRatioMode::KeepAspectRatio))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_ASPECTRATIOMODE_IDX],
        module, "KeepAspectRatioByExpanding", (long) Katie::Qt::AspectRatioMode::KeepAspectRatioByExpanding))
        return SBK_MODULE_INIT_ERROR;
    // Register converter for enum 'Katie::Qt::AspectRatioMode'.
    {
        SbkConverter* converter = Shiboken::Conversions::createConverter(SbkKtCoreTypes[SBK_KATIE_QT_ASPECTRATIOMODE_IDX],
            Katie_Qt_AspectRatioMode_CppToPython_Katie_Qt_AspectRatioMode);
        Shiboken::Conversions::addPythonToCppValueConversion(converter,
            Katie_Qt_AspectRatioMode_PythonToCpp_Katie_Qt_AspectRatioMode,
            is_Katie_Qt_AspectRatioMode_PythonToCpp_Katie_Qt_AspectRatioMode_Convertible);
        Shiboken::Enum::setTypeConverter(SbkKtCoreTypes[SBK_KATIE_QT_ASPECTRATIOMODE_IDX], converter);
        Shiboken::Conversions::registerConverterName(converter, "Katie::Qt::AspectRatioMode");
        Shiboken::Conversions::registerConverterName(converter, "Qt::AspectRatioMode");
        Shiboken::Conversions::registerConverterName(converter, "AspectRatioMode");
    }
    // End of 'AspectRatioMode' enum.

    // Initialization of enum 'AnchorAttribute'.
    SbkKtCoreTypes[SBK_KATIE_QT_ANCHORATTRIBUTE_IDX] = Shiboken::Enum::createGlobalEnum(module,
        "AnchorAttribute",
        "KtCore.Katie.Qt.AnchorAttribute",
        "Katie::Qt::AnchorAttribute");
    if (!SbkKtCoreTypes[SBK_KATIE_QT_ANCHORATTRIBUTE_IDX])
        return SBK_MODULE_INIT_ERROR;

    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_ANCHORATTRIBUTE_IDX],
        module, "AnchorName", (long) Katie::Qt::AnchorAttribute::AnchorName))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_ANCHORATTRIBUTE_IDX],
        module, "AnchorHref", (long) Katie::Qt::AnchorAttribute::AnchorHref))
        return SBK_MODULE_INIT_ERROR;
    // Register converter for enum 'Katie::Qt::AnchorAttribute'.
    {
        SbkConverter* converter = Shiboken::Conversions::createConverter(SbkKtCoreTypes[SBK_KATIE_QT_ANCHORATTRIBUTE_IDX],
            Katie_Qt_AnchorAttribute_CppToPython_Katie_Qt_AnchorAttribute);
        Shiboken::Conversions::addPythonToCppValueConversion(converter,
            Katie_Qt_AnchorAttribute_PythonToCpp_Katie_Qt_AnchorAttribute,
            is_Katie_Qt_AnchorAttribute_PythonToCpp_Katie_Qt_AnchorAttribute_Convertible);
        Shiboken::Enum::setTypeConverter(SbkKtCoreTypes[SBK_KATIE_QT_ANCHORATTRIBUTE_IDX], converter);
        Shiboken::Conversions::registerConverterName(converter, "Katie::Qt::AnchorAttribute");
        Shiboken::Conversions::registerConverterName(converter, "Qt::AnchorAttribute");
        Shiboken::Conversions::registerConverterName(converter, "AnchorAttribute");
    }
    // End of 'AnchorAttribute' enum.

    // Initialization of enum 'DockWidgetArea'.
    SbkKtCoreTypes[SBK_KATIE_QT_DOCKWIDGETAREA_IDX] = Shiboken::Enum::createGlobalEnum(module,
        "DockWidgetArea",
        "KtCore.Katie.Qt.DockWidgetArea",
        "Katie::Qt::DockWidgetArea");
    if (!SbkKtCoreTypes[SBK_KATIE_QT_DOCKWIDGETAREA_IDX])
        return SBK_MODULE_INIT_ERROR;

    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_DOCKWIDGETAREA_IDX],
        module, "LeftDockWidgetArea", (long) Katie::Qt::DockWidgetArea::LeftDockWidgetArea))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_DOCKWIDGETAREA_IDX],
        module, "RightDockWidgetArea", (long) Katie::Qt::DockWidgetArea::RightDockWidgetArea))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_DOCKWIDGETAREA_IDX],
        module, "TopDockWidgetArea", (long) Katie::Qt::DockWidgetArea::TopDockWidgetArea))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_DOCKWIDGETAREA_IDX],
        module, "BottomDockWidgetArea", (long) Katie::Qt::DockWidgetArea::BottomDockWidgetArea))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_DOCKWIDGETAREA_IDX],
        module, "DockWidgetArea_Mask", (long) Katie::Qt::DockWidgetArea::DockWidgetArea_Mask))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_DOCKWIDGETAREA_IDX],
        module, "AllDockWidgetAreas", (long) Katie::Qt::DockWidgetArea::AllDockWidgetAreas))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_DOCKWIDGETAREA_IDX],
        module, "NoDockWidgetArea", (long) Katie::Qt::DockWidgetArea::NoDockWidgetArea))
        return SBK_MODULE_INIT_ERROR;
    // Register converter for enum 'Katie::Qt::DockWidgetArea'.
    {
        SbkConverter* converter = Shiboken::Conversions::createConverter(SbkKtCoreTypes[SBK_KATIE_QT_DOCKWIDGETAREA_IDX],
            Katie_Qt_DockWidgetArea_CppToPython_Katie_Qt_DockWidgetArea);
        Shiboken::Conversions::addPythonToCppValueConversion(converter,
            Katie_Qt_DockWidgetArea_PythonToCpp_Katie_Qt_DockWidgetArea,
            is_Katie_Qt_DockWidgetArea_PythonToCpp_Katie_Qt_DockWidgetArea_Convertible);
        Shiboken::Enum::setTypeConverter(SbkKtCoreTypes[SBK_KATIE_QT_DOCKWIDGETAREA_IDX], converter);
        Shiboken::Conversions::registerConverterName(converter, "Katie::Qt::DockWidgetArea");
        Shiboken::Conversions::registerConverterName(converter, "Qt::DockWidgetArea");
        Shiboken::Conversions::registerConverterName(converter, "DockWidgetArea");
    }
    // End of 'DockWidgetArea' enum.

    // Initialization of enum 'DockWidgetAreaSizes'.
    SbkKtCoreTypes[SBK_KATIE_QT_DOCKWIDGETAREASIZES_IDX] = Shiboken::Enum::createGlobalEnum(module,
        "DockWidgetAreaSizes",
        "KtCore.Katie.Qt.DockWidgetAreaSizes",
        "Katie::Qt::DockWidgetAreaSizes");
    if (!SbkKtCoreTypes[SBK_KATIE_QT_DOCKWIDGETAREASIZES_IDX])
        return SBK_MODULE_INIT_ERROR;

    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_DOCKWIDGETAREASIZES_IDX],
        module, "NDockWidgetAreas", (long) Katie::Qt::DockWidgetAreaSizes::NDockWidgetAreas))
        return SBK_MODULE_INIT_ERROR;
    // Register converter for enum 'Katie::Qt::DockWidgetAreaSizes'.
    {
        SbkConverter* converter = Shiboken::Conversions::createConverter(SbkKtCoreTypes[SBK_KATIE_QT_DOCKWIDGETAREASIZES_IDX],
            Katie_Qt_DockWidgetAreaSizes_CppToPython_Katie_Qt_DockWidgetAreaSizes);
        Shiboken::Conversions::addPythonToCppValueConversion(converter,
            Katie_Qt_DockWidgetAreaSizes_PythonToCpp_Katie_Qt_DockWidgetAreaSizes,
            is_Katie_Qt_DockWidgetAreaSizes_PythonToCpp_Katie_Qt_DockWidgetAreaSizes_Convertible);
        Shiboken::Enum::setTypeConverter(SbkKtCoreTypes[SBK_KATIE_QT_DOCKWIDGETAREASIZES_IDX], converter);
        Shiboken::Conversions::registerConverterName(converter, "Katie::Qt::DockWidgetAreaSizes");
        Shiboken::Conversions::registerConverterName(converter, "Qt::DockWidgetAreaSizes");
        Shiboken::Conversions::registerConverterName(converter, "DockWidgetAreaSizes");
    }
    // End of 'DockWidgetAreaSizes' enum.

    // Initialization of enum 'ToolBarArea'.
    SbkKtCoreTypes[SBK_KATIE_QT_TOOLBARAREA_IDX] = Shiboken::Enum::createGlobalEnum(module,
        "ToolBarArea",
        "KtCore.Katie.Qt.ToolBarArea",
        "Katie::Qt::ToolBarArea");
    if (!SbkKtCoreTypes[SBK_KATIE_QT_TOOLBARAREA_IDX])
        return SBK_MODULE_INIT_ERROR;

    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_TOOLBARAREA_IDX],
        module, "LeftToolBarArea", (long) Katie::Qt::ToolBarArea::LeftToolBarArea))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_TOOLBARAREA_IDX],
        module, "RightToolBarArea", (long) Katie::Qt::ToolBarArea::RightToolBarArea))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_TOOLBARAREA_IDX],
        module, "TopToolBarArea", (long) Katie::Qt::ToolBarArea::TopToolBarArea))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_TOOLBARAREA_IDX],
        module, "BottomToolBarArea", (long) Katie::Qt::ToolBarArea::BottomToolBarArea))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_TOOLBARAREA_IDX],
        module, "ToolBarArea_Mask", (long) Katie::Qt::ToolBarArea::ToolBarArea_Mask))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_TOOLBARAREA_IDX],
        module, "AllToolBarAreas", (long) Katie::Qt::ToolBarArea::AllToolBarAreas))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_TOOLBARAREA_IDX],
        module, "NoToolBarArea", (long) Katie::Qt::ToolBarArea::NoToolBarArea))
        return SBK_MODULE_INIT_ERROR;
    // Register converter for enum 'Katie::Qt::ToolBarArea'.
    {
        SbkConverter* converter = Shiboken::Conversions::createConverter(SbkKtCoreTypes[SBK_KATIE_QT_TOOLBARAREA_IDX],
            Katie_Qt_ToolBarArea_CppToPython_Katie_Qt_ToolBarArea);
        Shiboken::Conversions::addPythonToCppValueConversion(converter,
            Katie_Qt_ToolBarArea_PythonToCpp_Katie_Qt_ToolBarArea,
            is_Katie_Qt_ToolBarArea_PythonToCpp_Katie_Qt_ToolBarArea_Convertible);
        Shiboken::Enum::setTypeConverter(SbkKtCoreTypes[SBK_KATIE_QT_TOOLBARAREA_IDX], converter);
        Shiboken::Conversions::registerConverterName(converter, "Katie::Qt::ToolBarArea");
        Shiboken::Conversions::registerConverterName(converter, "Qt::ToolBarArea");
        Shiboken::Conversions::registerConverterName(converter, "ToolBarArea");
    }
    // End of 'ToolBarArea' enum.

    // Initialization of enum 'ToolBarAreaSizes'.
    SbkKtCoreTypes[SBK_KATIE_QT_TOOLBARAREASIZES_IDX] = Shiboken::Enum::createGlobalEnum(module,
        "ToolBarAreaSizes",
        "KtCore.Katie.Qt.ToolBarAreaSizes",
        "Katie::Qt::ToolBarAreaSizes");
    if (!SbkKtCoreTypes[SBK_KATIE_QT_TOOLBARAREASIZES_IDX])
        return SBK_MODULE_INIT_ERROR;

    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_TOOLBARAREASIZES_IDX],
        module, "NToolBarAreas", (long) Katie::Qt::ToolBarAreaSizes::NToolBarAreas))
        return SBK_MODULE_INIT_ERROR;
    // Register converter for enum 'Katie::Qt::ToolBarAreaSizes'.
    {
        SbkConverter* converter = Shiboken::Conversions::createConverter(SbkKtCoreTypes[SBK_KATIE_QT_TOOLBARAREASIZES_IDX],
            Katie_Qt_ToolBarAreaSizes_CppToPython_Katie_Qt_ToolBarAreaSizes);
        Shiboken::Conversions::addPythonToCppValueConversion(converter,
            Katie_Qt_ToolBarAreaSizes_PythonToCpp_Katie_Qt_ToolBarAreaSizes,
            is_Katie_Qt_ToolBarAreaSizes_PythonToCpp_Katie_Qt_ToolBarAreaSizes_Convertible);
        Shiboken::Enum::setTypeConverter(SbkKtCoreTypes[SBK_KATIE_QT_TOOLBARAREASIZES_IDX], converter);
        Shiboken::Conversions::registerConverterName(converter, "Katie::Qt::ToolBarAreaSizes");
        Shiboken::Conversions::registerConverterName(converter, "Qt::ToolBarAreaSizes");
        Shiboken::Conversions::registerConverterName(converter, "ToolBarAreaSizes");
    }
    // End of 'ToolBarAreaSizes' enum.

    // Initialization of enum 'DateFormat'.
    SbkKtCoreTypes[SBK_KATIE_QT_DATEFORMAT_IDX] = Shiboken::Enum::createGlobalEnum(module,
        "DateFormat",
        "KtCore.Katie.Qt.DateFormat",
        "Katie::Qt::DateFormat");
    if (!SbkKtCoreTypes[SBK_KATIE_QT_DATEFORMAT_IDX])
        return SBK_MODULE_INIT_ERROR;

    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_DATEFORMAT_IDX],
        module, "TextDate", (long) Katie::Qt::DateFormat::TextDate))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_DATEFORMAT_IDX],
        module, "ISODate", (long) Katie::Qt::DateFormat::ISODate))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_DATEFORMAT_IDX],
        module, "SystemLocaleDate", (long) Katie::Qt::DateFormat::SystemLocaleDate))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_DATEFORMAT_IDX],
        module, "LocalDate", (long) Katie::Qt::DateFormat::LocalDate))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_DATEFORMAT_IDX],
        module, "LocaleDate", (long) Katie::Qt::DateFormat::LocaleDate))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_DATEFORMAT_IDX],
        module, "SystemLocaleShortDate", (long) Katie::Qt::DateFormat::SystemLocaleShortDate))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_DATEFORMAT_IDX],
        module, "SystemLocaleLongDate", (long) Katie::Qt::DateFormat::SystemLocaleLongDate))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_DATEFORMAT_IDX],
        module, "DefaultLocaleShortDate", (long) Katie::Qt::DateFormat::DefaultLocaleShortDate))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_DATEFORMAT_IDX],
        module, "DefaultLocaleLongDate", (long) Katie::Qt::DateFormat::DefaultLocaleLongDate))
        return SBK_MODULE_INIT_ERROR;
    // Register converter for enum 'Katie::Qt::DateFormat'.
    {
        SbkConverter* converter = Shiboken::Conversions::createConverter(SbkKtCoreTypes[SBK_KATIE_QT_DATEFORMAT_IDX],
            Katie_Qt_DateFormat_CppToPython_Katie_Qt_DateFormat);
        Shiboken::Conversions::addPythonToCppValueConversion(converter,
            Katie_Qt_DateFormat_PythonToCpp_Katie_Qt_DateFormat,
            is_Katie_Qt_DateFormat_PythonToCpp_Katie_Qt_DateFormat_Convertible);
        Shiboken::Enum::setTypeConverter(SbkKtCoreTypes[SBK_KATIE_QT_DATEFORMAT_IDX], converter);
        Shiboken::Conversions::registerConverterName(converter, "Katie::Qt::DateFormat");
        Shiboken::Conversions::registerConverterName(converter, "Qt::DateFormat");
        Shiboken::Conversions::registerConverterName(converter, "DateFormat");
    }
    // End of 'DateFormat' enum.

    // Initialization of enum 'TimeSpec'.
    SbkKtCoreTypes[SBK_KATIE_QT_TIMESPEC_IDX] = Shiboken::Enum::createGlobalEnum(module,
        "TimeSpec",
        "KtCore.Katie.Qt.TimeSpec",
        "Katie::Qt::TimeSpec");
    if (!SbkKtCoreTypes[SBK_KATIE_QT_TIMESPEC_IDX])
        return SBK_MODULE_INIT_ERROR;

    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_TIMESPEC_IDX],
        module, "LocalTime", (long) Katie::Qt::TimeSpec::LocalTime))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_TIMESPEC_IDX],
        module, "UTC", (long) Katie::Qt::TimeSpec::UTC))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_TIMESPEC_IDX],
        module, "OffsetFromUTC", (long) Katie::Qt::TimeSpec::OffsetFromUTC))
        return SBK_MODULE_INIT_ERROR;
    // Register converter for enum 'Katie::Qt::TimeSpec'.
    {
        SbkConverter* converter = Shiboken::Conversions::createConverter(SbkKtCoreTypes[SBK_KATIE_QT_TIMESPEC_IDX],
            Katie_Qt_TimeSpec_CppToPython_Katie_Qt_TimeSpec);
        Shiboken::Conversions::addPythonToCppValueConversion(converter,
            Katie_Qt_TimeSpec_PythonToCpp_Katie_Qt_TimeSpec,
            is_Katie_Qt_TimeSpec_PythonToCpp_Katie_Qt_TimeSpec_Convertible);
        Shiboken::Enum::setTypeConverter(SbkKtCoreTypes[SBK_KATIE_QT_TIMESPEC_IDX], converter);
        Shiboken::Conversions::registerConverterName(converter, "Katie::Qt::TimeSpec");
        Shiboken::Conversions::registerConverterName(converter, "Qt::TimeSpec");
        Shiboken::Conversions::registerConverterName(converter, "TimeSpec");
    }
    // End of 'TimeSpec' enum.

    // Initialization of enum 'DayOfWeek'.
    SbkKtCoreTypes[SBK_KATIE_QT_DAYOFWEEK_IDX] = Shiboken::Enum::createGlobalEnum(module,
        "DayOfWeek",
        "KtCore.Katie.Qt.DayOfWeek",
        "Katie::Qt::DayOfWeek");
    if (!SbkKtCoreTypes[SBK_KATIE_QT_DAYOFWEEK_IDX])
        return SBK_MODULE_INIT_ERROR;

    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_DAYOFWEEK_IDX],
        module, "Monday", (long) Katie::Qt::DayOfWeek::Monday))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_DAYOFWEEK_IDX],
        module, "Tuesday", (long) Katie::Qt::DayOfWeek::Tuesday))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_DAYOFWEEK_IDX],
        module, "Wednesday", (long) Katie::Qt::DayOfWeek::Wednesday))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_DAYOFWEEK_IDX],
        module, "Thursday", (long) Katie::Qt::DayOfWeek::Thursday))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_DAYOFWEEK_IDX],
        module, "Friday", (long) Katie::Qt::DayOfWeek::Friday))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_DAYOFWEEK_IDX],
        module, "Saturday", (long) Katie::Qt::DayOfWeek::Saturday))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_DAYOFWEEK_IDX],
        module, "Sunday", (long) Katie::Qt::DayOfWeek::Sunday))
        return SBK_MODULE_INIT_ERROR;
    // Register converter for enum 'Katie::Qt::DayOfWeek'.
    {
        SbkConverter* converter = Shiboken::Conversions::createConverter(SbkKtCoreTypes[SBK_KATIE_QT_DAYOFWEEK_IDX],
            Katie_Qt_DayOfWeek_CppToPython_Katie_Qt_DayOfWeek);
        Shiboken::Conversions::addPythonToCppValueConversion(converter,
            Katie_Qt_DayOfWeek_PythonToCpp_Katie_Qt_DayOfWeek,
            is_Katie_Qt_DayOfWeek_PythonToCpp_Katie_Qt_DayOfWeek_Convertible);
        Shiboken::Enum::setTypeConverter(SbkKtCoreTypes[SBK_KATIE_QT_DAYOFWEEK_IDX], converter);
        Shiboken::Conversions::registerConverterName(converter, "Katie::Qt::DayOfWeek");
        Shiboken::Conversions::registerConverterName(converter, "Qt::DayOfWeek");
        Shiboken::Conversions::registerConverterName(converter, "DayOfWeek");
    }
    // End of 'DayOfWeek' enum.

    // Initialization of enum 'ScrollBarPolicy'.
    SbkKtCoreTypes[SBK_KATIE_QT_SCROLLBARPOLICY_IDX] = Shiboken::Enum::createGlobalEnum(module,
        "ScrollBarPolicy",
        "KtCore.Katie.Qt.ScrollBarPolicy",
        "Katie::Qt::ScrollBarPolicy");
    if (!SbkKtCoreTypes[SBK_KATIE_QT_SCROLLBARPOLICY_IDX])
        return SBK_MODULE_INIT_ERROR;

    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_SCROLLBARPOLICY_IDX],
        module, "ScrollBarAsNeeded", (long) Katie::Qt::ScrollBarPolicy::ScrollBarAsNeeded))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_SCROLLBARPOLICY_IDX],
        module, "ScrollBarAlwaysOff", (long) Katie::Qt::ScrollBarPolicy::ScrollBarAlwaysOff))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_SCROLLBARPOLICY_IDX],
        module, "ScrollBarAlwaysOn", (long) Katie::Qt::ScrollBarPolicy::ScrollBarAlwaysOn))
        return SBK_MODULE_INIT_ERROR;
    // Register converter for enum 'Katie::Qt::ScrollBarPolicy'.
    {
        SbkConverter* converter = Shiboken::Conversions::createConverter(SbkKtCoreTypes[SBK_KATIE_QT_SCROLLBARPOLICY_IDX],
            Katie_Qt_ScrollBarPolicy_CppToPython_Katie_Qt_ScrollBarPolicy);
        Shiboken::Conversions::addPythonToCppValueConversion(converter,
            Katie_Qt_ScrollBarPolicy_PythonToCpp_Katie_Qt_ScrollBarPolicy,
            is_Katie_Qt_ScrollBarPolicy_PythonToCpp_Katie_Qt_ScrollBarPolicy_Convertible);
        Shiboken::Enum::setTypeConverter(SbkKtCoreTypes[SBK_KATIE_QT_SCROLLBARPOLICY_IDX], converter);
        Shiboken::Conversions::registerConverterName(converter, "Katie::Qt::ScrollBarPolicy");
        Shiboken::Conversions::registerConverterName(converter, "Qt::ScrollBarPolicy");
        Shiboken::Conversions::registerConverterName(converter, "ScrollBarPolicy");
    }
    // End of 'ScrollBarPolicy' enum.

    // Initialization of enum 'CaseSensitivity'.
    SbkKtCoreTypes[SBK_KATIE_QT_CASESENSITIVITY_IDX] = Shiboken::Enum::createGlobalEnum(module,
        "CaseSensitivity",
        "KtCore.Katie.Qt.CaseSensitivity",
        "Katie::Qt::CaseSensitivity");
    if (!SbkKtCoreTypes[SBK_KATIE_QT_CASESENSITIVITY_IDX])
        return SBK_MODULE_INIT_ERROR;

    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_CASESENSITIVITY_IDX],
        module, "CaseInsensitive", (long) Katie::Qt::CaseSensitivity::CaseInsensitive))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_CASESENSITIVITY_IDX],
        module, "CaseSensitive", (long) Katie::Qt::CaseSensitivity::CaseSensitive))
        return SBK_MODULE_INIT_ERROR;
    // Register converter for enum 'Katie::Qt::CaseSensitivity'.
    {
        SbkConverter* converter = Shiboken::Conversions::createConverter(SbkKtCoreTypes[SBK_KATIE_QT_CASESENSITIVITY_IDX],
            Katie_Qt_CaseSensitivity_CppToPython_Katie_Qt_CaseSensitivity);
        Shiboken::Conversions::addPythonToCppValueConversion(converter,
            Katie_Qt_CaseSensitivity_PythonToCpp_Katie_Qt_CaseSensitivity,
            is_Katie_Qt_CaseSensitivity_PythonToCpp_Katie_Qt_CaseSensitivity_Convertible);
        Shiboken::Enum::setTypeConverter(SbkKtCoreTypes[SBK_KATIE_QT_CASESENSITIVITY_IDX], converter);
        Shiboken::Conversions::registerConverterName(converter, "Katie::Qt::CaseSensitivity");
        Shiboken::Conversions::registerConverterName(converter, "Qt::CaseSensitivity");
        Shiboken::Conversions::registerConverterName(converter, "CaseSensitivity");
    }
    // End of 'CaseSensitivity' enum.

    // Initialization of enum 'Corner'.
    SbkKtCoreTypes[SBK_KATIE_QT_CORNER_IDX] = Shiboken::Enum::createGlobalEnum(module,
        "Corner",
        "KtCore.Katie.Qt.Corner",
        "Katie::Qt::Corner");
    if (!SbkKtCoreTypes[SBK_KATIE_QT_CORNER_IDX])
        return SBK_MODULE_INIT_ERROR;

    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_CORNER_IDX],
        module, "TopLeftCorner", (long) Katie::Qt::Corner::TopLeftCorner))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_CORNER_IDX],
        module, "TopRightCorner", (long) Katie::Qt::Corner::TopRightCorner))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_CORNER_IDX],
        module, "BottomLeftCorner", (long) Katie::Qt::Corner::BottomLeftCorner))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_CORNER_IDX],
        module, "BottomRightCorner", (long) Katie::Qt::Corner::BottomRightCorner))
        return SBK_MODULE_INIT_ERROR;
    // Register converter for enum 'Katie::Qt::Corner'.
    {
        SbkConverter* converter = Shiboken::Conversions::createConverter(SbkKtCoreTypes[SBK_KATIE_QT_CORNER_IDX],
            Katie_Qt_Corner_CppToPython_Katie_Qt_Corner);
        Shiboken::Conversions::addPythonToCppValueConversion(converter,
            Katie_Qt_Corner_PythonToCpp_Katie_Qt_Corner,
            is_Katie_Qt_Corner_PythonToCpp_Katie_Qt_Corner_Convertible);
        Shiboken::Enum::setTypeConverter(SbkKtCoreTypes[SBK_KATIE_QT_CORNER_IDX], converter);
        Shiboken::Conversions::registerConverterName(converter, "Katie::Qt::Corner");
        Shiboken::Conversions::registerConverterName(converter, "Qt::Corner");
        Shiboken::Conversions::registerConverterName(converter, "Corner");
    }
    // End of 'Corner' enum.

    // Initialization of enum 'ConnectionType'.
    SbkKtCoreTypes[SBK_KATIE_QT_CONNECTIONTYPE_IDX] = Shiboken::Enum::createGlobalEnum(module,
        "ConnectionType",
        "KtCore.Katie.Qt.ConnectionType",
        "Katie::Qt::ConnectionType");
    if (!SbkKtCoreTypes[SBK_KATIE_QT_CONNECTIONTYPE_IDX])
        return SBK_MODULE_INIT_ERROR;

    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_CONNECTIONTYPE_IDX],
        module, "AutoConnection", (long) Katie::Qt::ConnectionType::AutoConnection))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_CONNECTIONTYPE_IDX],
        module, "DirectConnection", (long) Katie::Qt::ConnectionType::DirectConnection))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_CONNECTIONTYPE_IDX],
        module, "QueuedConnection", (long) Katie::Qt::ConnectionType::QueuedConnection))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_CONNECTIONTYPE_IDX],
        module, "BlockingQueuedConnection", (long) Katie::Qt::ConnectionType::BlockingQueuedConnection))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_CONNECTIONTYPE_IDX],
        module, "UniqueConnection", (long) Katie::Qt::ConnectionType::UniqueConnection))
        return SBK_MODULE_INIT_ERROR;
    // Register converter for enum 'Katie::Qt::ConnectionType'.
    {
        SbkConverter* converter = Shiboken::Conversions::createConverter(SbkKtCoreTypes[SBK_KATIE_QT_CONNECTIONTYPE_IDX],
            Katie_Qt_ConnectionType_CppToPython_Katie_Qt_ConnectionType);
        Shiboken::Conversions::addPythonToCppValueConversion(converter,
            Katie_Qt_ConnectionType_PythonToCpp_Katie_Qt_ConnectionType,
            is_Katie_Qt_ConnectionType_PythonToCpp_Katie_Qt_ConnectionType_Convertible);
        Shiboken::Enum::setTypeConverter(SbkKtCoreTypes[SBK_KATIE_QT_CONNECTIONTYPE_IDX], converter);
        Shiboken::Conversions::registerConverterName(converter, "Katie::Qt::ConnectionType");
        Shiboken::Conversions::registerConverterName(converter, "Qt::ConnectionType");
        Shiboken::Conversions::registerConverterName(converter, "ConnectionType");
    }
    // End of 'ConnectionType' enum.

    // Initialization of enum 'ShortcutContext'.
    SbkKtCoreTypes[SBK_KATIE_QT_SHORTCUTCONTEXT_IDX] = Shiboken::Enum::createGlobalEnum(module,
        "ShortcutContext",
        "KtCore.Katie.Qt.ShortcutContext",
        "Katie::Qt::ShortcutContext");
    if (!SbkKtCoreTypes[SBK_KATIE_QT_SHORTCUTCONTEXT_IDX])
        return SBK_MODULE_INIT_ERROR;

    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_SHORTCUTCONTEXT_IDX],
        module, "WidgetShortcut", (long) Katie::Qt::ShortcutContext::WidgetShortcut))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_SHORTCUTCONTEXT_IDX],
        module, "WindowShortcut", (long) Katie::Qt::ShortcutContext::WindowShortcut))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_SHORTCUTCONTEXT_IDX],
        module, "ApplicationShortcut", (long) Katie::Qt::ShortcutContext::ApplicationShortcut))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_SHORTCUTCONTEXT_IDX],
        module, "WidgetWithChildrenShortcut", (long) Katie::Qt::ShortcutContext::WidgetWithChildrenShortcut))
        return SBK_MODULE_INIT_ERROR;
    // Register converter for enum 'Katie::Qt::ShortcutContext'.
    {
        SbkConverter* converter = Shiboken::Conversions::createConverter(SbkKtCoreTypes[SBK_KATIE_QT_SHORTCUTCONTEXT_IDX],
            Katie_Qt_ShortcutContext_CppToPython_Katie_Qt_ShortcutContext);
        Shiboken::Conversions::addPythonToCppValueConversion(converter,
            Katie_Qt_ShortcutContext_PythonToCpp_Katie_Qt_ShortcutContext,
            is_Katie_Qt_ShortcutContext_PythonToCpp_Katie_Qt_ShortcutContext_Convertible);
        Shiboken::Enum::setTypeConverter(SbkKtCoreTypes[SBK_KATIE_QT_SHORTCUTCONTEXT_IDX], converter);
        Shiboken::Conversions::registerConverterName(converter, "Katie::Qt::ShortcutContext");
        Shiboken::Conversions::registerConverterName(converter, "Qt::ShortcutContext");
        Shiboken::Conversions::registerConverterName(converter, "ShortcutContext");
    }
    // End of 'ShortcutContext' enum.

    // Initialization of enum 'FillRule'.
    SbkKtCoreTypes[SBK_KATIE_QT_FILLRULE_IDX] = Shiboken::Enum::createGlobalEnum(module,
        "FillRule",
        "KtCore.Katie.Qt.FillRule",
        "Katie::Qt::FillRule");
    if (!SbkKtCoreTypes[SBK_KATIE_QT_FILLRULE_IDX])
        return SBK_MODULE_INIT_ERROR;

    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_FILLRULE_IDX],
        module, "OddEvenFill", (long) Katie::Qt::FillRule::OddEvenFill))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_FILLRULE_IDX],
        module, "WindingFill", (long) Katie::Qt::FillRule::WindingFill))
        return SBK_MODULE_INIT_ERROR;
    // Register converter for enum 'Katie::Qt::FillRule'.
    {
        SbkConverter* converter = Shiboken::Conversions::createConverter(SbkKtCoreTypes[SBK_KATIE_QT_FILLRULE_IDX],
            Katie_Qt_FillRule_CppToPython_Katie_Qt_FillRule);
        Shiboken::Conversions::addPythonToCppValueConversion(converter,
            Katie_Qt_FillRule_PythonToCpp_Katie_Qt_FillRule,
            is_Katie_Qt_FillRule_PythonToCpp_Katie_Qt_FillRule_Convertible);
        Shiboken::Enum::setTypeConverter(SbkKtCoreTypes[SBK_KATIE_QT_FILLRULE_IDX], converter);
        Shiboken::Conversions::registerConverterName(converter, "Katie::Qt::FillRule");
        Shiboken::Conversions::registerConverterName(converter, "Qt::FillRule");
        Shiboken::Conversions::registerConverterName(converter, "FillRule");
    }
    // End of 'FillRule' enum.

    // Initialization of enum 'MaskMode'.
    SbkKtCoreTypes[SBK_KATIE_QT_MASKMODE_IDX] = Shiboken::Enum::createGlobalEnum(module,
        "MaskMode",
        "KtCore.Katie.Qt.MaskMode",
        "Katie::Qt::MaskMode");
    if (!SbkKtCoreTypes[SBK_KATIE_QT_MASKMODE_IDX])
        return SBK_MODULE_INIT_ERROR;

    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_MASKMODE_IDX],
        module, "MaskInColor", (long) Katie::Qt::MaskMode::MaskInColor))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_MASKMODE_IDX],
        module, "MaskOutColor", (long) Katie::Qt::MaskMode::MaskOutColor))
        return SBK_MODULE_INIT_ERROR;
    // Register converter for enum 'Katie::Qt::MaskMode'.
    {
        SbkConverter* converter = Shiboken::Conversions::createConverter(SbkKtCoreTypes[SBK_KATIE_QT_MASKMODE_IDX],
            Katie_Qt_MaskMode_CppToPython_Katie_Qt_MaskMode);
        Shiboken::Conversions::addPythonToCppValueConversion(converter,
            Katie_Qt_MaskMode_PythonToCpp_Katie_Qt_MaskMode,
            is_Katie_Qt_MaskMode_PythonToCpp_Katie_Qt_MaskMode_Convertible);
        Shiboken::Enum::setTypeConverter(SbkKtCoreTypes[SBK_KATIE_QT_MASKMODE_IDX], converter);
        Shiboken::Conversions::registerConverterName(converter, "Katie::Qt::MaskMode");
        Shiboken::Conversions::registerConverterName(converter, "Qt::MaskMode");
        Shiboken::Conversions::registerConverterName(converter, "MaskMode");
    }
    // End of 'MaskMode' enum.

    // Initialization of enum 'ClipOperation'.
    SbkKtCoreTypes[SBK_KATIE_QT_CLIPOPERATION_IDX] = Shiboken::Enum::createGlobalEnum(module,
        "ClipOperation",
        "KtCore.Katie.Qt.ClipOperation",
        "Katie::Qt::ClipOperation");
    if (!SbkKtCoreTypes[SBK_KATIE_QT_CLIPOPERATION_IDX])
        return SBK_MODULE_INIT_ERROR;

    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_CLIPOPERATION_IDX],
        module, "NoClip", (long) Katie::Qt::ClipOperation::NoClip))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_CLIPOPERATION_IDX],
        module, "ReplaceClip", (long) Katie::Qt::ClipOperation::ReplaceClip))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_CLIPOPERATION_IDX],
        module, "IntersectClip", (long) Katie::Qt::ClipOperation::IntersectClip))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_CLIPOPERATION_IDX],
        module, "UniteClip", (long) Katie::Qt::ClipOperation::UniteClip))
        return SBK_MODULE_INIT_ERROR;
    // Register converter for enum 'Katie::Qt::ClipOperation'.
    {
        SbkConverter* converter = Shiboken::Conversions::createConverter(SbkKtCoreTypes[SBK_KATIE_QT_CLIPOPERATION_IDX],
            Katie_Qt_ClipOperation_CppToPython_Katie_Qt_ClipOperation);
        Shiboken::Conversions::addPythonToCppValueConversion(converter,
            Katie_Qt_ClipOperation_PythonToCpp_Katie_Qt_ClipOperation,
            is_Katie_Qt_ClipOperation_PythonToCpp_Katie_Qt_ClipOperation_Convertible);
        Shiboken::Enum::setTypeConverter(SbkKtCoreTypes[SBK_KATIE_QT_CLIPOPERATION_IDX], converter);
        Shiboken::Conversions::registerConverterName(converter, "Katie::Qt::ClipOperation");
        Shiboken::Conversions::registerConverterName(converter, "Qt::ClipOperation");
        Shiboken::Conversions::registerConverterName(converter, "ClipOperation");
    }
    // End of 'ClipOperation' enum.

    // Initialization of enum 'ItemSelectionMode'.
    SbkKtCoreTypes[SBK_KATIE_QT_ITEMSELECTIONMODE_IDX] = Shiboken::Enum::createGlobalEnum(module,
        "ItemSelectionMode",
        "KtCore.Katie.Qt.ItemSelectionMode",
        "Katie::Qt::ItemSelectionMode");
    if (!SbkKtCoreTypes[SBK_KATIE_QT_ITEMSELECTIONMODE_IDX])
        return SBK_MODULE_INIT_ERROR;

    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_ITEMSELECTIONMODE_IDX],
        module, "ContainsItemShape", (long) Katie::Qt::ItemSelectionMode::ContainsItemShape))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_ITEMSELECTIONMODE_IDX],
        module, "IntersectsItemShape", (long) Katie::Qt::ItemSelectionMode::IntersectsItemShape))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_ITEMSELECTIONMODE_IDX],
        module, "ContainsItemBoundingRect", (long) Katie::Qt::ItemSelectionMode::ContainsItemBoundingRect))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_ITEMSELECTIONMODE_IDX],
        module, "IntersectsItemBoundingRect", (long) Katie::Qt::ItemSelectionMode::IntersectsItemBoundingRect))
        return SBK_MODULE_INIT_ERROR;
    // Register converter for enum 'Katie::Qt::ItemSelectionMode'.
    {
        SbkConverter* converter = Shiboken::Conversions::createConverter(SbkKtCoreTypes[SBK_KATIE_QT_ITEMSELECTIONMODE_IDX],
            Katie_Qt_ItemSelectionMode_CppToPython_Katie_Qt_ItemSelectionMode);
        Shiboken::Conversions::addPythonToCppValueConversion(converter,
            Katie_Qt_ItemSelectionMode_PythonToCpp_Katie_Qt_ItemSelectionMode,
            is_Katie_Qt_ItemSelectionMode_PythonToCpp_Katie_Qt_ItemSelectionMode_Convertible);
        Shiboken::Enum::setTypeConverter(SbkKtCoreTypes[SBK_KATIE_QT_ITEMSELECTIONMODE_IDX], converter);
        Shiboken::Conversions::registerConverterName(converter, "Katie::Qt::ItemSelectionMode");
        Shiboken::Conversions::registerConverterName(converter, "Qt::ItemSelectionMode");
        Shiboken::Conversions::registerConverterName(converter, "ItemSelectionMode");
    }
    // End of 'ItemSelectionMode' enum.

    // Initialization of enum 'TransformationMode'.
    SbkKtCoreTypes[SBK_KATIE_QT_TRANSFORMATIONMODE_IDX] = Shiboken::Enum::createGlobalEnum(module,
        "TransformationMode",
        "KtCore.Katie.Qt.TransformationMode",
        "Katie::Qt::TransformationMode");
    if (!SbkKtCoreTypes[SBK_KATIE_QT_TRANSFORMATIONMODE_IDX])
        return SBK_MODULE_INIT_ERROR;

    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_TRANSFORMATIONMODE_IDX],
        module, "FastTransformation", (long) Katie::Qt::TransformationMode::FastTransformation))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_TRANSFORMATIONMODE_IDX],
        module, "SmoothTransformation", (long) Katie::Qt::TransformationMode::SmoothTransformation))
        return SBK_MODULE_INIT_ERROR;
    // Register converter for enum 'Katie::Qt::TransformationMode'.
    {
        SbkConverter* converter = Shiboken::Conversions::createConverter(SbkKtCoreTypes[SBK_KATIE_QT_TRANSFORMATIONMODE_IDX],
            Katie_Qt_TransformationMode_CppToPython_Katie_Qt_TransformationMode);
        Shiboken::Conversions::addPythonToCppValueConversion(converter,
            Katie_Qt_TransformationMode_PythonToCpp_Katie_Qt_TransformationMode,
            is_Katie_Qt_TransformationMode_PythonToCpp_Katie_Qt_TransformationMode_Convertible);
        Shiboken::Enum::setTypeConverter(SbkKtCoreTypes[SBK_KATIE_QT_TRANSFORMATIONMODE_IDX], converter);
        Shiboken::Conversions::registerConverterName(converter, "Katie::Qt::TransformationMode");
        Shiboken::Conversions::registerConverterName(converter, "Qt::TransformationMode");
        Shiboken::Conversions::registerConverterName(converter, "TransformationMode");
    }
    // End of 'TransformationMode' enum.

    // Initialization of enum 'Axis'.
    SbkKtCoreTypes[SBK_KATIE_QT_AXIS_IDX] = Shiboken::Enum::createGlobalEnum(module,
        "Axis",
        "KtCore.Katie.Qt.Axis",
        "Katie::Qt::Axis");
    if (!SbkKtCoreTypes[SBK_KATIE_QT_AXIS_IDX])
        return SBK_MODULE_INIT_ERROR;

    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_AXIS_IDX],
        module, "XAxis", (long) Katie::Qt::Axis::XAxis))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_AXIS_IDX],
        module, "YAxis", (long) Katie::Qt::Axis::YAxis))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_AXIS_IDX],
        module, "ZAxis", (long) Katie::Qt::Axis::ZAxis))
        return SBK_MODULE_INIT_ERROR;
    // Register converter for enum 'Katie::Qt::Axis'.
    {
        SbkConverter* converter = Shiboken::Conversions::createConverter(SbkKtCoreTypes[SBK_KATIE_QT_AXIS_IDX],
            Katie_Qt_Axis_CppToPython_Katie_Qt_Axis);
        Shiboken::Conversions::addPythonToCppValueConversion(converter,
            Katie_Qt_Axis_PythonToCpp_Katie_Qt_Axis,
            is_Katie_Qt_Axis_PythonToCpp_Katie_Qt_Axis_Convertible);
        Shiboken::Enum::setTypeConverter(SbkKtCoreTypes[SBK_KATIE_QT_AXIS_IDX], converter);
        Shiboken::Conversions::registerConverterName(converter, "Katie::Qt::Axis");
        Shiboken::Conversions::registerConverterName(converter, "Qt::Axis");
        Shiboken::Conversions::registerConverterName(converter, "Axis");
    }
    // End of 'Axis' enum.

    // Initialization of enum 'FocusReason'.
    SbkKtCoreTypes[SBK_KATIE_QT_FOCUSREASON_IDX] = Shiboken::Enum::createGlobalEnum(module,
        "FocusReason",
        "KtCore.Katie.Qt.FocusReason",
        "Katie::Qt::FocusReason");
    if (!SbkKtCoreTypes[SBK_KATIE_QT_FOCUSREASON_IDX])
        return SBK_MODULE_INIT_ERROR;

    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_FOCUSREASON_IDX],
        module, "MouseFocusReason", (long) Katie::Qt::FocusReason::MouseFocusReason))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_FOCUSREASON_IDX],
        module, "TabFocusReason", (long) Katie::Qt::FocusReason::TabFocusReason))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_FOCUSREASON_IDX],
        module, "BacktabFocusReason", (long) Katie::Qt::FocusReason::BacktabFocusReason))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_FOCUSREASON_IDX],
        module, "ActiveWindowFocusReason", (long) Katie::Qt::FocusReason::ActiveWindowFocusReason))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_FOCUSREASON_IDX],
        module, "PopupFocusReason", (long) Katie::Qt::FocusReason::PopupFocusReason))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_FOCUSREASON_IDX],
        module, "ShortcutFocusReason", (long) Katie::Qt::FocusReason::ShortcutFocusReason))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_FOCUSREASON_IDX],
        module, "MenuBarFocusReason", (long) Katie::Qt::FocusReason::MenuBarFocusReason))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_FOCUSREASON_IDX],
        module, "OtherFocusReason", (long) Katie::Qt::FocusReason::OtherFocusReason))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_FOCUSREASON_IDX],
        module, "NoFocusReason", (long) Katie::Qt::FocusReason::NoFocusReason))
        return SBK_MODULE_INIT_ERROR;
    // Register converter for enum 'Katie::Qt::FocusReason'.
    {
        SbkConverter* converter = Shiboken::Conversions::createConverter(SbkKtCoreTypes[SBK_KATIE_QT_FOCUSREASON_IDX],
            Katie_Qt_FocusReason_CppToPython_Katie_Qt_FocusReason);
        Shiboken::Conversions::addPythonToCppValueConversion(converter,
            Katie_Qt_FocusReason_PythonToCpp_Katie_Qt_FocusReason,
            is_Katie_Qt_FocusReason_PythonToCpp_Katie_Qt_FocusReason_Convertible);
        Shiboken::Enum::setTypeConverter(SbkKtCoreTypes[SBK_KATIE_QT_FOCUSREASON_IDX], converter);
        Shiboken::Conversions::registerConverterName(converter, "Katie::Qt::FocusReason");
        Shiboken::Conversions::registerConverterName(converter, "Qt::FocusReason");
        Shiboken::Conversions::registerConverterName(converter, "FocusReason");
    }
    // End of 'FocusReason' enum.

    // Initialization of enum 'ContextMenuPolicy'.
    SbkKtCoreTypes[SBK_KATIE_QT_CONTEXTMENUPOLICY_IDX] = Shiboken::Enum::createGlobalEnum(module,
        "ContextMenuPolicy",
        "KtCore.Katie.Qt.ContextMenuPolicy",
        "Katie::Qt::ContextMenuPolicy");
    if (!SbkKtCoreTypes[SBK_KATIE_QT_CONTEXTMENUPOLICY_IDX])
        return SBK_MODULE_INIT_ERROR;

    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_CONTEXTMENUPOLICY_IDX],
        module, "NoContextMenu", (long) Katie::Qt::ContextMenuPolicy::NoContextMenu))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_CONTEXTMENUPOLICY_IDX],
        module, "DefaultContextMenu", (long) Katie::Qt::ContextMenuPolicy::DefaultContextMenu))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_CONTEXTMENUPOLICY_IDX],
        module, "ActionsContextMenu", (long) Katie::Qt::ContextMenuPolicy::ActionsContextMenu))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_CONTEXTMENUPOLICY_IDX],
        module, "CustomContextMenu", (long) Katie::Qt::ContextMenuPolicy::CustomContextMenu))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_CONTEXTMENUPOLICY_IDX],
        module, "PreventContextMenu", (long) Katie::Qt::ContextMenuPolicy::PreventContextMenu))
        return SBK_MODULE_INIT_ERROR;
    // Register converter for enum 'Katie::Qt::ContextMenuPolicy'.
    {
        SbkConverter* converter = Shiboken::Conversions::createConverter(SbkKtCoreTypes[SBK_KATIE_QT_CONTEXTMENUPOLICY_IDX],
            Katie_Qt_ContextMenuPolicy_CppToPython_Katie_Qt_ContextMenuPolicy);
        Shiboken::Conversions::addPythonToCppValueConversion(converter,
            Katie_Qt_ContextMenuPolicy_PythonToCpp_Katie_Qt_ContextMenuPolicy,
            is_Katie_Qt_ContextMenuPolicy_PythonToCpp_Katie_Qt_ContextMenuPolicy_Convertible);
        Shiboken::Enum::setTypeConverter(SbkKtCoreTypes[SBK_KATIE_QT_CONTEXTMENUPOLICY_IDX], converter);
        Shiboken::Conversions::registerConverterName(converter, "Katie::Qt::ContextMenuPolicy");
        Shiboken::Conversions::registerConverterName(converter, "Qt::ContextMenuPolicy");
        Shiboken::Conversions::registerConverterName(converter, "ContextMenuPolicy");
    }
    // End of 'ContextMenuPolicy' enum.

    // Initialization of enum 'ToolButtonStyle'.
    SbkKtCoreTypes[SBK_KATIE_QT_TOOLBUTTONSTYLE_IDX] = Shiboken::Enum::createGlobalEnum(module,
        "ToolButtonStyle",
        "KtCore.Katie.Qt.ToolButtonStyle",
        "Katie::Qt::ToolButtonStyle");
    if (!SbkKtCoreTypes[SBK_KATIE_QT_TOOLBUTTONSTYLE_IDX])
        return SBK_MODULE_INIT_ERROR;

    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_TOOLBUTTONSTYLE_IDX],
        module, "ToolButtonIconOnly", (long) Katie::Qt::ToolButtonStyle::ToolButtonIconOnly))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_TOOLBUTTONSTYLE_IDX],
        module, "ToolButtonTextOnly", (long) Katie::Qt::ToolButtonStyle::ToolButtonTextOnly))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_TOOLBUTTONSTYLE_IDX],
        module, "ToolButtonTextBesideIcon", (long) Katie::Qt::ToolButtonStyle::ToolButtonTextBesideIcon))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_TOOLBUTTONSTYLE_IDX],
        module, "ToolButtonTextUnderIcon", (long) Katie::Qt::ToolButtonStyle::ToolButtonTextUnderIcon))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_TOOLBUTTONSTYLE_IDX],
        module, "ToolButtonFollowStyle", (long) Katie::Qt::ToolButtonStyle::ToolButtonFollowStyle))
        return SBK_MODULE_INIT_ERROR;
    // Register converter for enum 'Katie::Qt::ToolButtonStyle'.
    {
        SbkConverter* converter = Shiboken::Conversions::createConverter(SbkKtCoreTypes[SBK_KATIE_QT_TOOLBUTTONSTYLE_IDX],
            Katie_Qt_ToolButtonStyle_CppToPython_Katie_Qt_ToolButtonStyle);
        Shiboken::Conversions::addPythonToCppValueConversion(converter,
            Katie_Qt_ToolButtonStyle_PythonToCpp_Katie_Qt_ToolButtonStyle,
            is_Katie_Qt_ToolButtonStyle_PythonToCpp_Katie_Qt_ToolButtonStyle_Convertible);
        Shiboken::Enum::setTypeConverter(SbkKtCoreTypes[SBK_KATIE_QT_TOOLBUTTONSTYLE_IDX], converter);
        Shiboken::Conversions::registerConverterName(converter, "Katie::Qt::ToolButtonStyle");
        Shiboken::Conversions::registerConverterName(converter, "Qt::ToolButtonStyle");
        Shiboken::Conversions::registerConverterName(converter, "ToolButtonStyle");
    }
    // End of 'ToolButtonStyle' enum.

    // Initialization of enum 'LayoutDirection'.
    SbkKtCoreTypes[SBK_KATIE_QT_LAYOUTDIRECTION_IDX] = Shiboken::Enum::createGlobalEnum(module,
        "LayoutDirection",
        "KtCore.Katie.Qt.LayoutDirection",
        "Katie::Qt::LayoutDirection");
    if (!SbkKtCoreTypes[SBK_KATIE_QT_LAYOUTDIRECTION_IDX])
        return SBK_MODULE_INIT_ERROR;

    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_LAYOUTDIRECTION_IDX],
        module, "LayoutDirectionAuto", (long) Katie::Qt::LayoutDirection::LayoutDirectionAuto))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_LAYOUTDIRECTION_IDX],
        module, "LeftToRight", (long) Katie::Qt::LayoutDirection::LeftToRight))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_LAYOUTDIRECTION_IDX],
        module, "RightToLeft", (long) Katie::Qt::LayoutDirection::RightToLeft))
        return SBK_MODULE_INIT_ERROR;
    // Register converter for enum 'Katie::Qt::LayoutDirection'.
    {
        SbkConverter* converter = Shiboken::Conversions::createConverter(SbkKtCoreTypes[SBK_KATIE_QT_LAYOUTDIRECTION_IDX],
            Katie_Qt_LayoutDirection_CppToPython_Katie_Qt_LayoutDirection);
        Shiboken::Conversions::addPythonToCppValueConversion(converter,
            Katie_Qt_LayoutDirection_PythonToCpp_Katie_Qt_LayoutDirection,
            is_Katie_Qt_LayoutDirection_PythonToCpp_Katie_Qt_LayoutDirection_Convertible);
        Shiboken::Enum::setTypeConverter(SbkKtCoreTypes[SBK_KATIE_QT_LAYOUTDIRECTION_IDX], converter);
        Shiboken::Conversions::registerConverterName(converter, "Katie::Qt::LayoutDirection");
        Shiboken::Conversions::registerConverterName(converter, "Qt::LayoutDirection");
        Shiboken::Conversions::registerConverterName(converter, "LayoutDirection");
    }
    // End of 'LayoutDirection' enum.

    // Initialization of enum 'AnchorPoint'.
    SbkKtCoreTypes[SBK_KATIE_QT_ANCHORPOINT_IDX] = Shiboken::Enum::createGlobalEnum(module,
        "AnchorPoint",
        "KtCore.Katie.Qt.AnchorPoint",
        "Katie::Qt::AnchorPoint");
    if (!SbkKtCoreTypes[SBK_KATIE_QT_ANCHORPOINT_IDX])
        return SBK_MODULE_INIT_ERROR;

    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_ANCHORPOINT_IDX],
        module, "AnchorLeft", (long) Katie::Qt::AnchorPoint::AnchorLeft))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_ANCHORPOINT_IDX],
        module, "AnchorHorizontalCenter", (long) Katie::Qt::AnchorPoint::AnchorHorizontalCenter))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_ANCHORPOINT_IDX],
        module, "AnchorRight", (long) Katie::Qt::AnchorPoint::AnchorRight))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_ANCHORPOINT_IDX],
        module, "AnchorTop", (long) Katie::Qt::AnchorPoint::AnchorTop))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_ANCHORPOINT_IDX],
        module, "AnchorVerticalCenter", (long) Katie::Qt::AnchorPoint::AnchorVerticalCenter))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_ANCHORPOINT_IDX],
        module, "AnchorBottom", (long) Katie::Qt::AnchorPoint::AnchorBottom))
        return SBK_MODULE_INIT_ERROR;
    // Register converter for enum 'Katie::Qt::AnchorPoint'.
    {
        SbkConverter* converter = Shiboken::Conversions::createConverter(SbkKtCoreTypes[SBK_KATIE_QT_ANCHORPOINT_IDX],
            Katie_Qt_AnchorPoint_CppToPython_Katie_Qt_AnchorPoint);
        Shiboken::Conversions::addPythonToCppValueConversion(converter,
            Katie_Qt_AnchorPoint_PythonToCpp_Katie_Qt_AnchorPoint,
            is_Katie_Qt_AnchorPoint_PythonToCpp_Katie_Qt_AnchorPoint_Convertible);
        Shiboken::Enum::setTypeConverter(SbkKtCoreTypes[SBK_KATIE_QT_ANCHORPOINT_IDX], converter);
        Shiboken::Conversions::registerConverterName(converter, "Katie::Qt::AnchorPoint");
        Shiboken::Conversions::registerConverterName(converter, "Qt::AnchorPoint");
        Shiboken::Conversions::registerConverterName(converter, "AnchorPoint");
    }
    // End of 'AnchorPoint' enum.

    // Initialization of enum 'DropAction'.
    SbkKtCoreTypes[SBK_KATIE_QT_DROPACTION_IDX] = Shiboken::Enum::createGlobalEnum(module,
        "DropAction",
        "KtCore.Katie.Qt.DropAction",
        "Katie::Qt::DropAction");
    if (!SbkKtCoreTypes[SBK_KATIE_QT_DROPACTION_IDX])
        return SBK_MODULE_INIT_ERROR;

    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_DROPACTION_IDX],
        module, "CopyAction", (long) Katie::Qt::DropAction::CopyAction))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_DROPACTION_IDX],
        module, "MoveAction", (long) Katie::Qt::DropAction::MoveAction))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_DROPACTION_IDX],
        module, "LinkAction", (long) Katie::Qt::DropAction::LinkAction))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_DROPACTION_IDX],
        module, "ActionMask", (long) Katie::Qt::DropAction::ActionMask))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_DROPACTION_IDX],
        module, "TargetMoveAction", (long) Katie::Qt::DropAction::TargetMoveAction))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_DROPACTION_IDX],
        module, "IgnoreAction", (long) Katie::Qt::DropAction::IgnoreAction))
        return SBK_MODULE_INIT_ERROR;
    // Register converter for enum 'Katie::Qt::DropAction'.
    {
        SbkConverter* converter = Shiboken::Conversions::createConverter(SbkKtCoreTypes[SBK_KATIE_QT_DROPACTION_IDX],
            Katie_Qt_DropAction_CppToPython_Katie_Qt_DropAction);
        Shiboken::Conversions::addPythonToCppValueConversion(converter,
            Katie_Qt_DropAction_PythonToCpp_Katie_Qt_DropAction,
            is_Katie_Qt_DropAction_PythonToCpp_Katie_Qt_DropAction_Convertible);
        Shiboken::Enum::setTypeConverter(SbkKtCoreTypes[SBK_KATIE_QT_DROPACTION_IDX], converter);
        Shiboken::Conversions::registerConverterName(converter, "Katie::Qt::DropAction");
        Shiboken::Conversions::registerConverterName(converter, "Qt::DropAction");
        Shiboken::Conversions::registerConverterName(converter, "DropAction");
    }
    // End of 'DropAction' enum.

    // Initialization of enum 'CheckState'.
    SbkKtCoreTypes[SBK_KATIE_QT_CHECKSTATE_IDX] = Shiboken::Enum::createGlobalEnum(module,
        "CheckState",
        "KtCore.Katie.Qt.CheckState",
        "Katie::Qt::CheckState");
    if (!SbkKtCoreTypes[SBK_KATIE_QT_CHECKSTATE_IDX])
        return SBK_MODULE_INIT_ERROR;

    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_CHECKSTATE_IDX],
        module, "Unchecked", (long) Katie::Qt::CheckState::Unchecked))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_CHECKSTATE_IDX],
        module, "PartiallyChecked", (long) Katie::Qt::CheckState::PartiallyChecked))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_CHECKSTATE_IDX],
        module, "Checked", (long) Katie::Qt::CheckState::Checked))
        return SBK_MODULE_INIT_ERROR;
    // Register converter for enum 'Katie::Qt::CheckState'.
    {
        SbkConverter* converter = Shiboken::Conversions::createConverter(SbkKtCoreTypes[SBK_KATIE_QT_CHECKSTATE_IDX],
            Katie_Qt_CheckState_CppToPython_Katie_Qt_CheckState);
        Shiboken::Conversions::addPythonToCppValueConversion(converter,
            Katie_Qt_CheckState_PythonToCpp_Katie_Qt_CheckState,
            is_Katie_Qt_CheckState_PythonToCpp_Katie_Qt_CheckState_Convertible);
        Shiboken::Enum::setTypeConverter(SbkKtCoreTypes[SBK_KATIE_QT_CHECKSTATE_IDX], converter);
        Shiboken::Conversions::registerConverterName(converter, "Katie::Qt::CheckState");
        Shiboken::Conversions::registerConverterName(converter, "Qt::CheckState");
        Shiboken::Conversions::registerConverterName(converter, "CheckState");
    }
    // End of 'CheckState' enum.

    // Initialization of enum 'ItemDataRole'.
    SbkKtCoreTypes[SBK_KATIE_QT_ITEMDATAROLE_IDX] = Shiboken::Enum::createGlobalEnum(module,
        "ItemDataRole",
        "KtCore.Katie.Qt.ItemDataRole",
        "Katie::Qt::ItemDataRole");
    if (!SbkKtCoreTypes[SBK_KATIE_QT_ITEMDATAROLE_IDX])
        return SBK_MODULE_INIT_ERROR;

    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_ITEMDATAROLE_IDX],
        module, "DisplayRole", (long) Katie::Qt::ItemDataRole::DisplayRole))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_ITEMDATAROLE_IDX],
        module, "DecorationRole", (long) Katie::Qt::ItemDataRole::DecorationRole))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_ITEMDATAROLE_IDX],
        module, "EditRole", (long) Katie::Qt::ItemDataRole::EditRole))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_ITEMDATAROLE_IDX],
        module, "ToolTipRole", (long) Katie::Qt::ItemDataRole::ToolTipRole))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_ITEMDATAROLE_IDX],
        module, "StatusTipRole", (long) Katie::Qt::ItemDataRole::StatusTipRole))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_ITEMDATAROLE_IDX],
        module, "WhatsThisRole", (long) Katie::Qt::ItemDataRole::WhatsThisRole))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_ITEMDATAROLE_IDX],
        module, "FontRole", (long) Katie::Qt::ItemDataRole::FontRole))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_ITEMDATAROLE_IDX],
        module, "TextAlignmentRole", (long) Katie::Qt::ItemDataRole::TextAlignmentRole))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_ITEMDATAROLE_IDX],
        module, "BackgroundRole", (long) Katie::Qt::ItemDataRole::BackgroundRole))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_ITEMDATAROLE_IDX],
        module, "ForegroundRole", (long) Katie::Qt::ItemDataRole::ForegroundRole))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_ITEMDATAROLE_IDX],
        module, "CheckStateRole", (long) Katie::Qt::ItemDataRole::CheckStateRole))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_ITEMDATAROLE_IDX],
        module, "AccessibleTextRole", (long) Katie::Qt::ItemDataRole::AccessibleTextRole))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_ITEMDATAROLE_IDX],
        module, "AccessibleDescriptionRole", (long) Katie::Qt::ItemDataRole::AccessibleDescriptionRole))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_ITEMDATAROLE_IDX],
        module, "SizeHintRole", (long) Katie::Qt::ItemDataRole::SizeHintRole))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_ITEMDATAROLE_IDX],
        module, "InitialSortOrderRole", (long) Katie::Qt::ItemDataRole::InitialSortOrderRole))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_ITEMDATAROLE_IDX],
        module, "DisplayPropertyRole", (long) Katie::Qt::ItemDataRole::DisplayPropertyRole))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_ITEMDATAROLE_IDX],
        module, "DecorationPropertyRole", (long) Katie::Qt::ItemDataRole::DecorationPropertyRole))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_ITEMDATAROLE_IDX],
        module, "ToolTipPropertyRole", (long) Katie::Qt::ItemDataRole::ToolTipPropertyRole))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_ITEMDATAROLE_IDX],
        module, "StatusTipPropertyRole", (long) Katie::Qt::ItemDataRole::StatusTipPropertyRole))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_ITEMDATAROLE_IDX],
        module, "WhatsThisPropertyRole", (long) Katie::Qt::ItemDataRole::WhatsThisPropertyRole))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_ITEMDATAROLE_IDX],
        module, "UserRole", (long) Katie::Qt::ItemDataRole::UserRole))
        return SBK_MODULE_INIT_ERROR;
    // Register converter for enum 'Katie::Qt::ItemDataRole'.
    {
        SbkConverter* converter = Shiboken::Conversions::createConverter(SbkKtCoreTypes[SBK_KATIE_QT_ITEMDATAROLE_IDX],
            Katie_Qt_ItemDataRole_CppToPython_Katie_Qt_ItemDataRole);
        Shiboken::Conversions::addPythonToCppValueConversion(converter,
            Katie_Qt_ItemDataRole_PythonToCpp_Katie_Qt_ItemDataRole,
            is_Katie_Qt_ItemDataRole_PythonToCpp_Katie_Qt_ItemDataRole_Convertible);
        Shiboken::Enum::setTypeConverter(SbkKtCoreTypes[SBK_KATIE_QT_ITEMDATAROLE_IDX], converter);
        Shiboken::Conversions::registerConverterName(converter, "Katie::Qt::ItemDataRole");
        Shiboken::Conversions::registerConverterName(converter, "Qt::ItemDataRole");
        Shiboken::Conversions::registerConverterName(converter, "ItemDataRole");
    }
    // End of 'ItemDataRole' enum.

    // Initialization of enum 'ItemFlag'.
    SbkKtCoreTypes[SBK_KATIE_QT_ITEMFLAG_IDX] = Shiboken::Enum::createGlobalEnum(module,
        "ItemFlag",
        "KtCore.Katie.Qt.ItemFlag",
        "Katie::Qt::ItemFlag");
    if (!SbkKtCoreTypes[SBK_KATIE_QT_ITEMFLAG_IDX])
        return SBK_MODULE_INIT_ERROR;

    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_ITEMFLAG_IDX],
        module, "NoItemFlags", (long) Katie::Qt::ItemFlag::NoItemFlags))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_ITEMFLAG_IDX],
        module, "ItemIsSelectable", (long) Katie::Qt::ItemFlag::ItemIsSelectable))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_ITEMFLAG_IDX],
        module, "ItemIsEditable", (long) Katie::Qt::ItemFlag::ItemIsEditable))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_ITEMFLAG_IDX],
        module, "ItemIsDragEnabled", (long) Katie::Qt::ItemFlag::ItemIsDragEnabled))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_ITEMFLAG_IDX],
        module, "ItemIsDropEnabled", (long) Katie::Qt::ItemFlag::ItemIsDropEnabled))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_ITEMFLAG_IDX],
        module, "ItemIsUserCheckable", (long) Katie::Qt::ItemFlag::ItemIsUserCheckable))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_ITEMFLAG_IDX],
        module, "ItemIsEnabled", (long) Katie::Qt::ItemFlag::ItemIsEnabled))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_ITEMFLAG_IDX],
        module, "ItemIsTristate", (long) Katie::Qt::ItemFlag::ItemIsTristate))
        return SBK_MODULE_INIT_ERROR;
    // Register converter for enum 'Katie::Qt::ItemFlag'.
    {
        SbkConverter* converter = Shiboken::Conversions::createConverter(SbkKtCoreTypes[SBK_KATIE_QT_ITEMFLAG_IDX],
            Katie_Qt_ItemFlag_CppToPython_Katie_Qt_ItemFlag);
        Shiboken::Conversions::addPythonToCppValueConversion(converter,
            Katie_Qt_ItemFlag_PythonToCpp_Katie_Qt_ItemFlag,
            is_Katie_Qt_ItemFlag_PythonToCpp_Katie_Qt_ItemFlag_Convertible);
        Shiboken::Enum::setTypeConverter(SbkKtCoreTypes[SBK_KATIE_QT_ITEMFLAG_IDX], converter);
        Shiboken::Conversions::registerConverterName(converter, "Katie::Qt::ItemFlag");
        Shiboken::Conversions::registerConverterName(converter, "Qt::ItemFlag");
        Shiboken::Conversions::registerConverterName(converter, "ItemFlag");
    }
    // End of 'ItemFlag' enum.

    // Initialization of enum 'MatchFlag'.
    SbkKtCoreTypes[SBK_KATIE_QT_MATCHFLAG_IDX] = Shiboken::Enum::createGlobalEnum(module,
        "MatchFlag",
        "KtCore.Katie.Qt.MatchFlag",
        "Katie::Qt::MatchFlag");
    if (!SbkKtCoreTypes[SBK_KATIE_QT_MATCHFLAG_IDX])
        return SBK_MODULE_INIT_ERROR;

    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_MATCHFLAG_IDX],
        module, "MatchExactly", (long) Katie::Qt::MatchFlag::MatchExactly))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_MATCHFLAG_IDX],
        module, "MatchContains", (long) Katie::Qt::MatchFlag::MatchContains))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_MATCHFLAG_IDX],
        module, "MatchStartsWith", (long) Katie::Qt::MatchFlag::MatchStartsWith))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_MATCHFLAG_IDX],
        module, "MatchEndsWith", (long) Katie::Qt::MatchFlag::MatchEndsWith))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_MATCHFLAG_IDX],
        module, "MatchRegExp", (long) Katie::Qt::MatchFlag::MatchRegExp))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_MATCHFLAG_IDX],
        module, "MatchWildcard", (long) Katie::Qt::MatchFlag::MatchWildcard))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_MATCHFLAG_IDX],
        module, "MatchFixedString", (long) Katie::Qt::MatchFlag::MatchFixedString))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_MATCHFLAG_IDX],
        module, "MatchCaseSensitive", (long) Katie::Qt::MatchFlag::MatchCaseSensitive))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_MATCHFLAG_IDX],
        module, "MatchWrap", (long) Katie::Qt::MatchFlag::MatchWrap))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_MATCHFLAG_IDX],
        module, "MatchRecursive", (long) Katie::Qt::MatchFlag::MatchRecursive))
        return SBK_MODULE_INIT_ERROR;
    // Register converter for enum 'Katie::Qt::MatchFlag'.
    {
        SbkConverter* converter = Shiboken::Conversions::createConverter(SbkKtCoreTypes[SBK_KATIE_QT_MATCHFLAG_IDX],
            Katie_Qt_MatchFlag_CppToPython_Katie_Qt_MatchFlag);
        Shiboken::Conversions::addPythonToCppValueConversion(converter,
            Katie_Qt_MatchFlag_PythonToCpp_Katie_Qt_MatchFlag,
            is_Katie_Qt_MatchFlag_PythonToCpp_Katie_Qt_MatchFlag_Convertible);
        Shiboken::Enum::setTypeConverter(SbkKtCoreTypes[SBK_KATIE_QT_MATCHFLAG_IDX], converter);
        Shiboken::Conversions::registerConverterName(converter, "Katie::Qt::MatchFlag");
        Shiboken::Conversions::registerConverterName(converter, "Qt::MatchFlag");
        Shiboken::Conversions::registerConverterName(converter, "MatchFlag");
    }
    // End of 'MatchFlag' enum.

    // Initialization of enum 'WindowModality'.
    SbkKtCoreTypes[SBK_KATIE_QT_WINDOWMODALITY_IDX] = Shiboken::Enum::createGlobalEnum(module,
        "WindowModality",
        "KtCore.Katie.Qt.WindowModality",
        "Katie::Qt::WindowModality");
    if (!SbkKtCoreTypes[SBK_KATIE_QT_WINDOWMODALITY_IDX])
        return SBK_MODULE_INIT_ERROR;

    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WINDOWMODALITY_IDX],
        module, "NonModal", (long) Katie::Qt::WindowModality::NonModal))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WINDOWMODALITY_IDX],
        module, "WindowModal", (long) Katie::Qt::WindowModality::WindowModal))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WINDOWMODALITY_IDX],
        module, "ApplicationModal", (long) Katie::Qt::WindowModality::ApplicationModal))
        return SBK_MODULE_INIT_ERROR;
    // Register converter for enum 'Katie::Qt::WindowModality'.
    {
        SbkConverter* converter = Shiboken::Conversions::createConverter(SbkKtCoreTypes[SBK_KATIE_QT_WINDOWMODALITY_IDX],
            Katie_Qt_WindowModality_CppToPython_Katie_Qt_WindowModality);
        Shiboken::Conversions::addPythonToCppValueConversion(converter,
            Katie_Qt_WindowModality_PythonToCpp_Katie_Qt_WindowModality,
            is_Katie_Qt_WindowModality_PythonToCpp_Katie_Qt_WindowModality_Convertible);
        Shiboken::Enum::setTypeConverter(SbkKtCoreTypes[SBK_KATIE_QT_WINDOWMODALITY_IDX], converter);
        Shiboken::Conversions::registerConverterName(converter, "Katie::Qt::WindowModality");
        Shiboken::Conversions::registerConverterName(converter, "Qt::WindowModality");
        Shiboken::Conversions::registerConverterName(converter, "WindowModality");
    }
    // End of 'WindowModality' enum.

    // Initialization of enum 'TextInteractionFlag'.
    SbkKtCoreTypes[SBK_KATIE_QT_TEXTINTERACTIONFLAG_IDX] = Shiboken::Enum::createGlobalEnum(module,
        "TextInteractionFlag",
        "KtCore.Katie.Qt.TextInteractionFlag",
        "Katie::Qt::TextInteractionFlag");
    if (!SbkKtCoreTypes[SBK_KATIE_QT_TEXTINTERACTIONFLAG_IDX])
        return SBK_MODULE_INIT_ERROR;

    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_TEXTINTERACTIONFLAG_IDX],
        module, "NoTextInteraction", (long) Katie::Qt::TextInteractionFlag::NoTextInteraction))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_TEXTINTERACTIONFLAG_IDX],
        module, "TextSelectableByMouse", (long) Katie::Qt::TextInteractionFlag::TextSelectableByMouse))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_TEXTINTERACTIONFLAG_IDX],
        module, "TextSelectableByKeyboard", (long) Katie::Qt::TextInteractionFlag::TextSelectableByKeyboard))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_TEXTINTERACTIONFLAG_IDX],
        module, "LinksAccessibleByMouse", (long) Katie::Qt::TextInteractionFlag::LinksAccessibleByMouse))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_TEXTINTERACTIONFLAG_IDX],
        module, "LinksAccessibleByKeyboard", (long) Katie::Qt::TextInteractionFlag::LinksAccessibleByKeyboard))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_TEXTINTERACTIONFLAG_IDX],
        module, "TextEditable", (long) Katie::Qt::TextInteractionFlag::TextEditable))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_TEXTINTERACTIONFLAG_IDX],
        module, "TextEditorInteraction", (long) Katie::Qt::TextInteractionFlag::TextEditorInteraction))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_TEXTINTERACTIONFLAG_IDX],
        module, "TextBrowserInteraction", (long) Katie::Qt::TextInteractionFlag::TextBrowserInteraction))
        return SBK_MODULE_INIT_ERROR;
    // Register converter for enum 'Katie::Qt::TextInteractionFlag'.
    {
        SbkConverter* converter = Shiboken::Conversions::createConverter(SbkKtCoreTypes[SBK_KATIE_QT_TEXTINTERACTIONFLAG_IDX],
            Katie_Qt_TextInteractionFlag_CppToPython_Katie_Qt_TextInteractionFlag);
        Shiboken::Conversions::addPythonToCppValueConversion(converter,
            Katie_Qt_TextInteractionFlag_PythonToCpp_Katie_Qt_TextInteractionFlag,
            is_Katie_Qt_TextInteractionFlag_PythonToCpp_Katie_Qt_TextInteractionFlag_Convertible);
        Shiboken::Enum::setTypeConverter(SbkKtCoreTypes[SBK_KATIE_QT_TEXTINTERACTIONFLAG_IDX], converter);
        Shiboken::Conversions::registerConverterName(converter, "Katie::Qt::TextInteractionFlag");
        Shiboken::Conversions::registerConverterName(converter, "Qt::TextInteractionFlag");
        Shiboken::Conversions::registerConverterName(converter, "TextInteractionFlag");
    }
    // End of 'TextInteractionFlag' enum.

    // Initialization of enum 'EventPriority'.
    SbkKtCoreTypes[SBK_KATIE_QT_EVENTPRIORITY_IDX] = Shiboken::Enum::createGlobalEnum(module,
        "EventPriority",
        "KtCore.Katie.Qt.EventPriority",
        "Katie::Qt::EventPriority");
    if (!SbkKtCoreTypes[SBK_KATIE_QT_EVENTPRIORITY_IDX])
        return SBK_MODULE_INIT_ERROR;

    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_EVENTPRIORITY_IDX],
        module, "HighEventPriority", (long) Katie::Qt::EventPriority::HighEventPriority))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_EVENTPRIORITY_IDX],
        module, "NormalEventPriority", (long) Katie::Qt::EventPriority::NormalEventPriority))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_EVENTPRIORITY_IDX],
        module, "LowEventPriority", (long) Katie::Qt::EventPriority::LowEventPriority))
        return SBK_MODULE_INIT_ERROR;
    // Register converter for enum 'Katie::Qt::EventPriority'.
    {
        SbkConverter* converter = Shiboken::Conversions::createConverter(SbkKtCoreTypes[SBK_KATIE_QT_EVENTPRIORITY_IDX],
            Katie_Qt_EventPriority_CppToPython_Katie_Qt_EventPriority);
        Shiboken::Conversions::addPythonToCppValueConversion(converter,
            Katie_Qt_EventPriority_PythonToCpp_Katie_Qt_EventPriority,
            is_Katie_Qt_EventPriority_PythonToCpp_Katie_Qt_EventPriority_Convertible);
        Shiboken::Enum::setTypeConverter(SbkKtCoreTypes[SBK_KATIE_QT_EVENTPRIORITY_IDX], converter);
        Shiboken::Conversions::registerConverterName(converter, "Katie::Qt::EventPriority");
        Shiboken::Conversions::registerConverterName(converter, "Qt::EventPriority");
        Shiboken::Conversions::registerConverterName(converter, "EventPriority");
    }
    // End of 'EventPriority' enum.

    // Initialization of enum 'SizeHint'.
    SbkKtCoreTypes[SBK_KATIE_QT_SIZEHINT_IDX] = Shiboken::Enum::createGlobalEnum(module,
        "SizeHint",
        "KtCore.Katie.Qt.SizeHint",
        "Katie::Qt::SizeHint");
    if (!SbkKtCoreTypes[SBK_KATIE_QT_SIZEHINT_IDX])
        return SBK_MODULE_INIT_ERROR;

    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_SIZEHINT_IDX],
        module, "MinimumSize", (long) Katie::Qt::SizeHint::MinimumSize))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_SIZEHINT_IDX],
        module, "PreferredSize", (long) Katie::Qt::SizeHint::PreferredSize))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_SIZEHINT_IDX],
        module, "MaximumSize", (long) Katie::Qt::SizeHint::MaximumSize))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_SIZEHINT_IDX],
        module, "MinimumDescent", (long) Katie::Qt::SizeHint::MinimumDescent))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_SIZEHINT_IDX],
        module, "NSizeHints", (long) Katie::Qt::SizeHint::NSizeHints))
        return SBK_MODULE_INIT_ERROR;
    // Register converter for enum 'Katie::Qt::SizeHint'.
    {
        SbkConverter* converter = Shiboken::Conversions::createConverter(SbkKtCoreTypes[SBK_KATIE_QT_SIZEHINT_IDX],
            Katie_Qt_SizeHint_CppToPython_Katie_Qt_SizeHint);
        Shiboken::Conversions::addPythonToCppValueConversion(converter,
            Katie_Qt_SizeHint_PythonToCpp_Katie_Qt_SizeHint,
            is_Katie_Qt_SizeHint_PythonToCpp_Katie_Qt_SizeHint_Convertible);
        Shiboken::Enum::setTypeConverter(SbkKtCoreTypes[SBK_KATIE_QT_SIZEHINT_IDX], converter);
        Shiboken::Conversions::registerConverterName(converter, "Katie::Qt::SizeHint");
        Shiboken::Conversions::registerConverterName(converter, "Qt::SizeHint");
        Shiboken::Conversions::registerConverterName(converter, "SizeHint");
    }
    // End of 'SizeHint' enum.

    // Initialization of enum 'WindowFrameSection'.
    SbkKtCoreTypes[SBK_KATIE_QT_WINDOWFRAMESECTION_IDX] = Shiboken::Enum::createGlobalEnum(module,
        "WindowFrameSection",
        "KtCore.Katie.Qt.WindowFrameSection",
        "Katie::Qt::WindowFrameSection");
    if (!SbkKtCoreTypes[SBK_KATIE_QT_WINDOWFRAMESECTION_IDX])
        return SBK_MODULE_INIT_ERROR;

    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WINDOWFRAMESECTION_IDX],
        module, "NoSection", (long) Katie::Qt::WindowFrameSection::NoSection))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WINDOWFRAMESECTION_IDX],
        module, "LeftSection", (long) Katie::Qt::WindowFrameSection::LeftSection))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WINDOWFRAMESECTION_IDX],
        module, "TopLeftSection", (long) Katie::Qt::WindowFrameSection::TopLeftSection))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WINDOWFRAMESECTION_IDX],
        module, "TopSection", (long) Katie::Qt::WindowFrameSection::TopSection))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WINDOWFRAMESECTION_IDX],
        module, "TopRightSection", (long) Katie::Qt::WindowFrameSection::TopRightSection))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WINDOWFRAMESECTION_IDX],
        module, "RightSection", (long) Katie::Qt::WindowFrameSection::RightSection))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WINDOWFRAMESECTION_IDX],
        module, "BottomRightSection", (long) Katie::Qt::WindowFrameSection::BottomRightSection))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WINDOWFRAMESECTION_IDX],
        module, "BottomSection", (long) Katie::Qt::WindowFrameSection::BottomSection))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WINDOWFRAMESECTION_IDX],
        module, "BottomLeftSection", (long) Katie::Qt::WindowFrameSection::BottomLeftSection))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_WINDOWFRAMESECTION_IDX],
        module, "TitleBarArea", (long) Katie::Qt::WindowFrameSection::TitleBarArea))
        return SBK_MODULE_INIT_ERROR;
    // Register converter for enum 'Katie::Qt::WindowFrameSection'.
    {
        SbkConverter* converter = Shiboken::Conversions::createConverter(SbkKtCoreTypes[SBK_KATIE_QT_WINDOWFRAMESECTION_IDX],
            Katie_Qt_WindowFrameSection_CppToPython_Katie_Qt_WindowFrameSection);
        Shiboken::Conversions::addPythonToCppValueConversion(converter,
            Katie_Qt_WindowFrameSection_PythonToCpp_Katie_Qt_WindowFrameSection,
            is_Katie_Qt_WindowFrameSection_PythonToCpp_Katie_Qt_WindowFrameSection_Convertible);
        Shiboken::Enum::setTypeConverter(SbkKtCoreTypes[SBK_KATIE_QT_WINDOWFRAMESECTION_IDX], converter);
        Shiboken::Conversions::registerConverterName(converter, "Katie::Qt::WindowFrameSection");
        Shiboken::Conversions::registerConverterName(converter, "Qt::WindowFrameSection");
        Shiboken::Conversions::registerConverterName(converter, "WindowFrameSection");
    }
    // End of 'WindowFrameSection' enum.

    // Initialization of enum 'Initialization'.
    SbkKtCoreTypes[SBK_KATIE_QT_INITIALIZATION_IDX] = Shiboken::Enum::createGlobalEnum(module,
        "Initialization",
        "KtCore.Katie.Qt.Initialization",
        "Katie::Qt::Initialization");
    if (!SbkKtCoreTypes[SBK_KATIE_QT_INITIALIZATION_IDX])
        return SBK_MODULE_INIT_ERROR;

    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_INITIALIZATION_IDX],
        module, "Uninitialized", (long) Katie::Qt::Initialization::Uninitialized))
        return SBK_MODULE_INIT_ERROR;
    // Register converter for enum 'Katie::Qt::Initialization'.
    {
        SbkConverter* converter = Shiboken::Conversions::createConverter(SbkKtCoreTypes[SBK_KATIE_QT_INITIALIZATION_IDX],
            Katie_Qt_Initialization_CppToPython_Katie_Qt_Initialization);
        Shiboken::Conversions::addPythonToCppValueConversion(converter,
            Katie_Qt_Initialization_PythonToCpp_Katie_Qt_Initialization,
            is_Katie_Qt_Initialization_PythonToCpp_Katie_Qt_Initialization_Convertible);
        Shiboken::Enum::setTypeConverter(SbkKtCoreTypes[SBK_KATIE_QT_INITIALIZATION_IDX], converter);
        Shiboken::Conversions::registerConverterName(converter, "Katie::Qt::Initialization");
        Shiboken::Conversions::registerConverterName(converter, "Qt::Initialization");
        Shiboken::Conversions::registerConverterName(converter, "Initialization");
    }
    // End of 'Initialization' enum.

    // Initialization of enum 'CoordinateSystem'.
    SbkKtCoreTypes[SBK_KATIE_QT_COORDINATESYSTEM_IDX] = Shiboken::Enum::createGlobalEnum(module,
        "CoordinateSystem",
        "KtCore.Katie.Qt.CoordinateSystem",
        "Katie::Qt::CoordinateSystem");
    if (!SbkKtCoreTypes[SBK_KATIE_QT_COORDINATESYSTEM_IDX])
        return SBK_MODULE_INIT_ERROR;

    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_COORDINATESYSTEM_IDX],
        module, "DeviceCoordinates", (long) Katie::Qt::CoordinateSystem::DeviceCoordinates))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_COORDINATESYSTEM_IDX],
        module, "LogicalCoordinates", (long) Katie::Qt::CoordinateSystem::LogicalCoordinates))
        return SBK_MODULE_INIT_ERROR;
    // Register converter for enum 'Katie::Qt::CoordinateSystem'.
    {
        SbkConverter* converter = Shiboken::Conversions::createConverter(SbkKtCoreTypes[SBK_KATIE_QT_COORDINATESYSTEM_IDX],
            Katie_Qt_CoordinateSystem_CppToPython_Katie_Qt_CoordinateSystem);
        Shiboken::Conversions::addPythonToCppValueConversion(converter,
            Katie_Qt_CoordinateSystem_PythonToCpp_Katie_Qt_CoordinateSystem,
            is_Katie_Qt_CoordinateSystem_PythonToCpp_Katie_Qt_CoordinateSystem_Convertible);
        Shiboken::Enum::setTypeConverter(SbkKtCoreTypes[SBK_KATIE_QT_COORDINATESYSTEM_IDX], converter);
        Shiboken::Conversions::registerConverterName(converter, "Katie::Qt::CoordinateSystem");
        Shiboken::Conversions::registerConverterName(converter, "Qt::CoordinateSystem");
        Shiboken::Conversions::registerConverterName(converter, "CoordinateSystem");
    }
    // End of 'CoordinateSystem' enum.

    // Initialization of enum 'TouchPointState'.
    SbkKtCoreTypes[SBK_KATIE_QT_TOUCHPOINTSTATE_IDX] = Shiboken::Enum::createGlobalEnum(module,
        "TouchPointState",
        "KtCore.Katie.Qt.TouchPointState",
        "Katie::Qt::TouchPointState");
    if (!SbkKtCoreTypes[SBK_KATIE_QT_TOUCHPOINTSTATE_IDX])
        return SBK_MODULE_INIT_ERROR;

    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_TOUCHPOINTSTATE_IDX],
        module, "TouchPointPressed", (long) Katie::Qt::TouchPointState::TouchPointPressed))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_TOUCHPOINTSTATE_IDX],
        module, "TouchPointMoved", (long) Katie::Qt::TouchPointState::TouchPointMoved))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_TOUCHPOINTSTATE_IDX],
        module, "TouchPointStationary", (long) Katie::Qt::TouchPointState::TouchPointStationary))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_TOUCHPOINTSTATE_IDX],
        module, "TouchPointReleased", (long) Katie::Qt::TouchPointState::TouchPointReleased))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_TOUCHPOINTSTATE_IDX],
        module, "TouchPointStateMask", (long) Katie::Qt::TouchPointState::TouchPointStateMask))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_TOUCHPOINTSTATE_IDX],
        module, "TouchPointPrimary", (long) Katie::Qt::TouchPointState::TouchPointPrimary))
        return SBK_MODULE_INIT_ERROR;
    // Register converter for enum 'Katie::Qt::TouchPointState'.
    {
        SbkConverter* converter = Shiboken::Conversions::createConverter(SbkKtCoreTypes[SBK_KATIE_QT_TOUCHPOINTSTATE_IDX],
            Katie_Qt_TouchPointState_CppToPython_Katie_Qt_TouchPointState);
        Shiboken::Conversions::addPythonToCppValueConversion(converter,
            Katie_Qt_TouchPointState_PythonToCpp_Katie_Qt_TouchPointState,
            is_Katie_Qt_TouchPointState_PythonToCpp_Katie_Qt_TouchPointState_Convertible);
        Shiboken::Enum::setTypeConverter(SbkKtCoreTypes[SBK_KATIE_QT_TOUCHPOINTSTATE_IDX], converter);
        Shiboken::Conversions::registerConverterName(converter, "Katie::Qt::TouchPointState");
        Shiboken::Conversions::registerConverterName(converter, "Qt::TouchPointState");
        Shiboken::Conversions::registerConverterName(converter, "TouchPointState");
    }
    // End of 'TouchPointState' enum.

    // Initialization of enum 'GestureState'.
    SbkKtCoreTypes[SBK_KATIE_QT_GESTURESTATE_IDX] = Shiboken::Enum::createGlobalEnum(module,
        "GestureState",
        "KtCore.Katie.Qt.GestureState",
        "Katie::Qt::GestureState");
    if (!SbkKtCoreTypes[SBK_KATIE_QT_GESTURESTATE_IDX])
        return SBK_MODULE_INIT_ERROR;

    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_GESTURESTATE_IDX],
        module, "NoGesture", (long) Katie::Qt::GestureState::NoGesture))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_GESTURESTATE_IDX],
        module, "GestureStarted", (long) Katie::Qt::GestureState::GestureStarted))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_GESTURESTATE_IDX],
        module, "GestureUpdated", (long) Katie::Qt::GestureState::GestureUpdated))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_GESTURESTATE_IDX],
        module, "GestureFinished", (long) Katie::Qt::GestureState::GestureFinished))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_GESTURESTATE_IDX],
        module, "GestureCanceled", (long) Katie::Qt::GestureState::GestureCanceled))
        return SBK_MODULE_INIT_ERROR;
    // Register converter for enum 'Katie::Qt::GestureState'.
    {
        SbkConverter* converter = Shiboken::Conversions::createConverter(SbkKtCoreTypes[SBK_KATIE_QT_GESTURESTATE_IDX],
            Katie_Qt_GestureState_CppToPython_Katie_Qt_GestureState);
        Shiboken::Conversions::addPythonToCppValueConversion(converter,
            Katie_Qt_GestureState_PythonToCpp_Katie_Qt_GestureState,
            is_Katie_Qt_GestureState_PythonToCpp_Katie_Qt_GestureState_Convertible);
        Shiboken::Enum::setTypeConverter(SbkKtCoreTypes[SBK_KATIE_QT_GESTURESTATE_IDX], converter);
        Shiboken::Conversions::registerConverterName(converter, "Katie::Qt::GestureState");
        Shiboken::Conversions::registerConverterName(converter, "Qt::GestureState");
        Shiboken::Conversions::registerConverterName(converter, "GestureState");
    }
    // End of 'GestureState' enum.

    // Initialization of enum 'GestureType'.
    SbkKtCoreTypes[SBK_KATIE_QT_GESTURETYPE_IDX] = Shiboken::Enum::createGlobalEnum(module,
        "GestureType",
        "KtCore.Katie.Qt.GestureType",
        "Katie::Qt::GestureType");
    if (!SbkKtCoreTypes[SBK_KATIE_QT_GESTURETYPE_IDX])
        return SBK_MODULE_INIT_ERROR;

    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_GESTURETYPE_IDX],
        module, "TapGesture", (long) Katie::Qt::GestureType::TapGesture))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_GESTURETYPE_IDX],
        module, "TapAndHoldGesture", (long) Katie::Qt::GestureType::TapAndHoldGesture))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_GESTURETYPE_IDX],
        module, "PanGesture", (long) Katie::Qt::GestureType::PanGesture))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_GESTURETYPE_IDX],
        module, "PinchGesture", (long) Katie::Qt::GestureType::PinchGesture))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_GESTURETYPE_IDX],
        module, "SwipeGesture", (long) Katie::Qt::GestureType::SwipeGesture))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_GESTURETYPE_IDX],
        module, "CustomGesture", (long) Katie::Qt::GestureType::CustomGesture))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_GESTURETYPE_IDX],
        module, "LastGestureType", (long) Katie::Qt::GestureType::LastGestureType))
        return SBK_MODULE_INIT_ERROR;
    // Register converter for enum 'Katie::Qt::GestureType'.
    {
        SbkConverter* converter = Shiboken::Conversions::createConverter(SbkKtCoreTypes[SBK_KATIE_QT_GESTURETYPE_IDX],
            Katie_Qt_GestureType_CppToPython_Katie_Qt_GestureType);
        Shiboken::Conversions::addPythonToCppValueConversion(converter,
            Katie_Qt_GestureType_PythonToCpp_Katie_Qt_GestureType,
            is_Katie_Qt_GestureType_PythonToCpp_Katie_Qt_GestureType_Convertible);
        Shiboken::Enum::setTypeConverter(SbkKtCoreTypes[SBK_KATIE_QT_GESTURETYPE_IDX], converter);
        Shiboken::Conversions::registerConverterName(converter, "Katie::Qt::GestureType");
        Shiboken::Conversions::registerConverterName(converter, "Qt::GestureType");
        Shiboken::Conversions::registerConverterName(converter, "GestureType");
    }
    // End of 'GestureType' enum.

    // Initialization of enum 'GestureFlag'.
    SbkKtCoreTypes[SBK_KATIE_QT_GESTUREFLAG_IDX] = Shiboken::Enum::createGlobalEnum(module,
        "GestureFlag",
        "KtCore.Katie.Qt.GestureFlag",
        "Katie::Qt::GestureFlag");
    if (!SbkKtCoreTypes[SBK_KATIE_QT_GESTUREFLAG_IDX])
        return SBK_MODULE_INIT_ERROR;

    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_GESTUREFLAG_IDX],
        module, "DontStartGestureOnChildren", (long) Katie::Qt::GestureFlag::DontStartGestureOnChildren))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_GESTUREFLAG_IDX],
        module, "ReceivePartialGestures", (long) Katie::Qt::GestureFlag::ReceivePartialGestures))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_GESTUREFLAG_IDX],
        module, "IgnoredGesturesPropagateToParent", (long) Katie::Qt::GestureFlag::IgnoredGesturesPropagateToParent))
        return SBK_MODULE_INIT_ERROR;
    // Register converter for enum 'Katie::Qt::GestureFlag'.
    {
        SbkConverter* converter = Shiboken::Conversions::createConverter(SbkKtCoreTypes[SBK_KATIE_QT_GESTUREFLAG_IDX],
            Katie_Qt_GestureFlag_CppToPython_Katie_Qt_GestureFlag);
        Shiboken::Conversions::addPythonToCppValueConversion(converter,
            Katie_Qt_GestureFlag_PythonToCpp_Katie_Qt_GestureFlag,
            is_Katie_Qt_GestureFlag_PythonToCpp_Katie_Qt_GestureFlag_Convertible);
        Shiboken::Enum::setTypeConverter(SbkKtCoreTypes[SBK_KATIE_QT_GESTUREFLAG_IDX], converter);
        Shiboken::Conversions::registerConverterName(converter, "Katie::Qt::GestureFlag");
        Shiboken::Conversions::registerConverterName(converter, "Qt::GestureFlag");
        Shiboken::Conversions::registerConverterName(converter, "GestureFlag");
    }
    // End of 'GestureFlag' enum.

    // Initialization of enum 'NavigationMode'.
    SbkKtCoreTypes[SBK_KATIE_QT_NAVIGATIONMODE_IDX] = Shiboken::Enum::createGlobalEnum(module,
        "NavigationMode",
        "KtCore.Katie.Qt.NavigationMode",
        "Katie::Qt::NavigationMode");
    if (!SbkKtCoreTypes[SBK_KATIE_QT_NAVIGATIONMODE_IDX])
        return SBK_MODULE_INIT_ERROR;

    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_NAVIGATIONMODE_IDX],
        module, "NavigationModeNone", (long) Katie::Qt::NavigationMode::NavigationModeNone))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_NAVIGATIONMODE_IDX],
        module, "NavigationModeKeypadTabOrder", (long) Katie::Qt::NavigationMode::NavigationModeKeypadTabOrder))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_NAVIGATIONMODE_IDX],
        module, "NavigationModeKeypadDirectional", (long) Katie::Qt::NavigationMode::NavigationModeKeypadDirectional))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_NAVIGATIONMODE_IDX],
        module, "NavigationModeCursorAuto", (long) Katie::Qt::NavigationMode::NavigationModeCursorAuto))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_NAVIGATIONMODE_IDX],
        module, "NavigationModeCursorForceVisible", (long) Katie::Qt::NavigationMode::NavigationModeCursorForceVisible))
        return SBK_MODULE_INIT_ERROR;
    // Register converter for enum 'Katie::Qt::NavigationMode'.
    {
        SbkConverter* converter = Shiboken::Conversions::createConverter(SbkKtCoreTypes[SBK_KATIE_QT_NAVIGATIONMODE_IDX],
            Katie_Qt_NavigationMode_CppToPython_Katie_Qt_NavigationMode);
        Shiboken::Conversions::addPythonToCppValueConversion(converter,
            Katie_Qt_NavigationMode_PythonToCpp_Katie_Qt_NavigationMode,
            is_Katie_Qt_NavigationMode_PythonToCpp_Katie_Qt_NavigationMode_Convertible);
        Shiboken::Enum::setTypeConverter(SbkKtCoreTypes[SBK_KATIE_QT_NAVIGATIONMODE_IDX], converter);
        Shiboken::Conversions::registerConverterName(converter, "Katie::Qt::NavigationMode");
        Shiboken::Conversions::registerConverterName(converter, "Qt::NavigationMode");
        Shiboken::Conversions::registerConverterName(converter, "NavigationMode");
    }
    // End of 'NavigationMode' enum.

    // Initialization of enum 'CursorMoveStyle'.
    SbkKtCoreTypes[SBK_KATIE_QT_CURSORMOVESTYLE_IDX] = Shiboken::Enum::createGlobalEnum(module,
        "CursorMoveStyle",
        "KtCore.Katie.Qt.CursorMoveStyle",
        "Katie::Qt::CursorMoveStyle");
    if (!SbkKtCoreTypes[SBK_KATIE_QT_CURSORMOVESTYLE_IDX])
        return SBK_MODULE_INIT_ERROR;

    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_CURSORMOVESTYLE_IDX],
        module, "LogicalMoveStyle", (long) Katie::Qt::CursorMoveStyle::LogicalMoveStyle))
        return SBK_MODULE_INIT_ERROR;
    if (!Shiboken::Enum::createGlobalEnumItem(SbkKtCoreTypes[SBK_KATIE_QT_CURSORMOVESTYLE_IDX],
        module, "VisualMoveStyle", (long) Katie::Qt::CursorMoveStyle::VisualMoveStyle))
        return SBK_MODULE_INIT_ERROR;
    // Register converter for enum 'Katie::Qt::CursorMoveStyle'.
    {
        SbkConverter* converter = Shiboken::Conversions::createConverter(SbkKtCoreTypes[SBK_KATIE_QT_CURSORMOVESTYLE_IDX],
            Katie_Qt_CursorMoveStyle_CppToPython_Katie_Qt_CursorMoveStyle);
        Shiboken::Conversions::addPythonToCppValueConversion(converter,
            Katie_Qt_CursorMoveStyle_PythonToCpp_Katie_Qt_CursorMoveStyle,
            is_Katie_Qt_CursorMoveStyle_PythonToCpp_Katie_Qt_CursorMoveStyle_Convertible);
        Shiboken::Enum::setTypeConverter(SbkKtCoreTypes[SBK_KATIE_QT_CURSORMOVESTYLE_IDX], converter);
        Shiboken::Conversions::registerConverterName(converter, "Katie::Qt::CursorMoveStyle");
        Shiboken::Conversions::registerConverterName(converter, "Qt::CursorMoveStyle");
        Shiboken::Conversions::registerConverterName(converter, "CursorMoveStyle");
    }
    // End of 'CursorMoveStyle' enum.

    // Register primitive types converters.
    Shiboken::Conversions::registerConverterName(Shiboken::Conversions::PrimitiveTypeConverter<long long>(), "qint64");
    Shiboken::Conversions::registerConverterName(Shiboken::Conversions::PrimitiveTypeConverter<unsigned char>(), "uchar");
    Shiboken::Conversions::registerConverterName(Shiboken::Conversions::PrimitiveTypeConverter<unsigned int>(), "uint");

    Shiboken::Module::registerTypes(module, SbkKtCoreTypes);
    Shiboken::Module::registerTypeConverters(module, SbkKtCoreTypeConverters);

    if (PyErr_Occurred()) {
        PyErr_Print();
        Py_FatalError("can't initialize module KtCore");
    }
    FinishSignatureInitialization(module, KtCore_SignatureStrings);

SBK_MODULE_INIT_FUNCTION_END
