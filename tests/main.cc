#ifndef EXPECTED_MANUAL_TEST
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#else
#define VIEN_EXPECTED_EXTENDED
#include "expected.h"

int main() {
    return 0;
}
#endif
