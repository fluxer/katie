/*
 *  Copyright (C) 2005, 2007, 2008 Apple Inc. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public License
 *  along with this library; see the file COPYING.LIB.  If not, write to
 *  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *  Boston, MA 02110-1301, USA.
 *
 */

#ifndef PropertySlot_h
#define PropertySlot_h

#include "Identifier.h"
#include "JSValue.h"
#include "Register.h"
#include <wtf/Assertions.h>
#include <wtf/NotFound.h>

namespace JSC {

    class ExecState;
    class JSObject;

#define JSC_VALUE_SLOT_MARKER 0
#define JSC_REGISTER_SLOT_MARKER reinterpret_cast<GetValueFunc>(1)

    class PropertySlot {
    public:
        PropertySlot()
            : m_getterFunc(nullptr),
            m_valueSlot(nullptr),
            m_registerSlot(nullptr)
        {
            clearBase();
            clearOffset();
            clearValue();
        }

        explicit PropertySlot(const JSValue base)
            : m_slotBase(base),
            m_getterFunc(nullptr),
            m_valueSlot(nullptr),
            m_registerSlot(nullptr)
        {
            clearOffset();
            clearValue();
        }

        typedef JSValue (*GetValueFunc)(ExecState*, const Identifier&, const PropertySlot&);

        JSValue getValue(ExecState* exec, const Identifier& propertyName) const
        {
            if (m_getValue == JSC_VALUE_SLOT_MARKER)
                return *m_valueSlot;
            if (m_getValue == JSC_REGISTER_SLOT_MARKER)
                return (*m_registerSlot).jsValue();
            return m_getValue(exec, propertyName, *this);
        }

        JSValue getValue(ExecState* exec, unsigned propertyName) const
        {
            if (m_getValue == JSC_VALUE_SLOT_MARKER)
                return *m_valueSlot;
            if (m_getValue == JSC_REGISTER_SLOT_MARKER)
                return (*m_registerSlot).jsValue();
            return m_getValue(exec, Identifier::from(exec, propertyName), *this);
        }

        bool isCacheable() const { return m_offset != WTF::notFound; }
        size_t cachedOffset() const
        {
            Q_ASSERT(isCacheable());
            return m_offset;
        }

        void setValueSlot(JSValue* valueSlot) 
        {
            Q_ASSERT(valueSlot);
            clearBase();
            clearOffset();
            m_getValue = JSC_VALUE_SLOT_MARKER;
            m_valueSlot = valueSlot;
        }
        
        void setValueSlot(JSValue slotBase, JSValue* valueSlot)
        {
            Q_ASSERT(valueSlot);
            m_getValue = JSC_VALUE_SLOT_MARKER;
            m_slotBase = slotBase;
            m_valueSlot = valueSlot;
        }
        
        void setValueSlot(JSValue slotBase, JSValue* valueSlot, size_t offset)
        {
            Q_ASSERT(valueSlot);
            m_getValue = JSC_VALUE_SLOT_MARKER;
            m_slotBase = slotBase;
            m_valueSlot = valueSlot;
            m_offset = offset;
        }
        
        void setValue(JSValue value)
        {
            Q_ASSERT(value);
            clearBase();
            clearOffset();
            m_getValue = JSC_VALUE_SLOT_MARKER;
            m_value = value;
            m_valueSlot = &m_value;
        }

        void setRegisterSlot(Register* registerSlot)
        {
            Q_ASSERT(registerSlot);
            clearBase();
            clearOffset();
            m_getValue = JSC_REGISTER_SLOT_MARKER;
            m_registerSlot = registerSlot;
        }

        void setCustom(JSValue slotBase, GetValueFunc getValue)
        {
            Q_ASSERT(slotBase);
            Q_ASSERT(getValue);
            m_getValue = getValue;
            m_slotBase = slotBase;
        }

        void setCustomIndex(JSValue slotBase, unsigned index, GetValueFunc getValue)
        {
            Q_ASSERT(slotBase);
            Q_ASSERT(getValue);
            m_getValue = getValue;
            m_slotBase = slotBase;
            m_index = index;
        }
        
        void setGetterSlot(JSObject* getterFunc)
        {
            Q_ASSERT(getterFunc);
            m_getValue = functionGetter;
            m_getterFunc = getterFunc;
        }
        
        void setUndefined()
        {
            setValue(jsUndefined());
        }

        JSValue slotBase() const
        {
            return m_slotBase;
        }

        void setBase(JSValue base)
        {
            Q_ASSERT(m_slotBase);
            Q_ASSERT(base);
            m_slotBase = base;
        }

        void clearBase()
        {
#ifndef QT_NO_DEBUG
            m_slotBase = JSValue();
#endif
        }

        void clearValue()
        {
#ifndef QT_NO_DEBUG
            m_value = JSValue();
#endif
        }

        void clearOffset()
        {
            // Clear offset even in release builds, in case this PropertySlot has been used before.
            // (For other data members, we don't need to clear anything because reuse would meaningfully overwrite them.)
            m_offset = WTF::notFound;
        }

        unsigned index() const { return m_index; }

    private:
        static JSValue functionGetter(ExecState*, const Identifier&, const PropertySlot&);

        GetValueFunc m_getValue;
        
        JSValue m_slotBase;
        JSValue m_value;

        JSObject* m_getterFunc;
        JSValue* m_valueSlot;
        Register* m_registerSlot;
        unsigned m_index;

        size_t m_offset;
    };

} // namespace JSC

#endif // PropertySlot_h
