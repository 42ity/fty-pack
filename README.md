# fty-pack

DTO Tools library

fty-pack is a data transfer object (DTO) library for converting data between `json`, `yaml`, `zconfig`, `protobuf` representations and C++ structs.

##  Short description
Typical C++ struct looks like:

```cpp
struct MyData: public pack::Node
{
    pack::String     str  = FIELD("str-value");
    pack::Float      fl   = FIELD("float-value", 10.f);
    pack::StringList list = FIELD("mylist");

    using pack::Node::Node;
    META(MyData, str, fl, list);
};
```
For example, to convert `MyData` to `yaml` just use:

```cpp
MyData data;
data.str = "string";
data.list = {"one", "two"};

if (auto ret = pack::yaml::serialize(data)) {
    std::cout << "Yaml content: " << *ret << std::endl;
} else {
    std::cerr << "Serialization error: " << ret.error() << std::endl;
}
```
or from `json` to `MyData`:
```cpp
MyData data;
if (auto ret = pack::json::deserialize(jsonContent, data)) {
    // Use data
} else {
    std::cerr << "Deserialization error: " << ret.error() << std::endl;
}
```

## More information
* [Basic types](./doc/Basic-types.md)
* [Serializer and deserializer](./doc/Serializer.md)
* [Complex types](./doc/Complex-types.md)
  * [pack::Enum](./doc/Enum-type.md)  
  * [pack::ObjectList](./doc/ObjectList-type.md)  
  * [pack::ValueList](./doc/ValueList-type.md)
  * [pack::Map](./doc/Map-type.md)
  * [pack::ValueMap](./doc/ValueMap-type.md)
  * [pack::Node](./doc/Node.md)
  * [pack::Variant](./doc/Variant.md)
