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
#include <algorithm>
#include <map>
#include <regex>
#include <fmt/core.h>

namespace pack {

// =========================================================================================================================================

class IMap : public Attribute
{
public:
    IMap()
        : Attribute(NodeType::Map, nullptr, "")
    {
    }

    IMap(Attribute* parent, const std::string& key = {})
        : Attribute(NodeType::Map, parent, key)
    {
    }
};

// =========================================================================================================================================

/// Object map interface.
///
/// This container is used to keep Node types
class IObjectMap : public IMap
{
public:
    using IMap::IMap;

public:
    /// Returns INode interface by index
    virtual const Attribute&   get(const std::string& key) const = 0;
    virtual Attribute&         create(const std::string& key)    = 0;
    virtual int                size() const                      = 0;
    virtual const std::string& keyByIndex(int index) const       = 0;
};

// =========================================================================================================================================

/// Values list interface.
///
/// This container is used to keep values simple types
class IValueMap : public IMap
{
public:
    using IMap::IMap;

public:
    /// Returns values type
    virtual Type valueType() const = 0;
};

// =========================================================================================================================================

/// Object map class implementation.
///
/// Map is a unsorted associative container that contains key-value pairs.
/// Value T is specialized to use pack::Node derivatives.
template <typename T>
class Map : public IObjectMap
{
public:
    using MapType       = std::vector<std::pair<std::string, T>>;
    using Iterator      = typename MapType::iterator;
    using ConstIterator = typename MapType::const_iterator;

public:
    using IObjectMap::IObjectMap;

public:
    /// Iterators definition
    ConstIterator begin() const;
    ConstIterator end() const;
    Iterator      begin();
    Iterator      end();

    /// Returns internal std container from the Map
    const MapType& value() const;
    /// Set values of the map by internal representation
    void           setValue(const MapType& val);
    /// Returns true if the key exists in the map
    bool           contains(const std::string& key) const;
    /// Returns first reference to Map value by it's key. If the key
    /// doesn't exist, return the new created element (default value)
    T&             operator[](const std::string& key);
    /// Returns first reference to Map value by it's key. If the key
    /// doesn't exist, return an exception
    const T&       operator[](const std::string& key) const;
    /// Return size of the map
    int            size() const override;
    /// Operators definition
    Map&           operator=(const Map& other);
                   operator const T&() const;
    Map&           operator=(const MapType& val);

    /// Return type info
    static std::string typeInfo();
    /// Append new empty element to map and return it (even if the key already exist)
    T&                 append(const std::string& key);
    /// Append new element to map (even if the key already exist)
    void               append(const std::string& key, const T& val);
    /// Get attribute of element
    const Attribute&   get(const std::string& key) const override;
    /// Set value of an element. If the key not find, create a new element
    void               set(const std::string& key, const T& val);
    /// Create empty attribute of element and return a reference on it
    Attribute&         create(const std::string& key) override;
    /// Return key element at index
    const std::string& keyByIndex(int index) const override;

public:
    bool        compare(const Attribute& other) const override;
    std::string typeName() const override;
    void        set(const Attribute& other) override;
    void        set(Attribute&& other) override;
    bool        hasValue() const override;
    void        clear() override;

private:
    MapType m_value;
};

// =========================================================================================================================================

/// Values list class implementation.
///
/// ValueMap is a unsorted associative container that contains key-value pairs.
/// ValType is specialized to use Basic types.
template <Type ValType>
class ValueMap : public IValueMap
{
public:
    static constexpr Type ThisType = ValType;

    using CppType       = typename ResolveType<ValType>::type;
    using MapType       = std::map<std::string, CppType>;
    using Iterator      = typename MapType::iterator;
    using ConstIterator = typename MapType::const_iterator;

public:
    using IValueMap::IValueMap;
    ValueMap(const ValueMap&) = default;

public:
    /// Iterators definition
    ConstIterator begin() const;
    ConstIterator end() const;
    Iterator      begin();
    Iterator      end();

    /// Get map
    const MapType& value() const;
    /// Set values of the map by internal representation
    void           setValue(const MapType& val);
    /// Returns true if the key exists in the map
    bool           contains(const std::string& key) const;
    /// Returns first reference to Map value by it's key. If the key
    /// doesn't exist, return the new created element (default value)
    CppType&       operator[](const std::string& key);
    /// Returns first reference to Map value by it's key. If the key
    /// doesn't exist, return an exception
    const CppType& operator[](const std::string& key) const;
    /// Return size of the map
    int            size() const;
    /// Operators definition
    ValueMap&      operator=(const ValueMap& other);
    ValueMap&      operator=(const MapType& val);
    /// Append new element to map (even if the key already exist)
    void           append(const std::string& key, const CppType& val);
    /// Set value of an element. If the key not find, create a new element
    void           set(const std::string& key, const CppType& val);

