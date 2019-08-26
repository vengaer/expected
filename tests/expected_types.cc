#ifndef EXPECTED_MANUAL_TEST
#include "catch.hpp"
#include "expected.h"

using namespace vien;

TEST_CASE("T may be cv void", "[expected][void]") {
    expected<void const volatile, int> e1(unexpect, 10);

    REQUIRE(std::is_same_v<decltype(e1.emplace()), void>);

    e1.emplace();
    REQUIRE(bool(e1));
}

#endif
