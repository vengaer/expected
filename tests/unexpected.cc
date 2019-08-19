#ifndef EXPECTED_MANUAL_TEST
#include "catch.hpp"
#include "expected.h"
#include "traits.h"
#include <type_traits>
#include <string>
#include <string_view>

using namespace vien;

TEST_CASE("unexpected lvalue conversion ctor conditionally explicit", "[unexpected][conversion][explicit]") {
    REQUIRE(std::is_convertible_v<unexpected<char const*> const&,
                                  unexpected<std::string>>);
    REQUIRE(!std::is_convertible_v<unexpected<std::string_view> const&,
                                   unexpected<std::string>>);
    REQUIRE(is_explicitly_convertible_v<unexpected<std::string_view> const&,
                                        unexpected<std::string>>);
}

TEST_CASE("unexpected rvalue conversion ctor conditionally explicit", "[unexpected][conversion][explicit]") {
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

TEST_CASE("std::swap specialization", "[unexpected][swap]") {
    unexpected<int> u1{2};
    unexpected<int> u2{5};
    std::swap(u1, u2);
    REQUIRE(u1.value() == 5);
    REQUIRE(u2.value() == 2);
}

TEST_CASE("std::swap not availble if E not swappable", "[unexpected][swap]") {
    struct non_swappable_t { 
        non_swappable_t& operator=(non_swappable_t const&) = delete;
        non_swappable_t& operator=(non_swappable_t&&) = default;
    };
    REQUIRE(!overloaded_for_swapping_v<unexpected<non_swappable_t>>);
    REQUIRE(!std::is_swappable_v<non_swappable_t>);
}

#endif
