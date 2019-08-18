#ifndef EXPECTED_MANUAL_TEST
#include "catch.hpp"
#include "expected.h"
#include "traits.h"
#include "test_utils.h"
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

    SECTION("Neither lhs nor rhs has value") {
        unexpected<double> u1(20.0);
        unexpected<double> u2(30.0);

        expected<TestType, double> e1(u1);
        expected<TestType, double> e2(u2);

        REQUIRE( THROWS(e1.value(), bad_expected_access<double>) );
        REQUIRE( THROWS(e2.value(), bad_expected_access<double>) );

        REQUIRE(e1.error() != e2.error());

        e2 = e1;

        REQUIRE(e1.error() == e2.error());

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

    SECTION("Neither lhs nor rhs has value") {
        unexpected<double> u1(20.0);
        unexpected<double> u2(30.0);

        expected<void, double> e1(u1);
        expected<void, double> e2(u2);

        REQUIRE( THROWS(e1.value(), bad_expected_access<double>) );
        REQUIRE( THROWS(e2.value(), bad_expected_access<double>) );

        REQUIRE(e1.error() != e2.error());

        e2 = e1;

        REQUIRE(e1.error() == e2.error());

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
        REQUIRE(!bool(e2));
        REQUIRE(bool(e2) == bool(e1));
        REQUIRE(e2.error() == e1.error());
    }

    SECTION("Assignment when bool(lhs) == false and bool(rhs) == true") {
        e1 = e2;
        REQUIRE( !THROWS_ANY(e1.value()) );
        REQUIRE(bool(e1));
        REQUIRE(bool(e2) == bool(e1));
    }
}

TEST_CASE("Copy assignment satisfies strong exception guarantee", "[expected][assignment][copy]") {
    nth_instance_throws_on_creation_t<2>::reset_instance_count();

    SECTION("Assignment where bool(lhs) == false and bool(rhs) == true") {
        unexpected<int> u(5);
        nth_instance_throws_on_creation_t<2> ni{5};
        expected<nth_instance_throws_on_creation_t<2>, int> e1(u);
        expected<nth_instance_throws_on_creation_t<2>, int> e2(std::move(ni));
        REQUIRE( THROWS(e1 = e2, std::runtime_error) );
        REQUIRE( THROWS(e1.value(), bad_expected_access<int>) );
        REQUIRE( !THROWS_ANY(e2.value()) );
        REQUIRE(e1.error() == u.value());
        REQUIRE(e2.value().i == ni.i);
    }

    SECTION("Assignment where bool(lhs) == true and bool(rhs) == true") {
        int ni_val = 5;
        nth_instance_throws_on_creation_t<2> ni{ni_val};
        unexpected<nth_instance_throws_on_creation_t<2>> u(std::move(ni));
        expected<int, nth_instance_throws_on_creation_t<2>> e1(1);
        expected<int, nth_instance_throws_on_creation_t<2>> e2(std::move(u));
        REQUIRE( THROWS(e1 = e2, std::runtime_error) );
        REQUIRE( !THROWS_ANY(e1.value()) );

        /* e2.value() will copy E and throw a runtime_error if not reset */
        nth_instance_throws_on_creation_t<2>::reset_instance_count();

        REQUIRE( THROWS(e2.value(), bad_expected_access<void>) );
        REQUIRE(e1.value() == 1);
        REQUIRE(e2.error().i == ni_val);
    }
}

TEST_CASE("Move assignment disallowed when appropriate", "[expected][assignment][move]") {
    struct not_move_constructible_t {
        not_move_constructible_t(not_move_constructible_t&&) = delete;
        not_move_constructible_t& operator=(not_move_constructible_t&&) noexcept { 
            return *this; 
        }
    };

    struct not_move_assignable_t {
        not_move_assignable_t(not_move_assignable_t&&) noexcept { }
        not_move_assignable_t& operator=(not_move_assignable_t&&) = delete;
    };

    struct move_ctor_may_throw_t {
        move_ctor_may_throw_t(move_ctor_may_throw_t&&) { }
        move_ctor_may_throw_t& operator=(move_ctor_may_throw_t&&) noexcept {
            return *this;
        }
    };

    struct move_assign_may_throw_t {
        move_assign_may_throw_t(move_assign_may_throw_t&&) noexcept { }
        move_assign_may_throw_t& operator=(move_assign_may_throw_t&&) {
            return *this;
        }
    };
        
    REQUIRE(!std::is_move_assignable_v<expected<void, move_ctor_may_throw_t>>);
    REQUIRE(!std::is_move_assignable_v<expected<void, move_assign_may_throw_t>>);
    REQUIRE(!std::is_move_assignable_v<expected<not_move_constructible_t, int>>);
    REQUIRE(!std::is_move_assignable_v<expected<not_move_assignable_t, int>>);
    REQUIRE(!std::is_move_assignable_v<expected<not_move_assignable_t, move_ctor_may_throw_t>>);
    REQUIRE(!std::is_move_assignable_v<expected<not_move_constructible_t, 
                                                move_ctor_may_throw_t>>);
    REQUIRE(!std::is_move_assignable_v<expected<not_move_constructible_t, 
                                                move_assign_may_throw_t>>);
    REQUIRE(!std::is_move_assignable_v<expected<not_move_constructible_t, 
                                                move_assign_may_throw_t>>);

    REQUIRE(std::is_move_assignable_v<expected<void, int>>);
    REQUIRE(std::is_move_assignable_v<expected<void, std::string>>);
    REQUIRE(std::is_move_assignable_v<expected<int, double>>);
    REQUIRE(std::is_move_assignable_v<expected<int, std::string>>);
}

