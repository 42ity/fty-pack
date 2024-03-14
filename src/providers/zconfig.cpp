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

#include "pack/pack.h"
#include "pack/visitor.h"
#include <cassert>
#include <czmq.h>
#include <fty/convert.h>
#include <memory>
#include <yaml-cpp/yaml.h>
#include <zconfig.h>

namespace pack {

// =========================================================================================================================================

template <Type ValType>
struct Convert
{
    using CppType = typename ResolveType<ValType>::type;

    static void decode(Value<ValType>& node, zconfig_t* zconf)
    {
        node = fty::convert<CppType>(zconfig_value(zconf));
    }

    static void decode(ValueList<ValType>& node, zconfig_t* zconf)
    {
        if constexpr (ValType == Type::UChar) {
            node.setValue(typename ValueList<ValType>::ListType(YAML::DecodeBase64(zconfig_value(zconf))));
        } else {
            for (zconfig_t* item = zconfig_child(zconf); item; item = zconfig_next(item)) {
                node.append(fty::convert<CppType>(zconfig_value(item)));
            }
        }
    }

    static void decode(ValueMap<ValType>& node, zconfig_t* zconf)
    {
        for (zconfig_t* item = zconfig_child(zconf); item; item = zconfig_next(item)) {
            node.append(fty::convert<std::string>(zconfig_name(item)), fty::convert<CppType>(zconfig_value(item)));
        }
    }

    static void encode(const Value<ValType>& node, zconfig_t* zconf)
    {
        zconfig_set_value(zconf, "%s", fty::convert<std::string>(node.value()).c_str());
    }

    static void encode(const ValueList<ValType>& node, zconfig_t* zconf)
    {
        int i = 1;
        if constexpr (ValType == Type::Bool) {
            for (auto it : node) {
                auto child = zconfig_new(fty::convert<std::string>(i++).c_str(), zconf);
                zconfig_set_value(child, "%s", fty::convert<std::string>(it).c_str());
            }
        } else if constexpr (ValType == Type::UChar) {
            zconfig_set_value(zconf, "%s", YAML::EncodeBase64(node.value().data(), node.size()).c_str());
        } else {
            for (const auto& it : node) {
                auto child = zconfig_new(fty::convert<std::string>(i++).c_str(), zconf);
                zconfig_set_value(child, "%s", fty::convert<std::string>(it).c_str());
            }
        }
    }

    static void encode(const ValueMap<ValType>& node, zconfig_t* zconf)
    {
        for (const auto& it : node) {
            auto child = zconfig_new(fty::convert<std::string>(it.first).c_str(), zconf);
            zconfig_set_value(child, "%s", fty::convert<std::string>(it.second).c_str());
        }
    }
};

static void copy(zconfig_t* dest, zconfig_t* src)
{
    for (zconfig_t* item = zconfig_child(src); item; item = zconfig_next(item)) {
        auto child = zconfig_new(zconfig_name(item), dest);
        if (zconfig_value(item)) {
            zconfig_set_value(child, "%s", zconfig_value(item));
        } else if (zconfig_child(child)) {
            copy(child, item);
        }
    }
}

// =========================================================================================================================================

class ZSerializer : public Serialize<ZSerializer>
{
public:
    template <typename T>
    static void packValue(const T& val, zconfig_t* zconf, Option opt)
    {
        if (val.hasValue() || fty::isSet(opt, Option::WithDefaults)) {
            Convert<T::ThisType>::encode(val, zconf);
        }
    }

    static void packValue(const IObjectMap& /*val*/, zconfig_t* /*zconf*/, Option /*opt*/)
    {
    }

    static void packValue(const IObjectList& val, zconfig_t* zconf, Option opt)
    {
        for (size_t i = 0; i < val.size(); ++i) {
            const Attribute& node  = val.get(i);
            auto             child = zconfig_new(fty::convert<std::string>(i + 1).c_str(), zconf);
            visit(node, child, opt);
        }
    }

