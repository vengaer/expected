#ifndef EXPECTED_TEST_TYPES_H
#define EXPECTED_TEST_TYPES_H

#include <stdexcept>

/* Test class template that throws an std::runtime_error when its 
 * nth instance is created */
template <int N>
struct nth_instance_throws_on_creation_t {
    nth_instance_throws_on_creation_t(int j) : i{j} {
        if(instances++ >= N-1)
            throw std::runtime_error("default ctor");
    }

    nth_instance_throws_on_creation_t(nth_instance_throws_on_creation_t const& rhs) 
        : i{rhs.i} {
        if(instances++ >= N-1)
            throw std::runtime_error("copy ctor");
    }

    nth_instance_throws_on_creation_t(nth_instance_throws_on_creation_t&& rhs) noexcept = default;

    nth_instance_throws_on_creation_t& 
        operator=(nth_instance_throws_on_creation_t const& rhs) {
        i = rhs.i;
        if(instances++ >= N-1)
            throw std::runtime_error("copy assign");
        return *this;
    }

    nth_instance_throws_on_creation_t& 
        operator=(nth_instance_throws_on_creation_t&&) noexcept = default;

    static void reset_instance_count() {
        instances = 0;
    }

    int i;
    static int instances;
};

template <int N>
int nth_instance_throws_on_creation_t<N>::instances{0};

template <int N>
struct throws_on_nth_move_t {
    throws_on_nth_move_t(int j) : i{j} { }

    throws_on_nth_move_t(throws_on_nth_move_t const& rhs) : i{rhs.i} { }

    throws_on_nth_move_t(throws_on_nth_move_t&& rhs) : i{std::move(rhs.i)} {
        if(instances++ >= N-1)
            throw std::runtime_error("move ctor");
    }

    throws_on_nth_move_t& operator=(throws_on_nth_move_t const& rhs) {
        i = rhs.i;
        return *this;
    }

    throws_on_nth_move_t& operator=(throws_on_nth_move_t&& rhs) {
        if(instances++ >= N-1)
            throw std::runtime_error("move assign");
        i = std::move(rhs.i);
        return *this;
    }

    static void reset_instance_count() {
        instances = 0;
    }

    int i;
    static int instances;
};

template <int N>
int throws_on_nth_move_t<N>::instances{0};

struct variadic_t {
    template <typename... Args>
    variadic_t(std::initializer_list<int>& il, Args&&... args) noexcept { 
        i = std::accumulate(std::begin(il), std::end(il), 0);
        i += 1000 * (args + ...);
    }
    variadic_t(variadic_t const& rhs) : i{rhs.i} { }
    variadic_t& operator=(variadic_t const& rhs) {
        i = rhs.i;
        return *this;
    }

    bool operator==(variadic_t const& rhs) const {
        return i == rhs.i;
    }

    bool operator!=(variadic_t const& rhs) const {
        return !(*this == rhs);
    }

    int i;
};

template <typename = void>
struct swap_test_t {
    swap_test_t() = default;
    swap_test_t(swap_test_t const&) = default;
    swap_test_t(swap_test_t&&) { }
    swap_test_t& operator=(swap_test_t const&) { 
        return *this;
    }
    swap_test_t& operator=(swap_test_t&&) { 
        return *this;
    }

    void swap(swap_test_t) { }
};

namespace std {
    template <typename T>
    void swap(swap_test_t<T>&, swap_test_t<T>&) { }
}

#endif
