#ifndef EXPECTED_MANUAL_TEST
#include "catch.hpp"
#include "expected.h"
#include "traits.h"
#include <string.h>
#include <cstddef>
#include <string>
#include <type_traits>

using namespace vien;

TEST_CASE("operator-> returns correct address", "[expected][accessor]") {
    std::string const incorrect = "incorrect";
    char const correct[] = "correct";

    expected<std::string, int> e(incorrect);
    char* c = e->data();
    
    std::size_t i = 0;
    for(; i < strlen(correct) && i < incorrect.size(); i++)
        *(c + i) = correct[i];
    *(c + i) = '\0';

    REQUIRE(strcmp(e.value().data(), correct) == 0);
}

TEST_CASE("operator-> const when appropriate", "[expected][accessor]") {
    struct int_generator_t {
        int get() { return 0; }
        int get() const { return 1; }
    }; 

    expected<int_generator_t, int> e1;
    expected<int_generator_t, int> const e2;

    REQUIRE(e1->get() == 0);
    REQUIRE(e2->get() == 1);
}

TEST_CASE("operator* returns ref", "[expected][accessor]") {
    expected<int, std::string> e(30);;
    *e = 20;
    REQUIRE(e.value() == 20);
}

TEST_CASE("operator* does not throw", "[expected][accessor]") {
    expected<int, std::string> e(unexpect, "string");
    REQUIRE( !THROWS_ANY(*e) );
}

TEMPLATE_TEST_CASE("value()  returns T", "[expected][value][accessor]", int, std::string) {
    REQUIRE(std::is_same_v<TestType&&, 
                           decltype(std::declval<expected<TestType, double>>().value())>);
    REQUIRE(std::is_same_v<TestType const&&, 
                           decltype(std::declval<expected<TestType, double> const>().value())>);
    REQUIRE(std::is_same_v<TestType&, 
                           decltype(std::declval<expected<TestType, double>&>().value())>);
    REQUIRE(std::is_same_v<TestType const&, 
                           decltype(std::declval<expected<TestType, double> const&>().value())>);
}

TEST_CASE("value() returns void iff T is void", "[expected][value][accessor]") {
    REQUIRE(std::is_same_v<void, decltype(std::declval<expected<void, double>>().value())>);
    REQUIRE(!std::is_same_v<void, decltype(std::declval<expected<int, double>>().value())>);
}

TEST_CASE("value() throws if !bool(*this)", "[expected][value][accessor]") {
    unexpected<int> u(1);
    expected<double, int> e(u);
    REQUIRE( THROWS(e.value(), bad_expected_access<int>) );
}

TEMPLATE_TEST_CASE("error() returns E", "[expected][error][accessor]", int, std::string) {
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
