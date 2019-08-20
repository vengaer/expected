#ifndef EXPECTED_TRAITS_H
#define EXPECTED_TRAITS_H

#include <functional>
#include <type_traits>
#include <utility>

namespace vien {

template <typename From, typename To, typename = void>
struct is_explicitly_convertible : std::false_type { };

template <typename From, typename To>
struct is_explicitly_convertible<From, To, std::void_t<decltype(To(std::declval<From>()))>> 
    : std::true_type { };

template <typename From, typename To>
inline bool is_explicitly_convertible_v = is_explicitly_convertible<From, To>::value;

template <typename T, typename = void>
struct overloaded_for_swapping : std::false_type { };

template <typename T>
struct overloaded_for_swapping<T, std::void_t<decltype(std::swap(std::declval<T>(), 
                                                                     std::declval<T>()))>> 
    : std::true_type { };

template <typename T>
inline bool constexpr overloaded_for_swapping_v =
    overloaded_for_swapping<T>::value;

template <typename T>
struct internal_swap_is_noexcept
    : std::bool_constant<noexcept(std::declval<T&>().swap(std::declval<T&>()))> { };

template <typename T>
inline bool constexpr internal_swap_is_noexcept_v =
    internal_swap_is_noexcept<T>::value;

template <typename T>
struct standard_swap_is_noexcept
    : std::bool_constant<noexcept(std::swap(std::declval<T&>(), std::declval<T&>()))> { };

template <typename T>
inline bool constexpr standard_swap_is_noexcept_v =
    standard_swap_is_noexcept<T>::value;

template <typename T>
struct has_map_range_helper {
    T operator()(T) const {
        return T{};
    }
};

template <typename T, typename = void>
struct has_map_range : std::false_type { };

template <typename T>
struct has_map_range<T, std::void_t<
        decltype(std::declval<T&>().map_range(
            std::declval<has_map_range_helper<typename T::value_type::value_type>>()))>>
    : std::true_type { };

template <typename T>
inline bool constexpr has_map_range_v = has_map_range<T>::value;

} /* namespace vien */

#define THROWS(expr, except)  \
std::invoke([&]() -> bool { \
    try { \
        return [&]() -> bool { \
            try { \
                expr; \
            } \
            catch(except&) { \
                return true; \
            } \
            return false; \
        }(); \
    } \
    catch(...) { \
        return false; \
    } \
})

#define THROWS_ANY(expr) \
std::invoke([&]() -> bool { \
    try { \
        expr; \
    } \
    catch(...) { \
        return true; \
    } \
    return false; \
})



#endif
