#ifndef EXPECTED_MANUAL_TEST
#include "catch.hpp"
#include "expected.h"

using namespace vien;

TEST_CASE("unexpect_t not an aggregate", "[unexpect_t]") {
    REQUIRE(!std::is_aggregate_v<unexpect_t>);
}

TEST_CASE("unexpect_t not default constructible", "[unexpect_t]") {
    REQUIRE(!std::is_default_constructible_v<unexpect_t>);
}

#endif
