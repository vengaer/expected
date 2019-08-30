#ifndef EXPECTED_MANUAL_TEST
#include "catch.hpp"
#include "expected.h"
#include <type_traits>

TEST_CASE("Destructor trivial iff T and E trivially destructible", "[expected][destructor][conditional][trivial]") {
    REQUIRE(std::is_trivially_destructible_v<vien::expected<int, double>>);
    REQUIRE(!std::is_trivially_destructible_v<vien::expected<std::string, double>>);
    REQUIRE(!std::is_trivially_destructible_v<vien::expected<double, std::string>>);
    REQUIRE(!std::is_trivially_destructible_v<vien::expected<std::string, std::string>>);
    REQUIRE(std::is_trivially_destructible_v<vien::expected<void, double>>);
    REQUIRE(!std::is_trivially_destructible_v<vien::expected<void, std::string>>);
}

#endif
