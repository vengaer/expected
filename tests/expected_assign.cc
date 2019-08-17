#ifndef EXPECTED_MANUAL_TEST
#include "catch.hpp"
#include "expected.h"
#include "traits.h"
#include <string>
#include <type_traits>

using namespace vien;

TEST_CASE("Copy assignment disallowed when appropriate", "[expected][assignment][copy]") {
    struct not_copy_constructible_t {
        not_copy_constructible_t(not_copy_constructible_t const&) = delete;
        not_copy_constructible_t(not_copy_constructible_t&&) noexcept = default;
        not_copy_constructible_t& operator=(not_copy_constructible_t const&) = default;
    };

    struct not_copy_assignable_t {
        not_copy_assignable_t(not_copy_assignable_t const&) = default;
        not_copy_assignable_t(not_copy_assignable_t&&) noexcept = default;
        not_copy_assignable_t operator=(not_copy_assignable_t const&) = delete;
    };

    struct move_ctor_may_throw_t {
        move_ctor_may_throw_t(move_ctor_may_throw_t&&) = default;
        move_ctor_may_throw_t& operator=(move_ctor_may_throw_t const&) = default;
    };

    REQUIRE(!std::is_copy_assignable_v<expected<void, not_copy_assignable_t>>);
    REQUIRE(!std::is_copy_assignable_v<expected<void, not_copy_constructible_t>>);

    REQUIRE(!std::is_copy_assignable_v<expected<not_copy_assignable_t, std::string>>);
    REQUIRE(!std::is_copy_assignable_v<expected<not_copy_constructible_t, std::string>>);

    REQUIRE(!std::is_copy_assignable_v<expected<std::string, not_copy_assignable_t>>);
    REQUIRE(!std::is_copy_assignable_v<expected<std::string, not_copy_constructible_t>>);

    /* Either T or E must be nothrow constructible */
    REQUIRE(!std::is_copy_assignable_v<expected<move_ctor_may_throw_t, move_ctor_may_throw_t>>);

    REQUIRE(std::is_copy_assignable_v<expected<void, std::string>>);
    REQUIRE(std::is_copy_assignable_v<expected<int, std::string>>);
}

TEST_CASE("Copy assignment noexcept if appropriate", "[expected][assignment][copy][noexcept]") {
    struct nothrow_copy_assign_t {
        nothrow_copy_assign_t& operator=(nothrow_copy_assign_t const&) noexcept {
                return *this;
        }
    };

    struct copy_assign_may_throw_t {
        copy_assign_may_throw_t(copy_assign_may_throw_t const&) noexcept { }
        copy_assign_may_throw_t& operator=(copy_assign_may_throw_t const&) {
            return *this;
        };
    };

    struct copy_ctor_may_throw_t {
        copy_ctor_may_throw_t(copy_ctor_may_throw_t const&) { }
        copy_ctor_may_throw_t& operator=(copy_ctor_may_throw_t const&) noexcept {
            return *this;
        };
    };

    REQUIRE(std::is_nothrow_copy_assignable_v<expected<void, nothrow_copy_assign_t>>);
    REQUIRE(std::is_nothrow_copy_assignable_v<expected<void, copy_assign_may_throw_t>>);
    REQUIRE(std::is_nothrow_copy_assignable_v<expected<void, copy_ctor_may_throw_t>>);
    REQUIRE(std::is_nothrow_copy_assignable_v<expected<int, nothrow_copy_assign_t>>);
    REQUIRE(std::is_nothrow_copy_assignable_v<expected<int, copy_assign_may_throw_t>>);
    REQUIRE(std::is_nothrow_copy_assignable_v<expected<int, copy_ctor_may_throw_t>>);
    REQUIRE(std::is_nothrow_copy_assignable_v<expected<int, double>>);
    REQUIRE(std::is_nothrow_copy_assignable_v<expected<nothrow_copy_assign_t, int>>);

    REQUIRE(!std::is_nothrow_copy_assignable_v<expected<copy_assign_may_throw_t, int>>);
    REQUIRE(!std::is_nothrow_copy_assignable_v<expected<copy_ctor_may_throw_t, int>>);
}

