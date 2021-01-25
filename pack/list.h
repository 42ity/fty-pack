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
#include "pack/attribute.h"
#include "pack/types.h"
#include <algorithm>

namespace pack {

// =====================================================================================================================

/// Common interface to list data container
class IList : public Attribute
{
public:
    IList()
        : Attribute(NodeType::List, nullptr, "")
    {
    }

    IList(Attribute* parent, const std::string& key = {})
        : Attribute(NodeType::List, parent, key)
    {
    }

    /// Returns the size of the list
    virtual int size() const = 0;
};

// =====================================================================================================================

/// Object list interface.
///
/// This container is used to keep Node types
class IObjectList : public IList
{
public:
    using IList::IList;

public:
    /// Returns INode interface by index
    virtual const Attribute& get(int index) const = 0;
    virtual Attribute&       create()             = 0;
};

// =====================================================================================================================

template <Type ValType>
class ValueList;

/// Values list interface.
///
/// This container is used to keep values simple types
class IValueList : public IList
{
public:
    using IList::IList;

public:
    /// Returns values type
    virtual Type valueType() const = 0;
};

// =====================================================================================================================

template <typename T>
class ObjectList : public IObjectList
{
public:
    using IObjectList::IObjectList;
    using ListType      = std::vector<T>;
    using Iterator      = typename ListType::iterator;
    using ConstIterator = typename ListType::const_iterator;

public:
    ObjectList();
    ObjectList(const ObjectList& other);
    ObjectList(ObjectList&& other);
    ObjectList& operator=(const ObjectList& other);

public:
    ConstIterator begin() const;
    ConstIterator end() const;
    Iterator      begin();
    Iterator      end();

public:
    const ListType& value() const;
    void            setValue(const ListType& val);
    void            append(const T& value);
    void            append(T&& value);
    T&              append();

    template <typename Func>
    std::optional<T> find(Func&& func);
    template <typename Func>
    bool remove(Func&& func);
    template <typename Func>
    void     sort(Func&& func);
    const T& operator[](int index) const;
    bool     empty() const;

    static std::string typeInfo();

public:
    int              size() const override;
    bool             compare(const Attribute& other) const override;
    std::string      typeName() const override;
    void             set(const Attribute& other) override;
    void             set(Attribute&& other) override;
    bool             hasValue() const override;
    const Attribute& get(int index) const override;
    Attribute&       create() override;
    void             clear() override;

private:
    ListType m_value;
};

// ===========================================================================================================

template <Type ValType>
class ValueList : public IValueList
{
public:
    using CppType                  = typename ResolveType<ValType>::type;
    static constexpr Type ThisType = ValType;

    using IValueList::IValueList;
    using ListType      = std::vector<CppType>;
    using Iterator      = typename ListType::iterator;
    using ConstIterator = typename ListType::const_iterator;

public:
    ValueList();
    ValueList(const ValueList& other);
    ValueList(ValueList&& other);
    ValueList& operator=(const ValueList& other);

public:
    ConstIterator begin() const;
    ConstIterator end() const;
    Iterator      begin();
    Iterator      end();

public:
    const ListType& value() const;
    void            setValue(const ListType& val);
    void            append(const CppType& value);
    void            append(CppType&& value);

    bool           find(const CppType& func) const;
    bool           remove(const CppType& toRemove);
    const CppType& operator[](int index) const;
    void           clear() override;
    bool           empty() const;

    template <typename Func>
    void sort(Func&& func);

