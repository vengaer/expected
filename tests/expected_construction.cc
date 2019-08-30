#ifndef EXPECTED_MANUAL_TEST
#include "catch.hpp"
#include "expected.h"
#include "traits.h"
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

using vien::unexpect;

TEMPLATE_TEST_CASE("Default constructible iff T is default constructible or void", "[expected][constructor][conditional][default]", int, std::string, void) {

    REQUIRE(std::is_default_constructible_v<vien::expected<TestType, double>>);
    REQUIRE(std::is_default_constructible_v<vien::expected<TestType, std::string>>);
}

TEST_CASE("Not default constructible if T is not default constructible or void", "[expected][constructor][conditional][default]") {
    struct non_default_constructible_t {
        non_default_constructible_t() = delete;
    };

    REQUIRE(!std::is_default_constructible_v<vien::expected<non_default_constructible_t, double>>);
}

TEST_CASE("Perfect forwarding conversion ctor conditionally explicit", "[expected][constructor][explicit]") {
    REQUIRE(std::is_convertible_v<int, vien::expected<int, double>>);
    REQUIRE(std::is_convertible_v<int&&, vien::expected<int, double>>);
    REQUIRE(!std::is_convertible_v<std::string_view, vien::expected<std::string, double>>);
    REQUIRE(!std::is_convertible_v<std::string_view&&, vien::expected<std::string, double>>);
    REQUIRE(vien::is_explicitly_convertible_v<std::string_view, vien::expected<std::string, double>>);
    REQUIRE(vien::is_explicitly_convertible_v<std::string_view&&, vien::expected<std::string, double>>);
}

TEST_CASE("Lvalue conversion ctor conditionally explicit", "[expected][constructor][explicit]") {
    REQUIRE(std::is_convertible_v<vien::expected<int, double> const&, 
                                  vien::expected<double, double>>);
    REQUIRE(!std::is_convertible_v<vien::expected<std::string_view, double> const&, 
                                   vien::expected<std::string, double>>);
    REQUIRE(vien::is_explicitly_convertible_v<vien::expected<std::string_view, double> const&,
                                              vien::expected<std::string, double>>);
}

TEST_CASE("Rvalue conversion ctor conditionally explicit", "[expected][constructor][explicit]") {
    REQUIRE(std::is_convertible_v<vien::expected<int, double>&&, 
                                  vien::expected<double, double>>);
    REQUIRE(!std::is_convertible_v<vien::expected<std::string_view, double>&&, 
                                   vien::expected<std::string, double>>);
    REQUIRE(vien::is_explicitly_convertible_v<vien::expected<std::string_view, double>&&,
                                              vien::expected<std::string, double>>);
}

TEMPLATE_TEST_CASE("unexpected<E> lvalue ctor conditionally explicit", "[expected][unexpected][explicit][constructor]", void, int, double) {
    REQUIRE(std::is_convertible_v<vien::unexpected<int> const&, vien::expected<TestType, int>>);
    REQUIRE(!std::is_convertible_v<vien::unexpected<std::string_view> const&, 
                                   vien::expected<TestType, std::string>>);
    REQUIRE(vien::is_explicitly_convertible_v<vien::unexpected<std::string_view> const&, 
                                              vien::expected<TestType, std::string>>);
}

TEMPLATE_TEST_CASE("unexpected<E> rvalue ctor conditionally explicit", "[expected][unexpected][explicit][constructor]", void, int, double) {
    REQUIRE(std::is_convertible_v<vien::unexpected<int>&&, vien::expected<TestType, int>>);
    REQUIRE(!std::is_convertible_v<vien::unexpected<std::string_view>&&, 
                                   vien::expected<TestType, std::string>>);
    REQUIRE(vien::is_explicitly_convertible_v<vien::unexpected<std::string_view>&&, 
                                              vien::expected<TestType, std::string>>);
}

