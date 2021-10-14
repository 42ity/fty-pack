### pack::Map\<T\> 

pack::Map is a unsorted associative container that contains key-value pairs. **T is specialized to use pack::Node derivatives.**

Some useful methods
* Returns internal std container from the Map (std::vector\<std::pair\<std::string, T\>\>)
  ```cpp
    const MapType& value() const;
  ```
* Sets value of the map by internal representation (std::vector\<std::pair\<std::string, T\>\>)
  ```cpp
    void setValue(const MapType& val);
  ```
* Returns true if the key exists in the map
  ```cpp
    bool contains(const std::string& key) const;
  ```
* Returns a reference to Map value by it's key. In case if map contains few same keys then returns first.
  ```cpp
    const T& operator[](const std::string& key) const;
  ```
* Appends to map.
  ```cpp
    T&   append(const std::string& key);
    void append(const std::string& key, const T& val);
  ```

### Example

```cpp
struct MapObj: public pack::Node
{
    pack::String value = FIELD("value");

    using pack::Node::Node;
    META(MapObj, value);
};

pack::Map<MapObj> map;
{
   auto& it = map.append("key1");
   it.value = "Some string";
}
{
    MapObj it;
    it.value = "Some other value";
    map.append("key2", ins);
}

std::cerr << *pack::json::serialize(map) << std::endl; 
```
Will produce:
```json
{
    "key1": {
        "value": "Some string"
    }, 
    "key2": {
        "value": "Some other value"
    }
}
```