#ifndef EXPECTED_MANUAL_TEST
#include "catch.hpp"
#include "expected.h"
#include <string>
#include <type_traits>

using namespace vien;

TEMPLATE_TEST_CASE("Default constructible iff T is default constructible or void", "[constructor][conditional][default]", int, std::string, void) {

    REQUIRE(std::is_default_constructible_v<expected<TestType, double>>);
    REQUIRE(std::is_default_constructible_v<expected<TestType, std::string>>);
}

TEST_CASE("Not default constructible if T is not default constructible or void", "[constructor][conditional][default]") {
    struct non_default_constructible_t {
        non_default_constructible_t() = delete;
    };

    REQUIRE(!std::is_default_constructible_v<expected<non_default_constructible_t, double>>);
}

TEST_CASE("Destructor trivial iff T and E trivially destructible", "[destructor][conditional][trivial]") {
    REQUIRE(std::is_trivially_destructible_v<expected<int, double>>);
    REQUIRE(!std::is_trivially_destructible_v<expected<std::string, double>>);
    REQUIRE(!std::is_trivially_destructible_v<expected<double, std::string>>);
    REQUIRE(!std::is_trivially_destructible_v<expected<std::string, std::string>>);
    REQUIRE(std::is_trivially_destructible_v<expected<void, double>>);
    REQUIRE(!std::is_trivially_destructible_v<expected<void, std::string>>);
}

TEST_CASE("Copy constructible iff type parameters are", "[copy][conditional]") {
    struct not_copyable_t {
        not_copyable_t(not_copyable_t const&) = delete;
    };

    REQUIRE(std::is_copy_constructible_v<expected<std::string, int>>);
    REQUIRE(!std::is_copy_constructible_v<expected<not_copyable_t, int>>);
    REQUIRE(!std::is_copy_constructible_v<expected<int, not_copyable_t>>);
}

#endif
