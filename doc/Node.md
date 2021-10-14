### pack::Node

pack::Node is base type of all structured types in pack. 

Here is typical usage of pack::Node
```cpp
struct Obj: public pack::Node
{
    pack::String value  = FIELD("value");
    pack::Int32  intVal = FIELD("int-value");

    using pack::Node::Node;
    META(Obj, value, intVal);
};
```
In node you should specify all fields in this struct.
```cpp
pack::String value = FIELD("value");
```
Where is:
* ```pack::String``` is type of field
* ```value``` is name of field used in cpp code
* ```FIELD("value")``` is a definition of field serialization, "value" is name of serialized field. FIELD has a second parameter - default value of the field: f.e. FIELD("value", "My default")

```META``` is necessary macro to create meta information of this struct. Format is META(This class name, Fields...)

### Aggregation
For aggregation everything what you need is just define sub structure somewhere. For example in body of your node. (it could be outside... everywhere)
```cpp
struct Obj: public pack::Node
{
   // Completely the same definition
   struct SubObj: public pack::Node
   {
      pack::Int64 value = FIELD("value");

      using pack::Node::Node;
      META(SubObj, value);
   };

    pack::String value  = FIELD("value");
    pack::Int32  intVal = FIELD("int-value");
    SubObj       sub    = FIELD("sub"); // <- use SubObj

    using pack::Node::Node;
    META(Obj, value, intVal, sub);
};
```
### Aggregation example

```cpp
Obj obj;
obj.value = "Is it life?";
obj.intVal = 42;
obj.sub.value = 84;

std::cerr << *pack::json::serialize(obj) << std::endl;
```

Json output would be:
```json
{
    "value": "Is it life?", 
    "int-value": "42", 
    "sub": {
        "value": "84"
    }
}
```

### Inheritance

Nodes can be inherited. For example:

```cpp
struct Child: public Obj
{
    pack::String child = FIELD("child");

    using Obj::Obj;
    META_BASE(Child, Obj, child);
};
```
Here you should use different META macro -> META_BASE(This struct name, Parent name, This struct fields...). All meta information will inherited too.

### Inheritance example
```cpp
Child child;
child.value = "Is it life?";
child.intVal = 42;
child.child = "Or not";

std::cerr << *pack::json::serialize(child) << std::endl;
```

Will produce such json
```json
{
    "value": "Is it life?", 
    "int-value": "42", 
    "child": "Or not"
}
```