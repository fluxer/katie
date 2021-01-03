/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016-2021 Ivailo Monev
**
** This file is part of the tools applications of the Katie Toolkit.
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
** As a special exception, The Qt Company gives you certain additional
** rights. These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "generator.h"
#include "utils.h"
#include "qmetatype.h"
#include "qmetaobject_p.h" //for the flags.
#include "qplatformdefs.h"

QT_BEGIN_NAMESPACE

static uint qvariant_nameToType(const char* name)
{
    if (!name)
        return 0;

    if (strcmp(name, "QVariant") == 0)
        return QVariant::LastType;

    uint tp = QMetaType::type(name);
    return tp < QMetaType::User ? tp : 0;
}

/*
  Returns true if the type is a QVariant types.
*/
static bool isVariantType(const char* type)
{
    return qvariant_nameToType(type) != 0;
}

/*!
  Returns true if the type is qreal.
*/
static bool isQRealType(const char *type)
{
    return strcmp(type, "qreal") == 0;
}

Generator::Generator(ClassDef *classDef, const QList<QByteArray> &metaTypes, int outfile)
    : out(outfile), cdef(classDef), metaTypes(metaTypes)
{
    if (cdef->superclassList.size())
        purestSuperClass = cdef->superclassList.first().first;
}

static inline int lengthOfEscapeSequence(const QByteArray &s, int i)
{
    if (s.at(i) != '\\' || i >= s.length() - 1)
        return 1;
    const int startPos = i;
    ++i;
    char ch = s.at(i);
    if (ch == 'x') {
        ++i;
        while (i < s.length() && is_hex_char(s.at(i)))
            ++i;
    } else if (is_octal_char(ch)) {
        while (i < startPos + 4
               && i < s.length()
               && is_octal_char(s.at(i))) {
            ++i;
        }
    } else { // single character escape sequence
        i = qMin(i + 1, s.length());
    }
    return i - startPos;
}

int Generator::strreg(const char *s)
{
    int idx = 0;
    if (!s)
        s = "";
    for (int i = 0; i < strings.size(); ++i) {
        const QByteArray &str = strings.at(i);
        if (str == s)
            return idx;
        idx += str.length() + 1;
        for (int i = 0; i < str.length(); ++i) {
            if (str.at(i) == '\\') {
                int cnt = lengthOfEscapeSequence(str, i) - 1;
                idx -= cnt;
                i += cnt;
            }
        }
    }
    strings.append(s);
    return idx;
}

