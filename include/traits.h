#ifndef EXPECTED_TRAITS_H
#define EXPECTED_TRAITS_H

#include <type_traits>
#include <utility>

namespace vien {

template <typename From, typename To, typename = void>
struct is_explicitly_convertible : std::false_type { };

template <typename From, typename To>
struct is_explicitly_convertible<From, To, std::void_t<decltype(static_cast<To>(std::declval<From>()))>> 
    : std::true_type { };

template <typename From, typename To>
inline bool is_explicitly_convertible_v = is_explicitly_convertible<From, To>::value;

} /* namespace vien */

#endif
