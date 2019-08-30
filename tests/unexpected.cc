#ifndef EXPECTED_MANUAL_TEST
#include "catch.hpp"
#include "expected.h"
#include "traits.h"
#include <type_traits>
#include <string>
#include <string_view>

TEST_CASE("unexpected lvalue conversion ctor conditionally explicit", "[unexpected][conversion][explicit]") {
    REQUIRE(std::is_convertible_v<vien::unexpected<char const*> const&,
                                  vien::unexpected<std::string>>);
    REQUIRE(!std::is_convertible_v<vien::unexpected<std::string_view> const&,
                                   vien::unexpected<std::string>>);
    REQUIRE(vien::is_explicitly_convertible_v<vien::unexpected<std::string_view> const&,
                                              vien::unexpected<std::string>>);
}

TEST_CASE("unexpected rvalue conversion ctor conditionally explicit", "[unexpected][conversion][explicit]") {
    REQUIRE(std::is_convertible_v<vien::unexpected<char const*>&&,
                                  vien::unexpected<std::string>>);
    REQUIRE(!std::is_convertible_v<vien::unexpected<std::string_view>&&,
                                   vien::unexpected<std::string>>);
    REQUIRE(vien::is_explicitly_convertible_v<vien::unexpected<std::string_view>&&,
                                              vien::unexpected<std::string>>);
}

TEST_CASE("val_ modifiable through value()", "[unexpected][value]") {
    vien::unexpected<int> u1{42};
    REQUIRE(u1.value() == 42);

    u1.value() = 30;
    REQUIRE(u1.value() == 30);
}

TEST_CASE("Comparisons type agnostic", "[unexpected][operator==][operator!=]") {
    vien::unexpected<int>  u1{5};
    vien::unexpected<int>  u2{10};
    vien::unexpected<long> u3{10};

    REQUIRE(u1 != u2);
    REQUIRE(u2 == u3);
}

TEST_CASE("std::swap specialization", "[unexpected][swap]") {
    vien::unexpected<int> u1{2};
    vien::unexpected<int> u2{5};
    std::swap(u1, u2);
    REQUIRE(u1.value() == 5);
    REQUIRE(u2.value() == 2);
}

TEST_CASE("std::swap not availble if E not swappable", "[unexpected][swap]") {
    struct non_swappable_t { 
        non_swappable_t& operator=(non_swappable_t const&) = delete;
        non_swappable_t& operator=(non_swappable_t&&) = default;
    };
    REQUIRE(!vien::overloaded_for_swapping_v<vien::unexpected<non_swappable_t>>);
    REQUIRE(!std::is_swappable_v<non_swappable_t>);
}

#endif
