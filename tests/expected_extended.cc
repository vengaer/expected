#ifndef EXPECTED_MANUAL_TEST
#define VIEN_EXPECTED_EXTENDED
#include "catch.hpp"
#include "expected.h"
#include "traits.h"
#include <array>
#include <forward_list>
#include <functional>
#include <map>
#include <set>
#include <string>
#include <type_traits>
#include <unordered_set>
#include <unordered_map>
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

TEST_CASE("and_then creates no unnecessary copies", "[expected][extended][and_then]") {
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

TEST_CASE("rebind_unary_template meta function", "[expected_detail][rebind_unary_template]") {
    REQUIRE(std::is_same_v<expected_detail::rebind_unary_template_t<std::less<int>, std::string>,
                           std::less<std::string>>);
    REQUIRE(std::is_same_v<expected_detail::rebind_if_hash_t<std::hash<int>, std::string>,
                           std::hash<std::string>>);
}

TEST_CASE("rebind associative container", "[expected_detail][rebind]") {
    /* std::map<int, std::string> -> std::map<std::string, int> */
    REQUIRE(std::is_same_v<expected_detail::rebind_t<std::map<int, std::string>,
                                          std::pair<std::string, int>>,
                            std::map<std::string, int>>);

    /* std::map<int, std::string> -> std::map<int, int> */
    REQUIRE(std::is_same_v<expected_detail::rebind_t<std::map<int, std::string>, int>,
                            std::map<int, int>>);

    /* std::unordered_map<std::string, int> -> std::unordered_map<std::string, std::string> */
    REQUIRE(std::is_same_v<expected_detail::rebind_t<std::unordered_map<std::string, int>,
                                        std::pair<std::string, std::string>>,
                            std::unordered_map<std::string, std::string>>);

    /* std::unordered_mulimap<std::string, int> ->
     *      std::unordered_multimap<std::vector<int>, std::string */
    REQUIRE(std::is_same_v<std::unordered_multimap<std::vector<int>, std::string>,
                           expected_detail::rebind_t<std::unordered_multimap<std::string, int>,
                                          std::pair<std::vector<int>, std::string>>>);
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

TEST_CASE("map_range works for standard array with non-default constructible value_type", "[expected][extended][map_range][array]") {
    struct non_default_t {
        non_default_t(int j) : i{j} { };
        non_default_t(non_default_t const& other) : i{other.i} { }
        non_default_t& operator=(non_default_t const& other) {
            i = other.i;
            return *this;
        }

        bool operator==(non_default_t const& other) const {
            return i == other.i;
        }

        int i;
    };

    std::array<int, 5> ai{1,2,3,4,5};
    std::array<non_default_t, 5> an{1,2,3,4,5};

    expected<std::array<int,5>, int> e1{std::move(ai)};

    auto e2 = e1.map_range([](int i) {
        return non_default_t(i);
    });

    REQUIRE(bool(e2));
    REQUIRE(e2 == an);
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

TEST_CASE("map_range works for associative container with pair type", "[expected][extended][map_range]") {
    std::map<std::string, int> m1;
    m1.insert({"1", 1});
    m1.insert({"2", 2});
    m1.insert({"3", 3});

    std::map<int, std::string> m2;
    m2.insert({1, "1"});
    m2.insert({2, "2"});
    m2.insert({3, "3"});

    expected<std::map<std::string, int>, double> e1(std::move(m1));

    auto e2 = e1.map_range([](auto&& pair) {
        return std::make_pair(std::forward<decltype(pair.second)>(pair.second),
                              std::forward<decltype(pair.first)>(pair.first));
    });

    REQUIRE(std::is_same_v<decltype(e2), expected<std::map<int, std::string>, double>>);
    REQUIRE(bool(e2));
    REQUIRE(e2 == m2);
}

TEST_CASE("map_range works for associative container with non-pair type", "[expected][extended][map_range]") {
    std::unordered_multimap<std::string, int> m1;
    m1.insert({"1", 1});
    m1.insert({"1", 2});
    m1.insert({"2", 1});
    m1.insert({"2", 2});

    std::unordered_multimap<std::string, std::string> m2;
    m2.insert({"1", "1"});
    m2.insert({"1", "2"});
    m2.insert({"2", "1"});
    m2.insert({"2", "2"});

    expected<std::unordered_multimap<std::string, int>, int> e1(std::move(m1));

    auto e2 = e1.map_range([](auto&& pair) {
        return std::to_string(std::forward<decltype(pair.second)>(pair.second));
    });

    REQUIRE(std::is_same_v<expected<std::unordered_multimap<std::string, std::string>,
                                    int>,
                           decltype(e2)>);
    REQUIRE(bool(e2));
    REQUIRE(e2 == m2);
}

TEST_CASE("map_or_else invokes callables correctly", "[expected][extended][map_or_else]") {
    expected<int, std::string> e1(unexpect, "12");
    expected<int, std::string> e2(10);

    REQUIRE(12 == e1.map_or_else([](int i) { return 2 * i; },
                                 [](auto const& str) { return std::atoi(str.data()); }));
    REQUIRE(20 == e2.map_or_else([](int i) { return 2 * i; },
                                 [](auto const& str) { return std::atoi(str.data()); }));
}

TEMPLATE_TEST_CASE("or_else invokes returns correct value", "[expected][extended][or_else]", void, std::string) {

    expected<TestType, int> e1(unexpect, 10);
    expected<TestType, int> e2{};
    if constexpr(std::is_same_v<std::string, TestType>)
        e2 = "str";

    auto e3 = e1.or_else([](int i) { return 2 * i; });

    REQUIRE(!bool(e3));
    REQUIRE(e3.error() == 20);

    auto e4 = e2.or_else([](int i) { return 2 * i; });
    REQUIRE(bool(e4));

    if constexpr(std::is_same_v<std::string, TestType>)
        REQUIRE(e4 == "str");
}

SCENARIO("rebind meta function", "[expected_detail][rebind]") {
    GIVEN("An instance of type vector<int>") {
        std::vector<int> v;

        WHEN("Rebinding the value type to std::string") {
            using result_t = expected_detail::rebind_t<decltype(v), std::string>;

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
            using result_t = expected_detail::rebind_t<decltype(a), std::string>;

            THEN("The value type is rebound correcty") {
                REQUIRE(std::is_same_v<result_t, std::array<std::string, 4>>);
            }
        }
    }
    GIVEN("A standard set of type std::set<int>") {
        std::set<int> s;

        WHEN("Rebinding the value type to std::string") {
            using result_t = expected_detail::rebind_t<decltype(s), std::string>;
            THEN("The value type, allocator and comparator are rebound correctly") {
                REQUIRE(std::is_same_v<typename result_t::value_type,
                                       std::string>);
                REQUIRE(std::is_same_v<typename result_t::allocator_type,
                                       std::allocator<std::string>>);
                REQUIRE(std::is_same_v<typename result_t::key_compare,
                                       std::less<std::string>>);
            }
        }
    }
    GIVEN("A hashed container of type std::unordered_set<int>") {
        std::unordered_set<int> s;

        WHEN("Rebinding the value type to std::string") {
            using result_t = expected_detail::rebind_t<decltype(s), std::string>;
            THEN("The value type, allocator and hash type are rebound correctly") {
                REQUIRE(std::is_same_v<typename result_t::value_type,
                                       std::string>);
                REQUIRE(std::is_same_v<typename result_t::allocator_type,
                                       std::allocator<std::string>>);
                REQUIRE(std::is_same_v<typename result_t::hasher,
                                       std::hash<std::string>>);
            }
        }
    }
    GIVEN("A hashed, associative container of type std::unordered_multimap<std::string, int>") {
        std::unordered_multimap<std::string, int> umm;

        WHEN("Rebinding the mapped type to double via non-pair") {
            using result_t = expected_detail::rebind_t<decltype(umm), double>;
            THEN("The mapped type, allocator, hasher and comparator are rebound correctly") {
                REQUIRE(std::is_same_v<typename result_t::mapped_type, double>);
                REQUIRE(std::is_same_v<typename result_t::hasher,
                                       std::hash<std::string>>);
                REQUIRE(std::is_same_v<typename result_t::allocator_type,
                                       std::allocator<std::pair<std::string const, double>>>);
                REQUIRE(std::is_same_v<typename result_t::key_equal,
                                       std::equal_to<std::string>>);
            }
        }
    }
}

#endif
