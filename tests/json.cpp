/*  ========================================================================================================================================
    Copyright (C) 2020 Eaton
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
    ========================================================================================================================================
*/
#include <catch2/catch.hpp>
#include <iostream>
#include <pack/pack.h>

struct Empty : public pack::Node
{
    pack::String value = FIELD("value");

    using pack::Node::Node;
    META(Empty, value);
};

TEST_CASE("Empty json")
{
    {
        pack::String str;
        auto         json = *pack::json::serialize(str, pack::Option::WithDefaults);
        CHECK(json == "\"\"");
    }
    {
        Empty val;
        auto  json = *pack::json::serialize(val);
        CHECK(json == "{}");
    }
    {
        pack::ObjectList<Empty> val;
        auto                    json = *pack::json::serialize(val, pack::Option::WithDefaults);
        CHECK(json == "[]");
    }
    {
        pack::StringList val;
        auto             json = *pack::json::serialize(val, pack::Option::WithDefaults);
        CHECK(json == "[]");
    }
    {
        pack::StringMap val;
        auto            json = *pack::json::serialize(val, pack::Option::WithDefaults);
        CHECK(json == "{}");
    }
}


struct MyData : public pack::Node
{
    pack::String c = FIELD("c");
    pack::String b = FIELD("b");
    pack::String a = FIELD("a");

    using pack::Node::Node;
    META(MyData, c, b, a);
};

TEST_CASE("Orderding data in json")
{
    std::string unorderedData = R"({"a":"A", "b":"", "c":"C"})";
    MyData      data;

    if (auto ret = pack::json::deserialize(unorderedData, data); !ret) {
        FAIL(ret.error());
    }

    auto json = *pack::json::serialize(data);
    CHECK(json == R"({"c":"C","a":"A"})"); // Ordered as in pack structure and without default value
}