    /// Return type info
    static std::string typeInfo();

    /// Search functions
    template <typename T>
    Iterator find(const T& pred);
    Iterator find(const std::string& key);
    Iterator find(const std::regex& rex);

public:
    bool        compare(const Attribute& other) const override;
    std::string typeName() const override;
    void        set(const Attribute& other) override;
    void        set(Attribute&& other) override;
    bool        hasValue() const override;
    void        clear() override;
    Type        valueType() const override;

private:
    MapType m_value;
};

// =========================================================================================================================================

template <typename T>
const typename Map<T>::MapType& Map<T>::value() const
{
    return m_value;
}

template <typename T>
typename Map<T>::ConstIterator Map<T>::begin() const
{
    return m_value.begin();
}

template <typename T>
typename Map<T>::ConstIterator Map<T>::end() const
{
    return m_value.end();
}

template <typename T>
typename Map<T>::Iterator Map<T>::begin()
{
    return m_value.begin();
}

template <typename T>
typename Map<T>::Iterator Map<T>::end()
{
    return m_value.end();
}

template <typename T>
int Map<T>::size() const
{
    return m_value.size();
}

template <typename T>
T& Map<T>::operator[](const std::string& key)
{
    const auto found = std::find_if(m_value.begin(), m_value.end(), [&](const auto& pair) {
        return pair.first == key;
    });

    if (found != m_value.end()) {
        return found->second;
    }

    // if not found, return new element inserted
    T val;
    m_value.push_back(std::make_pair(key, val));
    return m_value.back().second;
}

template <typename T>
const T& Map<T>::operator[](const std::string& key) const
{
    const auto found = std::find_if(m_value.begin(), m_value.end(), [&](const auto& pair) {
        return pair.first == key;
    });

    if (found != m_value.end()) {
        return found->second;
    }

    // if not found, return an exception
    throw std::out_of_range(fmt::format("Key {} was not found", key));
}

template <typename T>
Map<T>& Map<T>::operator=(const Map& other)
{
    setValue(other.m_value);
    return *this;
}

template <typename T>
Map<T>::operator const T&() const
{
    return m_value;
}

template <typename T>
Map<T>& Map<T>::operator=(const MapType& val)
{
    setValue(val);
    return *this;
}

template <typename T>
void Map<T>::setValue(const MapType& val)
{
    if (m_value == val)
        return;

    m_value = val;
}

template <typename T>
bool Map<T>::contains(const std::string& key) const
{
    return std::find_if(m_value.begin(), m_value.end(), [&](const auto& pair) {
               return pair.first == key;
           }) != m_value.end();
}

template <typename T>
bool Map<T>::compare(const Attribute& other) const
{
    if (auto casted = dynamic_cast<const Map<T>*>(&other)) {
        return casted->value() == value();
    }
    return false;
}

template <typename T>
std::string Map<T>::typeName() const
{
    return "Map";
}

template <typename T>
void Map<T>::set(const Attribute& other)
{
    if (auto casted = dynamic_cast<const Map<T>*>(&other)) {
        setValue(casted->value());
    }
}

template <typename T>
void Map<T>::set(Attribute&& other)
{
    if (auto casted = dynamic_cast<Map<T>*>(&other)) {
        setValue(std::move(casted->value()));
    }
}

template <typename T>
bool Map<T>::hasValue() const
{
    return !m_value.empty();
}

template <typename T>
void Map<T>::clear()
{
    m_value.clear();
}

template <typename T>
std::string Map<T>::typeInfo()
{
    return "Map<" + T::typeInfo() + ">";
}

template <typename T>
T& Map<T>::append(const std::string& key)
{
    T it;
    // append element even if the key exist
    m_value.push_back(std::make_pair(key, it));
    return m_value.back().second;
}

template <typename T>
void Map<T>::append(const std::string& key, const T& val)
{
    // append element even if the key exist
    m_value.push_back(std::make_pair(key, val));
}

template <typename T>
const Attribute& Map<T>::get(const std::string& key) const
{
    // get element if the key exist, else an exception is triggered
    auto found = std::find_if(m_value.begin(), m_value.end(), [&](const auto& pair) {
        return pair.first == key;
    });

    if (found != m_value.end()) {
        return found->second;
    }

    throw std::out_of_range(fmt::format("Key {} was not found", key));
}

template <typename T>
void Map<T>::set(const std::string& key, const T& val)
{
    auto found = std::find_if(m_value.begin(), m_value.end(), [&](const auto& pair) {
        return pair.first == key;
    });

    if (found != m_value.end()) {
        found->second = val;
    } else {
        // insert value if not found
        m_value.push_back(std::make_pair(key, val));
    }
}

template <typename T>
Attribute& Map<T>::create(const std::string& key)
{
    return append(key);
}

template <typename T>
const std::string& Map<T>::keyByIndex(int index) const
{
    return m_value.at(index).first;
}


// =========================================================================================================================================

template <Type ValType>
const typename ValueMap<ValType>::MapType& ValueMap<ValType>::value() const
{
    return m_value;
}

template <Type ValType>
typename ValueMap<ValType>::ConstIterator ValueMap<ValType>::begin() const
{
    return m_value.begin();
}

template <Type ValType>
typename ValueMap<ValType>::ConstIterator ValueMap<ValType>::end() const
{
    return m_value.end();
}

template <Type ValType>
typename ValueMap<ValType>::Iterator ValueMap<ValType>::begin()
{
    return m_value.begin();
}

template <Type ValType>
typename ValueMap<ValType>::Iterator ValueMap<ValType>::end()
{
    return m_value.end();
}

template <Type ValType>
int ValueMap<ValType>::size() const
{
    return int(m_value.size());
}

template <Type ValType>
typename ValueMap<ValType>::CppType& ValueMap<ValType>::operator[](const std::string& key)
{
    return m_value[key];
}

template <Type ValType>
const typename ValueMap<ValType>::CppType& ValueMap<ValType>::operator[](const std::string& key) const
{
    auto found = m_value.find(key);
    if (found != m_value.end()) {
        return found->second;
    }
    // if not found, return an exception
    throw std::out_of_range(fmt::format("Key {} was not found", key));
}

template <Type ValType>
ValueMap<ValType>& ValueMap<ValType>::operator=(const ValueMap& other)
{
    setValue(other.m_value);
    return *this;
}

template <Type ValType>
ValueMap<ValType>& ValueMap<ValType>::operator=(const MapType& val)
{
    setValue(val);
    return *this;
}

template <Type ValType>
void ValueMap<ValType>::setValue(const MapType& val)
{
    if (m_value == val)
        return;

    m_value = val;
}

template <Type ValType>
bool ValueMap<ValType>::contains(const std::string& key) const
{
    return std::find_if(m_value.begin(), m_value.end(), [&](const auto& pair) {
               return pair.first == key;
           }) != m_value.end();
}

template <Type ValType>
bool ValueMap<ValType>::compare(const Attribute& other) const
{
    if (auto casted = dynamic_cast<const ValueMap<ValType>*>(&other)) {
        return casted->value() == value();
    }
    return false;
}

template <Type ValType>
std::string ValueMap<ValType>::typeName() const
{
    return "Map";
}

template <Type ValType>
void ValueMap<ValType>::set(const Attribute& other)
{
    if (auto casted = dynamic_cast<const ValueMap<ValType>*>(&other)) {
        setValue(casted->value());
    }
}

template <Type ValType>
void ValueMap<ValType>::set(Attribute&& other)
{
    if (auto casted = dynamic_cast<ValueMap<ValType>*>(&other)) {
        setValue(std::move(casted->value()));
    }
}

template <Type ValType>
bool ValueMap<ValType>::hasValue() const
{
    return !m_value.empty();
}

template <Type ValType>
void ValueMap<ValType>::clear()
{
    m_value.clear();
}

template <Type ValType>
Type ValueMap<ValType>::valueType() const
{
    return ThisType;
}

template <Type ValType>
void ValueMap<ValType>::append(const std::string& key, const CppType& val)
{
    m_value.emplace(key, val);
}

template <Type ValType>
void ValueMap<ValType>::set(const std::string& key, const CppType& val)
{
    auto found = m_value.find(key);
    if (found != m_value.end()) {
        found->second = val;
    } else {
        m_value.emplace(key, val);
    }
}

template <Type ValType>
std::string ValueMap<ValType>::typeInfo()
{
    return fmt::format("ValueMap<{}>", valueTypeName(ValType));
}

template <Type ValType>
template <typename T>
typename ValueMap<ValType>::Iterator ValueMap<ValType>::find(const T& pred)
{
    return std::find_if(m_value.begin(), m_value.end(), pred);
}

template <Type ValType>
typename ValueMap<ValType>::Iterator ValueMap<ValType>::find(const std::string& key)
{
    return m_value.find(key);
}

template <Type ValType>
typename ValueMap<ValType>::Iterator ValueMap<ValType>::find(const std::regex& rex)
{
    return std::find_if(m_value.begin(), m_value.end(), [&](const auto& pair) {
        return std::regex_match(pair.fist, rex);
    });
}

// =========================================================================================================================================

} // namespace pack
