/*  ========================================================================================================================================
    Copyright (C) 2020 Eaton
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
    ========================================================================================================================================
*/

#pragma once
#include "pack/attribute.h"
#include "pack/types.h"
#include <cassert>
#include <cmath>
#include <functional>

namespace pack {

// =========================================================================================================================================

template <Type>
class Value;

class IValue : public Attribute
{
public:
    IValue(Attribute* parent, const std::string& key)
        : Attribute(NodeType::Value, parent, key)
    {
    }

    IValue()
        : Attribute(NodeType::Value, nullptr)
    {
    }


    virtual Type valueType() const = 0;
};

// =========================================================================================================================================

template <Type ValType>
class Value : public IValue
{
public:
    using CppType                  = typename ResolveType<ValType>::type;
    static constexpr Type ThisType = ValType;

public:
    Value(Attribute* parent, const std::string& key, const CppType& def = {});
    Value(const Value& other);
    Value(Value&& other);
    Value();

public:
    const CppType& value() const;
    void           setValue(const CppType& val);
    Value&         operator=(const CppType& val);
    Value&         operator=(const Value& other);
                   operator CppType() const;

public:
    bool        compare(const Attribute& other) const override;
    std::string typeName() const override;
    void        set(const Attribute& other) override;
    void        set(Attribute&& other) override;
    bool        hasValue() const override;
    Type        valueType() const override;
    void        clear() override;

private:
    CppType m_val = {};
    CppType m_def = {};
};

// =========================================================================================================================================

template <Type ValType>
Value<ValType>::Value(Attribute* parent, const std::string& key, const CppType& def)
    : IValue(parent, key)
    , m_val(def)
    , m_def(def)
{
}

template <Type ValType>
Value<ValType>::Value(const Value& other)
    : IValue(other)
{
    m_val = other.m_val;
}

template <Type ValType>
Value<ValType>::Value(Value&& other)
    : IValue(other)
{
    m_val = std::move(other.m_val);
}

template <Type ValType>
Value<ValType>::Value()
    : IValue()
{
}

template <Type ValType>
const typename Value<ValType>::CppType& Value<ValType>::value() const
{
    return m_val;
}

template <Type ValType>
void Value<ValType>::setValue(const CppType& val)
{
    if constexpr (ValType == Type::Float) {
        if (std::fabs(value() - val) <= std::numeric_limits<float>::epsilon()) {
            return;
        }
    } else if constexpr (ValType == Type::Double) {
        if (std::fabs(value() - val) <= std::numeric_limits<double>::epsilon()) {
            return;
        }
    } else {
        if (value() == val) {
            return;
        }
    }

    m_val = val;
}

template <Type ValType>
Value<ValType>& Value<ValType>::operator=(const CppType& val)
{
    setValue(val);
    return *this;
}

template <Type ValType>
Value<ValType>& Value<ValType>::operator=(const Value& other)
{
    m_val = other.m_val;
    return *this;
}

template <Type ValType>
Value<ValType>::operator CppType() const
{
    return value();
}

template <Type ValType>
bool Value<ValType>::compare(const Attribute& other) const
{
    if (auto casted = dynamic_cast<const Value<ValType>*>(&other)) {
        return casted->value() == value();
    }
    return false;
}

template <Type ValType>
std::string Value<ValType>::typeName() const
{
    return "Value";
}

template <Type ValType>
void Value<ValType>::set(const Attribute& other)
{
    if (auto casted = dynamic_cast<const Value<ValType>*>(&other)) {
        setValue(*casted);
    }
}

template <Type ValType>
void Value<ValType>::set(Attribute&& other)
{
    if (auto casted = dynamic_cast<const Value<ValType>*>(&other)) {
        setValue(std::move(*casted));
    }
}

template <Type ValType>
bool Value<ValType>::hasValue() const
{
    if constexpr (ValType == Type::Float) {
        return std::fabs(m_val - m_def) > std::numeric_limits<float>::epsilon();
    } else if constexpr (ValType == Type::Double) {
        return std::fabs(m_val - m_def) > std::numeric_limits<double>::epsilon();
    } else {
        return m_val != m_def;
    }
}

template <Type ValType>
Type Value<ValType>::valueType() const
{
    return ValType;
}

template <Type ValType>
void Value<ValType>::clear()
{
    setValue(m_def);
}

// =========================================================================================================================================

} // namespace pack