void Generator::generateCode()
{
    bool isQt = (cdef->classname == "Qt");
    bool isQObject = (cdef->classname == "QObject");
    bool isConstructible = !cdef->constructorList.isEmpty();

//
// build the data array
//
    int i = 0;


    // filter out undeclared enumerators and sets
    {
        QList<EnumDef> enumList;
        for (i = 0; i < cdef->enumList.count(); ++i) {
            EnumDef def = cdef->enumList.at(i);
            if (cdef->enumDeclarations.contains(def.name)) {
                enumList += def;
            }
            QByteArray alias = cdef->flagAliases.value(def.name);
            if (cdef->enumDeclarations.contains(alias)) {
                def.name = alias;
                enumList += def;
            }
        }
        cdef->enumList = enumList;
    }


    QByteArray qualifiedClassNameIdentifier = cdef->qualified;
    qualifiedClassNameIdentifier.replace(':', '_');

    int index = 14;
    dprintf(out, "static const uint qt_meta_data_%s[] = {\n", qualifiedClassNameIdentifier.constData());
    dprintf(out, "\n // content:\n");
    dprintf(out, "    %4d,       // revision\n", 6);
    dprintf(out, "    %4d,       // classname\n", strreg(cdef->qualified.constData()));
    dprintf(out, "    %4d, %4d, // classinfo\n", cdef->classInfoList.count(), cdef->classInfoList.count() ? index : 0);
    index += cdef->classInfoList.count() * 2;

    int methodCount = cdef->signalList.count() + cdef->slotList.count() + cdef->methodList.count();
    dprintf(out, "    %4d, %4d, // methods\n", methodCount, methodCount ? index : 0);
    index += methodCount * 5;
    if (cdef->revisionedMethods)
        index += methodCount;
    dprintf(out, "    %4d, %4d, // properties\n", cdef->propertyList.count(), cdef->propertyList.count() ? index : 0);
    index += cdef->propertyList.count() * 3;
    if(cdef->notifyableProperties)
        index += cdef->propertyList.count();
    if (cdef->revisionedProperties)
        index += cdef->propertyList.count();
    dprintf(out, "    %4d, %4d, // enums/sets\n", cdef->enumList.count(), cdef->enumList.count() ? index : 0);

    int enumsIndex = index;
    for (i = 0; i < cdef->enumList.count(); ++i)
        index += 4 + (cdef->enumList.at(i).values.count() * 2);
    dprintf(out, "    %4d, %4d, // constructors\n", isConstructible ? cdef->constructorList.count() : 0,
            isConstructible ? index : 0);

    dprintf(out, "    %4d,       // flags\n", 0);
    dprintf(out, "    %4d,       // signalCount\n", cdef->signalList.count());


//
// Build classinfo array
//
    generateClassInfos();

//
// Build signals array first, otherwise the signal indices would be wrong
//
    generateFunctions(cdef->signalList, "signal", MethodSignal);

//
// Build slots array
//
    generateFunctions(cdef->slotList, "slot", MethodSlot);

//
// Build method array
//
    generateFunctions(cdef->methodList, "method", MethodMethod);

//
// Build method version arrays
//
    if (cdef->revisionedMethods) {
        generateFunctionRevisions(cdef->signalList, "signal");
        generateFunctionRevisions(cdef->slotList, "slot");
        generateFunctionRevisions(cdef->methodList, "method");
    }

//
// Build property array
//
    generateProperties();

//
// Build enums array
//
    generateEnums(enumsIndex);

//
// Build constructors array
//
    if (isConstructible)
        generateFunctions(cdef->constructorList, "constructor", MethodConstructor);

//
// Terminate data array
//
    dprintf(out, "\n       0        // eod\n};\n\n");

//
// Build stringdata array
//
    dprintf(out, "static const char qt_meta_stringdata_%s[] = {\n", qualifiedClassNameIdentifier.constData());
    dprintf(out, "    \"");
    int col = 0;
    int len = 0;
    for (i = 0; i < strings.size(); ++i) {
        QByteArray s = strings.at(i);
        len = s.length();
        if (col && col + len >= 72) {
            dprintf(out, "\"\n    \"");
            col = 0;
        } else if (len && s.at(0) >= '0' && s.at(0) <= '9') {
            dprintf(out, "\"\"");
            len += 2;
        }
        int idx = 0;
        while (idx < s.length()) {
            if (idx > 0) {
                col = 0;
                dprintf(out, "\"\n    \"");
            }
            int spanLen = qMin(70, s.length() - idx);
            // don't cut escape sequences at the end of a line
            int backSlashPos = s.lastIndexOf('\\', idx + spanLen - 1);
            if (backSlashPos >= idx) {
                int escapeLen = lengthOfEscapeSequence(s, backSlashPos);
                spanLen = qBound(spanLen, backSlashPos + escapeLen - idx, s.length() - idx);
            }
            QT_WRITE(out, s.constData() + idx, spanLen);
            idx += spanLen;
            col += spanLen;
        }

        dprintf(out, "\\0");
        col += len + 2;
    }
    dprintf(out, "\"\n};\n\n");

//
// Generate internal qt_static_metacall() function
//
    if (cdef->hasQObject && !isQt)
        generateStaticMetacall();

//
// Build extra array
//
    QList<QByteArray> extraList;
    for (int i = 0; i < cdef->propertyList.count(); ++i) {
        const PropertyDef &p = cdef->propertyList.at(i);
        if (!isVariantType(p.type.constData()) && !metaTypes.contains(p.type) && !p.type.contains('*') &&
                !p.type.contains('<') && !p.type.contains('>')) {
            int s = p.type.lastIndexOf("::");
            if (s > 0) {
                QByteArray scope = p.type.left(s);
                if (scope != "Qt" && scope != cdef->classname && !extraList.contains(scope))
                    extraList += scope;
            }
        }
    }
    if (!extraList.isEmpty()) {
        dprintf(out, "#ifdef Q_NO_DATA_RELOCATION\n");
        dprintf(out, "static const QMetaObjectAccessor qt_meta_extradata_%s[] = {\n    ", qualifiedClassNameIdentifier.constData());
        for (int i = 0; i < extraList.count(); ++i) {
            dprintf(out, "    %s::getStaticMetaObject,\n", extraList.at(i).constData());
        }
        dprintf(out, "#else\n");
        dprintf(out, "static const QMetaObject *qt_meta_extradata_%s[] = {\n    ", qualifiedClassNameIdentifier.constData());
        for (int i = 0; i < extraList.count(); ++i) {
            dprintf(out, "    &%s::staticMetaObject,\n", extraList.at(i).constData());
        }
        dprintf(out, "#endif //Q_NO_DATA_RELOCATION\n");
        dprintf(out, "    Q_NULLPTR\n};\n\n");
    }

//
// Finally create and initialize the static meta object
//
    if (isQt)
        dprintf(out, "const QMetaObject QObject::staticQtMetaObject = {\n");
    else
        dprintf(out, "const QMetaObject %s::staticMetaObject = {\n", cdef->qualified.constData());

    if (isQObject)
        dprintf(out, "    { Q_NULLPTR, ");
    else if (cdef->superclassList.size())
        dprintf(out, "    { &%s::staticMetaObject, ", purestSuperClass.constData());
    else
        dprintf(out, "    { Q_NULLPTR, ");
    dprintf(out, "qt_meta_stringdata_%s,\n      qt_meta_data_%s, ",
             qualifiedClassNameIdentifier.constData(), qualifiedClassNameIdentifier.constData());
    if (cdef->hasQObject && !isQt)
        dprintf(out, " qt_static_metacall, ");
    else
        dprintf(out, " Q_NULLPTR, ");

    if (extraList.isEmpty())
        dprintf(out, "Q_NULLPTR, ");
    else
        dprintf(out, "qt_meta_extradata_%s, ", qualifiedClassNameIdentifier.constData());
    dprintf(out, "Q_NULLPTR}\n};\n\n");

    if(isQt)
        return;

//
// Generate static meta object accessor (needed for symbian, because DLLs do not support data imports.
//
    dprintf(out, "\n#ifdef Q_NO_DATA_RELOCATION\n");
    dprintf(out, "const QMetaObject &%s::getStaticMetaObject() { return staticMetaObject; }\n", cdef->qualified.constData());
    dprintf(out, "#endif //Q_NO_DATA_RELOCATION\n");

    if (!cdef->hasQObject)
        return;

    dprintf(out, "\nconst QMetaObject *%s::metaObject() const\n{\n    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;\n}\n",
            cdef->qualified.constData());

//
// Generate smart cast function
//
    dprintf(out, "\nvoid *%s::qt_metacast(const char *_clname)\n{\n", cdef->qualified.constData());
    dprintf(out, "    if (!_clname) return Q_NULLPTR;\n");
    dprintf(out, "    if (!strcmp(_clname, qt_meta_stringdata_%s))\n"
                  "        return static_cast<void*>(this);\n",
            qualifiedClassNameIdentifier.constData());
    for (int i = 1; i < cdef->superclassList.size(); ++i) { // for all superclasses but the first one
        if (cdef->superclassList.at(i).second == FunctionDef::Private)
            continue;
        const char *cname = cdef->superclassList.at(i).first.constData();
        dprintf(out, "    if (!strcmp(_clname, \"%s\"))\n        return static_cast<%s*>(const_cast<%s*>(this));\n",
                cname, cname, cdef->classname.constData());
    }
    for (int i = 0; i < cdef->interfaceList.size(); ++i) {
        const QList<ClassDef::Interface> &iface = cdef->interfaceList.at(i);
        for (int j = 0; j < iface.size(); ++j) {
            dprintf(out, "    if (!strcmp(_clname, %s))\n        return ", iface.at(j).interfaceId.constData());
            for (int k = j; k >= 0; --k)
                dprintf(out, "static_cast<%s*>(", iface.at(k).className.constData());
            dprintf(out, "const_cast<%s*>(this)%s;\n",
                    cdef->classname.constData(), QByteArray(j+1, ')').constData());
        }
    }
    if (!purestSuperClass.isEmpty() && !isQObject) {
        dprintf(out, "    return %s::qt_metacast(_clname);\n", purestSuperClass.constData());
    } else {
        dprintf(out, "    return Q_NULLPTR;\n");
    }
    dprintf(out, "}\n");

//
// Generate internal qt_metacall()  function
//
    generateMetacall();

//
// Generate internal signal functions
//
    for (int signalindex = 0; signalindex < cdef->signalList.size(); ++signalindex)
        generateSignal(&cdef->signalList[signalindex], signalindex);
}


