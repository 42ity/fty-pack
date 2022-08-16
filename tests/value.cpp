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
#include "pack/pack.h"
#include <catch2/catch.hpp>

class A : public pack::Node
{
  public:
    pack::String a = FIELD("a");
    pack::String b = FIELD("b");
    pack::UInt32 c = FIELD("c");
    pack::UInt64 d = FIELD("d");

    using pack::Node::Node;
    META(A, a, b, c, d); 
};

TEST_CASE("Value")
{
    // setup structure
    A data;
    data.a = "test";
    data.b = "test";
    data.c = 12;
    data.d = 13;
    
    //check tests on string
    REQUIRE(data.a.value() == "test");
    REQUIRE(data.a == data.b);
    REQUIRE(data.a.compareValue("test"));
    REQUIRE(data.a == "test");
    
    // check test with UInt32/64 and int
    REQUIRE(data.c == 12);
    REQUIRE(data.d == 13);
    
}
