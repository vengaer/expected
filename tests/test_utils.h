#ifndef EXPECTED_TEST_UTILS_H
#define EXPECTED_TEST_UTILS_H

#include <stdexcept>

/* Test class template that throws an std::runtime_error when its 
 * nth instance is created */
template <int N>
struct nth_instance_throws_on_creation_t {
    nth_instance_throws_on_creation_t(int j) : i{j} {
        if(instances++ > N-1)
            throw std::runtime_error("default ctor");
    }

    nth_instance_throws_on_creation_t(nth_instance_throws_on_creation_t const& rhs) 
        : i{rhs.i} {
        if(instances++ > N-1)
            throw std::runtime_error("copy ctor");
    }

    nth_instance_throws_on_creation_t(nth_instance_throws_on_creation_t&& rhs) noexcept = default;

    nth_instance_throws_on_creation_t& 
        operator=(nth_instance_throws_on_creation_t const& rhs) {
        i = rhs.i;
        if(instances++ > N-1)
            throw std::runtime_error("copy assign");
        return *this;
    }

    nth_instance_throws_on_creation_t& 
        operator=(nth_instance_throws_on_creation_t&&) noexcept = default;

    int i;
    static int instances;
};

template <int N>
int nth_instance_throws_on_creation_t<N>::instances{1};

#endif
