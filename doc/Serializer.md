# Serializer and deserializer

Pack provide different serializer and deserializer. There is for:
* Yaml
* Json
* Protobuf
* Zconfig

## Examples

In the following example we are going to use the following struct
```cpp
struct MyData: public pack::Node
{
    pack::String     strVal     = FIELD("strValue");
    pack::Double     doubleVal  = FIELD("doubleValue");
    pack::Bool       boolVal    = FIELD("boolValue");

    using pack::Node::Node;
    META(MyData, strVal, doubleVal, boolVal);
};
```
Usage with Yaml
```cpp
    MyData myData;
    ...
    #How to serialize
    fty::Expected<std::string> ret = pack::yaml::serialize(myData);
    if (ret) {
        std::cout << "Yaml content: " << *ret << std::endl;
    } else {
        std::cerr << "Serialization error: " << ret.error() << std::endl;
    }

    #How to deserialize
    std::string jsonContent = {...};
    fty::Expected<void> ret = pack::json::deserialize(jsonContent, myData);
    if (!ret) {
         std::cerr << "Deserialization error: " << ret.error() << std::endl;
    }
```

Usage is similar with zproject, json and protobuf

## Options
There is some options for serializing object:

* pack::Option::WithDefaults: This option serialized also default value which are not serialized by default to increase efficiency of transport layer.

* pack::Option::ValueAsString: All the values, including, boolean and number, are serialized as string. (JSON only)

* pack::Option::PrettyPrint: Pretty Print the output with 4 spaces. (JSON only)