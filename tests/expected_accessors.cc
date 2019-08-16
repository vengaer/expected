#ifndef EXPECTED_MANUAL_TEST
#include "catch.hpp"
#include "expected.h"
#include <string>
#include <type_traits>

using namespace vien;

TEMPLATE_TEST_CASE("value() returns T", "[expected][value]", int, std::string) {
    REQUIRE(std::is_same_v<TestType&&, 
                           decltype(std::declval<expected<TestType, double>>().value())>);
    REQUIRE(std::is_same_v<TestType const&&, 
                           decltype(std::declval<expected<TestType, double> const>().value())>);
    REQUIRE(std::is_same_v<TestType&, 
                           decltype(std::declval<expected<TestType, double>&>().value())>);
    REQUIRE(std::is_same_v<TestType const&, 
                           decltype(std::declval<expected<TestType, double> const&>().value())>);
}

TEST_CASE("value() returns void iff T is void", "[expected][void]") {
    REQUIRE(std::is_same_v<void, decltype(std::declval<expected<void, double>>().value())>);
    REQUIRE(!std::is_same_v<void, decltype(std::declval<expected<int, double>>().value())>);
}

TEMPLATE_TEST_CASE("error() returns E", "[expected][error]", int, std::string) {
    REQUIRE(std::is_same_v<TestType&&, 
                           decltype(std::declval<expected<double, TestType>>().error())>);
    REQUIRE(std::is_same_v<TestType const&&, 
                           decltype(std::declval<expected<double, TestType> const>().error())>);
    REQUIRE(std::is_same_v<TestType&, 
                           decltype(std::declval<expected<double, TestType>&>().error())>);
    REQUIRE(std::is_same_v<TestType const&, 
                           decltype(std::declval<expected<double, TestType> const&>().error())>);
}

#endif
