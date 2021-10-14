#include "pack/pack.h"
#include "pack/serialization.h"
#include "pack/visitor.h"
#include "utils.h"
#include <fty/flags.h>
#include <nlohmann/json.hpp>

namespace pack::json {

// =========================================================================================================================================

template <Type ValType>
struct Convert
{
    using CppType = typename ResolveType<ValType>::type;

    static void decode(Value<ValType>& node, const nlohmann::ordered_json& json)
    {
        try {
            node = json.get<CppType>();
        } catch (const nlohmann::json::type_error& err) {
            node = fty::convert<CppType>(json.get<std::string>());
        }
    }

    static void decode(ValueList<ValType>& node, const nlohmann::ordered_json& json)
    {
        if constexpr (ValType == Type::UChar) {
            if (!json.is_null()) {
                auto it = json.get<typename ValueList<ValType>::ListType>();
                node.setValue(it);
            }
        } else {
            for (const auto& it : json) {
                node.append(it.get<CppType>());
            }
        }
    }

    static void decode(ValueMap<ValType>& node, const nlohmann::ordered_json& json)
    {
        for (const auto& it : json.items()) {
            auto val = it.value().get<CppType>();
            node.append(it.key(), val);
        }
    }

    static void encode(const Value<ValType>& node, nlohmann::ordered_json& json)
    {
        json = node.value();
    }

    static void encode(const ValueList<ValType>& node, nlohmann::ordered_json& json)
    {
        if (node.size()) {
            if constexpr (ValType == Type::Bool) {
                for (auto it : node) {
                    json.push_back(it);
                }
            } else if constexpr (ValType == Type::UChar) {
                json = node.value();
            } else {
                for (const auto& it : node) {
                    json.push_back(it);
                }
            }
        } else {
            json = nlohmann::json::array();
        }
    }

    static void encode(const ValueMap<ValType>& node, nlohmann::ordered_json& json)
    {
        if (node.size()) {
            for (const auto& [key, value] : node) {
                json[key] = value;
            }
        } else {
            json = nlohmann::json::object();
        }
    }
};

// =========================================================================================================================================

class JsonSerializer : public Serialize<JsonSerializer>
{
public:
    template <typename T>
    static void packValue(const T& val, nlohmann::ordered_json& json, Option opt)
    {
        if (val.hasValue() || fty::isSet(opt, Option::WithDefaults)) {
            Convert<T::ThisType>::encode(val, json);
        }
    }

    static void packValue(const IObjectMap& val, nlohmann::ordered_json& json, Option opt)
    {
        if (val.size()) {
            for (int i = 0; i < val.size(); ++i) {
                const auto&            key  = val.keyByIndex(i);
                const Attribute&       node = val.get(key);
                nlohmann::ordered_json child;

                visit(node, child, opt);
                json[key] = child;
            }
        } else if (fty::isSet(opt, Option::WithDefaults)) {
            json = nlohmann::json::object();
        }
    }

    static void packValue(const IObjectList& val, nlohmann::ordered_json& json, Option opt)
    {
        if (val.size()) {
            for (int i = 0; i < val.size(); ++i) {
                const Attribute&       node = val.get(i);
                nlohmann::ordered_json child;
                visit(node, child, opt);
                json.push_back(child);
            }
        } else if (fty::isSet(opt, Option::WithDefaults)) {
            json = nlohmann::json::array();
        }
    }

    static void packValue(const INode& node, nlohmann::ordered_json& json, Option opt)
    {
        json = nlohmann::json::object();
        for (auto& it : node.fields()) {
            if (node.hasValue() || fty::isSet(opt, Option::WithDefaults)) {
                nlohmann::ordered_json& child = json[it->key()];
                visit(*it, child, opt);
            }
        }
    }

    static void packValue(const IEnum& en, nlohmann::ordered_json& json, Option /*opt*/)
    {
        json = en.asString();
    }

    static void packValue(const IProtoMap& map, nlohmann::ordered_json& json, Option opt)
    {
        for (int i = 0; i < map.size(); ++i) {
            const INode& node = map.get(i);

            nlohmann::ordered_json temp;
            packValue(node, temp, opt);

            std::string key = temp["key"];
            json[key]       = temp["value"];
        }
    }

    static void packValue(const IVariant& var, nlohmann::ordered_json& yaml, Option opt)
    {
        if (auto ptr = var.get()) {
            packValue(static_cast<const INode&>(*ptr), yaml, opt);
        }
    }
};

// =========================================================================================================================================

class JsonDeserializer : public Deserialize<JsonDeserializer>
{
public:
    template <typename T>
    static void unpackValue(T& val, const nlohmann::ordered_json& json)
    {
        Convert<T::ThisType>::decode(val, json);
    }

    static void unpackValue(IEnum& en, const nlohmann::ordered_json& json)
    {
        en.fromString(json.get<std::string>());
    }

    static void unpackValue(IObjectMap& map, const nlohmann::ordered_json& json)
    {
        for (const auto& [key, value] : json.items()) {
            auto& obj = map.create(key);
            visit(obj, value);
        }
    }

    static void unpackValue(IObjectList& list, const nlohmann::ordered_json& json)
    {
        for (const auto& child : json) {
            auto& obj = list.create();
            visit(obj, child);
        }
    }

    static void unpackValue(INode& node, const nlohmann::ordered_json& json)
    {
        for (auto& it : node.fields()) {
            if (json.contains(it->key())) {
                visit(*it, json[it->key()]);
            }
        }
    }

    static void unpackValue(IProtoMap& map, const nlohmann::ordered_json& json)
    {
        for (const auto& [key, value] : json.items()) {
            INode& obj = map.create();

            nlohmann::ordered_json temp;
            temp["key"]   = key;
            temp["value"] = value;

            visit(obj, temp);
        }
    }

    static void unpackValue(IVariant& var, const nlohmann::ordered_json& json)
    {
        std::vector<std::string> keys;
        for (const auto& it : json.items()) {
            keys.push_back(it.key());
        }
        if (var.findBetter(keys)) {
            if (auto ptr = var.get()) {
                unpackValue(static_cast<INode&>(*ptr), json);
            }
        }
    }
};


// =========================================================================================================================================

fty::Expected<std::string> serialize(const Attribute& node, Option opt)
{
    try {
        nlohmann::ordered_json json;
        JsonSerializer::visit(node, json, opt);
        return json.dump();
    } catch (const std::exception& e) {
        return fty::unexpected(e.what());
    }
}

fty::Expected<void> deserialize(const std::string& content, Attribute& node)
{
    try {
        nlohmann::ordered_json json = nlohmann::ordered_json::parse(content);
        JsonDeserializer::visit(node, json);
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

} // namespace pack::json
