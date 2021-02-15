#pragma once
#include "pack/attribute.h"
#include <algorithm>
#include <stdexcept>
#include <variant>

namespace pack {

// =====================================================================================================================

class IVariant : public Attribute
{
public:
    IVariant(Attribute* parent, const std::string& key)
        : Attribute(NodeType::Variant, parent, key)
    {
    }

    virtual const Attribute* get() const                                        = 0;
    virtual Attribute*       get()                                              = 0;
    virtual bool             findBetter(const std::vector<std::string>& fields) = 0;
};

// =====================================================================================================================

template <typename... Types>
class Variant : public IVariant
{
public:
    using IVariant::IVariant;

    Variant();
    Variant(const Variant& other);
    Variant(Variant&& other);

    Variant& operator=(const Variant& other);
    Variant& operator=(Variant&& other);

    template <typename T, typename=std::enable_if_t<!std::is_same_v<std::decay_t<T>, Variant>>>
    Variant(T&& val)
        : IVariant(nullptr, {})
        , m_value(val)
    {
    }

    template <typename T>
    bool is() const;

    template <typename T>
    const T& get() const;

    template <typename T>
    T& get();

    template <typename T>
    T& reset();

public:
    const Attribute*   get() const override;
    Attribute*         get() override;
    bool               findBetter(const std::vector<std::string>& fields) override;
    static std::string typeInfo();

public:
    bool        compare(const Attribute& other) const override;
    std::string typeName() const override;
    void        set(const Attribute& other) override;
    void        set(Attribute&& other) override;
    bool        hasValue() const override;
    void        clear() override;

private:
    std::variant<Types...> m_value;
};

// =====================================================================================================================

template <typename... Types>
Variant<Types...>::Variant()
    : IVariant(nullptr, {})
{
}

template <typename... Types>
Variant<Types...>::Variant(const Variant& other)
    : IVariant(other)
    , m_value(other.m_value)
{
}

template <typename... Types>
Variant<Types...>::Variant(Variant&& other)
    : IVariant(other)
    , m_value(std::move(other.m_value))
{
}

template <typename... Types>
Variant<Types...>& Variant<Types...>::operator=(const Variant& other)
{
    m_value = other.m_value;
    return *this;
}

template <typename... Types>
Variant<Types...>& Variant<Types...>::operator=(Variant&& other)
{
    m_value = std::move(other.m_value);
    return *this;
}

template <typename... Types>
template <typename T>
bool Variant<Types...>::is() const
{
    return std::holds_alternative<T>(m_value);
}

template <typename... Types>
template <typename T>
const T& Variant<Types...>::get() const
{
    assert(is<T>());
    return std::get<T>(m_value);
}

template <typename... Types>
template <typename T>
T& Variant<Types...>::get()
{
    assert(is<T>());
    return std::get<T>(m_value);
}

template <typename... Types>
const Attribute* Variant<Types...>::get() const
{
    const Attribute* attr = nullptr;
    try {
        std::visit(
            [&](const auto& var) {
                using T = std::decay_t<decltype(var)>;
                if (std::is_base_of_v<Attribute, T>) {
                    attr = &var;
                }
            },
            m_value);
    } catch (const std::bad_variant_access&) {
        return nullptr;
    }

    return attr;
}

template <typename... Types>
Attribute* Variant<Types...>::get()
{
    Attribute* attr = nullptr;

    try {
        std::visit(
            [&](auto& var) {
                using T = std::decay_t<decltype(var)>;
                if (std::is_base_of_v<Attribute, T>) {
                    attr = &var;
                }
            },
            m_value);
    } catch (const std::bad_variant_access&) {
        return nullptr;
    }

    return attr;
}

template <typename... Types>
template <typename T>
T& Variant<Types...>::reset()
{
    m_value = T{};
    return get<T>();
}

template <typename... Types>
bool Variant<Types...>::compare(const Attribute& other) const
{
    if (auto casted = dynamic_cast<const Variant<Types...>*>(&other)) {
        return m_value == casted->m_value;
    }
    return false;
}

template <typename... Types>
std::string Variant<Types...>::typeName() const
{
    return typeInfo();
}

template <typename... Types>
std::string Variant<Types...>::typeInfo()
{
    return "Variant";
}


template <typename... Types>
void Variant<Types...>::set(const Attribute& other)
{
    if (auto casted = dynamic_cast<const Variant<Types...>*>(&other)) {
        m_value = casted->m_value;
    }
}

template <typename... Types>
void Variant<Types...>::set(Attribute&& other)
{
    if (auto casted = dynamic_cast<Variant<Types...>*>(&other)) {
        m_value = casted->m_value;
    }
}

template <typename... Types>
bool Variant<Types...>::hasValue() const
{
    return m_value.index() != std::variant_npos;
}

template <typename... Types>
void Variant<Types...>::clear()
{
    m_value = {};
}

template <typename T>
struct TypeHelper
{
    using Type = T;
};

template <typename... Ts, typename TF>
void foreachType(TF&& f)
{
    (f(TypeHelper<Ts>{}), ...);
}

template <typename... Types>
bool Variant<Types...>::findBetter(const std::vector<std::string>& fields)
{
    size_t typeHash;
    float  betterRating = 0.f;

    foreachType<Types...>([&](auto t) {
        using ImplType = typename decltype(t)::Type;

        int count = 0;
        for (const auto& fld : ImplType::staticFieldNames()) {
            count += std::find(fields.begin(), fields.end(), fld) == fields.end() ? 0 : 1;
        }
        float rating = count * 1.f / ImplType::staticFieldNames().size();
        if (rating >= betterRating) {
            betterRating = rating;
            typeHash     = typeid(ImplType).hash_code();
        }
    });

    foreachType<Types...>([&](auto t) {
        using ImplType = typename decltype(t)::Type;
        if (typeid(ImplType).hash_code() == typeHash) {
            m_value = ImplType{};
        }
    });

    return m_value.index() != std::variant_npos;
}

} // namespace pack
