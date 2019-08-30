#ifndef EXPECTED_MANUAL_TEST
#include "catch.hpp"
#include "expected.h"
#include "test_types.h"
#include "traits.h"
#include <type_traits>
#include <utility>

using vien::unexpect;

TEMPLATE_TEST_CASE("internal swap noexcept when appropriate", "[expected][swap]",
                    (vien::expected<void, int>), (vien::expected<int, double>) ){
    REQUIRE(vien::internal_swap_is_noexcept_v<TestType>);
}

TEST_CASE("internal swap not noexcept when not appropriate", "[expected][swap]") {
    REQUIRE(!vien::internal_swap_is_noexcept_v<vien::expected<void, swap_test_t<void>>>);
    REQUIRE(!vien::internal_swap_is_noexcept_v<vien::expected<int, swap_test_t<void>>>);
    REQUIRE(!vien::internal_swap_is_noexcept_v<vien::expected<swap_test_t<void>, int>>);
}

TEST_CASE("internal swap correct if bool(lhs) == bool(rhs)", "[expected][swap]") {
    SECTION("T not void and bool(e1) == bool(e2) == true") {
        vien::expected<std::string, double> e1("10");
        vien::expected<std::string, double> e2("20");

        REQUIRE(e1.value() == "10");
        REQUIRE(e2.value() == "20");

        e1.swap(e2);

        REQUIRE(e1.value() == "20");
        REQUIRE(e2.value() == "10");
    }

    SECTION("T is void and bool(e1) == bool(e2) == true") {
        vien::expected<void, double> e1{};
        vien::expected<void, double> e2{};

        REQUIRE(bool(e1));
        REQUIRE(bool(e2));

        e1.swap(e2);

        REQUIRE(bool(e1));
        REQUIRE(bool(e2));
    }

    SECTION("T not void and bool(e1) == bool(e2) == false") {
        vien::expected<std::string, int> e1(unexpect, 10);
        vien::expected<std::string, int> e2(unexpect, 20);

        REQUIRE(!bool(e1));
        REQUIRE(!bool(e2));

        REQUIRE(e1.error()  == 10);
        REQUIRE(e2.error()  == 20);

        e1.swap(e2);

        REQUIRE(!bool(e1));
        REQUIRE(!bool(e2));

        REQUIRE(e1.error()  == 20);
        REQUIRE(e2.error()  == 10);
    }

    SECTION("T is void and bool(e1) == bool(e2) == false") {
        vien::expected<void, int> e1(unexpect, 10);
        vien::expected<void, int> e2(unexpect, 20);

        REQUIRE(!bool(e1));
        REQUIRE(!bool(e2));

        REQUIRE(e1.error()  == 10);
        REQUIRE(e2.error()  == 20);

        e1.swap(e2);

        REQUIRE(!bool(e1));
        REQUIRE(!bool(e2));

        REQUIRE(e1.error()  == 20);
        REQUIRE(e2.error()  == 10);
    }
}

TEST_CASE("internal swap correct if bool(lhs) != bool(rhs)", "[expected][swap]") {
    SECTION("T not void and bool(e1) == true  and bool(e2) == false") {
        vien::expected<std::string, int> e1("10");
        vien::expected<std::string, int> e2(unexpect, 20);

        REQUIRE(bool(e1));
        REQUIRE(!bool(e2));

        REQUIRE(e1.value() == "10");
        REQUIRE(e2.error() == 20);

        e1.swap(e2);

        REQUIRE(!bool(e1));
        REQUIRE(bool(e2));

        REQUIRE(e1.error() == 20);
        REQUIRE(e2.value() == "10");
    }

    SECTION("T is void and bool(e1) == true and  bool(e2) == false") {
        vien::expected<void, int> e1{};
        vien::expected<void, int> e2(unexpect, 10);

        REQUIRE(bool(e1));
        REQUIRE(!bool(e2));

        REQUIRE(e2.error() == 10);

        e1.swap(e2);

        REQUIRE(!bool(e1));
        REQUIRE(bool(e2));

        REQUIRE(e1.error() == 10);
    }

    SECTION("T not void and bool(e1) == false and bool(e2) == true") {
        vien::expected<std::string, int> e1(unexpect, 20);
        vien::expected<std::string, int> e2("10");

        REQUIRE(!bool(e1));
        REQUIRE(bool(e2));

        REQUIRE(e1.error()  == 20);
        REQUIRE(e2.value()  == "10");

        e1.swap(e2);

        REQUIRE(bool(e1));
        REQUIRE(!bool(e2));

        REQUIRE(e1.value() == "10");
        REQUIRE(e2.error() == 20);
    }

    SECTION("T is void and bool(e1) == false and bool(e2) == true") {
        vien::expected<void, int> e1(unexpect, 10);
        vien::expected<void, int> e2{};

        REQUIRE(!bool(e1));
        REQUIRE(bool(e2));

        REQUIRE(e1.error()  == 10);
        REQUIRE( !THROWS_ANY(e2.value()) );

        e1.swap(e2);

        REQUIRE(bool(e1));
        REQUIRE(!bool(e2));

        REQUIRE( !THROWS_ANY(e1.value()) );
        REQUIRE(e2.error()  == 10);
    }
}