    static std::string typeInfo();

public:
    int         size() const override;
    bool        compare(const Attribute& other) const override;
    std::string typeName() const override;
    void        set(const Attribute& other) override;
    void        set(Attribute&& other) override;
    bool        hasValue() const override;
    Type        valueType() const override;

protected:
    ListType m_value;
};

// =====================================================================================================================
// Object list implementation
// =====================================================================================================================

template <typename T>
ObjectList<T>::ObjectList()
    : IObjectList()
{
}

template <typename T>
ObjectList<T>::ObjectList(const ObjectList& other)
    : IObjectList(other)
{
    setValue(other.value());
}

template <typename T>
ObjectList<T>::ObjectList(ObjectList&& other)
    : IObjectList(other)
{
    setValue(std::move(other.value()));
}

template <typename T>
ObjectList<T>& ObjectList<T>::operator=(const ObjectList& other)
{
    setValue(other.value());
    return *this;
}

template <typename T>
typename ObjectList<T>::ConstIterator ObjectList<T>::begin() const
{
    return m_value.begin();
}

template <typename T>
typename ObjectList<T>::ConstIterator ObjectList<T>::end() const
{
    return m_value.end();
}

template <typename T>
typename ObjectList<T>::Iterator ObjectList<T>::begin()
{
    return m_value.end();
}

template <typename T>
typename ObjectList<T>::Iterator ObjectList<T>::end()
{
    return m_value.end();
}

template <typename T>
const typename ObjectList<T>::ListType& ObjectList<T>::value() const
{
    return m_value;
}

template <typename T>
void ObjectList<T>::setValue(const ObjectList<T>::ListType& val)
{
    m_value = val;
}

template <typename T>
void ObjectList<T>::append(const T& value)
{
    m_value.push_back(value);
}

template <typename T>
void ObjectList<T>::append(T&& value)
{
    m_value.push_back(std::move(value));
}

template <typename T>
T& ObjectList<T>::append()
{
    return m_value.emplace_back();
}

template <typename T>
template <typename Func>
std::optional<T> ObjectList<T>::find(Func&& func)
{
    if (auto it = std::find_if(m_value.begin(), m_value.end(), func); it != m_value.end()) {
        return *it;
    }
    return std::nullopt;
}

template <typename T>
template <typename Func>
void ObjectList<T>::sort(Func&& func)
{
    std::sort(m_value.begin(), m_value.end(), std::forward<Func>(func));
}

template <typename T>
template <typename Func>
bool ObjectList<T>::remove(Func&& func)
{
    if (auto it = std::find_if(m_value.begin(), m_value.end(), func); it != m_value.end()) {
        m_value.erase(it);
        return true;
    }
    return false;
}

template <typename T>
const T& ObjectList<T>::operator[](int index) const
{
    return m_value[size_t(index)];
}

template <typename T>
void ObjectList<T>::clear()
{
    m_value.clear();
}

template <typename T>
int ObjectList<T>::size() const
{
    return int(m_value.size());
}

template <typename T>
bool ObjectList<T>::compare(const Attribute& other) const
{
    if (auto casted = dynamic_cast<const ObjectList<T>*>(&other)) {
        return m_value == casted->m_value;
    }
    return false;
}

template <typename T>
std::string ObjectList<T>::typeName() const
{
    return "List<" + T::typeInfo() + ">";
}

template <typename T>
void ObjectList<T>::set(const Attribute& other)
{
    if (auto casted = dynamic_cast<const ObjectList<T>*>(&other)) {
        m_value = casted->m_value;
    }
}

template <typename T>
void ObjectList<T>::set(Attribute&& other)
{
    if (auto casted = dynamic_cast<ObjectList<T>*>(&other)) {
        m_value = std::move(casted->m_value);
    }
}

template <typename T>
bool ObjectList<T>::hasValue() const
{
    return !m_value.empty();
}

template <typename T>
const Attribute& ObjectList<T>::get(int index) const
{
    return m_value[size_t(index)];
}

template <typename T>
Attribute& ObjectList<T>::create()
{
    return append();
}

template <typename T>
bool ObjectList<T>::empty() const
{
    return m_value.empty();
}

template <typename T>
std::string ObjectList<T>::typeInfo()
{
    return "List<" + T::typeInfo() + ">";
}

// =====================================================================================================================
// Values list implementation
// =====================================================================================================================

template <Type ValType>
ValueList<ValType>::ValueList()
    : IValueList()
{
}

template <Type ValType>
ValueList<ValType>::ValueList(const ValueList& other)
    : IValueList(other)
{
    setValue(other.value());
}

template <Type ValType>
ValueList<ValType>::ValueList(ValueList&& other)
    : IValueList(other)
{
    setValue(other.value());
}

template <Type ValType>
ValueList<ValType>& ValueList<ValType>::operator=(const ValueList& other)
{
    setValue(other.value());
    return *this;
}

template <Type ValType>
typename ValueList<ValType>::ConstIterator ValueList<ValType>::begin() const
{
    return m_value.begin();
}

template <Type ValType>
typename ValueList<ValType>::ConstIterator ValueList<ValType>::end() const
{
    return m_value.end();
}

template <Type ValType>
typename ValueList<ValType>::Iterator ValueList<ValType>::begin()
{
    return m_value.begin();
}

template <Type ValType>
typename ValueList<ValType>::Iterator ValueList<ValType>::end()
{
    return m_value.end();
}

template <Type ValType>
const typename ValueList<ValType>::ListType& ValueList<ValType>::value() const
{
    return m_value;
}

template <Type ValType>
void ValueList<ValType>::setValue(const ValueList<ValType>::ListType& val)
{
    m_value = val;
}

template <Type ValType>
void ValueList<ValType>::append(const CppType& value)
{
    m_value.push_back(value);
}

template <Type ValType>
void ValueList<ValType>::append(CppType&& value)
{
    m_value.push_back(std::move(value));
}

template <Type ValType>
bool ValueList<ValType>::find(const CppType& val) const
{
    auto it = std::find(m_value.begin(), m_value.end(), val);
    return it != m_value.end();
}

template <Type ValType>
template <typename Func>
void ValueList<ValType>::sort(Func&& func)
{
    std::sort(m_value.begin(), m_value.end(), std::forward<Func>(func));
}


template <Type ValType>
bool ValueList<ValType>::remove(const CppType& toRemove)
{
    if (auto it = std::find(m_value.begin(), m_value.end(), toRemove)) {
        m_value.erase(it);
        return true;
    }
    return false;
}

template <Type ValType>
const typename ValueList<ValType>::CppType& ValueList<ValType>::operator[](int index) const
{
    return m_value[size_t(index)];
}

template <Type ValType>
void ValueList<ValType>::clear()
{
    m_value.clear();
}

template <Type ValType>
int ValueList<ValType>::size() const
{
    return int(m_value.size());
}

template <Type ValType>
bool ValueList<ValType>::compare(const Attribute& other) const
{
    if (auto casted = dynamic_cast<const ValueList<ValType>*>(&other)) {
        return m_value == casted->m_value;
    }
    return false;
}

template <Type ValType>
std::string ValueList<ValType>::typeName() const
{
    return "List<" + valueTypeName(ValType) + ">";
}

template <Type ValType>
void ValueList<ValType>::set(const Attribute& other)
{
    if (auto casted = dynamic_cast<const ValueList<ValType>*>(&other)) {
        m_value = casted->m_value;
    }
}

template <Type ValType>
void ValueList<ValType>::set(Attribute&& other)
{
    if (auto casted = dynamic_cast<ValueList<ValType>*>(&other)) {
        m_value = std::move(casted->m_value);
    }
}

template <Type ValType>
bool ValueList<ValType>::hasValue() const
{
    return !m_value.empty();
}

template <Type ValType>
Type ValueList<ValType>::valueType() const
{
    return ValType;
}

template <Type ValType>
bool ValueList<ValType>::empty() const
{
    return m_value.empty();
}

template <Type ValType>
std::string ValueList<ValType>::typeInfo()
{
    return "List<" + valueTypeName(ValType) + ">";
}

// =====================================================================================================================

} // namespace pack
