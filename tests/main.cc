#ifndef EXPECTED_MANUAL_TEST
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#else
#include "expected.h"
#include "traits.h"
#include <iostream>
#include <string>
#include <string_view>

int main() {
    using namespace vien;
    /*
    unexpected u{40};
    std::cout << u.value() << "\n";
    std::cout << std::boolalpha << std::is_convertible_v<std::string_view, std::string> << "\n";
    std::string_view view{"asdf"};
    unexpected<std::string_view> usv{view};
    unexpected<std::string> us = static_cast<unexpected<std::string>>(usv);
    */

    return 0;
}
#endif