TEMPLATE_TEST_CASE("Copy assignment correct when T is not void and bool(lhs) == bool(rhs)", "[expected][assignment][copy]", int, std::string) {
    SECTION("Both lhs and rhs has value") {
        auto int_generator = [n = 1] () mutable {
            return n++;
        };

        auto instance_generator = [&]() {
            if constexpr(std::is_same_v<int, TestType>)
                return int_generator();
            else
                return "e" + std::to_string(int_generator());
        };

        expected<TestType, int> e1(instance_generator());
        expected<TestType, int> e2(instance_generator());

        REQUIRE(e1.value() != e2.value());
        e2 = e1;
        REQUIRE( !THROWS_ANY(e1.value()) );
        REQUIRE( !THROWS_ANY(e2.value()) );

        REQUIRE(e1.value() == e2.value());

        e1 = e2;

        REQUIRE(e1.value() == e2.value());
    }
    SECTION("Neither lhs not rhs has value") {
        unexpected<double> u1(20.0);
        unexpected<double> u2(30.0);

        expected<TestType, double> e1(u1);
        expected<TestType, double> e2(u2);

        REQUIRE( THROWS(e1.value(), bad_expected_access<double>) );
        REQUIRE( THROWS(e2.value(), bad_expected_access<double>) );

        REQUIRE(e1.error() != e2.error());

        e2 = e1;

        REQUIRE(e1.error() == e2.error());

        /* Test self assignment */
        e1 = e2;

        REQUIRE(e1.error() == e2.error());
    }
}

TEST_CASE("Copy assignment correct when T is void and bool(lhs) == bool(rhs)", "[expected][assignment][copy]") {
    SECTION("Both lhs and rhs has value") {
        expected<void, int> e1{};
        expected<void, int> e2{};

        REQUIRE(bool(e1) == bool(e2));
        e2 = e1;
        REQUIRE( !THROWS_ANY(e1.value()) );
        REQUIRE( !THROWS_ANY(e2.value()) );

        REQUIRE(bool(e1) == bool(e2));

        e1 = e2;

        REQUIRE(bool(e1) == bool(e2));
    }
    SECTION("Neither lhs not rhs has value") {
        unexpected<double> u1(20.0);
        unexpected<double> u2(30.0);

        expected<void, double> e1(u1);
        expected<void, double> e2(u2);

        REQUIRE( THROWS(e1.value(), bad_expected_access<double>) );
        REQUIRE( THROWS(e2.value(), bad_expected_access<double>) );

        REQUIRE(e1.error() != e2.error());

        e2 = e1;

        REQUIRE(e1.error() == e2.error());

        /* Test self assignment */
        e1 = e2;

        REQUIRE(e1.error() == e2.error());
    }
}

TEST_CASE("Copy assignment correct when T is not void and bool(lhs) != bool(rhs)", "[expected][assignment][copy]") {
    unexpected<int> u{30};

    expected<double, int> e1(u);
    expected<double, int> e2(20);

    REQUIRE( THROWS(e1.value(), bad_expected_access<int>) );
    REQUIRE( !THROWS_ANY(e2.value()) );
    REQUIRE(bool(e1) != bool(e2));

    SECTION("Assignment when bool(lhs) == true and bool(rhs) == false") {
        e2 = e1;
        REQUIRE( THROWS(e2.value(), bad_expected_access<int>) );
        REQUIRE(bool(e2) == bool(e1));
        REQUIRE(e2.error() == e1.error());
    }

    SECTION("Assignment when bool(lhs) == false and bool(rhs) == true") {
        e1 = e2;
        REQUIRE( !THROWS_ANY(e1.value()) );
        REQUIRE(bool(e2) == bool(e1));
        REQUIRE(e1.value() == e2.value());
    }
}

TEST_CASE("Copy assignment correct when T is void and bool(lhs) != bool(rhs)", "[expected][assignment][copy]") {
    unexpected<int> u{30};

    expected<void, int> e1(u);
    expected<void, int> e2{};

    REQUIRE( THROWS(e1.value(), bad_expected_access<int>) );
    REQUIRE( !THROWS_ANY(e2.value()) );
    REQUIRE(bool(e1) != bool(e2));

    SECTION("Assignment when bool(lhs) == true and bool(rhs) == false") {
        e2 = e1;
        REQUIRE( THROWS(e2.value(), bad_expected_access<int>) );
        REQUIRE(bool(e2) == bool(e1));
        REQUIRE(e2.error() == e1.error());
    }

    SECTION("Assignment when bool(lhs) == false and bool(rhs) == true") {
        e1 = e2;
        REQUIRE( !THROWS_ANY(e1.value()) );
        REQUIRE(bool(e2) == bool(e1));
    }
}

#endif
