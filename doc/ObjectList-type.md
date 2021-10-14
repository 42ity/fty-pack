## pack::ObjectList\<T\>

Object list where T is child class of the pack::Node. This type is like a `std::vector`

Some useful methods:
* Find in list by function object. Returns `std::optional` with T if found, `std::nullopt` otherwise
  ```cpp
    template <typename Func>
    std::optional<T> find(Func&& func) const;
  ```
* Find a index of the T by function object. Returns index >= 0 if found, -1 otherwise
  ```cpp
    template <typename Func>
    int findIndex(Func&& func) const;
  ```
* Removes elements from a list. Returns true if any elements were removed.
  ```cpp
    template <typename Func>
    bool remove(Func&& func);
  ```
* Sorts list by comparison function object. Comparison function object should be same as described in [std::sort](https://en.cppreference.com/w/cpp/algorithm/sort)
  ```cpp
    template <typename Func>
    void sort(Func&& func);
  ```
* Returns a reference to the object by specified index. No bounds checking is performed. Accessing a nonexistent object through this operator is undefined behavior. 
  ```cpp
    const T& operator[](int index) const;
    T&       operator[](int index);
  ```
* Checks if the container has no elements
  ```cpp
    bool empty() const;
  ```

## Example
```cpp
struct MyData: public pack::Node
{
    pack::String str = FIELD("str-value");
    pack::Int32  val = FIELD("value");

    using pack::Node::Node;
    META(MyData, str, val);
};

pack::ObjectList<MyData> list;
{
    // You could append to list this way, but beware, do not assign to the same reference name in same scope, reference will be broken -> segfault
    MyData& it = list.append();
    it.str = "String";
    it.val = 10;
}
{
    // Or you could append it classical way
    MyData it;
    it.str = "String2";
    it.val = 11;
    list.append(it);
}
```
Find object:
```cpp
std::string val = "String"
auto it = list.find([&](const MyData& el) {
   return val == el.str;
});

if (it) {
  ... process *it
}
```

Serialization:
```cpp
std::cerr << *pack::json::serialize(list) << std::endl;
```
The result of this would be:
```json
[
    {
        "str-value": "String", 
        "value": "10"
    }, 
    {
        "str-value": "String2", 
        "value": "11"
    }
]
```
