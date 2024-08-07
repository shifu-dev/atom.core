module;
#include "catch2/catch_test_macros.hpp"

module atom_core.tests:string;

import atom_core;

using namespace atom;

TEST_CASE("atom_core.string")
{
    string str;

    REQUIRE(str.get_count() == 0);
    REQUIRE(str.get_data() == nullptr);

    REQUIRE(str.get_iterator() == str.get_iterator_end());
    REQUIRE(str.get_iterator() == str.get_iterator_end());

    string str0(move(str));

    //     str.insert_at(str.get_iterator(), 0);
    //     str.insert_at(str.get_iterator(), { 1, 2 });
    //
    //     str.insert_first(0);
    //     str.insert_first({ 1, 2 });
    //
    //     str.insert_last(0);
    //     str.insert_last({ 1, 2 });
    //
    //     str.remove_at(str.get_iterator());
    //     str.remove_range({ str.get_iterator(), str.end() });
}
