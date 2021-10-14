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
#include "examples/example1.h"

TEST_CASE("Simple serialization/deserialization")
{
    test::Person origin;
    origin.email = "person@email.org";
    origin.id    = 42;
    origin.name  = "Person";
    origin.binary.setString("some bin data");

    auto check = [](const test::Person& item) {
        REQUIRE(42 == item.id);
        REQUIRE("person@email.org" == item.email);
        REQUIRE("Person" == item.name);
        REQUIRE("some bin data" == item.binary.asString());
    };

    check(origin);

    SECTION("Serialization yaml")
    {
        std::string cnt = *pack::yaml::serialize(origin);
        REQUIRE(!cnt.empty());
        INFO("yaml content:" << cnt)

        test::Person restored;
        pack::yaml::deserialize(cnt, restored);

        check(restored);
    }

    SECTION("Serialization json")
    {
        std::string cnt = *pack::json::serialize(origin);
        REQUIRE(!cnt.empty());

        test::Person restored;
        pack::json::deserialize(cnt, restored);

        check(restored);
    }

    SECTION("Serialization zconfig")
    {
        std::string cnt = *pack::zconfig::serialize(origin);
        REQUIRE(!cnt.empty());

        test::Person restored;
        pack::zconfig::deserialize(cnt, restored);

        check(restored);
    }

    SECTION("Serialization protobuf bin")
    {
        std::string cnt = *pack::protobuf::serialize(origin);
        REQUIRE(!cnt.empty());

        test::Person restored;
        pack::protobuf::deserialize(cnt, restored);

        check(restored);
    }
}

TEST_CASE("UTF-8 test")
{
    test::Person origin;
    origin.email = "成語";
    origin.id    = 42;
    origin.name  = "Персона";

    auto check = [](const test::Person& item) {
        REQUIRE(42 == item.id);
        REQUIRE("成語" == item.email);
        REQUIRE("Персона" == item.name);
    };

    check(origin);

    SECTION("Serialization yaml")
    {
        std::string cnt = *pack::yaml::serialize(origin);
        REQUIRE(!cnt.empty());

        test::Person restored;
        pack::yaml::deserialize(cnt, restored);

        check(restored);
    }

    SECTION("Serialization json")
    {
        std::string cnt = *pack::json::serialize(origin);
        REQUIRE(!cnt.empty());

        test::Person restored;
        pack::json::deserialize(cnt, restored);

        check(restored);
    }

    SECTION("Serialization zconfig")
    {
        std::string cnt = *pack::zconfig::serialize(origin);
        REQUIRE(!cnt.empty());

        test::Person restored;
        pack::zconfig::deserialize(cnt, restored);

        check(restored);
    }

    SECTION("Serialization protobuf bin")
    {
        std::string cnt = *pack::protobuf::serialize(origin);
        REQUIRE(!cnt.empty());

        test::Person restored;
        pack::protobuf::deserialize(cnt, restored);

        check(restored);
    }
}
