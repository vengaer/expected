#ifndef EXPECTED_MANUAL_TEST
#include "catch.hpp"
#include "expected.h"


TEST_CASE("unexpect_t not an aggregate", "[unexpect_t]") {
    REQUIRE(!std::is_aggregate_v<vien::unexpect_t>);
}

TEST_CASE("unexpect_t not default constructible", "[unexpect_t]") {
    REQUIRE(!std::is_default_constructible_v<vien::unexpect_t>);
}

#endif
