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
#include "pack/pack.h"

namespace pack {

// =========================================================================================================================================

template <typename Worker>
class Deserialize
{
public:
    template <typename Resource>
    static void visit(INode& node, const Resource& res)
    {
        Worker::unpackValue(node, res);
    }

    template <typename Resource>
    static void visit(IEnum& en, const Resource& res)
    {
        Worker::unpackValue(en, res);
    }

    template <typename Resource>
    static void visit(IProtoMap& map, const Resource& res)
    {
        Worker::unpackValue(map, res);
    }

    template <typename Resource>
    static void visit(IList& list, const Resource& res)
    {
        if (auto casted = dynamic_cast<IObjectList*>(&list)) {
            Worker::unpackValue(*casted, res);
        }

        if (auto casted = dynamic_cast<IValueList*>(&list)) {
            switch (casted->valueType()) {
                case Type::Bool:
                    Worker::unpackValue(static_cast<ValueList<Type::Bool>&>(list), res);
                    break;
                case Type::Double:
                    Worker::unpackValue(static_cast<ValueList<Type::Double>&>(list), res);
                    break;
                case Type::Float:
                    Worker::unpackValue(static_cast<ValueList<Type::Float>&>(list), res);
                    break;
                case Type::String:
                    Worker::unpackValue(static_cast<ValueList<Type::String>&>(list), res);
                    break;
                case Type::Int32:
                    Worker::unpackValue(static_cast<ValueList<Type::Int32>&>(list), res);
                    break;
                case Type::UInt32:
                    Worker::unpackValue(static_cast<ValueList<Type::UInt32>&>(list), res);
                    break;
                case Type::Int64:
                    Worker::unpackValue(static_cast<ValueList<Type::Int64>&>(list), res);
                    break;
                case Type::UInt64:
                    Worker::unpackValue(static_cast<ValueList<Type::UInt64>&>(list), res);
                    break;
                case Type::UChar:
                    Worker::unpackValue(static_cast<ValueList<Type::UChar>&>(list), res);
                    break;
                case Type::Unknown:
                    throw std::runtime_error("Unsupported type to unpack");
            }
        }
    }

    template <typename Resource>
    static void visit(IMap& map, const Resource& res)
    {
        if (auto casted = dynamic_cast<IValueMap*>(&map)) {
            switch (casted->valueType()) {
                case Type::Bool:
                    Worker::unpackValue(static_cast<ValueMap<Type::Bool>&>(map), res);
                    break;
                case Type::Double:
                    Worker::unpackValue(static_cast<ValueMap<Type::Double>&>(map), res);
                    break;
                case Type::Float:
                    Worker::unpackValue(static_cast<ValueMap<Type::Float>&>(map), res);
                    break;
                case Type::String:
                    Worker::unpackValue(static_cast<ValueMap<Type::String>&>(map), res);
                    break;
                case Type::Int32:
                    Worker::unpackValue(static_cast<ValueMap<Type::Int32>&>(map), res);
                    break;
                case Type::UInt32:
                    Worker::unpackValue(static_cast<ValueMap<Type::UInt32>&>(map), res);
                    break;
                case Type::Int64:
                    Worker::unpackValue(static_cast<ValueMap<Type::Int64>&>(map), res);
                    break;
                case Type::UInt64:
                    Worker::unpackValue(static_cast<ValueMap<Type::UInt64>&>(map), res);
                    break;
                case Type::UChar:
                    Worker::unpackValue(static_cast<ValueMap<Type::UChar>&>(map), res);
                    break;
                case Type::Unknown:
                    throw std::runtime_error("Unsupported type to unpack");
            }
        }
    }

