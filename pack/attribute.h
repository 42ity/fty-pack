/*  ====================================================================================================================
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
    ====================================================================================================================
*/

#pragma once

#include <optional>
#include <string>
#include <vector>

namespace pack {

// =====================================================================================================================

// Add metainformation to the value
#define FIELD(key, ...)                                                                                                \
    {                                                                                                                  \
        this, key, ##__VA_ARGS__                                                                                       \
    }

#define META(className, ...)                                                                                           \
public:                                                                                                                \
    META_CTR(className, __VA_ARGS__)                                                                                   \
    META_FIELDS(className, __VA_ARGS__)

#define META_WOC(className, ...)                                                                                       \
public:                                                                                                                \
    META_FIELDS(className, __VA_ARGS__)

// =====================================================================================================================

#define META_CTR(className, ...)                                                                                       \
    className(const className& other)                                                                                  \
    {                                                                                                                  \
        for (auto& it : fields()) {                                                                                    \
            it->set(*other.fieldByKey(it->key()));                                                                     \
        }                                                                                                              \
    }                                                                                                                  \
    className(className&& other)                                                                                       \
    {                                                                                                                  \
        for (auto& it : fields()) {                                                                                    \
            it->set(std::move(*other.fieldByKey(it->key())));                                                          \
        }                                                                                                              \
    }                                                                                                                  \
    inline className& operator=(const className& other)                                                                \
    {                                                                                                                  \
        for (auto& it : fields()) {                                                                                    \
            it->set(*other.fieldByKey(it->key()));                                                                     \
        }                                                                                                              \
        return *this;                                                                                                  \
    }                                                                                                                  \
    inline className& operator=(className&& other)                                                                     \
    {                                                                                                                  \
        for (auto& it : fields()) {                                                                                    \
            it->set(std::move(*other.fieldByKey(it->key())));                                                          \
        }                                                                                                              \
        return *this;                                                                                                  \
    }

#define META_FIELDS(className, ...)                                                                                    \
public:                                                                                                                \
    inline std::vector<const pack::Attribute*> fields() const override                                                 \
    {                                                                                                                  \
        return std::apply(                                                                                             \
            [](const auto&... elems) {                                                                                 \
                return std::vector<const pack::Attribute*>{&elems...};                                                 \
            },                                                                                                         \
            std::forward_as_tuple(__VA_ARGS__));                                                                       \
    }                                                                                                                  \
    inline std::vector<pack::Attribute*> fields() override                                                             \
    {                                                                                                                  \
        return std::apply(                                                                                             \
            [](auto&... elems) {                                                                                       \
                return std::vector<pack::Attribute*>{&elems...};                                                       \
            },                                                                                                         \
            std::forward_as_tuple(__VA_ARGS__));                                                                       \
    }                                                                                                                  \
    inline std::vector<std::string> fieldNames() const override                                                        \
    {                                                                                                                  \
        return pack::split(#__VA_ARGS__);                                                                              \
    }                                                                                                                  \
    inline std::string typeName() const override                                                                       \
    {                                                                                                                  \
        return m_className;                                                                                            \
    }                                                                                                                  \
    static std::string typeInfo()                                                                                      \
    {                                                                                                                  \
        return m_className;                                                                                            \
    }                                                                                                                  \
    static constexpr const char* m_className = #className

// =====================================================================================================================

std::vector<std::string> split(const std::string& str);

// =====================================================================================================================

class Attribute
{
public:
    enum class NodeType
    {
        Node,
        Value,
        Enum,
        List,
        Map
    };

public:
    Attribute(NodeType type, Attribute* parent, const std::string& key = {});
    virtual ~Attribute();

    virtual bool        compare(const Attribute& other) const = 0;
    virtual std::string typeName() const                      = 0;
    virtual void        set(const Attribute& other)           = 0;
    virtual void        set(Attribute&& other)                = 0;
    virtual bool        hasValue() const                      = 0;
    virtual void        clear()                               = 0;

    const std::string& key() const;

    bool operator==(const Attribute& other) const;
    bool operator!=(const Attribute& other) const;

    const Attribute* parent() const;
    NodeType         type() const;

protected:
    Attribute*  m_parent = nullptr;
    std::string m_key;
    NodeType    m_type;
};

// =====================================================================================================================

} // namespace pack
