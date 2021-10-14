## pack::Enum<T>

pack::Enum is enum class holder. Typical usage is
```cpp
enum class MyEnum
{
    Value1,
    Value2
};

Enum<MyEnum> en = MyEnum::Value1; // Set value
MyEnum val = en; // Get value
```

Some useful function are:
```cpp
std::string asString() const                     // Converts enum value to string representation
void        fromString(const std::string& value) // Sets value from string representation
int         asInt() const                        // Returns integer value
void        fromInt(int value)                   // Sets from integer
```

Conversion to/from string you can make 2 different ways.

1. Common way: define pair of functions where you should convert enum value to/from string manually.
  * ```cpp std::ostream& operator<<(std::ostream& ss, MyEnum value)```
  * ```cpp std::istream& operator>>(std::istream& ss, MyEnum& value)```
2. Automatic way: you do **nothing**. Conversion is automatic, but requires at least gcc 9 or clang 5. For example: `MyEnum::Value1` became "Value1" etc. This conversion was made with help of awesome tool - [Magic enum](https://github.com/Neargye/magic_enum)

You can choose first way in case if your compiler is too old, or you need customized conversion.
Example of the conversions functions:
```cpp
std::ostream& operator<<(std::ostream& ss, MyEnum value)
{
    ss << [&](){
        switch(value){
            case MyEnum::Value1: return "Value1";
            case MyEnum::Value2: return "Value2";
        }
    }();
    return ss;
}

std::istream& operator>>(std::istream& ss, MyEnum& value)
{
    std::string strval;
    ss >> strval;
    if (strval == "Value1") {
        value = MyEnum::Value1;
    } else if (strval == "Value2") {
        value = MyEnum::Value2;
    }
    return ss;
}
```

## Example

Example of `pack::Enum` usage:
```cpp
struct Data: public pack::Node
{
    pack::Enum<MyEnum> en = FIELD("en");
    using pack::Node::Node;
    META(Data, en);
};

Data data;
data.en = MyEnum::Value2;

std::cout << *pack::json::serialize(data) << std::endl;
```
Which will produce this nice json:
```json
{"en": "Value2"}
```
