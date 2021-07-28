<<<<<<< HEAD
=======
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
>>>>>>> master
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
