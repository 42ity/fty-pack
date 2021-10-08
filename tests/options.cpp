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

struct Test : public pack::Node
{
    struct Inner : public Node
    {
        pack::String val = FIELD("val");

        using pack::Node::Node;
        META(Inner, val);
    };

    pack::String            value   = FIELD("value", "val");
    pack::Int32             intVal  = FIELD("intVal");
    pack::Bool              boolVal = FIELD("boolVal");
    pack::StringList        strList = FIELD("strList");
    pack::ObjectList<Inner> objList = FIELD("objList");
    pack::Map<Inner>        objMap  = FIELD("objMap");
    pack::StringMap         strMap  = FIELD("strMap");

    using pack::Node::Node;
    META(Test, value, intVal, boolVal, strList, objList, objMap, strMap);
};

TEST_CASE("Serialization options")
{
    Test tst;
    auto out1 = *pack::json::serialize(tst);
    CHECK(out1 == "{}");
    auto out2 = *pack::json::serialize(tst, pack::Option::WithDefaults);
    CHECK(out2 == R"({"value": "val", "intVal": "0", "boolVal": "false", "strList": [], "objList": [], "objMap": {}, "strMap": {}})");
}
