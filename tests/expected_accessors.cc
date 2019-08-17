#ifndef EXPECTED_MANUAL_TEST
#include "catch.hpp"
#include "expected.h"
#include "traits.h"
#include <string>
#include <type_traits>

using namespace vien;

TEMPLATE_TEST_CASE("value accessor returns T", "[expected][value][accessor]", int, std::string) {
    REQUIRE(std::is_same_v<TestType&&, 
                           decltype(std::declval<expected<TestType, double>>().value())>);
    REQUIRE(std::is_same_v<TestType const&&, 
                           decltype(std::declval<expected<TestType, double> const>().value())>);
    REQUIRE(std::is_same_v<TestType&, 
                           decltype(std::declval<expected<TestType, double>&>().value())>);
    REQUIRE(std::is_same_v<TestType const&, 
                           decltype(std::declval<expected<TestType, double> const&>().value())>);
}

TEST_CASE("value accessor returns void iff T is void", "[expected][value][accessor]") {
    REQUIRE(std::is_same_v<void, decltype(std::declval<expected<void, double>>().value())>);
    REQUIRE(!std::is_same_v<void, decltype(std::declval<expected<int, double>>().value())>);
}

TEST_CASE("value accessor throws if !bool(*this)", "[expected][value][accessor]") {
    unexpected<int> u(1);
    expected<double, int> e(u);
    REQUIRE(
        THROWS(
            e.value(), bad_expected_access<int>
        )
    );
}

TEMPLATE_TEST_CASE("error accessor returns E", "[expected][error][accessor]", int, std::string) {
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
