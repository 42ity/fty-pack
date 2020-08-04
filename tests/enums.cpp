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
#include "examples/example4.h"
#include <catch2/catch.hpp>

TEST_CASE("Enum serialization/deserialization")
{
    test4::Item origin;
    origin.enval = test4::Item::EnumValue::Value2;

    auto check = [](const test4::Item& item) {
        REQUIRE(test4::Item::EnumValue::Value2 == item.enval);
    };

    check(origin);

    SECTION("Serialization yaml")
    {
        std::string cnt = *pack::yaml::serialize(origin);
        REQUIRE(!cnt.empty());

        test4::Item restored;
        pack::yaml::deserialize(cnt, restored);

        check(restored);
    }

    SECTION("Serialization json")
    {
        std::string cnt = *pack::json::serialize(origin);
        REQUIRE(!cnt.empty());

        test4::Item restored;
        pack::json::deserialize(cnt, restored);

        check(restored);
    }

    SECTION("Serialization zconfig")
    {
        std::string cnt = *pack::zconfig::serialize(origin);
        REQUIRE(!cnt.empty());

        test4::Item restored;
        pack::zconfig::deserialize(cnt, restored);

        check(restored);
    }

    SECTION("Serialization protobuf")
    {
        std::string cnt = *pack::protobuf::serialize(origin);
        REQUIRE(!cnt.empty());

        test4::Item restored;
        pack::protobuf::deserialize(cnt, restored);

        check(restored);
    }
}

