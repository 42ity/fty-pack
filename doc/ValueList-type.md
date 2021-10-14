### pack::ValueList\<[BasicType](Basic-types)\>

ValueList is container specialized to hold [BasicType](Basic-types). Exist slight difference between this type and [ObjectList](ObjectList-type). 

Pack defines some useful ValuesList\<BasicType\> as types:
```cpp
using Int32List  = ValueList<Type::Int32>;
using Int64List  = ValueList<Type::Int64>;
using UInt32List = ValueList<Type::UInt32>;
using UInt64List = ValueList<Type::UInt64>;
using FloatList  = ValueList<Type::Float>;
using DoubleList = ValueList<Type::Double>;
using BoolList   = ValueList<Type::Bool>;
using StringList = ValueList<Type::String>;
```

Some useful methods (for BasicType::CppType see mapping in [BasicType](Basic-types)):
* Returns std::vector\<BasicType::CppType\>
  ```cpp
    const ListType& value() const;
  ```
* Sets std::vector\<BasicType::CppType\>
  ```cpp
    void setValue(const ListType& val);
  ```
* Appends BasicType::CppType to the container
  ```cpp
    void append(const CppType& value);
    void append(CppType&& value);
  ```
* Finds value in container (TODO: return int index as result)
  ```cpp
    bool find(const CppType& val) const;
  ```
* Remove values from container by it's value
  ```cpp
    bool remove(const CppType& toRemove);
  ```
* Return a reference to value by index
  ```cpp
    const CppType& operator[](int index) const;
  ```
* Returns true if container contains values
  ```cpp
    bool empty() const;
  ```
* Sorts list by comparison function object. Comparison function object should be same as described in [std::sort](https://en.cppreference.com/w/cpp/algorithm/sort)
  ```cpp
    template <typename Func>
    void sort(Func&& func);
  ```

### Example

Create list of strings:
```cpp
pack::StringList list = {"the", "blue", "parrot"};
```

Serialize it:
```cpp
std::cerr << *pack::json::serialize(list) << std::endl;
```

Output would be:
```json
["the", "blue", "parrot"]
```
