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
#include "examples/example5.h"
#include <iostream>
#include <string>
#include <catch2/catch.hpp>

TEST_CASE("Map serialization/deserialization")
{
    test5::Item origin;
    origin.name = "some name";

    origin.intMap.append("key1", 42);
    origin.intMap.append("key2", 66);

    auto check = [](const test5::Item& item) {
        try {
            REQUIRE("some name" == item.name);
            REQUIRE(2 == item.intMap.size());
            CHECK(item.intMap.contains("key1"));
            CHECK(item.intMap.contains("key2"));
            CHECK(42 == item.intMap["key1"]);
            CHECK(66 == item.intMap["key2"]);
        } catch (const std::exception& err) {
            FAIL(err.what());
        }
    };

    check(origin);

    SECTION("Serialization yaml")
    {
        std::string cnt = *pack::yaml::serialize(origin);
        REQUIRE(!cnt.empty());

        test5::Item restored;
        pack::yaml::deserialize(cnt, restored);

        check(restored);
    }

    SECTION("Serialization json")
    {
        std::string cnt = *pack::json::serialize(origin);
        REQUIRE(!cnt.empty());

        test5::Item restored;
        pack::json::deserialize(cnt, restored);

        check(restored);
    }

    SECTION("Serialization zconfig")
    {
        std::string cnt = *pack::zconfig::serialize(origin);
        REQUIRE(!cnt.empty());

        test5::Item restored;
        pack::zconfig::deserialize(cnt, restored);

        check(restored);
    }

    SECTION("Serialization protobuf bin")
    {
        std::string cnt = *pack::protobuf::serialize(origin);
        REQUIRE(!cnt.empty());

        test5::Item restored;
        pack::protobuf::deserialize(cnt, restored);

        check(restored);
    }
}

TEST_CASE("Map of structs serialization/deserialization")
{
    test5::Item1 origin;
    origin.name = "some name";

    test5::SubItem s1;
    s1.value = "value 1";

    test5::SubItem s2;
    s2.value = "value 2";

    origin.intMap.append("key1", s1);
    origin.intMap.append("key2", s2);

    auto check = [](const test5::Item1& item) {
        try {
            REQUIRE("some name" == item.name);
            REQUIRE(2 == item.intMap.size());
            CHECK(item.intMap.contains("key1"));
            CHECK(item.intMap.contains("key2"));
            CHECK("value 1" == item.intMap["key1"].value);
            CHECK("value 2" == item.intMap["key2"].value);
        } catch (const std::exception& err) {
            FAIL(err.what());
        }
    };

    check(origin);

    SECTION("Serialization yaml")
    {
        std::string cnt = *pack::yaml::serialize(origin);
        REQUIRE(!cnt.empty());

        test5::Item1 restored;
        pack::yaml::deserialize(cnt, restored);

        check(restored);
    }

    SECTION("Serialization json")
    {
        std::string cnt = *pack::json::serialize(origin);
        REQUIRE(!cnt.empty());

        test5::Item1 restored;
        pack::json::deserialize(cnt, restored);

        check(restored);
    }

    SECTION("Serialization zconfig")
    {
        std::string cnt = *pack::zconfig::serialize(origin);
        REQUIRE(!cnt.empty());

        test5::Item1 restored;
        pack::zconfig::deserialize(cnt, restored);

        check(restored);
    }

    SECTION("Serialization protobuf bin")
    {
        std::string cnt = *pack::protobuf::serialize(origin);
        REQUIRE(!cnt.empty());

        test5::Item1 restored;
        pack::protobuf::deserialize(cnt, restored);

        check(restored);
    }
}

struct TestMap : public pack::Node
{
    using pack::Node::Node;

    pack::StringMap strs = FIELD("strs");
    pack::Int32Map  ints = FIELD("ints");

    META(TestMap, strs, ints);
};

TEST_CASE("Simple map serialization/deserialization")
{
    TestMap origin;
    origin.strs.append("key1", "some name1");
    origin.strs.append("key2", "some name2");

    origin.ints.append("key1", 12);
    origin.ints.append("key2", 13);

    auto check = [](TestMap& item) {
        try {
            REQUIRE(2 == item.strs.size());
            REQUIRE(2 == item.ints.size());
            CHECK(item.strs.contains("key1"));
            CHECK(item.strs.contains("key2"));
            CHECK(item.ints.contains("key1"));
            CHECK(item.ints.contains("key2"));
            CHECK("some name1" == item.strs["key1"]);
            CHECK("some name2" == item.strs["key2"]);
            CHECK(12 == item.ints["key1"]);
            CHECK(13 == item.ints["key2"]);
        } catch (const std::exception& err) {
            FAIL(err.what());
        }
    };

    check(origin);

    SECTION("Serialization yaml")
    {
        std::string cnt = *pack::yaml::serialize(origin);
        REQUIRE(!cnt.empty());

        TestMap restored;
        pack::yaml::deserialize(cnt, restored);

        check(restored);
    }

    SECTION("Serialization json")
    {
        std::string cnt = *pack::json::serialize(origin);
        REQUIRE(!cnt.empty());

        TestMap restored;
        pack::json::deserialize(cnt, restored);

        check(restored);
    }

    SECTION("Serialization zconfig")
    {
        std::string cnt = *pack::zconfig::serialize(origin);
        REQUIRE(!cnt.empty());

        TestMap restored;
        pack::zconfig::deserialize(cnt, restored);

        check(restored);
    }
}

