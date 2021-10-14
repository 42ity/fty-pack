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

struct A : public pack::Node
{
    pack::String a = FIELD("a");
    pack::String b = FIELD("b");

    using pack::Node::Node;
    META(A, a, b);
};

struct B : public pack::Node
{
    pack::String a = FIELD("a");
    pack::String b = FIELD("b");
    pack::String c = FIELD("c");

    using pack::Node::Node;
    META(B, a, b, c);
};

TEST_CASE("Variant")
{
    try {
        A a;
        a.a = "a";
        a.b = "b";

        pack::Variant<A, B> var(a);
        std::string         yaml = *pack::yaml::serialize(var);

        pack::Variant<A, B> res;
        pack::yaml::deserialize(yaml, res);

        REQUIRE(res.is<A>());
        const A& check = res.get<A>();
        CHECK(a.a == check.a);
        CHECK(a.b == check.b);


        B b;
        b.a = "a";
        b.b = "b";
        b.c = "c";

        pack::Variant<A, B> var1(b);

        std::string yaml1 = *pack::yaml::serialize(var1);

        pack::Variant<A, B> res1;
        pack::yaml::deserialize(yaml1, res1);

        REQUIRE(res1.is<B>());
        const B& check1 = res1.get<B>();
        CHECK(b.a == check1.a);
        CHECK(b.b == check1.b);
        CHECK(b.c == check1.c);

        pack::ObjectList<pack::Variant<A, B>> list;
        auto&                                 v  = list.append();
        auto&                                 al = v.reset<A>();
        al.a                                     = "a1";
        al.b                                     = "b1";

        auto& v1 = list.append();
        auto& bl = v1.reset<B>();
        bl.a     = "ba1";
        bl.b     = "bb1";
        bl.c     = "bc1";

        std::string yaml2 = *pack::yaml::serialize(list);

        pack::ObjectList<pack::Variant<A, B>> lcheck;
        REQUIRE(pack::yaml::deserialize(yaml2, lcheck));
        REQUIRE(lcheck.size() == 2);
        REQUIRE(lcheck[0].is<A>());
        REQUIRE(lcheck[1].is<B>());
        CHECK(lcheck[0].get<A>().a == "a1");
        CHECK(lcheck[0].get<A>().b == "b1");
        CHECK(lcheck[1].get<B>().a == "ba1");
        CHECK(lcheck[1].get<B>().b == "bb1");
        CHECK(lcheck[1].get<B>().b == "bb1");
    } catch (std::bad_variant_access& err) {
        FAIL(err.what());
    }
}