void Generator::generateClassInfos()
{
    if (cdef->classInfoList.isEmpty())
        return;

    dprintf(out, "\n // classinfo: key, value\n");

    for (int i = 0; i < cdef->classInfoList.size(); ++i) {
        const ClassInfoDef &c = cdef->classInfoList.at(i);
        dprintf(out, "    %4d, %4d,\n", strreg(c.name.constData()), strreg(c.value.constData()));
    }
}

void Generator::generateFunctions(QList<FunctionDef>& list, const char *functype, int type)
{
    if (list.isEmpty())
        return;
    dprintf(out, "\n // %ss: signature, parameters, type, tag, flags\n", functype);

    for (int i = 0; i < list.count(); ++i) {
        const FunctionDef &f = list.at(i);

        QByteArray sig = f.name + '(';
        QByteArray arguments;

        for (int j = 0; j < f.arguments.count(); ++j) {
            const ArgumentDef &a = f.arguments.at(j);
            if (j) {
                sig += ",";
                arguments += ",";
            }
            sig += a.normalizedType;
            arguments += a.name;
        }
        sig += ')';

        unsigned char flags = type;
        if (f.access == FunctionDef::Private)
            flags |= AccessPrivate;
        else if (f.access == FunctionDef::Public)
            flags |= AccessPublic;
        else if (f.access == FunctionDef::Protected)
            flags |= AccessProtected;
        if (f.wasCloned)
            flags |= MethodCloned;
        if (f.isScriptable)
            flags |= MethodScriptable;
        if (f.revision > 0)
            flags |= MethodRevisioned;
        dprintf(out, "    %4d, %4d, %4d, %4d, 0x%02x,\n",
                strreg(sig.constData()),
                strreg(arguments.constData()),
                strreg(f.normalizedType.constData()),
                strreg(f.tag.constData()), flags);
    }
}