struct MapObj : public pack::Node
{
    pack::String value = FIELD("value");

    using pack::Node::Node;
    META(MapObj, value);
};

TEST_CASE("Object map get/set")
{
    pack::Map<MapObj> map;

    // key not exist during get
    auto newElt = map["key0"];
    CHECK(newElt.value.value().empty());
    CHECK(map.contains("key0"));
    CHECK(map.size() == 1);
    map.clear();
    CHECK(!map.contains("key0"));
    CHECK(map.size() == 0);

    // add new element
    MapObj elt;
    elt.value = "Some string";
    map.append("key1", elt);
    CHECK(map["key1"].value == "Some string");
    CHECK(map.contains("key1"));
    CHECK(map.size() == 1);
    // add another element
    elt.value = "Another string";
    map.append("key2", elt);
    CHECK(map["key1"].value == "Some string");
    CHECK(map["key2"].value == "Another string");
    CHECK(map.contains("key2"));
    CHECK(map.size() == 2);

    // save initial first value
    auto savElt = map["key1"];
    // change current value
    map["key1"].value = "Some string modified";
    // restore initial value
    map.set("key1", savElt);
    CHECK(map["key1"].value == "Some string");
    // key not exist during set
    map.set("key3", savElt);
    CHECK(map["key3"].value == "Some string");
    CHECK(map.contains("key3"));
    CHECK(map.size() == 3);
}

TEST_CASE("Object map serialization/deserialization")
{
    pack::Map<MapObj> origin;
    auto&             it = origin.append("key1");
    it.value             = "Some string";

    auto sav             = origin["key1"];
    origin["key1"].value = "Some string modified";
    CHECK(origin["key1"].value == "Some string modified");
    origin.set("key1", sav);
    CHECK(origin["key1"].value == "Some string");

    MapObj ins;
    ins.value = "Some other value";
    origin.append("key2", ins);

    auto check = [](pack::Map<MapObj>& item) {
        try {
            REQUIRE(2 == item.size());
            CHECK(item.contains("key1"));
            CHECK(item.contains("key2"));
            CHECK("Some string" == item["key1"].value);
            CHECK("Some other value" == item["key2"].value);
        } catch (const std::exception& err) {
            FAIL(err.what());
        }
    };

    check(origin);

    {
        std::string       cnt = *pack::yaml::serialize(origin);
        pack::Map<MapObj> checked;
        pack::yaml::deserialize(cnt, checked);
        check(checked);
    }

    {
        std::string       cnt = *pack::json::serialize(origin);
        pack::Map<MapObj> checked;
        pack::json::deserialize(cnt, checked);
        check(checked);
    }
}

TEST_CASE("Value map get/set")
{
    pack::StringMap map;

    // key not exist during get
    auto newElt = map["key0"];
    CHECK(newElt.empty());
    CHECK(map.contains("key0"));
    CHECK(map.size() == 1);
    map.clear();
    CHECK(!map.contains("key0"));
    CHECK(map.size() == 0);

    // add new element
    //pack::String elt(std::string("Some string"));
    std::string elt{"Some string"};
    map.append("key1", elt);
    CHECK(map["key1"] == "Some string");
    CHECK(map.contains("key1"));
    CHECK(map.size() == 1);
    // add another element
    elt = "Another string";
    map.append("key2", elt);
    CHECK(map["key1"] == "Some string");
    CHECK(map["key2"] == "Another string");
    CHECK(map.contains("key2"));
    CHECK(map.size() == 2);

    // save initial first value
    auto savElt = map["key1"];
    // change current value
    map["key1"] = "Some string modified";
    // restore initial value
    map.set("key1", savElt);
    CHECK(map["key1"] == "Some string");
    // key not exist during set
    map.set("key3", savElt);
    CHECK(map["key3"] == "Some string");
    CHECK(map.contains("key3"));
    CHECK(map.size() == 3);
}

TEST_CASE("Value map serialization/deserialization")
{
    pack::StringMap origin;
    origin.append("key1", "Some string");

    auto sav       = origin["key1"];
    origin["key1"] = "Some string modified";
    CHECK(origin["key1"] == "Some string modified");
    origin.set("key1", sav);
    CHECK(origin["key1"] == "Some string");

    origin.append("key2", "Some other value");

    auto check = [](pack::StringMap& item) {
        try {
            REQUIRE(2 == item.size());
            CHECK(item.contains("key1"));
            CHECK(item.contains("key2"));
            CHECK("Some string" == item["key1"]);
            CHECK("Some other value" == item["key2"]);
        } catch (const std::exception& err) {
            FAIL(err.what());
        }
    };

    check(origin);

    {
        std::string     cnt = *pack::yaml::serialize(origin);
        pack::StringMap checked;
        pack::yaml::deserialize(cnt, checked);
        check(checked);
    }

    {
        std::string     cnt = *pack::json::serialize(origin);
        pack::StringMap checked;
        pack::json::deserialize(cnt, checked);
        check(checked);
    }
}
