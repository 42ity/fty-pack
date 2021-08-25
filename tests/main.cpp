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
#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_DISABLE_EXCEPTIONS
#include <catch2/catch.hpp>
#include <pack/pack.h>

struct Val: public pack::Node
{
    pack::String strVal = FIELD("str-val");
    pack::Int32 intVal = FIELD("int-val");

    using pack::Node::Node;
    META(Val, strVal, intVal);
};

TEST_CASE("Formating")
{
    Val val;
    val.strVal = "Answer is";
    val.intVal = 42;

    std::string ret = fmt::format("value = {}", val);
    CHECK(ret == R"(value = {"str-val": "Answer is", "int-val": "42"})");
}
