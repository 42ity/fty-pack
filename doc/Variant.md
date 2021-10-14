### pack::Variant\<T...\>

pack::Variant is special type to use one from several possible types.

Some useful methods
* Determinate if variant contains T type
```cpp
    template <typename T>
    bool is() const;
```
* Returns a reference to the T type from variant. If T is not contained then std::bad_variant_access will throw.
```cpp
    template <typename T>
    const T& get() const;

    template <typename T>
    T& get();
```
* Reset variant value with new type and return a reference to the new value.
```cpp
    template <typename T>
    T& reset();
```

### Example
Definition
```cpp
struct A : public pack::Node
{
    pack::String a = FIELD("a");
    pack::String b = FIELD("b");

    using pack::Node::Node;
    META(A, a, b);
};

struct B : public pack::Node
{
    pack::String a = FIELD("a");
    pack::String b = FIELD("b");
    pack::String c = FIELD("c");

    using pack::Node::Node;
    META(B, a, b, c);
};
```
Usage
```cpp
    A a;
    a.a = "a";
    a.b = "b";

    pack::Variant<A, B> var(a);
```

Check variant type
```cpp
    std::cout << var.is<A>() << std::endl;
```
Output
```
1
```

Get the value from variant
```cpp
    const A& val = var.get<A>();
    std::cout << val.a.value() << " " << val.b.value() << std::endl;
```
Output
```
a b
```

Serialize the variant
```cpp
    std::cout << *pack::json::serialize(var) << std::endl;
```
Output
```json
{
   "a": "a", 
   "b": "b"
}
```

Reset the variant with different type
```cpp
    auto& b = var.reset<B>();
    b.a = "a";
    b.b = "b";
    b.c = "c";

    std::cout << *pack::json::serialize(var) << std::endl;
```
Output
```json
{
   "a": "a", 
   "b": "b",
   "c": "c",
}
```
