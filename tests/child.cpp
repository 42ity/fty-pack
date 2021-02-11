/*  ========================================================================
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
    ========================================================================
*/
#include <pack/pack.h>
#include <catch2/catch.hpp>
#include <iostream>

struct Parent: public pack::Node
{
    pack::String value = FIELD("value");
    pack::String field = FIELD("field");

    using pack::Node::Node;
    META(Parent, value, field);
};

struct Child: public Parent
{
    pack::String child = FIELD("child");

    using Parent::Parent;
    META_BASE(Child, Parent, child);
};

TEST_CASE("Child class")
{
    Child origin;
    origin.field = "field";
    origin.value = "value";
    origin.child = "child";

    auto check = [](const Child& item) {
        REQUIRE("value" == item.value);
        REQUIRE("field" == item.field);
        REQUIRE("child" == item.child);

    };

    check(origin);

    SECTION("Serialization yaml")
    {
        std::string cnt = *pack::yaml::serialize(origin);
        REQUIRE(!cnt.empty());

        Child restored;
        pack::yaml::deserialize(cnt, restored);

        check(restored);
    }

    SECTION("Serialization json")
    {
        std::string cnt = *pack::json::serialize(origin);
        REQUIRE(!cnt.empty());

        Child restored;
        pack::json::deserialize(cnt, restored);

        check(restored);
    }

    SECTION("Serialization zconfig")
    {
        std::string cnt = *pack::zconfig::serialize(origin);
        REQUIRE(!cnt.empty());

        Child restored;
        pack::zconfig::deserialize(cnt, restored);

        check(restored);
    }
}
