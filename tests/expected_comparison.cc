#ifndef EXPECTED_MANUAL_TEST
#include "catch.hpp"
#include "expected.h"
#include <string>

using namespace vien;

TEST_CASE("Comparison with expected", "[expected][comparison]") {
    expected<std::string, int> e1("string");
    expected<std::string, int> e2("string");
    expected<std::string, int> e3("other string");
    expected<std::string, int> e4(unexpect, 10);
    expected<std::string, int> e5(unexpect, 10);
    expected<std::string, int> e6(unexpect, 20);

    expected<void, int> e7{};
    expected<void, int> e8{};
    expected<void, int> e9(unexpect, 10);
    expected<void, int> e10(unexpect, 10);
    expected<void, int> e11(unexpect, 12);

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
    expected<std::string, int> e1(str);
    expected<std::string, int> e2(unexpect, 10);
    expected<std::string, int> e3("other");

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
    unexpected<int> u1(10);
    unexpected<int> u2(20);
    expected<std::string, int> e1(unexpect, 10);
    expected<std::string, int> e2("expected");

    REQUIRE(u1 == e1);
    REQUIRE(e1 != u2);
    REQUIRE(u1 != e2);
    REQUIRE(e2 != u1);
}

#endif
