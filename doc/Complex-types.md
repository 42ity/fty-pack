Pack contains few complex types to cover serialization/deserialization types.

| Pack type                                               | C++ analogue             |
| ------------------------------------------------------- | ------------------------ |
| [Enum](Enum-type.md)\<T\>                                  | enum class               |
| [ObjectList](ObjectList-type.md)\<T\>                      | std::vector<T>           |
| [ValueList](ValueList-type.md)\<[BasicType](Basic-types.md)\> | std::vector<BasicType>   |
| [Map](Map-type)\<T\>                                    | std::map<std::string, T> |
| [ValueMap](ValueMap-type.md)\<[BasicType](Basic-types.md)\>   | std::map<std::string, T> |
| [Node](Node.md)                                            | struct                   |
| [Variant](Variant.md)                                      | std::variant             |

All types are inherited from Attrbute class and have such methods:
* Compare with other attribute, returns true if both attributes are equal. If `other` is different type, returns false
  ```cpp
    bool compare(const Attribute& other) const;
  ```
* Gets human readable type name as, map, list, int32_t etc.
  ```cpp
    std::string typeName() const;
  ```
* Check if this attribute contains _non default_ value
  ```cpp
    bool hasValue() const;
  ```
* Clears the field
  ```cpp
    void clear();
  ```
