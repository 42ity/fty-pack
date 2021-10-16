### pack::ValueMap\<T\>

pack::ValueMap is a unsorted associative container that contains key-value pairs. T is specialized to use [Basic types](Basic-types.md).
Some useful types are already predifined:
```cpp
using Int32Map  = ValueMap<Type::Int32>;
using Int64Map  = ValueMap<Type::Int64>;
using UInt32Map = ValueMap<Type::UInt32>;
using UInt64Map = ValueMap<Type::UInt64>;
using FloatMap  = ValueMap<Type::Float>;
using DoubleMap = ValueMap<Type::Double>;
using BoolMap   = ValueMap<Type::Bool>;
using StringMap = ValueMap<Type::String>;
```

Methods mostly the same as in [Map](Map-type.md)

### Example
```cpp
pack::StringMap map;
map.append("key1", "value1");
map.append("key2", "value2");

std::cerr << *pack::json::serialize(map) << std::endl;
```

Will produce
```json
{
    "key1": "value1", 
    "key2": "value2"
}
```