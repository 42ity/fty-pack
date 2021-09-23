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
#include "pack/serialization.h"
#include "pack/visitor.h"
#include <fstream>
#include <fty/flags.h>
#include <yaml-cpp/yaml.h>

namespace pack {

// =========================================================================================================================================

template <Type ValType>
struct Convert
{
    using CppType = typename ResolveType<ValType>::type;

    static void decode(Value<ValType>& node, const YAML::Node& yaml)
    {
        node = yaml.as<CppType>();
    }

    static void decode(ValueList<ValType>& node, const YAML::Node& yaml)
    {
        if constexpr (ValType == Type::UChar) {
            YAML::Binary         binary = yaml.as<YAML::Binary>();
            const unsigned char* data   = binary.data();
            std::size_t          size   = binary.size();
            node.setValue({data, data + size});
        } else {
            for (const auto& it : yaml) {
                node.append(it.as<CppType>());
            }
        }
    }

    static void decode(ValueMap<ValType>& node, const YAML::Node& yaml)
    {
        for (const auto& it : yaml) {
            node.append(it.first.as<std::string>(), it.second.as<CppType>());
        }
    }

    static void encode(const Value<ValType>& node, YAML::Node& yaml)
    {
        yaml = YAML::convert<CppType>::encode(node.value());
    }

    static void encode(const ValueList<ValType>& node, YAML::Node& yaml)
    {
        if constexpr (ValType == Type::Bool) {
            for (auto it : node) {
                yaml.push_back(YAML::convert<CppType>::encode(it));
            }
        } else if constexpr (ValType == Type::UChar) {
            yaml = YAML::Binary(&node.value()[0], size_t(node.size()));
        } else {
            for (const auto& it : node) {
                yaml.push_back(YAML::convert<CppType>::encode(it));
            }
        }
    }

    static void encode(const ValueMap<ValType>& node, YAML::Node& yaml)
    {
        for (const auto& it : node) {
            yaml[it.first] = YAML::convert<CppType>::encode(it.second);
        }
    }
};

// =========================================================================================================================================

class YamlDeserializer : public Deserialize<YamlDeserializer>
{
public:
    template <typename T>
    static void unpackValue(T& val, const YAML::Node& yaml)
    {
        Convert<T::ThisType>::decode(val, yaml);
    }

    static void unpackValue(IEnum& en, const YAML::Node& yaml)
    {
        en.fromString(yaml.as<std::string>());
    }

    static void unpackValue(IObjectMap& map, const YAML::Node& yaml)
    {
        for (const auto& child : yaml) {
            std::string key = child.first.as<std::string>();
            auto& obj = map.create(key);
            visit(obj, child.second);
        }
    }

    static void unpackValue(IObjectList& list, const YAML::Node& yaml)
    {
        for (const auto& child : yaml) {
            auto& obj = list.create();
            visit(obj, child);
        }
    }

    static void unpackValue(INode& node, const YAML::Node& yaml)
    {
        for (auto& it : node.fields()) {
            auto found = yaml[it->key()];
            if (found.IsDefined()) {
                visit(*it, found);
            }
        }
    }

    static void unpackValue(IProtoMap& map, const YAML::Node& yaml)
    {
        for (const auto& child : yaml) {
            INode& obj = map.create();

            YAML::Node temp;
            temp["key"]   = child.first;
            temp["value"] = child.second;

            visit(obj, temp);
        }
    }

    static void unpackValue(IVariant& var, const YAML::Node& yaml)
    {
        std::vector<std::string> keys;
        for (const auto& it : yaml) {
            keys.push_back(it.first.as<std::string>());
        }
        if (var.findBetter(keys)) {
            if (auto ptr = var.get()) {
                unpackValue(static_cast<INode&>(*ptr), yaml);
            }
        }
    }
};

// =========================================================================================================================================

class YamlSerializer : public Serialize<YamlSerializer>
{
public:
    template <typename T>
    static void packValue(const T& val, YAML::Node& yaml, Option opt)
    {
        if (val.hasValue() || fty::isSet(opt, Option::WithDefaults)) {
            Convert<T::ThisType>::encode(val, yaml);
        }
    }

    static void packValue(const IObjectMap& val, YAML::Node& yaml, Option opt)
    {
        for (int i = 0; i < val.size(); ++i) {
            const auto&      key  = val.keyByIndex(i);
            const Attribute& node = val.get(key);
            YAML::Node       child;

            visit(node, child, opt);
            yaml[key] = child;
        }
    }

    static void packValue(const IObjectList& val, YAML::Node& yaml, Option opt)
    {
        for (int i = 0; i < val.size(); ++i) {
            const Attribute& node = val.get(i);
            YAML::Node       child;
            visit(node, child, opt);
            yaml.push_back(child);
        }
    }