    template <typename Resource>
    static void visit(IValue& value, const Resource& res)
    {
        switch (value.valueType()) {
            case Type::Bool:
                Worker::unpackValue(static_cast<Bool&>(value), res);
                break;
            case Type::Double:
                Worker::unpackValue(static_cast<Double&>(value), res);
                break;
            case Type::Float:
                Worker::unpackValue(static_cast<Float&>(value), res);
                break;
            case Type::String:
                Worker::unpackValue(static_cast<String&>(value), res);
                break;
            case Type::Int32:
                Worker::unpackValue(static_cast<Int32&>(value), res);
                break;
            case Type::UInt32:
                Worker::unpackValue(static_cast<UInt32&>(value), res);
                break;
            case Type::Int64:
                Worker::unpackValue(static_cast<Int64&>(value), res);
                break;
            case Type::UInt64:
                Worker::unpackValue(static_cast<UInt64&>(value), res);
                break;
            case Type::UChar:
                Worker::unpackValue(static_cast<UChar&>(value), res);
                break;
            case Type::Unknown:
                throw std::runtime_error("Unsupported type to unpack");
        }
    }

    template <typename Resource>
    static void visit(Attribute& node, const Resource& res)
    {
        switch (node.type()) {
            case Attribute::NodeType::Enum:
                visit(static_cast<IEnum&>(node), res);
                break;
            case Attribute::NodeType::List:
                visit(static_cast<IList&>(node), res);
                break;
            case Attribute::NodeType::Map:
                if (auto casted = dynamic_cast<IProtoMap*>(&node)) {
                    visit(*casted, res);
                }
                if (auto casted = dynamic_cast<IMap*>(&node)) {
                    visit(*casted, res);
                }
                break;
            case Attribute::NodeType::Node:
                visit(static_cast<INode&>(node), res);
                break;
            case Attribute::NodeType::Value:
                visit(static_cast<IValue&>(node), res);
                break;
            case Attribute::NodeType::Variant:
                visit(static_cast<IVariant&>(node), res);
                break;
        }
    }

    template <typename Resource>
    static void visit(IVariant& var, const Resource& res)
    {
        Worker::unpackValue(var, res);
    }

};

// =========================================================================================================================================

template <typename Worker>
class Serialize
{
public:
    template <typename Resource>
    static void visit(const INode& node, Resource& res, Option opt)
    {
        Worker::packValue(node, res, opt);
    }

    template <typename Resource>
    static void visit(const IEnum& en, Resource& res, Option opt)
    {
        Worker::packValue(en, res, opt);
    }

    template <typename Resource>
    static void visit(const IProtoMap& map, Resource& res, Option opt)
    {
        Worker::packValue(map, res, opt);
    }

    template <typename Resource>
    static void visit(const IList& list, Resource& res, Option opt)
    {
        if (auto casted = dynamic_cast<const IObjectList*>(&list)) {
            Worker::packValue(*casted, res, opt);
        }

        if (auto casted = dynamic_cast<const IValueList*>(&list)) {
            switch (casted->valueType()) {
                case Type::Bool:
                    Worker::packValue(static_cast<const ValueList<Type::Bool>&>(list), res, opt);
                    break;
                case Type::Double:
                    Worker::packValue(static_cast<const ValueList<Type::Double>&>(list), res, opt);
                    break;
                case Type::Float:
                    Worker::packValue(static_cast<const ValueList<Type::Float>&>(list), res, opt);
                    break;
                case Type::String:
                    Worker::packValue(static_cast<const ValueList<Type::String>&>(list), res, opt);
                    break;
                case Type::Int32:
                    Worker::packValue(static_cast<const ValueList<Type::Int32>&>(list), res, opt);
                    break;
                case Type::UInt32:
                    Worker::packValue(static_cast<const ValueList<Type::UInt32>&>(list), res, opt);
                    break;
                case Type::Int64:
                    Worker::packValue(static_cast<const ValueList<Type::Int64>&>(list), res, opt);
                    break;
                case Type::UInt64:
                    Worker::packValue(static_cast<const ValueList<Type::UInt64>&>(list), res, opt);
                    break;
                case Type::UChar:
                    Worker::packValue(static_cast<const ValueList<Type::UChar>&>(list), res, opt);
                    break;
                case Type::Unknown:
                    throw std::runtime_error("Unsupported type to unpack");
            }
        }
    }

