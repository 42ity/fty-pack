# fty-pack

DTO Tools library

##  short description

fty-pack is a small library for converting data between `json`, `yaml`, `zconfig`, `protobuf` respresentation and C++ structs.
Typical C++ struct looks like:
```
struct MyData: public pack::Node
{
    using pack::Node::Node;

    pack::String str      = FIELD("str-value");
    pack::Float  fl       = FIELD("float-value", 10.f);
    pack::StringList list = FIELD("mylist")

    META_FIELDS(MyData, str, fl, list)
};

```
Supported basic types are: Int32, Int64, UInt32, UInt64, Float, Double, Bool, String.


For example, to convert `MyData` to `yaml` just use:
```
MyData data;
data.str = "string";
data.list = {"one", "two"};

std::string cnt = pack::yaml::serialize(data);
```

or from `json` to `MyData`:
```
MyData data;
pack::json::deserialize(jsonContent, data);
```