void Generator::generateFunctionRevisions(QList<FunctionDef>& list, const char *functype)
{
    if (list.count())
        dprintf(out, "\n // %ss: revision\n", functype);
    for (int i = 0; i < list.count(); ++i) {
        const FunctionDef &f = list.at(i);
        dprintf(out, "    %4d,\n", f.revision);
    }
}

void Generator::generateProperties()
{
    //
    // Create meta data
    //

    if (cdef->propertyList.count())
        dprintf(out, "\n // properties: name, type, flags\n");
    for (int i = 0; i < cdef->propertyList.count(); ++i) {
        const PropertyDef &p = cdef->propertyList.at(i);
        uint flags = Invalid;
        if (!isVariantType(p.type.constData())) {
            flags |= EnumOrFlag;
        } else if (!isQRealType(p.type.constData())) {
            flags |= qvariant_nameToType(p.type.constData()) << 24;
        }
        if (!p.read.isEmpty())
            flags |= Readable;
        if (!p.write.isEmpty()) {
            flags |= Writable;
            if (p.stdCppSet())
                flags |= StdCppSet;
        }
        if (!p.reset.isEmpty())
            flags |= Resettable;

//         if (p.override)
//             flags |= Override;

        if (p.designable.isEmpty())
            flags |= ResolveDesignable;
        else if (p.designable != "false")
            flags |= Designable;

        if (p.scriptable.isEmpty())
            flags |= ResolveScriptable;
        else if (p.scriptable != "false")
            flags |= Scriptable;

        if (p.stored.isEmpty())
            flags |= ResolveStored;
        else if (p.stored != "false")
            flags |= Stored;

        if (p.editable.isEmpty())
            flags |= ResolveEditable;
        else if (p.editable != "false")
            flags |= Editable;

        if (p.user.isEmpty())
            flags |= ResolveUser;
        else if (p.user != "false")
            flags |= User;

        if (p.notifyId != -1)
            flags |= Notify;

        if (p.revision > 0)
            flags |= Revisioned;

        if (p.constant)
            flags |= Constant;
        if (p.final)
            flags |= Final;

        dprintf(out, "    %4d, %4d, ",
                strreg(p.name.constData()),
                strreg(p.type.constData()));
        if (!(flags >> 24) && isQRealType(p.type.constData()))
            dprintf(out, "((uint)QMetaType::QReal << 24) | ");
        dprintf(out, "0x%.8x,\n", flags);
    }

    if(cdef->notifyableProperties) {
        dprintf(out, "\n // properties: notify_signal_id\n");
        for (int i = 0; i < cdef->propertyList.count(); ++i) {
            const PropertyDef &p = cdef->propertyList.at(i);
            if(p.notifyId == -1)
                dprintf(out, "    %4d,\n",
                        0);
            else
                dprintf(out, "    %4d,\n",
                        p.notifyId);
        }
    }
    if (cdef->revisionedProperties) {
        dprintf(out, "\n // properties: revision\n");
        for (int i = 0; i < cdef->propertyList.count(); ++i) {
            const PropertyDef &p = cdef->propertyList.at(i);
            dprintf(out, "    %4d,\n", p.revision);
        }
    }
}

