#ifndef EXPECTED_MANUAL_TEST
#include "catch.hpp"
#include "expected.h"
#include "traits.h"
#include <string>
#include <string_view>
#include <type_traits>

using namespace vien;

TEMPLATE_TEST_CASE("Default constructible iff T is default constructible or void", "[expected][constructor][conditional][default]", int, std::string, void) {

    REQUIRE(std::is_default_constructible_v<expected<TestType, double>>);
    REQUIRE(std::is_default_constructible_v<expected<TestType, std::string>>);
}

TEST_CASE("Not default constructible if T is not default constructible or void", "[expected][constructor][conditional][default]") {
    struct non_default_constructible_t {
        non_default_constructible_t() = delete;
    };

    REQUIRE(!std::is_default_constructible_v<expected<non_default_constructible_t, double>>);
}

TEST_CASE("Perfect forwarding conversion ctor conditionally explicit", "[expected][constructor][explicit]") {
    REQUIRE(std::is_convertible_v<int, expected<int, double>>);
    REQUIRE(std::is_convertible_v<int&&, expected<int, double>>);
    REQUIRE(!std::is_convertible_v<std::string_view, expected<std::string, double>>);
    REQUIRE(!std::is_convertible_v<std::string_view&&, expected<std::string, double>>);
    REQUIRE(is_explicitly_convertible_v<std::string_view, expected<std::string, double>>);
    REQUIRE(is_explicitly_convertible_v<std::string_view&&, expected<std::string, double>>);
}

TEST_CASE("Lvalue conversion ctor conditionally explicit", "[expected][constructor][explicit]") {
    REQUIRE(std::is_convertible_v<expected<int, double> const&, 
                                  expected<double, double>>);
    REQUIRE(!std::is_convertible_v<expected<std::string_view, double> const&, 
                                   expected<std::string, double>>);
    REQUIRE(is_explicitly_convertible_v<expected<std::string_view, double> const&,
                                        expected<std::string, double>>);
}

TEST_CASE("Rvalue conversion ctor conditionally explicit", "[expected][constructor][explicit]") {
    REQUIRE(std::is_convertible_v<expected<int, double>&&, 
                                  expected<double, double>>);
    REQUIRE(!std::is_convertible_v<expected<std::string_view, double>&&, 
                                   expected<std::string, double>>);
    REQUIRE(is_explicitly_convertible_v<expected<std::string_view, double>&&,
                                        expected<std::string, double>>);
}

TEST_CASE("Destructor trivial iff T and E trivially destructible", "[expected][destructor][conditional][trivial]") {
    REQUIRE(std::is_trivially_destructible_v<expected<int, double>>);
    REQUIRE(!std::is_trivially_destructible_v<expected<std::string, double>>);
    REQUIRE(!std::is_trivially_destructible_v<expected<double, std::string>>);
    REQUIRE(!std::is_trivially_destructible_v<expected<std::string, std::string>>);
    REQUIRE(std::is_trivially_destructible_v<expected<void, double>>);
    REQUIRE(!std::is_trivially_destructible_v<expected<void, std::string>>);
}

TEST_CASE("Copy constructible iff type parameters are", "[expected][copy][conditional]") {
    struct not_copyable_t {
        not_copyable_t(not_copyable_t const&) = delete;
    };

    REQUIRE(std::is_copy_constructible_v<expected<std::string, int>>);
    REQUIRE(!std::is_copy_constructible_v<expected<not_copyable_t, int>>);
    REQUIRE(!std::is_copy_constructible_v<expected<int, not_copyable_t>>);
}

TEST_CASE("Move constructible iff type parameters are", "[expected][move][conditional]") {
    struct not_movable_t {
        not_movable_t(not_movable_t&&) = delete;
    };

    REQUIRE(std::is_move_constructible_v<expected<std::string, int>>);
    REQUIRE(!std::is_move_constructible_v<expected<not_movable_t, int>>);
    REQUIRE(!std::is_move_constructible_v<expected<int, not_movable_t>>);
}

#endif
