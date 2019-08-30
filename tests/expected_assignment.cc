#ifndef EXPECTED_MANUAL_TEST
#include "catch.hpp"
#include "expected.h"
#include "traits.h"
#include "test_types.h"
#include <initializer_list>
#include <string>
#include <type_traits>
#include <vector>

using vien::unexpect;

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

    REQUIRE(!std::is_copy_assignable_v<vien::expected<void, not_copy_assignable_t>>);
    REQUIRE(!std::is_copy_assignable_v<vien::expected<void, not_copy_constructible_t>>);

    REQUIRE(!std::is_copy_assignable_v<vien::expected<not_copy_assignable_t, std::string>>);
    REQUIRE(!std::is_copy_assignable_v<vien::expected<not_copy_constructible_t, std::string>>);

    REQUIRE(!std::is_copy_assignable_v<vien::expected<std::string, not_copy_assignable_t>>);
    REQUIRE(!std::is_copy_assignable_v<vien::expected<std::string, not_copy_constructible_t>>);

    /* Either T or E must be nothrow constructible */
    REQUIRE(!std::is_copy_assignable_v<vien::expected<move_ctor_may_throw_t, move_ctor_may_throw_t>>);

    REQUIRE(std::is_copy_assignable_v<vien::expected<void, std::string>>);
    REQUIRE(std::is_copy_assignable_v<vien::expected<int, std::string>>);
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

        vien::expected<TestType, int> e1(instance_generator());
        vien::expected<TestType, int> e2(instance_generator());

        REQUIRE(e1.value() != e2.value());
        e2 = e1;
        REQUIRE( !THROWS_ANY(e1.value()) );
        REQUIRE( !THROWS_ANY(e2.value()) );

        REQUIRE(e1.value() == e2.value());

        e1 = e2;

        REQUIRE(e1.value() == e2.value());
    }

    SECTION("Neither lhs nor rhs has value") {
        vien::unexpected<double> u1(20.0);
        vien::unexpected<double> u2(30.0);

        vien::expected<TestType, double> e1(u1);
        vien::expected<TestType, double> e2(u2);

        REQUIRE( THROWS(e1.value(), vien::bad_expected_access<double>) );
        REQUIRE( THROWS(e2.value(), vien::bad_expected_access<double>) );

        REQUIRE(e1.error() != e2.error());

        e2 = e1;

        REQUIRE(e1.error() == e2.error());

        e1 = e2;

        REQUIRE(e1.error() == e2.error());
    }
}

TEST_CASE("Copy assignment correct when T is void and bool(lhs) == bool(rhs)", "[expected][assignment][copy]") {
    SECTION("Both lhs and rhs has value") {
        vien::expected<void, int> e1{};
        vien::expected<void, int> e2{};

        REQUIRE(bool(e1) == bool(e2));
        e2 = e1;
        REQUIRE( !THROWS_ANY(e1.value()) );
        REQUIRE( !THROWS_ANY(e2.value()) );

        REQUIRE(bool(e1) == bool(e2));

        e1 = e2;

        REQUIRE(bool(e1) == bool(e2));
    }

    SECTION("Neither lhs nor rhs has value") {
        vien::unexpected<double> u1(20.0);
        vien::unexpected<double> u2(30.0);

        vien::expected<void, double> e1(u1);
        vien::expected<void, double> e2(u2);

        REQUIRE( THROWS(e1.value(), vien::bad_expected_access<double>) );
        REQUIRE( THROWS(e2.value(), vien::bad_expected_access<double>) );

        REQUIRE(e1.error() != e2.error());

        e2 = e1;

        REQUIRE(e1.error() == e2.error());

        e1 = e2;

        REQUIRE(e1.error() == e2.error());
    }
}

