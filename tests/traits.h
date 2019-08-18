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
