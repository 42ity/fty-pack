#include <pack/pack.h>
#include <catch2/catch.hpp>
#include <iostream>

struct Test: public pack::Node
{
    pack::String value = FIELD("value", "val");
    pack::Int32  intVal = FIELD("intVal");
    pack::Bool   boolVal = FIELD("boolVal");

    using pack::Node::Node;
    META(Test, value, intVal, boolVal);
};

TEST_CASE("Serialization options")
{
    Test tst;
    auto out1 = *pack::json::serialize(tst);
    CHECK(out1 == "{}");
    auto out2 = *pack::json::serialize(tst, pack::Option::WithDefaults);
    CHECK(out2 == R"({"value": "val", "intVal": "0", "boolVal": "false"})");
}