    template <typename Resource>
    static void visit(const IMap& map, Resource& res, Option opt)
    {
        if (auto casted = dynamic_cast<const IValueMap*>(&map)) {
            switch (casted->valueType()) {
                case Type::Bool:
                    Worker::packValue(static_cast<const ValueMap<Type::Bool>&>(map), res, opt);
                    break;
                case Type::Double:
                    Worker::packValue(static_cast<const ValueMap<Type::Double>&>(map), res, opt);
                    break;
                case Type::Float:
                    Worker::packValue(static_cast<const ValueMap<Type::Float>&>(map), res, opt);
                    break;
                case Type::String:
                    Worker::packValue(static_cast<const ValueMap<Type::String>&>(map), res, opt);
                    break;
                case Type::Int32:
                    Worker::packValue(static_cast<const ValueMap<Type::Int32>&>(map), res, opt);
                    break;
                case Type::UInt32:
                    Worker::packValue(static_cast<const ValueMap<Type::UInt32>&>(map), res, opt);
                    break;
                case Type::Int64:
                    Worker::packValue(static_cast<const ValueMap<Type::Int64>&>(map), res, opt);
                    break;
                case Type::UInt64:
                    Worker::packValue(static_cast<const ValueMap<Type::UInt64>&>(map), res, opt);
                    break;
                case Type::UChar:
                    Worker::packValue(static_cast<const ValueMap<Type::UChar>&>(map), res, opt);
                    break;
                case Type::Unknown:
                    throw std::runtime_error("Unsupported type to unpack");
            }
        }
    }

    template <typename Resource>
    static void visit(const IValue& value, Resource& res, Option opt)
    {
        switch (value.valueType()) {
            case Type::Bool:
                Worker::packValue(static_cast<const Bool&>(value), res, opt);
                break;
            case Type::Double:
                Worker::packValue(static_cast<const Double&>(value), res, opt);
                break;
            case Type::Float:
                Worker::packValue(static_cast<const Float&>(value), res, opt);
                break;
            case Type::String:
                Worker::packValue(static_cast<const String&>(value), res, opt);
                break;
            case Type::Int32:
                Worker::packValue(static_cast<const Int32&>(value), res, opt);
                break;
            case Type::UInt32:
                Worker::packValue(static_cast<const UInt32&>(value), res, opt);
                break;
            case Type::Int64:
                Worker::packValue(static_cast<const Int64&>(value), res, opt);
                break;
            case Type::UInt64:
                Worker::packValue(static_cast<const UInt64&>(value), res, opt);
                break;
            case Type::UChar:
                Worker::packValue(static_cast<const UChar&>(value), res, opt);
                break;
            case Type::Unknown:
                throw std::runtime_error("Unsupported type to unpack");
        }
    }

    template <typename Resource>
    static void visit(const Attribute& node, Resource& res, Option opt)
    {
        switch (node.type()) {
            case Attribute::NodeType::Enum:
                visit(static_cast<const IEnum&>(node), res, opt);
                break;
            case Attribute::NodeType::List:
                visit(static_cast<const IList&>(node), res, opt);
                break;
            case Attribute::NodeType::Map:
                if (auto casted = dynamic_cast<const IProtoMap*>(&node)) {
                    visit(*casted, res, opt);
                }
                if (auto casted = dynamic_cast<const IMap*>(&node)) {
                    visit(*casted, res, opt);
                }
                break;
            case Attribute::NodeType::Node:
                visit(static_cast<const INode&>(node), res, opt);
                break;
            case Attribute::NodeType::Value:
                visit(static_cast<const IValue&>(node), res, opt);
                break;
        case Attribute::NodeType::Variant:
            visit(static_cast<const IVariant&>(node), res, opt);
            break;
        }
    }

    template <typename Resource>
    static void visit(const IVariant& var, Resource& res, Option opt)
    {
        Worker::packValue(var, res, opt);
    }
};

// =========================================================================================================================================

} // namespace pack
