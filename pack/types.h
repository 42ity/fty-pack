/*  ========================================================================
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
    ========================================================================
*/
#pragma once
#include <string>
#include "pack/attribute.h"
#include "pack/node.h"
#include "pack/magic_enum.h"
#include <sstream>

namespace pack {

enum class Type
{
    Unknown,
    String,
    Int32,
    Int64,
    UInt32,
    UInt64,
    Float,
    Double,
    Bool,
    UChar
};

template<Type>
struct ResolveType;

template<> struct ResolveType<Type::String> { using type = std::string; };
template<> struct ResolveType<Type::Int32> { using type = int32_t; };
template<> struct ResolveType<Type::Int64> { using type = int64_t; };
template<> struct ResolveType<Type::UInt32> { using type = uint32_t; };
template<> struct ResolveType<Type::UInt64> { using type = uint64_t; };
template<> struct ResolveType<Type::Float> { using type = float; };
template<> struct ResolveType<Type::Double> { using type = double; };
template<> struct ResolveType<Type::Bool> { using type = bool; };
template<> struct ResolveType<Type::UChar> { using type = unsigned char; };

inline std::string valueTypeName(Type type)
{
    using namespace magic_enum::ostream_operators;
    std::stringstream ss;
    ss << type;
    return ss.str();
}

}
