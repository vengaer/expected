#ifndef EXPECTED_MANUAL_TEST
#define VIEN_EXPECTED_EXTENDED
#include "catch.hpp"
#include "expected.h"
#include <string>
#include <type_traits>

using namespace vien;

TEST_CASE("map returns correct value", "[expected][extended][map]") {
    expected<int, double> e1(10);
    expected<int, int> e2(unexpect, 20);
    auto e3 = e1.map([](int i) {
        return std::to_string(10*i);
    });

    REQUIRE(std::is_same_v<expected<std::string, double>, decltype(e3)>);
    REQUIRE(bool(e3));
    REQUIRE("100" == e3);

    auto e4 = e2.map([](int i) {
        return std::to_string(i);
    });

    REQUIRE(std::is_same_v<expected<std::string, int>, decltype(e4)>);
    REQUIRE(!bool(e4));
    REQUIRE(e4 == unexpected(20));
}

TEST_CASE("Chaining maps yields correct result", "[expected][extended][map]") {
    expected<int, double> e1(10);

    auto e2 = e1.map([](int i) {
        return std::to_string(i);
    }).map([](std::string const& str) {
        return std::to_string(std::atoi(str.data()) * 4);
    });
    REQUIRE(std::is_same_v<expected<std::string, double>, decltype(e2)>);
    REQUIRE(bool(e2));
    REQUIRE("40" == e2);
}

TEST_CASE("map when T is void", "[expected][extended][map]") {
    expected<void, int> e1{};

    auto e2 = e1.map([]() {
        return 10;
    });
    REQUIRE(std::is_same_v<expected<int, int>, decltype(e2)>);
    REQUIRE(e2 == 10);
}

TEST_CASE("map_error modifies error", "[expected][extended][map_error]") {
    expected<std::string, int> e1("string");
    expected<std::string, int> e2(unexpect, 10);

    auto e3 = e1.map_error([](int) {
        return double{};
    });

    REQUIRE(std::is_same_v<expected<std::string, double>, decltype(e3)>);
    REQUIRE(e3.has_value());
    REQUIRE(e3 == "string");

    auto e4 = e2.map_error([](int i) {
        return i * 200;
    }).map_error([](int i) {
        return std::to_string(i);
    });

    REQUIRE(std::is_same_v<expected<std::string, std::string>, decltype(e4)>);
    REQUIRE(!bool(e4));
    REQUIRE(e4 == unexpected("2000"));

    expected<void, int> e5(unexpect, 10);

    auto e6 = e5.map_error([](int i) {
        return 2 * i;
    });

    REQUIRE(e6 == unexpected(20));
}

#endif
