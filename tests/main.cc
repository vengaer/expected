#ifndef EXPECTED_MANUAL_TEST
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#else
#include "expected.h"
#include "test_types.h"
#include "traits.h"
#include <iostream>
#include <string>
#include <string_view>

#define PRINT_BOOL_EXPR(...) std::cout << std::boolalpha << __VA_ARGS__ << "\n";

int main() {
    using namespace vien;

    return 0;
}
#endif
