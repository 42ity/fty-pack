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

struct Empty: public pack::Node
{
    pack::String value = FIELD("value");

    using pack::Node::Node;
    META(Empty, value);
};

TEST_CASE("Empty json")
{
    {
        pack::String str;
        auto json = *pack::json::serialize(str);
        CHECK(json == "null");
    }
    {
        Empty val;
        auto json = *pack::json::serialize(val);
        CHECK(json == "{}");
    }
    {
        pack::ObjectList<Empty> val;
        auto json = *pack::json::serialize(val);
        CHECK(json == "[]");
    }
    {
        pack::StringList val;
        auto json = *pack::json::serialize(val);
        CHECK(json == "[]");
    }
    {
        pack::StringMap val;
        auto json = *pack::json::serialize(val);
        CHECK(json == "{}");
    }
}
