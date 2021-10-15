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

    pack::String            value    = FIELD("value", "val");
    pack::Int32             intVal   = FIELD("intVal");
    pack::Bool              boolVal  = FIELD("boolVal");
    pack::Float             floatVal = FIELD("floatVal");
    pack::StringList        strList  = FIELD("strList");
    pack::ObjectList<Inner> objList  = FIELD("objList");
    pack::Map<Inner>        objMap   = FIELD("objMap");
    pack::StringMap         strMap   = FIELD("strMap");

    using pack::Node::Node;
    META(Test, value, intVal, boolVal, floatVal, strList, objList, objMap, strMap);
};

TEST_CASE("Serialization options with default")
{
    Test tst;
    auto out1 = *pack::json::serialize(tst);
    CHECK(out1 == "{}");
    auto out2 = *pack::json::serialize(tst, pack::Option::WithDefaults);
    CHECK(out2 == R"({"value":"val","intVal":0,"boolVal":false,"floatVal":0.0,"strList":[],"objList":[],"objMap":{},"strMap":{}})");
}

struct Test1 : public pack::Node
{
    struct Inner1 : public Node
    {
        pack::UInt32 val = FIELD("val");

        using pack::Node::Node;
        META(Inner1, val);
    };

    pack::Int32              intVal    = FIELD("intVal");
    pack::Bool               boolVal   = FIELD("boolVal");
    pack::Double             doubleVal = FIELD("doubleVal");
    pack::ObjectList<Inner1> objList   = FIELD("objList");
    pack::Map<Inner1>        objMap    = FIELD("objMap");

    using pack::Node::Node;
    META(Test1, intVal, boolVal, doubleVal, objList, objMap);
};

TEST_CASE("Serialization options value as string")
{
    Test1 tst;
    tst.intVal    = 12;
    tst.boolVal   = true;
    tst.doubleVal = 8.569;

    auto& v = tst.objList.append();
    v.val   = 155;

    Test1::Inner1 kv;
    kv.val = 68;
    tst.objMap.append("key", kv);

    auto out1 = *pack::json::serialize(tst);
    CHECK(out1 == R"({"intVal":12,"boolVal":true,"doubleVal":8.569,"objList":[{"val":155}],"objMap":{"key":{"val":68}}})");
    auto out2 = *pack::json::serialize(tst, pack::Option::ValueAsString);
    CHECK(out2 == R"({"intVal":"12","boolVal":"true","doubleVal":"8.569","objList":[{"val":"155"}],"objMap":{"key":{"val":"68"}}})");
}

TEST_CASE("Serialization options pretty print 2")
{
    Test1 tst;
    tst.intVal    = 24;
    tst.boolVal   = true;
    tst.doubleVal = 58.74;

    auto& v = tst.objList.append();
    v.val   = 254;

    Test1::Inner1 kv;
    kv.val = 47;
    tst.objMap.append("key", kv);

    auto out1 = *pack::json::serialize(tst);
    CHECK(out1 == R"({"intVal":24,"boolVal":true,"doubleVal":58.74,"objList":[{"val":254}],"objMap":{"key":{"val":47}}})");

    auto out2 = *pack::json::serialize(tst, pack::Option::PrettyPrint);
    CHECK(out2 == 
R"({
    "intVal": 24,
    "boolVal": true,
    "doubleVal": 58.74,
    "objList": [
        {
            "val": 254
        }
    ],
    "objMap": {
        "key": {
            "val": 47
        }
    }
})");
}


TEST_CASE("Serialization options value as string and with default")
{
    Test1 tst;

    auto out1 = *pack::json::serialize(tst);
    CHECK(out1 == R"({})");

    auto out2 = *pack::json::serialize(tst, pack::Option::ValueAsString);
    CHECK(out2 == R"({})");

    auto out3 = *pack::json::serialize(tst, pack::Option::WithDefaults);
    CHECK(out3 == R"({"intVal":0,"boolVal":false,"doubleVal":0.0,"objList":[],"objMap":{}})");

    auto out4 = *pack::json::serialize(tst, pack::Option::ValueAsString | pack::Option::WithDefaults);
    CHECK(out4 == R"({"intVal":"0","boolVal":"false","doubleVal":"0","objList":[],"objMap":{}})");
}
