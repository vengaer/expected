#ifndef EXPECTED_MANUAL_TEST
#include "catch.hpp"
#include "expected.h"
#include "traits.h"
#include <type_traits>
#include <string>
#include <string_view>

using namespace vien;

TEST_CASE("Copy ctor conditionally explicit", "[unexpected][copy][explicit]") {
    REQUIRE(std::is_convertible_v<unexpected<char const*> const&,
                                  unexpected<std::string>>);
    REQUIRE(!std::is_convertible_v<unexpected<std::string_view> const&,
                                   unexpected<std::string>>);
    REQUIRE(is_explicitly_convertible_v<unexpected<std::string_view> const&,
                                        unexpected<std::string>>);
}

TEST_CASE("Move ctor conditionally explicit", "[unexpected][move][explicit]") {
    REQUIRE(std::is_convertible_v<unexpected<char const*>&&,
                                  unexpected<std::string>>);
    REQUIRE(!std::is_convertible_v<unexpected<std::string_view>&&,
                                   unexpected<std::string>>);
    REQUIRE(is_explicitly_convertible_v<unexpected<std::string_view>&&,
                                        unexpected<std::string>>);
}

TEST_CASE("val_ modifiable through value()", "[unexpected][value]") {
    unexpected<int> u1{42};
    REQUIRE(u1.value() == 42);

    u1.value() = 30;
    REQUIRE(u1.value() == 30);
}

TEST_CASE("Comparisons type agnostic", "[unexpected][operator==][operator!=]") {
    unexpected<int>  u1{5};
    unexpected<int>  u2{10};
    unexpected<long> u3{10};

    REQUIRE(u1 != u2);
    REQUIRE(u2 == u3);
}

#endif
