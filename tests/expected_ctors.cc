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

TEMPLATE_TEST_CASE("unexpected<E> lvalue ctor conditionally explicit", "[expected][unexpected][explicit][constructor]", void, int, double) {
    REQUIRE(std::is_convertible_v<unexpected<int> const&, expected<TestType, int>>);
    REQUIRE(!std::is_convertible_v<unexpected<std::string_view> const&, 
                                   expected<TestType, std::string>>);
    REQUIRE(is_explicitly_convertible_v<unexpected<std::string_view> const&, 
                                        expected<TestType, std::string>>);
}

TEMPLATE_TEST_CASE("unexpected<E> rvalue ctor conditionally explicit", "[expected][unexpected][explicit][constructor]", void, int, double) {
    REQUIRE(std::is_convertible_v<unexpected<int>&&, expected<TestType, int>>);
    REQUIRE(!std::is_convertible_v<unexpected<std::string_view>&&, 
                                   expected<TestType, std::string>>);
    REQUIRE(is_explicitly_convertible_v<unexpected<std::string_view>&&, 
                                        expected<TestType, std::string>>);
}

TEST_CASE("unexpected<E> rvalue ctor is nothrow", "[expected][unexpected][nothrow][constructor]") {
    struct may_throw_t { may_throw_t(int) { }; };
    struct nothrow_t { nothrow_t(int) noexcept { }; };

    REQUIRE(!std::is_nothrow_constructible_v<expected<std::string, may_throw_t>, 
                                             unexpected<int>>);
    REQUIRE(std::is_nothrow_constructible_v<expected<std::string, nothrow_t>, 
                                            unexpected<int>>);
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

TEST_CASE("Copy construction when bool(*this) == true", "[expected][copy]") {
    expected<int, double> e1{1};
    expected e2{e1};
    REQUIRE(e1.value() == e2.value());
}

TEMPLATE_TEST_CASE("Copy construction when bool(*this) == false", "[expected][copy]", int, void) {
    unexpected<double> u(1.0);
    expected<TestType, double> e1(u);
    expected e2(e1);
    REQUIRE( THROWS(e2.value(), bad_expected_access<double>) );
    REQUIRE(e1.error() == e2.error());
}

TEST_CASE("in_place_t variadic ctor not availble if T == void and sizeof...(Args) > 0", "[expected][constructor][in_place_t]") {
    REQUIRE(std::is_constructible_v<expected<void, int>, in_place_t>);
    REQUIRE(!std::is_constructible_v<expected<void, int>, in_place_t, int>);
}

TEST_CASE("in_place_t variadic ctor not available if T != void and is_constructible_v<T, Args...> = false", "[expected][constructor][in_place_t]") {
    REQUIRE(std::is_constructible_v<expected<int, int>, in_place_t, double>);
    REQUIRE(!std::is_constructible_v<expected<int, int>, in_place_t, std::string>);
}

#endif
