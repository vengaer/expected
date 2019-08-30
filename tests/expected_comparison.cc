#ifndef EXPECTED_MANUAL_TEST
#include "catch.hpp"
#include "expected.h"
#include <string>

using vien::unexpect;

TEST_CASE("Comparison with expected", "[expected][comparison]") {
    vien::expected<std::string, int> e1("string");
    vien::expected<std::string, int> e2("string");
    vien::expected<std::string, int> e3("other string");
    vien::expected<std::string, int> e4(unexpect, 10);
    vien::expected<std::string, int> e5(unexpect, 10);
    vien::expected<std::string, int> e6(unexpect, 20);

    vien::expected<void, int> e7{};
    vien::expected<void, int> e8{};
    vien::expected<void, int> e9(unexpect, 10);
    vien::expected<void, int> e10(unexpect, 10);
    vien::expected<void, int> e11(unexpect, 12);

    REQUIRE(e1 == e2);
    REQUIRE(e1 != e3);
    REQUIRE(e1 != e4);
    REQUIRE(e4 == e5);
    REQUIRE(e4 != e6);

    /* void always both equal and not equal if bool(e7) && bool(e8) */
    REQUIRE(e7 == e8);
    REQUIRE(e7 != e8);

    REQUIRE(e7 != e9);
    REQUIRE(e9 == e10);
    REQUIRE(e9 != e11);
}

TEST_CASE("Comparison with T", "[expected][comparison]") {
    std::string const str = "comparison";
    vien::expected<std::string, int> e1(str);
    vien::expected<std::string, int> e2(unexpect, 10);
    vien::expected<std::string, int> e3("other");

    REQUIRE(e1 == str);
    REQUIRE(str == e1);
    REQUIRE(!(e2 == str));
    REQUIRE(!(str == e2));
    REQUIRE(e3 != str);
    REQUIRE(str != e3);
    REQUIRE(!(e1 != str));
    REQUIRE(!(str != e1));
}

TEST_CASE("Comparison with unexpected<E>", "[expected][comparison][unexpected]") {
    vien::unexpected<int> u1(10);
    vien::unexpected<int> u2(20);
    vien::expected<std::string, int> e1(unexpect, 10);
    vien::expected<std::string, int> e2("expected");

    REQUIRE(u1 == e1);
    REQUIRE(e1 != u2);
    REQUIRE(u1 != e2);
    REQUIRE(e2 != u1);
}

#endif
