#pragma once

// =====================================================================================================================
//        className() CALLME(DEFCTOR_JOIN, (pack, Node), ##__VA_ARGS__)
//{
//}


#define PACK(className, ...)                                                                                           \
    CALLME(USING, (pack, Node), ##__VA_ARGS__)                                                                         \
    className(const className& other) CALLME(CTOR_JOIN, (pack, Node), ##__VA_ARGS__)                                   \
    {                                                                                                                  \
        copyFields(other);                                                                                             \
    }                                                                                                                  \
    className(className&& other) CALLME(CTOR_JOIN, (pack, Node), ##__VA_ARGS__)                                        \
    {                                                                                                                  \
        copyFields(other);                                                                                             \
    }                                                                                                                  \
    inline className& operator=(const className& other)                                                                \
    {                                                                                                                  \
        CALLME(ASSIGN, ##__VA_ARGS__)                                                                                  \
        copyFields(other);                                                                                             \
        return *this;                                                                                                  \
    }                                                                                                                  \
    inline className& operator=(className&& other)                                                                     \
    {                                                                                                                  \
        CALLME(ASSIGN, ##__VA_ARGS__)                                                                                  \
        copyFields(other);                                                                                             \
        return *this;                                                                                                  \
    }                                                                                                                  \
    inline std::string typeName() const override                                                                       \
    {                                                                                                                  \
        return m_className;                                                                                            \
    }                                                                                                                  \
    static std::string typeInfo()                                                                                      \
    {                                                                                                                  \
        return m_className;                                                                                            \
    }                                                                                                                  \
    auto parentFields() const                                                                                          \
    {                                                                                                                  \
        return std::tuple_cat(CALLME(FIELDS, ##__VA_ARGS__));                                                          \
    }                                                                                                                  \
    auto parentFields()                                                                                                \
    {                                                                                                                  \
        return std::tuple_cat(CALLME(FIELDS, ##__VA_ARGS__));                                                          \
    }                                                                                                                  \
    static constexpr const char* m_className = #className

// =====================================================================================================================

#define PACK_FIELDS(...)                                                                                               \
public:                                                                                                                \
    inline std::vector<const pack::Attribute*> fields() const override                                                 \
    {                                                                                                                  \
        return std::apply(                                                                                             \
            [](const auto&... elems) {                                                                                 \
                return std::vector<const pack::Attribute*>{&elems...};                                                 \
            },                                                                                                         \
            std::tuple_cat(parentFields(), std::forward_as_tuple(__VA_ARGS__)));                                       \
    }                                                                                                                  \
    inline std::vector<pack::Attribute*> fields() override                                                             \
    {                                                                                                                  \
        return std::apply(                                                                                             \
            [](auto&... elems) {                                                                                       \
                return std::vector<pack::Attribute*>{&elems...};                                                       \
            },                                                                                                         \
            std::tuple_cat(parentFields(), std::forward_as_tuple(__VA_ARGS__)));                                       \
    }                                                                                                                  \
    inline void copyFields(const Node& other)                                                                          \
    {                                                                                                                  \
        std::apply(                                                                                                    \
            [&](auto&... arg) {                                                                                        \
                (arg.set(*other.fieldByKey(arg.key())), ...);                                                          \
            },                                                                                                         \
            std::forward_as_tuple(__VA_ARGS__));                                                                       \
    }                                                                                                                  \
    inline std::vector<std::string> fieldNames() const override                                                        \
    {                                                                                                                  \
        static std::vector<std::string> names = std::apply(                                                            \
            [&](auto&... arg) {                                                                                        \
                return std::vector<std::string>{(arg.key(), ...)};                                                     \
            },                                                                                                         \
            std::tuple_cat(parentFields(), std::forward_as_tuple(__VA_ARGS__)));                                       \
        return names;                                                                                                  \
    }                                                                                                                  \
                                                                                                                       \
protected:                                                                                                             \
    auto thisFields() const                                                                                            \
    {                                                                                                                  \
        return std::forward_as_tuple(__VA_ARGS__);                                                                     \
    }                                                                                                                  \
    auto thisFields()                                                                                                  \
    {                                                                                                                  \
        return std::forward_as_tuple(__VA_ARGS__);                                                                     \
    }                                                                                                                  \
                                                                                                                       \
public:

// =====================================================================================================================

#define PRIMITIVE_CAT(x, y)                        x##y
#define CAT(x, y)                                  PRIMITIVE_CAT(x, y)
#define N_ARGS(...)                                N_ARGS_HELPER1(__VA_ARGS__, 4, 3, 2, 1)
#define N_ARGS_HELPER1(...)                        N_ARGS_HELPER2(__VA_ARGS__)
#define N_ARGS_HELPER2(_0, _1, _2, _3, _4, N, ...) N

#define CALL0(a)    a
#define CALL(a, b)  a b
#define CALL1(a, b) a b
#define CALL2(a, b) a b
#define CALL3(a, b) a b
#define CALL4(a, b) a b

#define CALLIN0(a)    a
#define CALLIN(a, b)  a b
#define CALLIN1(a, b) a b
#define CALLIN2(a, b) a b
#define CALLIN3(a, b) a b
#define CALLIN4(a, b) a b

#define ASSIGN_1(a1)             NS_CAT2 a1::operator=(other);
#define ASSIGN_2(a1, a2)         ASSIGN_1(a1) NS_CAT2 a2::operator=(other);
#define ASSIGN_3(a1, a2, a3)     ASSIGN_2(a1, a2) NS_CAT2 a3::operator=(other);
#define ASSIGN_4(a1, a2, a3, a4) ASSIGN_3(a1, a2, a3) NS_CAT2 a4::operator=(other);

#define DEFCTOR_JOIN_1(a1)             :NS_CAT a1()
#define DEFCTOR_JOIN_2(a1, a2)         DEFCTOR_JOIN_1(a1), NS_CAT a2()
#define DEFCTOR_JOIN_3(a1, a2, a3)     DEFCTOR_JOIN_2(a1, a2), NS_CAT a3()
#define DEFCTOR_JOIN_4(a1, a2, a3, a4) DEFCTOR_JOIN_3(a1, a2, a3), NS_CAT a4()

#define CTOR_JOIN_1(a1)             :NS_CAT a1(other)
#define CTOR_JOIN_2(a1, a2)         :NS_CAT a2(other)
#define CTOR_JOIN_3(a1, a2, a3)     CTOR_JOIN_2(a1, a2), NS_CAT a3(other)
#define CTOR_JOIN_4(a1, a2, a3, a4) CTOR_JOIN_3(a1, a2, a3), NS_CAT a4(other)

#define FIELDS_1(a1)             NS_CAT2 a1::thisFields()
#define FIELDS_2(a1, a2)         FIELDS_1(a1), NS_CAT2 a2::thisFields()
#define FIELDS_3(a1, a2, a3)     FIELDS_2(a1, a2), NS_CAT2 a3::thisFields()
#define FIELDS_4(a1, a2, a3, a4) FIELDS_3(a1, a2, a3), NS_CAT2 a4::thisFields()

#define NS_CAT_1(a1)     a1::a1
#define NS_CAT_2(a1, a2) a1::a2::a2

#define NS_CAT2_1(a1)     a1
#define NS_CAT2_2(a1, a2) a1::a2

#define DUMMY(m)
#define EXTENDS

#define USING_1(a1)     using NS_CAT a1;
#define USING_2(a1, a2) using NS_CAT a2;
#define USING_3(a1, a2, a3)                                                                                            \
    USING_2(a1, a2)                                                                                                    \
    using NS_CAT a3;

#define USING_4(a1, a2, a3, a4)                                                                                        \
    USING_3(a1, a2, a3)                                                                                                \
    using NS_CAT a4;

#define CALLME(what, ...)                                                                                              \
    CALL(N_ARGS(_0, ##__VA_ARGS__, CAT(what, _4), CAT(what, _3), CAT(what, _2), CAT(what, _1), DUMMY), (__VA_ARGS__))

#define NS_CAT(...) CALLIN(N_ARGS(_0, ##__VA_ARGS__, NS_CAT_4, NS_CAT_3, NS_CAT_2, NS_CAT_1, NS_CAT_0), (__VA_ARGS__))

#define NS_CAT2(...)                                                                                                   \
    CALLIN(N_ARGS(_0, ##__VA_ARGS__, NS_CAT2_4, NS_CAT2_3, NS_CAT2_2, NS_CAT2_1, NS_CAT2_0), (__VA_ARGS__))