    static void packValue(const INode& node, YAML::Node& yaml, Option opt)
    {
        for (auto& it : node.fields()) {
            if (node.hasValue() || fty::isSet(opt, Option::WithDefaults)) {
                YAML::Node child = yaml[it->key()];
                visit(*it, child, opt);
            }
        }
    }

    static void packValue(const IEnum& en, YAML::Node& yaml, Option /*opt*/)
    {
        yaml = YAML::convert<std::string>::encode(en.asString());
    }

    static void packValue(const IProtoMap& map, YAML::Node& yaml, Option opt)
    {
        for (int i = 0; i < map.size(); ++i) {
            const INode& node = map.get(i);

            YAML::Node temp;
            packValue(node, temp, opt);

            yaml[temp["key"]] = temp["value"];
        }
    }

    static void packValue(const IVariant& var, YAML::Node& yaml, Option opt)
    {
        if (auto ptr = var.get()) {
            packValue(static_cast<const INode&>(*ptr), yaml, opt);
        }
    }
};

static fty::Expected<std::string> read(const std::string& filename)
{
    std::ifstream st(filename);
    if (st.is_open()) {
        return fty::Expected<std::string>({std::istreambuf_iterator<char>(st), std::istreambuf_iterator<char>()});
    }
    return fty::unexpected("Cannot read file {}", filename);
}

static fty::Expected<void> write(const std::string& filename, const std::string& content)
{
    std::ofstream st(filename);
    if (st.is_open()) {
        st << content;
        st.close();
        return {};
    }
    return fty::unexpected("Cannot read file {}", filename);
}

// =========================================================================================================================================

namespace yaml {
    fty::Expected<std::string> serialize(const Attribute& node, Option opt)
    {
        try {
            YAML::Node yaml;
            YamlSerializer::visit(node, yaml, opt);
            return YAML::Dump(yaml);
        } catch (const std::exception& e) {
            return fty::unexpected(e.what());
        }
    }

    fty::Expected<void> deserialize(const std::string& content, Attribute& node)
    {
        try {
            YAML::Node yaml = YAML::Load(content);
            YamlDeserializer::visit(node, yaml);
            return {};
        } catch (const std::exception& e) {
            return fty::unexpected(e.what());
        }
    }

    fty::Expected<void> deserializeFile(const std::string& fileName, Attribute& node)
    {
        if (auto cnt = read(fileName)) {
            return deserialize(*cnt, node);
        } else {
            return fty::unexpected(cnt.error());
        }
    }

    fty::Expected<void> serializeFile(const std::string& fileName, const Attribute& node, Option opt)
    {
        if (auto ret = serialize(node, opt)) {
            if (auto res = write(fileName, *ret); !res) {
                return fty::unexpected(res.error());
            }
            return {};
        } else {
            return fty::unexpected(ret.error());
        }
    }
} // namespace yaml

// =========================================================================================================================================

namespace json {
    fty::Expected<std::string> serialize(const Attribute& node, Option opt)
    {
        try {
            YAML::Node yaml;
            YamlSerializer::visit(node, yaml, opt);

            YAML::Emitter out;
            out.SetIndent(4);

            if (!yaml.IsNull()) {
                out << YAML::DoubleQuoted << YAML::Flow << yaml;
                return std::string(out.c_str());
            } else {
                switch (node.type()) {
                    case Attribute::NodeType::List:
                        return std::string("[]");
                    case Attribute::NodeType::Node:
                    case Attribute::NodeType::Variant:
                    case Attribute::NodeType::Map:
                        return std::string("{}");
                    case Attribute::NodeType::Enum:
                    case Attribute::NodeType::Value:
                        return std::string("null");
                }
                return std::string("null");
            }
        } catch (const std::exception& e) {
            return fty::unexpected(e.what());
        }
    }

    fty::Expected<void> deserialize(const std::string& content, Attribute& node)
    {
        try {
            YAML::Node yaml = YAML::Load(content);
            YamlDeserializer::visit(node, yaml);
            return {};
        } catch (const std::exception& e) {
            return fty::unexpected(e.what());
        }
    }

    fty::Expected<void> deserializeFile(const std::string& fileName, Attribute& node)
    {
        if (auto cnt = read(fileName)) {
            return deserialize(*cnt, node);
        } else {
            return fty::unexpected(cnt.error());
        }
    }

    fty::Expected<void> serializeFile(const std::string& fileName, const Attribute& node, Option opt)
    {
        if (auto ret = serialize(node, opt)) {
            if (auto res = write(fileName, *ret); !res) {
                return fty::unexpected(res.error());
            }
            return {};
        } else {
            return fty::unexpected(ret.error());
        }
    }
} // namespace json

// =========================================================================================================================================

} // namespace pack