TEST_CASE("Move assignment noexcept if appropriate", "[expected][assignment][move][noexcept]") {
    struct nothrow_t {
        nothrow_t(nothrow_t&&) noexcept { };
        nothrow_t& operator=(nothrow_t&&) noexcept {
            return *this;
        }
    };

    struct move_assign_may_throw_t {
        move_assign_may_throw_t(move_assign_may_throw_t&&) noexcept { }
        move_assign_may_throw_t& operator=(move_assign_may_throw_t&&) {
            return *this;
        };
    };

    struct move_ctor_may_throw_t {
        move_ctor_may_throw_t(move_ctor_may_throw_t&&) { }
        move_ctor_may_throw_t& operator=(move_ctor_may_throw_t&&) noexcept {
            return *this;
        };
    };

    REQUIRE(std::is_nothrow_move_assignable_v<expected<int, nothrow_t>>);
    REQUIRE(std::is_nothrow_move_assignable_v<expected<int, double>>);
    REQUIRE(std::is_nothrow_move_assignable_v<expected<double, double>>);
    REQUIRE(std::is_nothrow_move_assignable_v<expected<nothrow_t, int>>);

    REQUIRE(!std::is_nothrow_move_assignable_v<expected<void, nothrow_t>>);
    REQUIRE(!std::is_nothrow_move_assignable_v<expected<void, int>>);
    REQUIRE(!std::is_nothrow_move_assignable_v<expected<move_assign_may_throw_t, int>>);
    REQUIRE(!std::is_nothrow_move_assignable_v<expected<move_ctor_may_throw_t, int>>);
    REQUIRE(!std::is_nothrow_move_assignable_v<expected<int, move_assign_may_throw_t>>);
    REQUIRE(!std::is_nothrow_move_assignable_v<expected<int, move_ctor_may_throw_t>>);
}


TEMPLATE_TEST_CASE("Move assignment correct when T is not void and bool(lhs) == bool(rhs)", "[expected][assignment][move]", int, std::string) {
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
        TestType t1 = instance_generator();
        TestType t2 = instance_generator();
        expected<TestType, int> e1(t1);
        expected<TestType, int> e2(t2);

        REQUIRE(e1.value() != e2.value());
        e2 = std::move(e1);
        REQUIRE( !THROWS_ANY(e1.value()) );
        REQUIRE( !THROWS_ANY(e2.value()) );

        REQUIRE(t1 == e2.value());
    }

    SECTION("Neither lhs nor rhs has value") {
        unexpected<double> u1(20.0);
        unexpected<double> u2(30.0);

        expected<TestType, double> e1(u1);
        expected<TestType, double> e2(u2);

        REQUIRE( THROWS(e1.value(), bad_expected_access<double>) );
        REQUIRE( THROWS(e2.value(), bad_expected_access<double>) );

        REQUIRE(e1.error() != e2.error());

        e2 = std::move(e1);

        REQUIRE(u1.value() == e2.error());
    }
}

TEST_CASE("Move assignment correct when T is void and bool(lhs) == bool(rhs)", "[expected][assignment][move]") {
    SECTION("Both lhs and rhs has value") {
        expected<void, int> e1{};
        expected<void, int> e2{};

        REQUIRE(bool(e1) == bool(e2));
        e2 = std::move(e1);
        REQUIRE( !THROWS_ANY(e1.value()) );
        REQUIRE( !THROWS_ANY(e2.value()) );

        REQUIRE(bool(e1) == bool(e2));
    }

    SECTION("Neither lhs nor rhs has value") {
        unexpected<double> u1(20.0);
        unexpected<double> u2(30.0);

        expected<void, double> e1(u1);
        expected<void, double> e2(u2);

        REQUIRE( THROWS(e1.value(), bad_expected_access<double>) );
        REQUIRE( THROWS(e2.value(), bad_expected_access<double>) );

        REQUIRE(e1.error() != e2.error());

        e2 = std::move(e1);

        REQUIRE(e2.error() == u1.value());
    }
}

TEST_CASE("Move assignment correct when T is void and bool(lhs) != bool(rhs)", "[expected][assignment][move]") {
    unexpected<int> u{30};

    expected<void, int> e1(u);
    expected<void, int> e2{};

    REQUIRE( THROWS(e1.value(), bad_expected_access<int>) );
    REQUIRE( !THROWS_ANY(e2.value()) );
    REQUIRE(bool(e1) != bool(e2));

    SECTION("Assignment when bool(lhs) == true and bool(rhs) == false") {
        e2 = std::move(e1);
        REQUIRE( THROWS(e2.value(), bad_expected_access<int>) );
        REQUIRE(!bool(e2));
        REQUIRE(bool(e2) == bool(e1));
        REQUIRE(e2.error() == u.value());
    }

    SECTION("Assignment when bool(lhs) == false and bool(rhs) == true") {
        e1 = std::move(e2);
        REQUIRE( !THROWS_ANY(e1.value()) );
        REQUIRE(bool(e1));
        REQUIRE(bool(e2) == bool(e1));
    }
}

TEST_CASE("Move assignment satisfies strong exception guarantee", "[expected][assignment][move]") {
    unexpected<int> u(5);
    throws_on_nth_move_t<1> ni{5};
    expected<throws_on_nth_move_t<1>, int> e1(u);
    expected<throws_on_nth_move_t<1>, int> e2(ni);
    REQUIRE( THROWS(e1 = std::move(e2), std::runtime_error) );
    REQUIRE( THROWS(e1.value(), bad_expected_access<int>) );
    REQUIRE( !THROWS_ANY(e2.value()) );
    REQUIRE(e1.error() == u.value());
    REQUIRE(e2.value().i == ni.i);
}

#endif