TEMPLATE_TEST_CASE("std::swap noexcept when appropriate", "[expected][swap]",
                    (vien::expected<void, int>), (vien::expected<int, double>) ){
    REQUIRE(vien::standard_swap_is_noexcept_v<TestType>);
}

TEST_CASE("std::swap not noexcept when not appropriate", "[expected][swap]") {
    REQUIRE(!vien::standard_swap_is_noexcept_v<vien::expected<void, swap_test_t<void>>>);
    REQUIRE(!vien::standard_swap_is_noexcept_v<vien::expected<int, swap_test_t<void>>>);
    REQUIRE(!vien::standard_swap_is_noexcept_v<vien::expected<swap_test_t<void>, int>>);
}

TEST_CASE("std::swap correct if bool(lhs) == bool(rhs)", "[expected][swap]") {
    SECTION("T not void and bool(e1) == bool(e2) == true") {
        vien::expected<std::string, double> e1("10");
        vien::expected<std::string, double> e2("20");

        REQUIRE(e1.value() == "10");
        REQUIRE(e2.value() == "20");

        std::swap(e1, e2);

        REQUIRE(e1.value() == "20");
        REQUIRE(e2.value() == "10");
    }

    SECTION("T is void and bool(e1) == bool(e2) == true") {
        vien::expected<void, double> e1{};
        vien::expected<void, double> e2{};

        REQUIRE(bool(e1));
        REQUIRE(bool(e2));

        std::swap(e1, e2);

        REQUIRE(bool(e1));
        REQUIRE(bool(e2));
    }

    SECTION("T not void and bool(e1) == bool(e2) == false") {
        vien::expected<std::string, int> e1(unexpect, 10);
        vien::expected<std::string, int> e2(unexpect, 20);

        REQUIRE(!bool(e1));
        REQUIRE(!bool(e2));

        REQUIRE(e1.error()  == 10);
        REQUIRE(e2.error()  == 20);

        std::swap(e1, e2);

        REQUIRE(!bool(e1));
        REQUIRE(!bool(e2));

        REQUIRE(e1.error()  == 20);
        REQUIRE(e2.error()  == 10);
    }

    SECTION("T is void and bool(e1) == bool(e2) == false") {
        vien::expected<void, int> e1(unexpect, 10);
        vien::expected<void, int> e2(unexpect, 20);

        REQUIRE(!bool(e1));
        REQUIRE(!bool(e2));

        REQUIRE(e1.error()  == 10);
        REQUIRE(e2.error()  == 20);

        std::swap(e1,e2);

        REQUIRE(!bool(e1));
        REQUIRE(!bool(e2));

        REQUIRE(e1.error()  == 20);
        REQUIRE(e2.error()  == 10);
    }
}

TEST_CASE("std::swap correct if bool(lhs) != bool(rhs)", "[expected][swap]") {
    SECTION("T not void and bool(e1) == true  and bool(e2) == false") {
        vien::expected<std::string, int> e1("10");
        vien::expected<std::string, int> e2(unexpect, 20);

        REQUIRE(bool(e1));
        REQUIRE(!bool(e2));

        REQUIRE(e1.value() == "10");
        REQUIRE(e2.error() == 20);

        std::swap(e1, e2);

        REQUIRE(!bool(e1));
        REQUIRE(bool(e2));

        REQUIRE(e1.error() == 20);
        REQUIRE(e2.value() == "10");
    }

    SECTION("T is void and bool(e1) == true and  bool(e2) == false") {
        vien::expected<void, int> e1{};
        vien::expected<void, int> e2(unexpect, 10);

        REQUIRE(bool(e1));
        REQUIRE(!bool(e2));

        REQUIRE(e2.error() == 10);

        std::swap(e1, e2);

        REQUIRE(!bool(e1));
        REQUIRE(bool(e2));

        REQUIRE(e1.error() == 10);
    }

    SECTION("T not void and bool(e1) == false and bool(e2) == true") {
        vien::expected<std::string, int> e1(unexpect, 20);
        vien::expected<std::string, int> e2("10");

        REQUIRE(!bool(e1));
        REQUIRE(bool(e2));

        REQUIRE(e1.error()  == 20);
        REQUIRE(e2.value()  == "10");

        std::swap(e1, e2);

        REQUIRE(bool(e1));
        REQUIRE(!bool(e2));

        REQUIRE(e1.value() == "10");
        REQUIRE(e2.error() == 20);
    }

    SECTION("T is void and bool(e1) == false and bool(e2) == true") {
        vien::expected<void, int> e1(unexpect, 10);
        vien::expected<void, int> e2{};

        REQUIRE(!bool(e1));
        REQUIRE(bool(e2));

        REQUIRE(e1.error()  == 10);
        REQUIRE( !THROWS_ANY(e2.value()) );

        std::swap(e1, e2);

        REQUIRE(bool(e1));
        REQUIRE(!bool(e2));

        REQUIRE( !THROWS_ANY(e1.value()) );
        REQUIRE(e2.error()  == 10);
    }
}

#endif