TEST_CASE("Copy assignment correct when T is not void and bool(lhs) != bool(rhs)", "[expected][assignment][copy]") {
    vien::unexpected<int> u{30};

    vien::expected<double, int> e1(u);
    vien::expected<double, int> e2(20);

    REQUIRE( THROWS(e1.value(), vien::bad_expected_access<int>) );
    REQUIRE( !THROWS_ANY(e2.value()) );
    REQUIRE(bool(e1) != bool(e2));

    SECTION("Assignment when bool(lhs) == true and bool(rhs) == false") {
        e2 = e1;
        REQUIRE( THROWS(e2.value(), vien::bad_expected_access<int>) );
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
    vien::unexpected<int> u{30};

    vien::expected<void, int> e1(u);
    vien::expected<void, int> e2{};

    REQUIRE( THROWS(e1.value(), vien::bad_expected_access<int>) );
    REQUIRE( !THROWS_ANY(e2.value()) );
    REQUIRE(bool(e1) != bool(e2));

    SECTION("Assignment when bool(lhs) == true and bool(rhs) == false") {
        e2 = e1;
        REQUIRE( THROWS(e2.value(), vien::bad_expected_access<int>) );
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
        vien::unexpected<int> u(5);
        nth_instance_throws_on_creation_t<2> ni{5};
        vien::expected<nth_instance_throws_on_creation_t<2>, int> e1(u);
        vien::expected<nth_instance_throws_on_creation_t<2>, int> e2(std::move(ni));
        REQUIRE( THROWS(e1 = e2, std::runtime_error) );
        REQUIRE( THROWS(e1.value(), vien::bad_expected_access<int>) );
        REQUIRE( !THROWS_ANY(e2.value()) );
        REQUIRE(e1.error() == u.value());
        REQUIRE(e2.value().i == ni.i);
        nth_instance_throws_on_creation_t<2>::reset_instance_count();
    }

    SECTION("Assignment where bool(lhs) == true and bool(rhs) == true") {
        int ni_val = 5;
        nth_instance_throws_on_creation_t<2> ni{ni_val};
        vien::unexpected<nth_instance_throws_on_creation_t<2>> u(std::move(ni));
        vien::expected<int, nth_instance_throws_on_creation_t<2>> e1(1);
        vien::expected<int, nth_instance_throws_on_creation_t<2>> e2(std::move(u));
        REQUIRE( THROWS(e1 = e2, std::runtime_error) );
        REQUIRE( !THROWS_ANY(e1.value()) );

        /* e2.value() will copy E when throwing bad_expected_access which 
         * throws a runtime_error if the count is not reset */
        nth_instance_throws_on_creation_t<2>::reset_instance_count();

        REQUIRE( THROWS(e2.value(), vien::bad_expected_access<void>) );
        REQUIRE(e1.value() == 1);
        REQUIRE(e2.error().i == ni_val);
        nth_instance_throws_on_creation_t<2>::reset_instance_count();
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
        
    REQUIRE(!std::is_move_assignable_v<vien::expected<void, move_ctor_may_throw_t>>);
    REQUIRE(!std::is_move_assignable_v<vien::expected<void, move_assign_may_throw_t>>);
    REQUIRE(!std::is_move_assignable_v<vien::expected<not_move_constructible_t, int>>);
    REQUIRE(!std::is_move_assignable_v<vien::expected<not_move_assignable_t, int>>);
    REQUIRE(!std::is_move_assignable_v<vien::expected<not_move_assignable_t, move_ctor_may_throw_t>>);
    REQUIRE(!std::is_move_assignable_v<vien::expected<not_move_constructible_t, 
                                                      move_ctor_may_throw_t>>);
    REQUIRE(!std::is_move_assignable_v<vien::expected<not_move_constructible_t, 
                                                      move_assign_may_throw_t>>);
    REQUIRE(!std::is_move_assignable_v<vien::expected<not_move_constructible_t, 
                                                      move_assign_may_throw_t>>);

    REQUIRE(std::is_move_assignable_v<vien::expected<void, int>>);
    REQUIRE(std::is_move_assignable_v<vien::expected<void, std::string>>);
    REQUIRE(std::is_move_assignable_v<vien::expected<int, double>>);
    REQUIRE(std::is_move_assignable_v<vien::expected<int, std::string>>);
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

    REQUIRE(std::is_nothrow_move_assignable_v<vien::expected<int, nothrow_t>>);
    REQUIRE(std::is_nothrow_move_assignable_v<vien::expected<int, double>>);
    REQUIRE(std::is_nothrow_move_assignable_v<vien::expected<double, double>>);
    REQUIRE(std::is_nothrow_move_assignable_v<vien::expected<nothrow_t, int>>);

    REQUIRE(!std::is_nothrow_move_assignable_v<vien::expected<void, nothrow_t>>);
    REQUIRE(!std::is_nothrow_move_assignable_v<vien::expected<void, int>>);
    REQUIRE(!std::is_nothrow_move_assignable_v<vien::expected<move_assign_may_throw_t, int>>);
    REQUIRE(!std::is_nothrow_move_assignable_v<vien::expected<move_ctor_may_throw_t, int>>);
    REQUIRE(!std::is_nothrow_move_assignable_v<vien::expected<int, move_assign_may_throw_t>>);
    REQUIRE(!std::is_nothrow_move_assignable_v<vien::expected<int, move_ctor_may_throw_t>>);
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
        vien::expected<TestType, int> e1(t1);
        vien::expected<TestType, int> e2(t2);

        REQUIRE(e1.value() != e2.value());
        e2 = std::move(e1);
        REQUIRE( !THROWS_ANY(e1.value()) );
        REQUIRE( !THROWS_ANY(e2.value()) );

        REQUIRE(t1 == e2.value());
    }

    SECTION("Neither lhs nor rhs has value") {
        vien::unexpected<double> u1(20.0);
        vien::unexpected<double> u2(30.0);

        vien::expected<TestType, double> e1(u1);
        vien::expected<TestType, double> e2(u2);

        REQUIRE( THROWS(e1.value(), vien::bad_expected_access<double>) );
        REQUIRE( THROWS(e2.value(), vien::bad_expected_access<double>) );

        REQUIRE(e1.error() != e2.error());

        e2 = std::move(e1);

        REQUIRE(u1.value() == e2.error());
    }
}

TEST_CASE("Move assignment correct when T is void and bool(lhs) == bool(rhs)", "[expected][assignment][move]") {
    SECTION("Both lhs and rhs has value") {
        vien::expected<void, int> e1{};
        vien::expected<void, int> e2{};

        REQUIRE(bool(e1) == bool(e2));
        e2 = std::move(e1);
        REQUIRE( !THROWS_ANY(e1.value()) );
        REQUIRE( !THROWS_ANY(e2.value()) );

        REQUIRE(bool(e1) == bool(e2));
    }

    SECTION("Neither lhs nor rhs has value") {
        vien::unexpected<double> u1(20.0);
        vien::unexpected<double> u2(30.0);

        vien::expected<void, double> e1(u1);
        vien::expected<void, double> e2(u2);

        REQUIRE( THROWS(e1.value(), vien::bad_expected_access<double>) );
        REQUIRE( THROWS(e2.value(), vien::bad_expected_access<double>) );

        REQUIRE(e1.error() != e2.error());

        e2 = std::move(e1);

        REQUIRE(e2.error() == u1.value());
    }
}

TEST_CASE("Move assignment correct when T is void and bool(lhs) != bool(rhs)", "[expected][assignment][move]") {
    vien::unexpected<int> u{30};

    vien::expected<void, int> e1(u);
    vien::expected<void, int> e2{};

    REQUIRE( THROWS(e1.value(), vien::bad_expected_access<int>) );
    REQUIRE( !THROWS_ANY(e2.value()) );
    REQUIRE(bool(e1) != bool(e2));

    SECTION("Assignment when bool(lhs) == true and bool(rhs) == false") {
        e2 = std::move(e1);
        REQUIRE( THROWS(e2.value(), vien::bad_expected_access<int>) );
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
    vien::unexpected<int> u(5);
    throws_on_nth_move_t<1> ni{5};
    vien::expected<throws_on_nth_move_t<1>, int> e1(u);
    vien::expected<throws_on_nth_move_t<1>, int> e2(ni);
    REQUIRE( THROWS(e1 = std::move(e2), std::runtime_error) );
    REQUIRE( THROWS(e1.value(), vien::bad_expected_access<int>) );
    REQUIRE( !THROWS_ANY(e2.value()) );
    REQUIRE(e1.error() == u.value());
    REQUIRE(e2.value().i == ni.i);
}

TEST_CASE("Unary forwarding assignment operator assigns correctly", "[expected][assignment][forwarding]") {
    vien::unexpected<int> u(1);

    vien::expected<int, int> e(10);

    e = 20;
    REQUIRE(bool(e));
    REQUIRE(e.value() == 20);

    e = u;
    REQUIRE(!bool(e));
    REQUIRE( THROWS(e.value(), vien::bad_expected_access<int>) );
    REQUIRE(e.error() == u.value());
}

TEST_CASE("Unary forwarding assignment operator satisfies strong exception guarantee", "[expected][assignment][forwarding]") {
    nth_instance_throws_on_creation_t<2> ni(10);

    vien::unexpected<int> u(20);
    vien::expected<nth_instance_throws_on_creation_t<2>, int> e(u);
    REQUIRE(!bool(e));
    REQUIRE( THROWS_ANY(e = ni) );
    REQUIRE(!bool(e));
    nth_instance_throws_on_creation_t<2>::reset_instance_count();
    REQUIRE( THROWS(e.value(), vien::bad_expected_access<int>) );
    REQUIRE(e.error() == 20);

    nth_instance_throws_on_creation_t<2>::reset_instance_count();
}

TEST_CASE("unexpected lvalue assignment operator assigns correctly", "[expected][assignment][unexpected]") {
    vien::unexpected<int> u(10);
    vien::expected<double, int> e(unexpect, 8);

    REQUIRE(!bool(e));

    e = u;
    REQUIRE(e.error() == 10);

    e = 0.0;
    REQUIRE(bool(e));

    e = u;
    REQUIRE(!bool(e));
    REQUIRE(e.error() == 10);
}

TEST_CASE("unexpected lvalue assignment operator fulfills strong exception guarantee", "[expected][assignment][unexpected]") {
    struct throws_on_copy_t {
        explicit throws_on_copy_t() = default;
        throws_on_copy_t(throws_on_copy_t const&) {
            throw std::runtime_error("copy");
        }
        throws_on_copy_t(throws_on_copy_t&&) = default;
        throws_on_copy_t& operator=(throws_on_copy_t const&) = default;
    };

    vien::unexpected<throws_on_copy_t> u(throws_on_copy_t{});
    vien::expected<int, throws_on_copy_t> e(unexpect, throws_on_copy_t{});

    REQUIRE( THROWS(e = u, std::runtime_error) );
    REQUIRE(!bool(e));

    e = 10;
    
    REQUIRE( THROWS(e = u, std::runtime_error) );
    REQUIRE(bool(e));
    REQUIRE(e.value() == 10);
}

TEST_CASE("unexpected rvalue assignment operator assigns correctly", "[expected][assignment][unexpected]") {
    vien::unexpected<int> u1(10);
    vien::unexpected<int> u2(20);
    vien::expected<double, int> e(unexpect, 8);

    REQUIRE(!bool(e));

    e = std::move(u1);
    REQUIRE(e.error() == 10);

    e = 0.0;
    REQUIRE(bool(e));

    e = std::move(u2);
    REQUIRE(!bool(e));
    REQUIRE(e.error() == 20);
}

TEST_CASE("unexpected rvalue assignment operator fulfills strong exception guarantee", "[expected][assignment][unexpected]") {
    struct throws_on_move_t {
        explicit throws_on_move_t() = default;
        throws_on_move_t(throws_on_move_t const&) = default;
        throws_on_move_t(throws_on_move_t&&) {
            throw std::runtime_error("move");
        }
        throws_on_move_t& operator=(throws_on_move_t const&) = default;
    };

    throws_on_move_t tom{};
    vien::unexpected<throws_on_move_t> u1(tom);
    vien::unexpected<throws_on_move_t> u2(tom);
    vien::expected<int, throws_on_move_t> e(unexpect, tom);

    REQUIRE( THROWS(e = std::move(u1), std::runtime_error) );
    REQUIRE(!bool(e));

    e = 10;
    
    REQUIRE( THROWS(e = std::move(u2), std::runtime_error) );
    REQUIRE(bool(e));
    REQUIRE(e.value() == 10);
}

TEST_CASE("expected<void,E>::emplace", "[expected][assignment][void]") {
    vien::expected<void, int> e{unexpect, 10};
    REQUIRE(!bool(e));
    e.emplace();
    REQUIRE(bool(e));
}

TEST_CASE("expected<T,E>::emplace(Args...)", "[expected][assignment]") {
    double d = 1.0;
    vien::expected<double, int> e(unexpect, 10);

    REQUIRE(!bool(e));

    REQUIRE(e.emplace(d) == d);
    REQUIRE(bool(e));
    REQUIRE(e.value() == d);
}

TEST_CASE("expected<T,E>::emplace(initializer_list, Args...)", "[expected][assignment]") {

    std::initializer_list<int> il{1, 2, 3, 4, 5, 6};
    variadic_t v(il, 1, 2, 3);

    vien::expected<variadic_t, int> e(unexpect, 10);

    REQUIRE(!bool(e));

    REQUIRE(e.emplace(il, 1, 2, 3) == v);
    REQUIRE(bool(e));
    REQUIRE(e.value() == v);
}

#endif