void Generator::generateEnums(int index)
{
    if (cdef->enumDeclarations.isEmpty())
        return;

    dprintf(out, "\n // enums: name, flags, count, data\n");
    index += 4 * cdef->enumList.count();
    int i;
    for (i = 0; i < cdef->enumList.count(); ++i) {
        const EnumDef &e = cdef->enumList.at(i);
        dprintf(out, "    %4d, 0x%.1x, %4d, %4d,\n",
                 strreg(e.name.constData()),
                 cdef->enumDeclarations.value(e.name) ? 1 : 0,
                 e.values.count(),
                 index);
        index += e.values.count() * 2;
    }

    dprintf(out, "\n // enum data: key, value\n");
    for (i = 0; i < cdef->enumList.count(); ++i) {
        const EnumDef &e = cdef->enumList.at(i);
        for (int j = 0; j < e.values.count(); ++j) {
            const QByteArray &val = e.values.at(j);
            dprintf(out, "    %4d, uint(%s::%s),\n",
                    strreg(val.constData()),
                    cdef->qualified.constData(),
                    val.constData());
        }
    }
}

void Generator::generateMetacall()
{
    bool isQObject = (cdef->classname == "QObject");

    dprintf(out, "\nint %s::qt_metacall(QMetaObject::Call _c, int _id, void **_a)\n{\n",
             cdef->qualified.constData());

    if (!purestSuperClass.isEmpty() && !isQObject) {
        QByteArray superClass = purestSuperClass;
        // workaround for VC6
        if (superClass.contains("::")) {
            dprintf(out, "    typedef %s QMocSuperClass;\n", superClass.constData());
            superClass = "QMocSuperClass";
        }
        dprintf(out, "    _id = %s::qt_metacall(_c, _id, _a);\n", superClass.constData());
    }

    dprintf(out, "    if (_id < 0)\n        return _id;\n");
    dprintf(out, "    ");

    bool needElse = false;
    QList<FunctionDef> methodList;
    methodList += cdef->signalList;
    methodList += cdef->slotList;
    methodList += cdef->methodList;

    if (methodList.size()) {
        needElse = true;
        dprintf(out, "if (_c == QMetaObject::InvokeMetaMethod) {\n");
        dprintf(out, "        if (_id < %d)\n", methodList.size());
        dprintf(out, "            qt_static_metacall(this, _c, _id, _a);\n");
        dprintf(out, "        _id -= %d;\n    }", methodList.size());
    }

    if (cdef->propertyList.size()) {
        bool needGet = false;
        bool needSet = false;
        bool needReset = false;
        bool needDesignable = false;
        bool needScriptable = false;
        bool needStored = false;
        bool needEditable = false;
        bool needUser = false;
        for (int i = 0; i < cdef->propertyList.size(); ++i) {
            const PropertyDef &p = cdef->propertyList.at(i);
            needGet |= !p.read.isEmpty();
            needSet |= !p.write.isEmpty();
            needReset |= !p.reset.isEmpty();
            needDesignable |= p.designable.endsWith(')');
            needScriptable |= p.scriptable.endsWith(')');
            needStored |= p.stored.endsWith(')');
            needEditable |= p.editable.endsWith(')');
            needUser |= p.user.endsWith(')');
        }
        dprintf(out, "\n#ifndef QT_NO_PROPERTIES\n     ");

        if (needElse)
            dprintf(out, " else ");
        dprintf(out, "if (_c == QMetaObject::ReadProperty) {\n");
        if (needGet) {
            dprintf(out, "        switch (_id) {\n");
            for (int propindex = 0; propindex < cdef->propertyList.size(); ++propindex) {
                const PropertyDef &p = cdef->propertyList.at(propindex);
                if (p.read.isEmpty())
                    continue;
                QByteArray prefix;
                if (p.inPrivateClass.size()) {
                    prefix = p.inPrivateClass;
                    prefix.append("->");
                }
                if (p.gspec == PropertyDef::PointerSpec)
                    dprintf(out, "        case %d: _a[0] = const_cast<void*>(static_cast<const void*>(%s%s())); break;\n",
                            propindex, prefix.constData(), p.read.constData());
                else if (p.gspec == PropertyDef::ReferenceSpec)
                    dprintf(out, "        case %d: _a[0] = const_cast<void*>(static_cast<const void*>(&%s%s())); break;\n",
                            propindex, prefix.constData(), p.read.constData());
                else if (cdef->enumDeclarations.value(p.type, false))
                    dprintf(out, "        case %d: *static_cast<int*>(_a[0]) = QFlag(%s%s()); break;\n",
                            propindex, prefix.constData(), p.read.constData());
                else
                    dprintf(out, "        case %d: *static_cast<%s*>(_a[0]) = %s%s(); break;\n",
                            propindex, p.type.constData(), prefix.constData(), p.read.constData());
            }
            dprintf(out, "        }\n");
        }

        dprintf(out,
                "        _id -= %d;\n"
                "    }", cdef->propertyList.count());

        dprintf(out, " else ");
        dprintf(out, "if (_c == QMetaObject::WriteProperty) {\n");

        if (needSet) {
            dprintf(out, "        switch (_id) {\n");
            for (int propindex = 0; propindex < cdef->propertyList.size(); ++propindex) {
                const PropertyDef &p = cdef->propertyList.at(propindex);
                if (p.write.isEmpty())
                    continue;
                QByteArray prefix;
                if (p.inPrivateClass.size()) {
                    prefix = p.inPrivateClass;
                    prefix.append("->");
                }
                if (cdef->enumDeclarations.value(p.type, false)) {
                    dprintf(out, "        case %d: %s%s(QFlag(*static_cast<int*>(_a[0]))); break;\n",
                            propindex, prefix.constData(), p.write.constData());
                } else {
                    dprintf(out, "        case %d: %s%s(*static_cast<%s*>(_a[0])); break;\n",
                            propindex, prefix.constData(), p.write.constData(), p.type.constData());
                }
            }
            dprintf(out, "        }\n");
        }

        dprintf(out,
                "        _id -= %d;\n"
                "    }", cdef->propertyList.count());

        dprintf(out, " else ");
        dprintf(out, "if (_c == QMetaObject::ResetProperty) {\n");
        if (needReset) {
            dprintf(out, "        switch (_id) {\n");
            for (int propindex = 0; propindex < cdef->propertyList.size(); ++propindex) {
                const PropertyDef &p = cdef->propertyList.at(propindex);
                if (!p.reset.endsWith(')'))
                    continue;
                QByteArray prefix;
                if (p.inPrivateClass.size()) {
                    prefix = p.inPrivateClass;
                    prefix.append("->");
                }
                dprintf(out, "        case %d: %s%s; break;\n",
                        propindex, prefix.constData(), p.reset.constData());
            }
            dprintf(out, "        }\n");
        }
        dprintf(out,
                "        _id -= %d;\n"
                "    }", cdef->propertyList.count());

        dprintf(out, " else ");
        dprintf(out, "if (_c == QMetaObject::QueryPropertyDesignable) {\n");
        if (needDesignable) {
            dprintf(out, "        switch (_id) {\n");
            for (int propindex = 0; propindex < cdef->propertyList.size(); ++propindex) {
                const PropertyDef &p = cdef->propertyList.at(propindex);
                if (!p.designable.endsWith(')'))
                    continue;
                dprintf(out, "        case %d: *static_cast<bool*>(_a[0]) = %s; break;\n",
                         propindex, p.designable.constData());
            }
            dprintf(out, "        }\n");
        }
        dprintf(out,
                "        _id -= %d;\n"
                "    }", cdef->propertyList.count());

        dprintf(out, " else if (_c == QMetaObject::QueryPropertyScriptable) {\n");
        if (needScriptable) {
            dprintf(out, "        switch (_id) {\n");
            for (int propindex = 0; propindex < cdef->propertyList.size(); ++propindex) {
                const PropertyDef &p = cdef->propertyList.at(propindex);
                if (!p.scriptable.endsWith(')'))
                    continue;
                dprintf(out, "        case %d: *static_cast<bool*>(_a[0]) = %s; break;\n",
                         propindex, p.scriptable.constData());
            }
            dprintf(out, "        }\n");
        }
        dprintf(out,
                "        _id -= %d;\n"
                "    }", cdef->propertyList.count());

        dprintf(out, " else if (_c == QMetaObject::QueryPropertyStored) {\n");
        if (needStored) {
            dprintf(out, "        switch (_id) {\n");
            for (int propindex = 0; propindex < cdef->propertyList.size(); ++propindex) {
                const PropertyDef &p = cdef->propertyList.at(propindex);
                if (!p.stored.endsWith(')'))
                    continue;
                dprintf(out, "        case %d: *static_cast<bool*>(_a[0]) = %s; break;\n",
                         propindex, p.stored.constData());
            }
            dprintf(out, "        }\n");
        }
        dprintf(out,
                "        _id -= %d;\n"
                "    }", cdef->propertyList.count());

        dprintf(out, " else if (_c == QMetaObject::QueryPropertyEditable) {\n");
        if (needEditable) {
            dprintf(out, "        switch (_id) {\n");
            for (int propindex = 0; propindex < cdef->propertyList.size(); ++propindex) {
                const PropertyDef &p = cdef->propertyList.at(propindex);
                if (!p.editable.endsWith(')'))
                    continue;
                dprintf(out, "        case %d: *static_cast<bool*>(_a[0]) = %s; break;\n",
                         propindex, p.editable.constData());
            }
            dprintf(out, "        }\n");
        }
        dprintf(out,
                "        _id -= %d;\n"
                "    }", cdef->propertyList.count());


        dprintf(out, " else if (_c == QMetaObject::QueryPropertyUser) {\n");
        if (needUser) {
            dprintf(out, "        switch (_id) {\n");
            for (int propindex = 0; propindex < cdef->propertyList.size(); ++propindex) {
                const PropertyDef &p = cdef->propertyList.at(propindex);
                if (!p.user.endsWith(')'))
                    continue;
                dprintf(out, "        case %d: *static_cast<bool*>(_a[0]) = %s; break;\n",
                         propindex, p.user.constData());
            }
            dprintf(out, "        }\n");
        }
        dprintf(out,
                "        _id -= %d;\n"
                "    }", cdef->propertyList.count());


        dprintf(out, "\n#endif // QT_NO_PROPERTIES");
    }
    if (methodList.size() || cdef->signalList.size() || cdef->propertyList.size())
        dprintf(out, "\n    ");
    dprintf(out,"return _id;\n}\n");
}

