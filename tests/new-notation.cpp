#include "new-notation.h"
#include "examples/example2.h"
#include "pack/pack.h"
#include <catch2/catch.hpp>
#include <iostream>

struct MyNode : virtual public pack::Node
{
    PACK(MyNode);

    pack::String fld = FIELD("fld");

    PACK_FIELDS(fld)
};

struct Other : public pack::Node
{
    PACK(Other);

    pack::String otherFld = FIELD("other");

    PACK_FIELDS(otherFld)
};

struct Node1 : public MyNode
{
    PACK(Node1, EXTENDS(MyNode));

    pack::String fld1 = FIELD("fld1");

    PACK_FIELDS(fld1)
};

//struct Node2 : public MyNode, public Other
//{
//    PACK(Node2, EXTENDS(MyNode), EXTENDS(Other));

//    pack::String fld2 = FIELD("fld2");

//    PACK_FIELDS(fld2)
//};

TEST_CASE("New notation")
{
    MyNode node;
    node.fld = "MyNode";

    for (const auto& key : node.fieldNames()) {
        std::cerr << "fld " << key << std::endl;
    }

    MyNode node2(node); // copy ctor
    CHECK(node2.fld == node.fld);

    MyNode node3 = node; // copy assigment
    CHECK(node3.fld == node.fld);

    std::cerr << "----------------------------" << std::endl;

    Node1 n;
    n.fld  = "fld";
    n.fld1 = "fld1";
    std::cerr << n.fld.value() << " " << n.fld1.value() << std::endl;

    Node1 n2(n); // copy ctor
    std::cerr << n2.fld.value() << " " << n2.fld1.value() << std::endl;
    CHECK(n2.fld1 == n.fld1);
    CHECK(n2.fld == n.fld);
}
