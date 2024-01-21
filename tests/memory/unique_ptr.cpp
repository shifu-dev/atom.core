import atom.core;
#include "helpers/tracked_type.h"
#include "catch2/catch_test_macros.hpp"

using namespace atom;
using namespace atom::tests;

TEST_CASE("atom.core.unique_ptr")
{
    SECTION("default constructor")
    {
        unique_ptr<tracked_type> ptr;

        REQUIRE(ptr.eq(nullptr));
    }

    SECTION("null constructor")
    {
        unique_ptr<tracked_type> ptr(nullptr);

        REQUIRE(ptr.eq(nullptr));
    }

    SECTION("value constructor")
    {
        tracked_type val;
        unique_ptr<tracked_type> ptr(&val);

        REQUIRE(ptr.eq(&val));
    }

    SECTION("copy constructor")
    {
        STATIC_REQUIRE(not rcopy_constructible<unique_ptr<tracked_type>>);
    }

    SECTION("move constructor")
    {
        tracked_type val;
        unique_ptr<tracked_type> ptr1(&val);
        unique_ptr<tracked_type> ptr2(mov(ptr1));

        REQUIRE(ptr1.eq(nullptr));
        REQUIRE(ptr2.eq(&val));
    }

    SECTION("destructor")
    {
        tracked_type val;

        {
            unique_ptr<tracked_type> ptr(&val);
        }

        REQUIRE(val.last_op == tracked_type::eoperation::destructor);
    }

    SECTION("copy operator")
    {
        STATIC_REQUIRE(not rcopy_assignable<unique_ptr<tracked_type>>);
    }

    SECTION("move operator")
    {
        tracked_type val1;
        tracked_type val2;
        unique_ptr<tracked_type> ptr1(&val1);
        unique_ptr<tracked_type> ptr2(&val2);

        ptr2 = mov(ptr1);

        REQUIRE(ptr1.eq(nullptr));
        REQUIRE(ptr2.eq(&val1));
        REQUIRE(val1.last_op == tracked_type::eoperation::default_constructor);
        REQUIRE(val2.last_op == tracked_type::eoperation::destructor);
    }

    SECTION("null operator")
    {
        tracked_type val;
        unique_ptr<tracked_type> ptr(&val);

        // ptr = nullptr;

        REQUIRE(val.last_op == tracked_type::eoperation::destructor);
    }

    SECTION("release()")
    {
        tracked_type val;
        unique_ptr<tracked_type> ptr(&val);

        ptr.release();

        REQUIRE(ptr.eq(nullptr));
        REQUIRE(val.last_op == tracked_type::eoperation::default_constructor);
    }

    SECTION("destroy()")
    {
        tracked_type val;
        unique_ptr<tracked_type> ptr(&val);

        ptr.destroy();

        REQUIRE(ptr.eq(nullptr));
        REQUIRE(val.last_op == tracked_type::eoperation::destructor);
    }
}