void Generator::generateStaticMetacall()
{
    dprintf(out, "void %s::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)\n{\n",
            cdef->qualified.constData());

    bool needElse = false;
    bool isUsed_a = false;

    if (!cdef->constructorList.isEmpty()) {
        dprintf(out, "    if (_c == QMetaObject::CreateInstance) {\n");
        dprintf(out, "        switch (_id) {\n");
        for (int ctorindex = 0; ctorindex < cdef->constructorList.count(); ++ctorindex) {
            dprintf(out, "        case %d: { %s *_r = new %s(", ctorindex,
                    cdef->classname.constData(), cdef->classname.constData());
            const FunctionDef &f = cdef->constructorList.at(ctorindex);
            int offset = 1;
            for (int j = 0; j < f.arguments.count(); ++j) {
                const ArgumentDef &a = f.arguments.at(j);
                if (j)
                    dprintf(out, ",");
                dprintf(out, "*static_cast<%s>(_a[%d])", a.typeNameForCast.constData(), offset++);
            }
            dprintf(out, ");\n");
            dprintf(out, "            if (_a[0]) *static_cast<QObject**>(_a[0]) = _r; } break;\n");
        }
        dprintf(out, "        }\n");
        dprintf(out, "    }");
        needElse = true;
        isUsed_a = true;
    }

    QList<FunctionDef> methodList;
    methodList += cdef->signalList;
    methodList += cdef->slotList;
    methodList += cdef->methodList;

    if (!methodList.isEmpty()) {
        if (needElse)
            dprintf(out, " else ");
        else
            dprintf(out, "    ");
        dprintf(out, "if (_c == QMetaObject::InvokeMetaMethod) {\n");
#ifndef QT_NO_DEBUG
        dprintf(out, "        Q_ASSERT(staticMetaObject.cast(_o));\n");
#endif
        dprintf(out, "        %s *_t = static_cast<%s *>(_o);\n", cdef->classname.constData(), cdef->classname.constData());
        dprintf(out, "        switch (_id) {\n");
        for (int methodindex = 0; methodindex < methodList.size(); ++methodindex) {
            const FunctionDef &f = methodList.at(methodindex);
            dprintf(out, "        case %d: ", methodindex);
            if (f.normalizedType.size())
                dprintf(out, "{ %s _r = ", noRef(f.normalizedType).constData());
            dprintf(out, "_t->");
            if (f.inPrivateClass.size())
                dprintf(out, "%s->", f.inPrivateClass.constData());
            dprintf(out, "%s(", f.name.constData());
            int offset = 1;
            for (int j = 0; j < f.arguments.count(); ++j) {
                const ArgumentDef &a = f.arguments.at(j);
                if (j)
                    dprintf(out, ",");
                dprintf(out, "*static_cast<%s>(_a[%d])",a.typeNameForCast.constData(), offset++);
                isUsed_a = true;
            }
            dprintf(out, ");");
            if (f.normalizedType.size()) {
                dprintf(out, "\n            if (_a[0]) *static_cast<%s*>(_a[0]) = _r; } ",
                        noRef(f.normalizedType).constData());
                isUsed_a = true;
            }
            dprintf(out, " break;\n");
        }
        dprintf(out, "        default: break;\n");
        dprintf(out, "        }\n");
        dprintf(out, "    }");
        needElse = true;
    }

    if (needElse)
        dprintf(out, "\n");

    if (methodList.isEmpty()) {
        dprintf(out, "    Q_UNUSED(_o);\n");
        if (cdef->constructorList.isEmpty()) {
            dprintf(out, "    Q_UNUSED(_id);\n");
            dprintf(out, "    Q_UNUSED(_c);\n");
        }
    }
    if (!isUsed_a)
        dprintf(out, "    Q_UNUSED(_a);\n");

    dprintf(out, "}\n\n");
}

