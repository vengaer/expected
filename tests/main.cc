#ifndef EXPECTED_MANUAL_TEST
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#else
#include "expected.h"
#include "traits.h"
#include <iostream>
#include <string>
#include <string_view>

#define PRINT_BOOL_EXPR(...) std::cout << std::boolalpha << __VA_ARGS__ << "\n";

int main() {
    using namespace vien;

    PRINT_BOOL_EXPR(
        std::is_convertible_v<expected<int, double>, expected<double, double>>
    )
    PRINT_BOOL_EXPR(
        THROWS(throw 20.0;, int)
    )
    
    expected<double, int> e;
    expected<int, int> e1(e);
    expected<int, int> e2(std::move(e));
    //e.val_ = expected(10);

    return 0;
}
#endif
