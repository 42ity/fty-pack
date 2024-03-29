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
#include "examples/example2.h"

TEST_CASE("List serialization/deserialization")
{
    test::Person2 origin;
    origin.name  = "Person";
    origin.value = 42;
    origin.items.append(11);
    origin.items.append(12);
    origin.items.append(13);

    auto& more = origin.more.append();
    more.name = "name number 1";

    auto& more1 = origin.more.append();
    more1.name = "name number 2";

    auto check = [](const test::Person2& item) {
        REQUIRE("Person" == item.name);
        REQUIRE(42 == item.value);

        REQUIRE(3 == item.items.size());
        REQUIRE(11 == item.items[0]);
        REQUIRE(12 == item.items[1]);
        REQUIRE(13 == item.items[2]);

        REQUIRE(2 == item.more.size());
        REQUIRE("name number 1" == item.more[0].name);
        REQUIRE("name number 2" == item.more[1].name);
    };

    check(origin);

    SECTION("Serialization yaml")
    {
        std::string cnt = *pack::yaml::serialize(origin);
        REQUIRE(!cnt.empty());
        INFO("yaml content:" << cnt)

        test::Person2 restored;
        pack::yaml::deserialize(cnt, restored);

        check(restored);
    }

    SECTION("Serialization json")
    {
        std::string cnt = *pack::json::serialize(origin);
        REQUIRE(!cnt.empty());

        test::Person2 restored;
        pack::json::deserialize(cnt, restored);

        check(restored);
    }

    SECTION("Serialization zconfig")
    {
        std::string cnt = *pack::zconfig::serialize(origin);
        REQUIRE(!cnt.empty());

        test::Person2 restored;
        pack::zconfig::deserialize(cnt, restored);

        check(restored);
    }

    SECTION("Serialization protobuf")
    {
        std::string cnt = *pack::protobuf::serialize(origin);
        REQUIRE(!cnt.empty());

        test::Person2 restored;
        pack::protobuf::deserialize(cnt, restored);

        check(restored);
    }
}