void Generator::generateSignal(FunctionDef *def,int index)
{
    if (def->wasCloned || def->isAbstract)
        return;
    dprintf(out, "\n// SIGNAL %d\n%s %s::%s(",
            index, def->type.name.constData(), cdef->qualified.constData(), def->name.constData());

    QByteArray thisPtr = "this";
    const char *constQualifier = "";

    if (def->isConst) {
        thisPtr = "const_cast<";
        thisPtr += cdef->qualified;
        thisPtr += " *>(this)";
        constQualifier = "const";
    }

    if (def->arguments.isEmpty() && def->normalizedType.isEmpty()) {
        dprintf(out, ")%s\n{\n"
                "    QMetaObject::activate(%s, &staticMetaObject, %d, Q_NULLPTR);\n"
                "}\n", constQualifier, thisPtr.constData(), index);
        return;
    }

    int offset = 1;
    for (int j = 0; j < def->arguments.count(); ++j) {
        const ArgumentDef &a = def->arguments.at(j);
        if (j)
            dprintf(out, ", ");
        dprintf(out, "%s _t%d%s", a.type.name.constData(), offset++, a.rightType.constData());
    }
    dprintf(out, ")%s\n{\n", constQualifier);
    if (def->type.name.size() && def->normalizedType.size())
        dprintf(out, "    %s _t0;\n", noRef(def->normalizedType).constData());

    dprintf(out, "    void *_a[] = { ");
    if (def->normalizedType.isEmpty()) {
        dprintf(out, "Q_NULLPTR");
    } else {
        if (def->returnTypeIsVolatile)
             dprintf(out, "const_cast<void*>(static_cast<const volatile void*>(&_t0))");
        else
             dprintf(out, "const_cast<void*>(static_cast<const void*>(&_t0))");
    }
    int i;
    for (i = 1; i < offset; ++i)
        if (def->arguments.at(i - 1).type.isVolatile)
            dprintf(out, ", const_cast<void*>(static_cast<const volatile void*>(&_t%d))", i);
        else
            dprintf(out, ", const_cast<void*>(static_cast<const void*>(&_t%d))", i);
    dprintf(out, " };\n");
    dprintf(out, "    QMetaObject::activate(%s, &staticMetaObject, %d, _a);\n", thisPtr.constData(), index);
    if (def->normalizedType.size())
        dprintf(out, "    return _t0;\n");
    dprintf(out, "}\n");
}

QT_END_NAMESPACE
