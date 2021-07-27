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
#include "fty/convert.h"
#include "pack/enum.h"
#include "pack/list.h"
#include "pack/map.h"
#include "pack/node.h"
#include "pack/proto-map.h"
#include "pack/serialization.h"
#include "pack/value.h"
#include "pack/variant.h"

namespace pack {

using Int32  = Value<Type::Int32>;
using Int64  = Value<Type::Int64>;
using UInt32 = Value<Type::UInt32>;
using UInt64 = Value<Type::UInt64>;
using Float  = Value<Type::Float>;
using Double = Value<Type::Double>;
using Bool   = Value<Type::Bool>;
using UChar  = Value<Type::UChar>;

// =========================================================================================================================================

class String : public Value<Type::String>
{
public:
    using pack::Value<Type::String>::Value;
    using pack::Value<Type::String>::operator=;

    inline bool empty() const;
    inline int  size() const;
};


// =========================================================================================================================================

class Binary : public ValueList<Type::UChar>
{
public:
    using pack::ValueList<Type::UChar>::ValueList;
    using pack::ValueList<Type::UChar>::operator=;
    using pack::ValueList<Type::UChar>::set;

    inline bool        empty() const;
    inline void        setString(const std::string& data);
    inline void        setString(const char* data, size_t size);
    inline std::string asString() const;

public:
    template <typename T>
    fty::Expected<T> decode() const;
};

// =========================================================================================================================================

using Int32List  = ValueList<Type::Int32>;
using Int64List  = ValueList<Type::Int64>;
using UInt32List = ValueList<Type::UInt32>;
using UInt64List = ValueList<Type::UInt64>;
using FloatList  = ValueList<Type::Float>;
using DoubleList = ValueList<Type::Double>;
using BoolList   = ValueList<Type::Bool>;
using StringList = ValueList<Type::String>;

using Int32Map  = ValueMap<Type::Int32>;
using Int64Map  = ValueMap<Type::Int64>;
using UInt32Map = ValueMap<Type::UInt32>;
using UInt64Map = ValueMap<Type::UInt64>;
using FloatMap  = ValueMap<Type::Float>;
using DoubleMap = ValueMap<Type::Double>;
using BoolMap   = ValueMap<Type::Bool>;
using StringMap = ValueMap<Type::String>;

// =========================================================================================================================================
// Implementation
// =========================================================================================================================================

template <typename T, typename=std::enable_if_t<!std::is_same_v<T, std::decay_t<String>>>>
inline bool operator==(const String& l, const T& r)
{
    return l.value() == r;
}

template <typename T, typename=std::enable_if_t<!std::is_same_v<T, std::decay_t<String>>>>
inline bool operator==(const T& l, const String& r)
{
    return r.value() == l;
}

bool String::empty() const
{
    return value().empty();
}

int String::size() const
{
    return int(value().size());
}

// =========================================================================================================================================

bool Binary::empty() const
{
    return value().empty();
}

void Binary::setString(const std::string& data)
{
    m_value = ListType(data.begin(), data.end());
}

void Binary::setString(const char* data, size_t size)
{
    m_value = ListType(data, data + size);
}

std::string Binary::asString() const
{
    return std::string(m_value.begin(), m_value.end());
}

template <typename T>
fty::Expected<T> Binary::decode() const
{
    if constexpr (std::is_base_of_v<Attribute, T>) {
        T out;
        if (auto res = json::deserialize(asString(), out)) {
            return out;
        } else {
            return fty::unexpected(res.error());
        }
    } else {
        return fty::convert<T>(asString());
    }
}

// =========================================================================================================================================

} // namespace pack