    static void packValue(const INode& node, zconfig_t* zconf, Option opt)
    {
        for (auto& it : node.fields()) {
            if (it->hasValue() || fty::isSet(opt, Option::WithDefaults)) {
                auto child = zconfig_new(it->key().c_str(), zconf);
                visit(*it, child, opt);
            }
        }
    }

    static void packValue(const IEnum& en, zconfig_t* zconf, Option /*opt*/)
    {
        zconfig_set_value(zconf, "%s", en.asString().c_str());
    }

    static void packValue(const IProtoMap& map, zconfig_t* zconf, Option opt)
    {
        for (size_t i = 0; i < map.size(); ++i) {
            const INode& node = map.get(i);

            auto temp = zconfig_new("", nullptr);
            packValue(node, temp, opt);

            auto zkey = zconfig_locate(temp, "key");
            auto zval = zconfig_locate(temp, "value");

            auto child = zconfig_new(zconfig_value(zkey), zconf);
            if (zconfig_child(zval)) {
                copy(child, zval);
            } else {
                zconfig_set_value(child, "%s", zconfig_value(zval));
            }

            zconfig_destroy(&temp);
        }
    }

    static void packValue(const IVariant& /*val*/, zconfig_t* /*zconf*/, Option /*opt*/)
    {
    }
};

// =========================================================================================================================================

class ZDeserializer : public Deserialize<ZDeserializer>
{
public:
    template <typename T>
    static void unpackValue(T& val, zconfig_t* conf)
    {
        Convert<T::ThisType>::decode(val, conf);
    }

    static void unpackValue(IEnum& en, zconfig_t* conf)
    {
        en.fromString(zconfig_value(conf));
    }

    static void unpackValue(IObjectMap& /*list*/, zconfig_t* /*conf*/)
    {
    }

    static void unpackValue(IObjectList& list, zconfig_t* conf)
    {
        if (auto first = zconfig_child(conf)) {
            auto& obj = list.create();
            visit(obj, first);
            while ((first = zconfig_next(first)) != nullptr) {
                auto& next = list.create();
                visit(next, first);
            }
        }
    }

    static void unpackValue(INode& node, zconfig_t* conf)
    {
        for (auto& it : node.fields()) {
            if (auto found = zconfig_locate(conf, it->key().c_str())) {
                visit(*it, found);
            }
        }
    }

    static void unpackValue(IProtoMap& map, zconfig_t* conf)
    {
        for (zconfig_t* item = zconfig_child(conf); item; item = zconfig_next(item)) {
            INode& obj = map.create();

            auto temp = zconfig_new(nullptr, nullptr);
            auto zkey = zconfig_new("key", temp);
            zconfig_set_value(zkey, "%s", zconfig_name(item));
            auto zval = zconfig_new("value", temp);

            if (zconfig_child(item)) {
                copy(zval, item);
            } else {
                zconfig_set_value(zval, "%s", zconfig_value(item));
            }

            visit(obj, temp);

            zconfig_destroy(&temp);
        }
    }

    static void unpackValue(IVariant& /*var*/, zconfig_t* /*conf*/)
    {
    }
};

// =========================================================================================================================================

namespace zconfig {
    fty::Expected<std::string> serialize(const Attribute& node, Option opt)
    {
        try {
            zconfig_t* config = zconfig_new("root", nullptr);
            ZSerializer::visit(node, config, opt);
            auto        zret = zconfig_str_save(config);
            std::string ret(zret);
            zstr_free(&zret);
            zconfig_destroy(&config);
            return std::move(ret);
        } catch (const std::exception& e) {
            return fty::unexpected(e.what());
        }
    }

    fty::Expected<void> deserialize(const std::string& content, Attribute& node)
    {
        try {
            zconfig_t* config = zconfig_str_load(content.c_str());
            ZDeserializer::visit(node, config);
            zconfig_destroy(&config);
            return {};
        } catch (const std::exception& e) {
            return fty::unexpected(e.what());
        }
    }
} // namespace zconfig

// =========================================================================================================================================

} // namespace pack