TEST_CASE("unexpected<E> rvalue ctor is nothrow", "[expected][unexpected][nothrow][constructor]") {
    struct may_throw_t { may_throw_t(int) { }; };
    struct nothrow_t { nothrow_t(int) noexcept { }; };

    REQUIRE(!std::is_nothrow_constructible_v<vien::expected<std::string, may_throw_t>, 
                                             vien::unexpected<int>>);
    REQUIRE(std::is_nothrow_constructible_v<vien::expected<std::string, nothrow_t>, 
                                            vien::unexpected<int>>);
}

TEST_CASE("Copy constructible iff type parameters are", "[expected][copy][conditional]") {
    struct not_copyable_t {
        not_copyable_t(not_copyable_t const&) = delete;
    };

    REQUIRE(std::is_copy_constructible_v<vien::expected<std::string, int>>);
    REQUIRE(!std::is_copy_constructible_v<vien::expected<not_copyable_t, int>>);
    REQUIRE(!std::is_copy_constructible_v<vien::expected<int, not_copyable_t>>);
}

TEST_CASE("Move constructible iff type parameters are", "[expected][move][conditional]") {
    struct not_movable_t {
        not_movable_t(not_movable_t&&) = delete;
    };

    REQUIRE(std::is_move_constructible_v<vien::expected<std::string, int>>);
    REQUIRE(!std::is_move_constructible_v<vien::expected<not_movable_t, int>>);
    REQUIRE(!std::is_move_constructible_v<vien::expected<int, not_movable_t>>);
}

TEST_CASE("Copy construction when bool(*this) == true", "[expected][copy]") {
    vien::expected<int, double> e1{1};
    vien::expected e2{e1};
    REQUIRE(e1.value() == e2.value());
}

TEMPLATE_TEST_CASE("Copy construction when bool(*this) == false", "[expected][copy]", int, void) {
    vien::unexpected<double> u(1.0);
    vien::expected<TestType, double> e1(u);
    vien::expected e2(e1);
    REQUIRE( THROWS(e2.value(), vien::bad_expected_access<double>) );
    REQUIRE(e1.error() == e2.error());
}

TEST_CASE("Move construction when bool(*this) == true", "[expected][move]") {
    int constexpr i = 1;
    vien::expected<int, double> e1{i};
    vien::expected e2{std::move(e1)};
    REQUIRE(i == e2.value());
}

TEMPLATE_TEST_CASE("Move construction when bool(*this) == false", "[expected][move]", int, void) {
    double constexpr d = 1.0;
    vien::unexpected<double> u(d);
    vien::expected<TestType, double> e1(std::move(u));
    vien::expected e2(std::move(e1));
    REQUIRE( THROWS(e2.value(), vien::bad_expected_access<double>) );
    REQUIRE(e2.error() == d);
}

TEST_CASE("in_place_t variadic ctor not availble if T == void and sizeof...(Args) > 0", "[expected][constructor][in_place_t]") {
    REQUIRE(std::is_constructible_v<vien::expected<void, int>, std::in_place_t>);
    REQUIRE(!std::is_constructible_v<vien::expected<void, int>, std::in_place_t, int>);
}

TEST_CASE("in_place_t variadic ctor not available if T != void and is_constructible_v<T, Args...> = false", "[expected][constructor][in_place_t]") {
    REQUIRE(std::is_constructible_v<vien::expected<int, int>, std::in_place_t, double>);
    REQUIRE(!std::is_constructible_v<vien::expected<int, int>, std::in_place_t, std::string>);
}

TEST_CASE("unexpect_t variadic ctor initializes s.t. bool(*this) == false", "[expected][constructor][unexpect_t]") {
    vien::expected<int, double> e(unexpect, 1);
    REQUIRE( THROWS(e.value(), vien::bad_expected_access<double>) );
    REQUIRE(e.error() == 1);
    REQUIRE(!bool(e));
}

TEST_CASE("unexpect_t initializer_list variadic ctor initializes s.t. bool(*this) == false", "[expected][constructor][unexpect_t]") {
    vien::expected<int, std::vector<int>> e(unexpect, {1,2,3});
    std::vector const v{1,2,3};
    REQUIRE( THROWS(e.value(), vien::bad_expected_access<std::vector<int>>) );
    REQUIRE(e.error() == v);
    REQUIRE(!bool(e));
}

#endif
