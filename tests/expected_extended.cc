#ifndef EXPECTED_MANUAL_TEST
#define VIEN_EXPECTED_EXTENDED
#include "catch.hpp"
#include "expected.h"
#include "traits.h"
#include <array>
#include <forward_list>
#include <functional>
#include <set>
#include <string>
#include <type_traits>
#include <unordered_set>
#include <vector>

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

TEST_CASE("map creates no unneccessary copies", "[expected][map]") {
    static int copies = 0;
    static int moves = 0;
    struct count_ops_t {
        count_ops_t() = default;
        count_ops_t(count_ops_t const&) { ++copies; }
        count_ops_t(count_ops_t&&) { ++moves; }
    };

    expected<count_ops_t, int> e1{};

    auto e2 = std::move(e1).map([](count_ops_t&& i) {
        return std::move(i);
    }).map([](count_ops_t&& i) {
        return std::move(i);
    });

    REQUIRE(copies == 0);
    REQUIRE(moves == 4);
    REQUIRE(bool(e2));
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

TEST_CASE("and_then yields correct values", "[expected][extended][and_then]") {
    auto squared = [](int i) { return i * i; };
    expected<int, std::string> e1(2);

    auto e2 = e1.and_then(squared).and_then(squared);
    REQUIRE(std::is_same_v<decltype(e1), decltype(e2)>);
    REQUIRE(e2 == 16);

    e2 = e2.and_then(squared);
    REQUIRE(std::is_same_v<decltype(e1), decltype(e2)>);
    REQUIRE(e2 == 256);

    expected<int, std::string> e3(unexpect, "string");
    auto e4 = e3.and_then(squared).and_then(squared);

    REQUIRE(std::is_same_v<decltype(e3), decltype(e4)>);
    REQUIRE(!bool(e4));
    REQUIRE(e4 == unexpected("string"));
}

TEST_CASE("and_then creates no unnecessary", "[expected][extended][and_then]") {
    static int copies = 0;
    static int moves = 0;
    struct count_ops_t {
        count_ops_t() = default;
        count_ops_t(count_ops_t const&) { ++copies; }
        count_ops_t(count_ops_t&&) { ++moves; }
    };

    expected<count_ops_t, int> e1{};

    auto e2 = std::move(e1).and_then([](count_ops_t&& i) {
        return std::move(i);
    }).and_then([](count_ops_t&& i) {
        return std::move(i);
    });

    REQUIRE(copies == 0);
    REQUIRE(moves == 4);
    REQUIRE(bool(e2));
}

TEST_CASE("map_range available iff T is container", "[expected][map_range]") {
    REQUIRE(has_map_range_v<expected<std::vector<int>, int>>);
    REQUIRE(!has_map_range_v<expected<int, int>>);
}

TEST_CASE("rebind_unary_template meta function", "[impl][rebind_comparator]") {
    REQUIRE(std::is_same_v<impl::rebind_unary_template_t<std::less<int>, std::string>,
                           std::less<std::string>>);
    REQUIRE(std::is_same_v<impl::rebind_if_comparator_t<std::less<int>, std::string>,
                           std::less<std::string>>);
    REQUIRE(std::is_same_v<impl::rebind_if_hash_t<std::hash<int>, std::string>,
                           std::hash<std::string>>);
}

SCENARIO("rebind meta function", "[impl][rebind]") {
    GIVEN("An instance of type vector<int>") {
        std::vector<int> v;

        WHEN("Rebinding the value type to std::string") {
            using result_t = impl::rebind_t<decltype(v), std::string>;

            THEN("The value and allocator types are rebound correctly") {
                REQUIRE(std::is_same_v<std::vector<std::string>, result_t>);
                REQUIRE(std::is_same_v<typename result_t::value_type, std::string>);
                REQUIRE(std::is_same_v<typename result_t::allocator_type,
                                       std::allocator<std::string>>);
            }
        }
    }
    GIVEN("A standard array of type array<int, 4>") {
        std::array<int, 4> a;

        WHEN("Rebinding the value type to std::string") {
            using result_t = impl::rebind_t<decltype(a), std::string>;

            THEN("The value type is rebound correcty") {
                REQUIRE(std::is_same_v<result_t, std::array<std::string, 4>>);
            }
        }
    }
    GIVEN("A standard set of type std::set<int>") {
        std::set<int> s;

        WHEN("Rebinding the value type to std::string") {
            using result_t = impl::rebind_t<decltype(s), std::string>;
            THEN("The value type and allocator is rebound correctly") {
                REQUIRE(std::is_same_v<typename result_t::value_type,
                                       std::string>);
                REQUIRE(std::is_same_v<typename result_t::allocator_type,
                                       std::allocator<std::string>>);

            }
        }
    }
}

TEST_CASE("map_range converts correctly when bool(*this)", "[expected][extended][map_range]") {
    std::vector<int> v1{1,2,3,4,5};
    std::vector<std::string> const v2{"1", "2", "3", "4", "5"};

    expected<std::vector<int>, double> e1(std::move(v1));

    auto e2 = e1.map_range( [](int i) { return std::to_string(i); } );

    REQUIRE(std::is_same_v<decltype(e2), expected<std::vector<std::string>, double>>);
    REQUIRE(bool(e2));
    REQUIRE(*e2 == v2);
}

TEST_CASE("map_range converts when !bool(*this)", "[expected][extended][map_range]") {
    std::string const str = "str";
    expected<std::vector<int>, std::string> e1(unexpect, str);

    auto e2 = e1.map_range( [](int i) { return std::to_string(i); } );

    REQUIRE(std::is_same_v<decltype(e2), expected<std::vector<std::string>, std::string>>);
    REQUIRE(!bool(e2));
    REQUIRE(e2.error() == str);
}

TEST_CASE("map_range works for standard array", "[expected][extended][map_range][array]") {
    std::array<int, 4> ai{1,2,3,4};
    std::array<std::string, 4> as{"1","2","3","4"};
    expected<std::array<std::string, 4>, int> e1(std::move(as));

    auto e2 = e1.map_range( [](std::string const& str) {
        return std::atoi(str.data());
    });

    REQUIRE(std::is_same_v<decltype(e2), expected<std::array<int, 4>, int>>);
    REQUIRE(bool(e2));
    REQUIRE(e2 == ai);
}

TEST_CASE("map_range works for insert-only container", "[expected][extended][map_range][insert]") {
    std::set<int> si{1,2};
    std::set<std::string> ss{"1","2", "4"};

    expected<std::set<int>, int> e1(std::move(si));

    auto e2 = e1.and_then([](std::set<int> s) {
        auto sc = s;
        for(auto const& i : sc)
            s.insert(2*i);
        return s;
    }).map_range([](int i) {
        return std::to_string(i);
    });

    REQUIRE(std::is_same_v<decltype(e2), expected<std::set<std::string>, int>>);
    REQUIRE(bool(e2));
    REQUIRE(e2 == ss);
}

TEST_CASE("map_range works for push_front-only container", "[expected][extended][map_range][push_front]") {
    std::forward_list<int> l1{1,2,3};
    std::forward_list<std::string> l2{"3","2","1"};

    expected<std::forward_list<int>, int> e1(std::move(l1));

    auto e2 = e1.map_range([](int i) {
        return std::to_string(i);
    });

    REQUIRE(std::is_same_v<decltype(e2), expected<std::forward_list<std::string>, int>>);
    REQUIRE(bool(e2));
    REQUIRE(e2 == l2);
}

TEST_CASE("map_range works for hashing container", "[expected][extended][map_range][hash]") {
    std::unordered_set<int> u1{1,2,3};
    std::unordered_set<std::string> u2{"1","2","3"};

    expected<std::unordered_set<int>, int> e1(std::move(u1));

    auto e2 = e1.map_range([](int i) {
        return std::to_string(i);
    });

    REQUIRE(std::is_same_v<decltype(e2), expected<std::unordered_set<std::string>, int>>);
    REQUIRE(bool(e2));
    REQUIRE(e2 == u2);
}

#endif
