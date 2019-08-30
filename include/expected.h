/* Based off of the 9th revision of the std::expected
 * proposal (p0323r9). Uses SFINAE to achieve the
 * conditionally explicit constructors to be
 * introduced in C++20.
 *
 * Define VIEN_EXPECTED_EXTENDED for functional
 * extensions not in the proposal. */

#ifndef VIEN_EXPECTED_H
#define VIEN_EXPECTED_H

/*
    --expected synopsis--

*** using the not yet available explicit(condition) syntax

namespace vien {
inline namespace v1 {


__ class template expected __

template <typename T, typename E>
class expected {
    public:
        using value_type = T;
        using error_type = E;
        using unexpected_type = unexpected<E>;

        template <typename U>
        using rebind = expected<U, error_type>;

        constexpr expected();
        constexpr expected(expected const&);
        constexpr expected(expected&&)
                        noexcept(std::is_nthrow_move_constructible_v<E> &&
                                (std::is_void_v<T> || std::is_nothrow_move_constructible_v<T>))

        template <typename U, typename G>
        constexpr explicit((!std::is_void_v<T> && !std::is_void_v<U> &&
                            !std::is_convertible_v<U const&, T>) ||
                            !std::is_convertible_v<G const&, E>)
        expected(expected<U,G> const&);

        template <typename U, typename G>
        constexpr explicit((!std::is_void_v<T> && !std::is_void_v<U> &&
                            !std::is_convertible_v<U const&, T>) ||
                            !std::is_convertible_v<G const&, E>)
        expected(expected<U,G>&&);

        template <typename U = T>
        constexpr explicit(!std::is_convertible_v<U&&,T>) expected(U&&);

        template <typename G = E>
        constexpr expected(unexpected<G> const&);
        template <typename G = E>
        constexpr expected(unexpected<G>&&);

        template <typename... Args>
        constexpr explicit expected(std::in_place_t, Args&&...);
        template <typename U, typename... Args>
        constexpr explicit expected(std::in_place_t, std::inializer_list<U>&, Args&&...);
        template <typename... Args>
        constexpr explicit expected(unexpect_t, Args&&...);
        template <typename U, typename... Args>
        constexpr explicit expected(unexpect_t, std::initializer_list<U>&, Args&&...);

        ~expected();

        expected& operator=(expected const&);
        expected& operator=(expected&&) noexcept(std::is_nothrow_move_constructible_v<T> &&
                                                 std::is_nothrow_move_assignable_v<T>) {
        template <typename U = T>
        expected& operator=(U&&);
        template <typename G = E>
        expected& operator=(unexpected<G> const&);
        template <typename G = E>
        expected& operator=(unexpected<G>&&);

        template <typename... Args>
        T& emplace(Args&&...);
        template <typename U, typename... Args>
        T& emplace(std::initializer_list<U>&, Args&&...);

        void swap(expected&) noexcept(std::is_nothrow_move_constructible_v<T> &&
                                      std::is_nothrow_swappable_v<T> &&
                                      std::is_nothrow_move_constructible_v<E> &&
                                      std::is_nothrow_swappable_v<E>);

        constexpr T* operator->();
        constexpr T const* operator->() const;
        constexpr T& operator*() &;
        constexpr T const& operator*() const &;
        constexpr T&& operator*() &&;
        constexpr T const&& operator*() const &&;

        constexpr explicit operator bool() const noexcept;
        constexpr bool has_value() const noexcept;
        constexpr T& value() &;
        constexpr T const& value() const &;
        constexpr T&& value() &&;
        constexpr T const&& value() const &&;

        constexpr E& error() &;
        constexpr E const& error() const &;
        constexpr E&& error() &&;
        constexpr E const&& error() const &&;

        template <typename U>
        constexpr T value_or(U&&) const&;
        temlpate <typename U>
        constexpr T value_or(U&&) &&;

        template <typename T1, typename E1, typename T2, typename E2>
        friend constexpr bool operator==(expected<T1,E1> const&, expected<T2,E2> const&);
        template <typename T1, typename E1, typename T2, typename E2>
        friend constexpr bool operator!=(expected<T1,E1> const&, expected<T2,E2> const&);

        template <typename T1, typename E1, typename T2>
        friend constexpr bool operator==(expected<T1,E1> const&, T2 const&);
        template <typename T1, typename E1, typename T2>
        friend constexpr bool operator==(T2 const&, expected<T1,E1> const&);
        template <typename T1, typename E1, typename T2>
        friend constexpr bool operator!=(expected<T1,E1> const&, T2 const&);
        template <typename T1, typename E1, typename T2>
        friend constexpr bool operator!=(T2 const&, expected<T1,E1> const&);

        template <typename T1, typename E1, typename E2>
        friend constexpr bool operator==(expected<T1,E1> const&, unexpected<E2> const&);
        template <typename T1, typename E1, typename E2>
        friend constexpr bool operator==(unexpected<E2> const&, expected<T1,E1> const&);
        template <typename T1, typename E1, typename E2>
        friend constexpr bool operator!=(expected<T1,E1> const&, unexpected<E2> const&);
        template <typename T1, typename E1, typename E2>
        friend constexpr bool operator!=(unexpected<E2> const&, expected<T1,E1> const&);

        template <typename T1, typename E1>
        friend void swap(expected<T1,E1>& x, expected<T1,E1>& y) noexcept(noexcept(x.swap(y)));

        #ifdef VIEN_EXPECTED_EXTENDED
        template <typename F>
        constexpr expected<std::decay_t<std::invoke_result_t<F,T>>, E>
            map(F&&) &;
        template <typename F>
        constexpr expected<std::decay_t<std::invoke_result_t<F,T>>, E>
            map(F&&) const &;
        template <typename F>
        constexpr expected<std::decay_t<std::invoke_result_t<F,T>>, E>
            map(F&&) &&;
        template <typename F>
        constexpr expected<std::decay_t<std::invoke_result_t<F,T>>, E>
            map(F&&) const &&;

        template <typename F>
        constexpr auto map_range(F&&) &;
        template <typename F>
        constexpr auto map_range(F&&) const &;
        template <typename F>
        constexpr auto map_range(F&&) &&;
        template <typename F>
        constexpr auto map_range(F&&) const &&;

        template <typename F>
        constexpr expected<T, std::decay_t<std::invoke_result_t<F,E>>>
            map_error(F&&) &;
        template <typename F>
        constexpr expected<T, std::decay_t<std::invoke_result_t<F,E>>>
            map_error(F&&) const &;
        template <typename F>
        constexpr expected<T, std::decay_t<std::invoke_result_t<F,E>>>
            map_error(F&&) &&;
        template <typename F>
        constexpr expected<T, std::decay_t<std::invoke_result_t<F,E>>>
            map_error(F&&) const &&;

        template <typename M, typename F>
        constexpr T map_or_else(M&&, T&&) &;
        template <typename M, typename F>
        constexpr T map_or_else(M&&, T&&) const &;
        template <typename M, typename F>
        constexpr T map_or_else(M&&, T&&) &&;
        template <typename M, typename F>
        constexpr T map_or_else(M&&, T&&) const &&;

        template <typename F>
        constexpr expected and_then(F&&) &;
        template <typename F>
        constexpr expected and_then(F&&) const &;
        template <typename F>
        constexpr expected and_then(F&&) &&;
        template <typename F>
        constexpr expected and_then(F&&) const &&;

        template <typename F>
        constexpr expected or_else(F&&) &;
        template <typename F>
        constexpr expected or_else(F&&) const &;
        template <typename F>
        constexpr expected or_else(F&&) &&;
        template <typename F>
        constexpr expected or_else(F&&) const &&;
        #endif

    private:
        bool has_val;
        union {
            value_type val;
            unexpected_type unexpect;
        };
};


__ class template unexpected __

template <typename E>
class unexpected {
    public:
        template <typename Err = E>
        constexpr explicit unexpected(Err&&);

        template <typename... Args>
        constexpr explicit unexpected(in_place_t, Args&&...);

        template <typename U, typename... Args>
        constexpr explicit unexpected(in_place_t, std::initializer_list<U>, Args&&...);

        constexpr unexpected(unexpected const&) = default;
        constexpr unexpected(unexpected&&) = default;

        template <typename Err>
        constexpr explicit(!std::is_convertible_v<Err, E>)
            unexpected(unexpected<Err> const&);

        template <typename Err>
        constexpr explicit(!std::is_convertible_v<Err&&,E>)
            unexpected(unexpected<Err>&&);

        constexpr unexpected& operator=(unexpected const&) = default;
        constexpr unexpected& operator=(unexpected&&) = default;
        template <typename Err = E>
        constexpr unexpected& operator=(unexpected<Err> const&);
        template <typename Err = E>
        constexpr unexpected& operator=(unexpected<Err>&&);

        constexpr E& value() & noexcept;
        constexpr E const& value() const & noexcept;
        constexpr E&& value() && noexcept;
        constexpr E const&& value() const && noexcept;

        void swap(unexpected&) noexcept(std::is_nothrow_swappable_v<Err>);

        template <typename E1, typename E2>
        friend constexpr bool operator==(unexpected<E1> const&, unexpected<E2> const&);
        template <typename E1, typename E2>
        friend constexpr bool operator!=(unexpected<E1> const&, unexpected<E2> const&);

        template <typename E1>
        friend void swap(unexpected<E1>& x, unexpected<E1>& y) noexcept(noexcept(x.swap(y)));

    private:
        E val_;
};

__ class template bad_expected_access __

template <>
class bad_expected_access<void> : public std::exception {
    public:
        explicit bad_expected_access() { };
};

template <typename E>
class bad_expected_access : public bad_expected_access<void> {
    public:
        explicit bad_expected_access(E);
        E& error() &;
        E const& error() const &;
        E&& error() &&;
        E const&& error() const &&;
        virtual char const* what() const noexcept override;
    private:
        E val_;
};

}
}
 */

#include <initializer_list>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <utility>

#ifdef VIEN_EXPECTED_EXTENDED
#include <algorithm>
#include <cstddef>
#include <functional>
#include <iterator>
#endif

#ifdef VIEN_EXPECTED_EXTENDED
/* Forward declare array */
namespace std {
    template <typename, std::size_t>
    struct array;
}
#endif

namespace vien {
using in_place_t = std::in_place_t;

struct unexpect_t {
    constexpr explicit unexpect_t(void*) { }
};

inline constexpr unexpect_t unexpect{nullptr};

inline namespace v1 {

template <typename, typename>
class expected;

template <typename>
class unexpected;

template <typename>
class bad_expected_access;


namespace expected_detail {

template <typename T>
using remove_cvref = std::remove_cv<std::remove_reference_t<T>>;

template <typename T>
using remove_cvref_t = typename remove_cvref<T>::type;

template <typename T, typename... P0toN>
struct is_one_of : std::disjunction<std::is_same<T, P0toN>...> { };

template <typename T, typename... P0toN>
inline bool constexpr is_one_of_v = is_one_of<T, P0toN...>::value;

template <typename T>
struct is_default_constructible_or_void
    : std::bool_constant<std::is_default_constructible_v<T> ||
                         std::is_void_v<T>> { };

template <typename T>
inline bool constexpr is_default_constructible_or_void_v =
        is_default_constructible_or_void<T>::value;

template <typename T>
struct is_copy_constructible_or_void
    : std::bool_constant<std::is_copy_constructible_v<T> ||
                         std::is_void_v<T>> { };

template <typename T>
inline bool constexpr is_copy_constructible_or_void_v =
        is_copy_constructible_or_void<T>::value;

template <typename T>
struct is_move_constructible_or_void
    : std::bool_constant<std::is_move_constructible_v<T> ||
                         std::is_void_v<T>> { };

template <typename T>
inline bool constexpr is_move_constructible_or_void_v =
        is_move_constructible_or_void<T>::value;

#ifdef __CYGWIN__
/* Work around bug where Cygwin tries to form reference to void */
template <typename T, typename = void>
struct is_trivially_copy_constructible_or_void
    : std::is_trivially_copy_constructible<T> { };

template <typename T>
struct is_trivially_copy_constructible_or_void<T, std::enable_if_t<
    is_one_of_v<T, void,
                   void const,
                   void volatile,
                   void const volatile>>> : std::true_type { };

#else
template <typename T>
struct is_trivially_copy_constructible_or_void
    : std::bool_constant<std::is_trivially_copy_constructible_v<T> ||
                         std::is_void_v<T>> { };
#endif

template <typename T>
inline bool constexpr is_trivially_copy_constructible_or_void_v =
        is_trivially_copy_constructible_or_void<T>::value;

#ifdef __CYGWIN__
/* Work around bug where Cygwin tries to form reference to void */
template <typename T, typename = void>
struct is_trivially_move_constructible_or_void
    : std:is_triially_move_constructible<T> { };

template <typename T>
struct is_trivially_move_constructible_or_void<T, std::enable_if_t<
    is_one_of_v<T, void,
                   void const,
                   void volatile,
                   void const volatile>> std::true_type { };
#else
template <typename T>
struct is_trivially_move_constructible_or_void
    : std::bool_constant<std::is_trivially_move_constructible_v<T> ||
                         std::is_void_v<T>> { };
#endif

template <typename T>
inline bool constexpr is_trivially_move_constructible_or_void_v =
        is_trivially_move_constructible_or_void<T>::value;

template <typename T>
struct is_nothrow_move_constructible_or_void
    : std::bool_constant<std::is_nothrow_move_constructible_v<T> ||
                         std::is_void_v<T>> { };

template <typename T>
inline bool constexpr is_nothrow_move_constructible_or_void_v =
    is_nothrow_move_constructible_or_void<T>::value;

template <typename T, typename... Args>
struct enable_if_constructible : std::enable_if<std::is_constructible_v<T,Args...>> { };

template <typename T, typename... Args>
using enable_if_constructible_t = typename enable_if_constructible<T,Args...>::type;

template <typename T, typename U>
struct enable_if_convertible : std::enable_if<std::is_convertible_v<T,U>> { };

template <typename T, typename U>
using enable_if_convertible_t = typename enable_if_convertible<T,U>::type;

/* True iff T can be constructed from Args... but Args... cannot be converted to T */
template <typename T, typename... Args>
struct enable_if_constructible_but_not_convertible
    : std::enable_if<std::is_constructible_v<T,Args...> &&
                    !std::is_convertible_v<Args...,T>> { };

template <typename T, typename... Args>
using enable_if_constructible_but_not_convertible_t =
    typename enable_if_constructible_but_not_convertible<T,Args...>::type;

template <typename T, typename... Args>
struct enable_if_nothrow_constructible
    : std::enable_if<std::is_nothrow_constructible_v<T, Args...>> { };

template <typename T, typename... Args>
using enable_if_nothrow_constructible_t =
    typename enable_if_nothrow_constructible<T, Args...>::type;

template <typename... Args>
struct enable_if_empty_pack
    : std::enable_if<sizeof...(Args) == 0> { };

template <typename... Args>
using enable_if_empty_pack_t = typename enable_if_empty_pack<Args...>::type;

template <typename T>
struct enable_if_swappable
    : std::enable_if<std::is_swappable_v<T>> { };

template <typename T>
using enable_if_swappable_t = typename enable_if_swappable<T>::type;

template <typename, typename = void>
struct has_internal_swap : std::false_type { };

template <typename T>
struct has_internal_swap<T, std::enable_if_t<std::is_void_v<decltype(std::declval<T&>().swap(std::declval<T&>()))>>>
    : std::true_type { };

template <typename T>
inline bool constexpr has_internal_swap_v = has_internal_swap<T>::value;

template <typename T>
struct enable_if_internally_swappable
    : std::enable_if<has_internal_swap_v<T>> { };

template <typename T>
using enable_if_internally_swappable_t =
    typename enable_if_internally_swappable<T>::type;

template <typename T, typename E, typename U, typename G>
struct expected_copy_conversion_available
    : std::bool_constant< std::is_constructible_v<T, U const&> &&
                         !std::is_constructible_v<T, expected<U,G>&> &&
                         !std::is_constructible_v<T, expected<U,G>&&> &&
                         !std::is_constructible_v<T, expected<U,G> const&> &&
                         !std::is_constructible_v<T, expected<U,G> const&&> &&
                         !std::is_convertible_v<expected<U,G>&,T> &&
                         !std::is_convertible_v<expected<U,G>&&, T> &&
                         !std::is_convertible_v<expected<U,G> const&, T> &&
                         !std::is_convertible_v<expected<U,G> const&&, T> &&
                          std::is_constructible_v<E, G const&> &&
                         !std::is_constructible_v<unexpected<E>, expected<U,G>&> &&
                         !std::is_constructible_v<unexpected<E>, expected<U,G>&&> &&
                         !std::is_constructible_v<unexpected<E>, expected<U,G> const&> &&
                         !std::is_constructible_v<unexpected<E>, expected<U,G> const&&> &&
                         !std::is_convertible_v<expected<U,G>&, unexpected<E>> &&
                         !std::is_convertible_v<expected<U,G>&&, unexpected<E>> &&
                         !std::is_convertible_v<expected<U,G> const&, unexpected<E>> &&
                         !std::is_convertible_v<expected<U,G> const&&, unexpected<E>>> { };

template <typename E, typename G>
struct expected_copy_conversion_available<void, E, void, G> : std::true_type { };

template <typename T, typename E, typename U, typename G>
inline bool constexpr expected_copy_conversion_available_v =
    expected_copy_conversion_available<T,E,U,G>::value;

template <typename T, typename E, typename U, typename G>
struct expected_implicit_copy_conversion_available
    : std::bool_constant<std::is_convertible_v<U const&, T> &&
                         std::is_convertible_v<G const&, E>> { };

template <typename E, typename G>
struct expected_implicit_copy_conversion_available<void, E, void, G>
    : std::bool_constant<std::is_convertible_v<G const&, E>> { };

template <typename T, typename E, typename U, typename G>
inline bool constexpr expected_implicit_copy_conversion_available_v =
    expected_implicit_copy_conversion_available<T,E,U,G>::value;

template <typename T, typename E, typename U, typename G>
struct expected_enable_copy_conversion
    : std::enable_if<expected_copy_conversion_available_v<T,E,U,G>> { };

template <typename T, typename E, typename U, typename G>
using expected_enable_copy_conversion_t =
    typename expected_enable_copy_conversion<T,E,U,G>::type;

template <typename T, typename E, typename U, typename G>
struct expected_enable_implicit_copy_conversion
    : std::enable_if<expected_implicit_copy_conversion_available_v<T,E,U,G>> { };

template <typename T, typename E, typename U, typename G>
using expected_enable_implicit_copy_conversion_t =
    typename expected_enable_implicit_copy_conversion<T,E,U,G>::type;

template <typename T, typename E, typename U, typename G>
struct expected_enable_explicit_copy_conversion
    : std::enable_if<expected_copy_conversion_available_v<T,E,U,G> &&
                    !expected_implicit_copy_conversion_available_v<T,E,U,G>> { };

template <typename T, typename E, typename U, typename G>
using expected_enable_explicit_copy_conversion_t =
    typename expected_enable_explicit_copy_conversion<T,E,U,G>::type;

template <typename T, typename E, typename U, typename G>
struct expected_move_conversion_available
    : std::bool_constant< std::is_constructible_v<T, U&&> &&
                         !std::is_constructible_v<T, expected<U,G>&> &&
                         !std::is_constructible_v<T, expected<U,G>&&> &&
                         !std::is_constructible_v<T, expected<U,G> const&> &&
                         !std::is_constructible_v<T, expected<U,G> const&&> &&
                         !std::is_convertible_v<expected<U,G>&,T> &&
                         !std::is_convertible_v<expected<U,G>&&, T> &&
                         !std::is_convertible_v<expected<U,G> const&, T> &&
                         !std::is_convertible_v<expected<U,G> const&&, T> &&
                          std::is_constructible_v<E, G&&> &&
                         !std::is_constructible_v<unexpected<E>, expected<U,G>&> &&
                         !std::is_constructible_v<unexpected<E>, expected<U,G>&&> &&
                         !std::is_constructible_v<unexpected<E>, expected<U,G> const&> &&
                         !std::is_constructible_v<unexpected<E>, expected<U,G> const&&> &&
                         !std::is_convertible_v<expected<U,G>&, unexpected<E>> &&
                         !std::is_convertible_v<expected<U,G>&&, unexpected<E>> &&
                         !std::is_convertible_v<expected<U,G> const&, unexpected<E>> &&
                         !std::is_convertible_v<expected<U,G> const&&, unexpected<E>>> { };

template <typename E, typename G>
struct expected_move_conversion_available<void, E, void, G> : std::true_type { };

template <typename T, typename E, typename U, typename G>
inline bool constexpr expected_move_conversion_available_v =
    expected_move_conversion_available<T,E,U,G>::value;

/* Use for enabling non-explicit move constructor */
template <typename T, typename E, typename U, typename G>
struct expected_implicit_move_conversion_available
    : std::bool_constant<std::is_convertible_v<U&&, T> &&
                         std::is_convertible_v<G&&, E>> { };

template <typename E, typename G>
struct expected_implicit_move_conversion_available<void, E, void, G>
    : std::bool_constant<std::is_convertible_v<G&&, E>> { };

template <typename T, typename E, typename U, typename G>
inline bool constexpr expected_implicit_move_conversion_available_v =
    expected_implicit_move_conversion_available<T,E,U,G>::value;

template <typename T, typename E, typename U, typename G>
struct expected_enable_move_conversion
    : std::enable_if<expected_move_conversion_available_v<T,E,U,G>> { };

template <typename T, typename E, typename U, typename G>
using expected_enable_move_conversion_t =
    typename expected_enable_move_conversion<T,E,U,G>::type;

template <typename T, typename E, typename U, typename G>
struct expected_enable_implicit_move_conversion
    : std::enable_if<expected_implicit_move_conversion_available_v<T,E,U,G>> { };

template <typename T, typename E, typename U, typename G>
using expected_enable_implicit_move_conversion_t =
    typename expected_enable_implicit_move_conversion<T,E,U,G>::type;

template <typename T, typename E, typename U, typename G>
struct expected_enable_explicit_move_conversion
    : std::enable_if<expected_move_conversion_available_v<T,E,U,G> &&
                    !expected_implicit_move_conversion_available_v<T,E,U,G>> { };

template <typename T, typename E, typename U, typename G>
using expected_enable_explicit_move_conversion_t =
    typename expected_enable_explicit_move_conversion<T,E,U,G>::type;

template <typename T, typename E, typename U>
struct expected_forwarding_ref_ctor_available
    : std::bool_constant<std::is_constructible_v<T, U&&> &&
                        !std::is_same_v<remove_cvref_t<U>, in_place_t> &&
                        !std::is_same_v<expected<T, E>, remove_cvref_t<U>> &&
                        !std::is_same_v<unexpected<E>, remove_cvref_t<U>>> { };

template <typename T, typename E, typename U>
inline bool constexpr expected_forwarding_ref_ctor_available_v =
    expected_forwarding_ref_ctor_available<T,E,U>::value;

template <typename T, typename U>
struct expected_implicit_forwarding_ref_ctor_available
    : std::bool_constant<std::is_convertible_v<U&&, T>> { };

template <typename T, typename U>
inline bool constexpr expected_implicit_forwarding_ref_ctor_available_v =
    expected_implicit_forwarding_ref_ctor_available<T,U>::value;

template <typename T, typename E, typename U>
struct expected_enable_forwarding_ref_ctor
    : std::enable_if<expected_forwarding_ref_ctor_available_v<T,E,U>> { };

template <typename T, typename E, typename U>
using expected_enable_forwarding_ref_ctor_t =
    typename expected_enable_forwarding_ref_ctor<T,E,U>::type;

template <typename T, typename U>
struct expected_enable_implicit_forwarding_ref_ctor
    : std::enable_if<expected_implicit_forwarding_ref_ctor_available_v<T,U>> { };

template <typename T, typename U>
using expected_enable_implicit_forwarding_ref_ctor_t =
    typename expected_enable_implicit_forwarding_ref_ctor<T,U>::type;

template <typename T, typename E, typename U>
struct expected_enable_explicit_forwarding_ref_ctor
    : std::enable_if<expected_forwarding_ref_ctor_available_v<T,E,U> &&
                    !expected_implicit_forwarding_ref_ctor_available_v<T,U>> { };

template <typename T, typename E, typename U>
using expected_enable_explicit_forwarding_ref_ctor_t =
    typename expected_enable_explicit_forwarding_ref_ctor<T,E,U>::type;

template <typename T, typename E>
struct expected_enable_copy_assignment
    : std::bool_constant<std::is_copy_assignable_v<T> &&
                         std::is_copy_constructible_v<T> &&
                         std::is_copy_assignable_v<E> &&
                         std::is_copy_constructible_v<E> &&
                        (std::is_nothrow_move_constructible_v<T> ||
                         std::is_nothrow_move_constructible_v<E>)> { };

template <typename E>
struct expected_enable_copy_assignment<void, E>
    : std::bool_constant<std::is_copy_assignable_v<E> &&
                         std::is_copy_constructible_v<E>> { };

template <typename T, typename E>
inline bool constexpr expected_enable_copy_assignment_v =
    expected_enable_copy_assignment<T,E>::value;

template <typename T, typename E>
struct expected_enable_move_assignment
    : std::bool_constant<std::is_move_constructible_v<T> &&
                         std::is_move_assignable_v<T> &&
                         std::is_nothrow_move_constructible_v<E> &&
                         std::is_nothrow_move_assignable_v<E>> { };

template <typename E>
struct expected_enable_move_assignment<void, E>
    : std::bool_constant<std::is_nothrow_move_constructible_v<E> &&
                         std::is_nothrow_move_assignable_v<E>> { };

template <typename T, typename E>
inline bool constexpr expected_enable_move_assignment_v =
    expected_enable_move_assignment<T,E>::value;

/* Tested only when T is not void, hence no void check */
template <typename T, typename E, typename U>
struct expected_unary_forwarding_assign_available
    : std::bool_constant<!std::is_same_v<expected<T,E>, remove_cvref_t<U>> &&
                         !std::conjunction_v<std::is_scalar<T>,
                                             std::is_same<T, std::decay_t<U>>> &&
                         std::is_constructible_v<T,U> &&
                         std::is_assignable_v<T&,U> &&
                         std::is_nothrow_move_constructible_v<E>> { };

template <typename T, typename E, typename U>
inline bool constexpr expected_unary_forwarding_assign_available_v =
    expected_unary_forwarding_assign_available<T,E,U>::value;

template <typename T, typename E, typename U>
struct expected_enable_unary_forwarding_assign
    : std::enable_if<expected_unary_forwarding_assign_available_v<T,E,U>> { };

template <typename T, typename E, typename U>
using expected_enable_unary_forwarding_assign_t =
    typename expected_enable_unary_forwarding_assign<T,E,U>::type;

template <typename T, typename E>
struct expected_swap_available
    : std::bool_constant<std::is_swappable_v<T&> &&
                         std::is_swappable_v<E&> &&
                         std::is_move_constructible_v<T> &&
                         std::is_move_constructible_v<E> &&
                        (std::is_nothrow_move_constructible_v<T> ||
                         std::is_nothrow_move_constructible_v<E>)> { };

template <typename E>
struct expected_swap_available<void,E>
    : std::is_swappable<E> { };

template <typename T, typename E>
inline bool constexpr expected_swap_available_v = expected_swap_available<T,E>::value;

template <typename T, typename E>
struct expected_enable_swap
    : std::enable_if<expected_swap_available_v<T,E>> { };

template <typename T, typename E>
using expected_enable_swap_t = typename expected_enable_swap<T,E>::type;

template <typename E>
struct expected_enable_unexpected_copy_assignment
    : std::enable_if<std::is_nothrow_copy_constructible_v<E> &&
                     std::is_copy_assignable_v<E>> { };

template <typename E>
using expected_enable_unexpected_copy_assignment_t =
    typename expected_enable_unexpected_copy_assignment<E>::type;

template <typename E>
struct expected_enable_unexpected_move_assignment
    : std::enable_if<std::is_nothrow_move_constructible_v<E> &&
                     std::is_move_assignable_v<E>> { };

template <typename E>
using expected_enable_unexpected_move_assignment_t =
    typename expected_enable_unexpected_move_assignment<E>::type;

template <typename E, typename Err>
struct unexpected_copy_conversion_available
    : std::bool_constant< std::is_constructible_v<E, Err const&> &&
                         !std::is_constructible_v<E, unexpected<Err>&> &&
                         !std::is_constructible_v<E, unexpected<Err>> &&
                         !std::is_constructible_v<E, unexpected<Err> const&> &&
                         !std::is_constructible_v<E, unexpected<Err> const> &&
                         !std::is_convertible_v<unexpected<Err>&, E> &&
                         !std::is_convertible_v<unexpected<Err>, E> &&
                         !std::is_convertible_v<unexpected<Err> const&, E> &&
                         !std::is_convertible_v<unexpected<Err> const, E>> { };

template <typename E, typename Err>
inline bool constexpr unexpected_copy_conversion_available_v
    = unexpected_copy_conversion_available<E,Err>::value;

template <typename E, typename Err>
struct unexpected_implicit_copy_conversion_available
    : std::is_convertible<Err const&, E> { };

template <typename E, typename Err>
inline bool constexpr unexpected_implicit_copy_conversion_available_v =
    unexpected_implicit_copy_conversion_available<E,Err>::value;

template <typename E, typename Err>
struct unexpected_enable_copy_conversion
    : std::enable_if<unexpected_copy_conversion_available_v<E,Err>> { };

template <typename E, typename Err>
using unexpected_enable_copy_conversion_t =
    typename unexpected_enable_copy_conversion<E,Err>::type;

template <typename E, typename Err>
struct unexpected_enable_implicit_copy_conversion
    : std::enable_if<unexpected_implicit_copy_conversion_available_v<E,Err>> { };

template <typename E, typename Err>
using unexpected_enable_implicit_copy_conversion_t =
    typename unexpected_enable_implicit_copy_conversion<E,Err>::type;

template <typename E, typename Err>
struct unexpected_enable_explicit_copy_conversion
    : std::enable_if<unexpected_copy_conversion_available_v<E,Err> &&
                    !unexpected_implicit_copy_conversion_available_v<E,Err>> { };

template <typename E, typename Err>
using unexpected_enable_explicit_copy_conversion_t =
    typename unexpected_enable_explicit_copy_conversion<E,Err>::type;

template <typename E, typename Err>
struct unexpected_move_conversion_available
    : std::bool_constant< std::is_constructible_v<E, Err> &&
                         !std::is_constructible_v<E, unexpected<Err>&> &&
                         !std::is_constructible_v<E, unexpected<Err>> &&
                         !std::is_constructible_v<E, unexpected<Err> const&> &&
                         !std::is_constructible_v<E, unexpected<Err> const> &&
                         !std::is_convertible_v<unexpected<Err>&, E> &&
                         !std::is_convertible_v<unexpected<Err>, E> &&
                         !std::is_convertible_v<unexpected<Err> const&, E> &&
                         !std::is_convertible_v<unexpected<Err> const, E>> { };

template <typename E, typename Err>
inline bool constexpr unexpected_move_conversion_available_v =
    unexpected_move_conversion_available<E,Err>::value;

template <typename E, typename Err>
struct unexpected_implicit_move_conversion_available
    : std::is_convertible<Err, E> { };

template <typename E, typename Err>
inline bool constexpr unexpected_implicit_move_conversion_available_v =
    unexpected_implicit_move_conversion_available<E,Err>::value;

template <typename E, typename Err>
struct unexpected_enable_move_conversion
    : std::enable_if<unexpected_move_conversion_available_v<E, Err>> { };

template <typename E, typename Err>
using unexpected_enable_move_conversion_t =
    typename unexpected_enable_move_conversion<E, Err>::type;

template <typename E, typename Err>
struct unexpected_enable_implicit_move_conversion
    : std::enable_if<unexpected_implicit_move_conversion_available_v<E, Err>> { };

template <typename E, typename Err>
using unexpected_enable_implicit_move_conversion_t
    = typename unexpected_enable_implicit_move_conversion<E, Err>::type;

template <typename E, typename Err>
struct unexpected_enable_explicit_move_conversion
    : std::enable_if<unexpected_move_conversion_available_v<E, Err> &&
                    !unexpected_implicit_move_conversion_available_v<E, Err>> { };

template <typename E, typename Err>
using unexpected_enable_explicit_move_conversion_t =
    typename unexpected_enable_explicit_move_conversion<E, Err>::type;



template <typename E, typename Err>
struct unexpected_copy_assignment_available
    : std::is_assignable<E, Err const&> { };

template <typename E, typename Err>
inline bool constexpr unexpected_copy_assignment_available_v =
    unexpected_copy_assignment_available<E,Err>::value;

template <typename E, typename Err>
struct unexpected_enable_copy_assignment
    : std::enable_if<unexpected_copy_assignment_available_v<E, Err>> { };

template <typename E, typename Err>
using unexpected_enable_copy_assignment_t =
    typename unexpected_enable_copy_assignment<E, Err>::type;

template <typename E, typename Err>
struct unexpected_move_assignment_available
    : std::is_assignable<E, Err> { };

template <typename E, typename Err>
inline bool constexpr unexpected_move_assignment_available_v =
    unexpected_move_assignment_available<E,Err>::value;

template <typename E, typename Err>
struct unexpected_enable_move_assignment
    : std::enable_if<unexpected_move_assignment_available_v<E, Err>> { };

template <typename E, typename Err>
using unexpected_enable_move_assignment_t =
    typename unexpected_enable_move_assignment<E, Err>::type;


#ifdef VIEN_EXPECTED_EXTENDED
inline constexpr in_place_t in_place{};

template <typename T>
struct type_is {
    using type = T;
};

/* Type returned by expected<T,E>::map */
template <typename T, typename E, typename F>
struct expected_mapped_type
    : type_is<expected<std::decay_t<std::invoke_result_t<F, T>>, E>>
{ };

/* Type returned by expected<void,E>::map */
template <typename E, typename F>
struct expected_mapped_type<void, E, F>
    : type_is<expected<std::decay_t<std::invoke_result_t<F>>, E>> { };

template <typename T, typename E, typename F>
using expected_mapped_type_t = typename expected_mapped_type<T,E,F>::type;

/* Type returned by expected<T,E>::map_error. T may be void, E may not */
template <typename T, typename E, typename F>
struct expected_mapped_error_type
    : type_is<expected<T, std::decay_t<std::invoke_result_t<F,E>>>> { };

template <typename T, typename E, typename F>
using expected_mapped_error_type_t = typename expected_mapped_error_type<T,E,F>::type;

/* Check if type is an allocator */
template <typename, typename = void>
struct is_allocator : std::false_type { };

/* true iff T has allocate and deallocate functions with
 * correct signatures */
template <typename T>
struct is_allocator<T, std::void_t<
    decltype(std::declval<T&>().allocate(std::declval<std::size_t>())),
    decltype(std::declval<T&>().deallocate(
        std::declval<decltype(std::declval<T&>().allocate(std::declval<std::size_t>()))>(),
        std::declval<std::size_t>())
    )>>
    : std::true_type { };

template <typename T>
inline bool constexpr is_allocator_v = is_allocator<T>::value;

/* Check if type is a comparator */
template <typename, typename = void>
struct is_comparator : std::false_type { };

/* true iff has an operator() taking two T refs and returns bool */
template <template <typename> class C, typename T>
struct is_comparator<C<T>, std::enable_if_t<
    std::is_same_v<bool, decltype(std::declval<C<T>>()(std::declval<T&>(), std::declval<T&>()))>
>> : std::true_type { };

template <typename T>
inline bool constexpr is_comparator_v = is_comparator<T>::value;

/* Check if type is a hashing type */
template <typename, typename = void>
struct is_hash : std::false_type { };

/* H<T> is deemed a hashing type iff it
 * - names an operator() taking a single instance of T and returns an std::size_t,
 * - is default_constructible
 * - is copy constructible
 * - is destructible
 * - is swappable */
template <template <typename> class H, typename T>
struct is_hash<H<T>,
    std::enable_if_t<
        std::is_same_v<std::size_t, decltype(
            std::declval<H<T>>().operator()(std::declval<T>()))
         > &&
         std::is_default_constructible_v<H<T>> &&
         std::is_copy_constructible_v<H<T>> &&
         std::is_destructible_v<H<T>> &&
         std::is_swappable_v<H<T>>
    >>
    : std::true_type { };

template <typename T>
inline bool constexpr is_hash_v = is_hash<T>::value;

template <typename, typename = void>
struct is_char_traits : std::false_type { };

/* T is char traits if it names the 5 char trait aliases */
template <typename T>
struct is_char_traits<T, std::void_t<typename T::char_type,
                                    typename T::int_type,
                                    typename T::off_type,
                                    typename T::pos_type,
                                    typename T::state_type>>
    : std::true_type { };

template <typename T>
inline bool constexpr is_char_traits_v = is_char_traits<T>::value;

/* Check whether a container is associative */
template <typename, typename = void>
struct is_associative : std::false_type { };

/* Deemed associative if it names nested key_type and mapped_type aliases */
template <typename Container>
struct is_associative<Container, std::void_t<typename Container::key_type,
                                             typename Container::mapped_type>>
    : std::true_type { };

template <typename Container>
inline bool constexpr is_associative_v = is_associative<Container>::value;

template <typename>
struct is_pair : std::false_type { };

template <typename K, typename M>
struct is_pair<std::pair<K,M>> : std::true_type { };

template <typename T>
inline bool constexpr is_pair_v = is_pair<T>::value;

template <typename, typename = void>
struct is_char_type : std::false_type { };

template <typename T>
struct is_char_type<T, std::enable_if_t<
    is_one_of_v<remove_cvref_t<T>, char, wchar_t, char16_t, char32_t>
>> : std::true_type { };

template <typename T>
inline bool constexpr is_char_type_v = is_char_type<T>::value;

template <typename, typename = void>
struct is_char_string : std::false_type { };

/* char string if T names a traits_type and its value_type is a char type */
template <typename T>
struct is_char_string<T,
    std::void_t<typename T::traits_type,
        std::enable_if_t<is_char_type_v<typename T::value_type>>
    >>
    : std::true_type { };

template <typename T>
inline bool constexpr is_char_string_v = is_char_string<T>::value;

/* Get value_type of T, if available. Otherwise, return T */
template <typename T, typename = void>
struct value_type_of : type_is<T> { };

template <typename T>
struct value_type_of<T, std::void_t<typename T::value_type>>
    : type_is<typename T::value_type> { };

template <typename T>
using value_type_of_t = typename value_type_of<T>::type;

/* Rebind unary type argument template */
template <typename, typename>
struct rebind_unary_template;

/* Rebind Template<From> to Template<To> */
template <template <typename> class Template, typename From , typename To>
struct rebind_unary_template<Template<From>, To> : type_is<Template<To>> { };

template <typename Template, typename T>
using rebind_unary_template_t = typename rebind_unary_template<Template, T>::type;

/* If A is an allocator, rebind its value_type to T, otherwise,
 * do nothing */
template <typename A, typename, typename = void>
struct rebind_if_alloc : type_is<A> { };

template <typename A, typename T>
struct rebind_if_alloc<A, T, std::enable_if_t<is_allocator_v<A>>>
    : type_is<typename std::allocator_traits<A>::template rebind_alloc<T>> { };

template <typename A, typename T>
using rebind_if_alloc_t = typename rebind_if_alloc<A,T>::type;

/* If C is a comparator, rebind it to compare T, otherwise, do nothing */
template <typename C, typename, typename = void>
struct rebind_if_comparator : type_is<C> { };

template <typename C, typename T>
struct rebind_if_comparator<C, T, std::enable_if_t<is_comparator_v<C>>>
    : type_is<rebind_unary_template_t<C,T>> { };

template <typename C, typename T>
using rebind_if_comparator_t = typename rebind_if_comparator<C,T>::type;

/* If H is generated from std::hash, rebind its "value type" to T */
template <typename H, typename, typename = void>
struct rebind_if_hash : type_is<H> { };

template <typename H, typename T>
struct rebind_if_hash<H, T, std::enable_if_t<is_hash_v<H>>>
    : type_is<rebind_unary_template_t<H,T>> { };

template <typename C, typename T>
using rebind_if_hash_t = typename rebind_if_hash<C,T>::type;

template <typename CT, typename, typename = void>
struct rebind_if_char_traits : type_is<CT> { };

template <typename CT, typename T>
struct rebind_if_char_traits<CT, T, std::enable_if_t<is_char_traits_v<CT>>>
    :type_is<rebind_unary_template_t<CT,T>> { };

template <typename CT, typename T>
using rebind_if_char_traits_t = typename rebind_if_char_traits<CT,T>::type;

/* Rebind container value_type, allocator_type, compare and
 * hasher typedefs */
template <typename Container, typename,
          bool = is_associative_v<Container>,
          bool = is_char_string_v<Container>>
struct rebind;

/* Container is a non-associative template,
 * rebind value_type from F to T,
 * allocator<F> (if any) to allocator<T>,
 * compare<F> (if any) to compare<T>, and
 * hasher<F> (if any) to hasher<T>*/
template <template <typename, typename...> class Container,
          typename F, typename T,
          typename... Args>
struct rebind<Container<F, Args...>, T, false, false>
    : type_is<
        Container<T,
            rebind_if_char_traits_t<
                rebind_if_hash_t<
                    rebind_if_comparator_t<
                        rebind_if_alloc_t<Args, T>,
                     T>,
                T>,
            T>
        ...>
      >
{
    static_assert(!std::is_void_v<T>, "Cannot bind value_type to void");
};

/* Container is a standard array, rebind value_type from F to T
 * (size remains the same) */
template <typename F, std::size_t N, typename T>
struct rebind<std::array<F,N>, T, false, false>
    : type_is<std::array<T,N>> {
    static_assert(!std::is_void_v<T>, "Cannot bind value_type to void");
};

/* Container is associative and new type is an std::pair<K2,M2>,
 * rebind key_type to K2,
 * mapped_type to M2,
 * allocator<std::pair<K1 const, M1>> (if any) to allocator<std::pair<K2 const, M2>>,
 * comparator<K1> (if any) to comparator<K2>,
 * hasher<K1> (if any) to hasher<K2> */
template <template <typename, typename, typename...> class Container,
          typename K1, typename M1,
          typename K2, typename M2,
          typename... Args>
struct rebind<Container<K1, M1, Args...>, std::pair<K2, M2>, true, false>
    : type_is<
        Container<std::remove_cv_t<K2>, M2,
            rebind_if_hash_t<
                rebind_if_comparator_t<
                    rebind_if_alloc_t<Args, std::pair<std::remove_cv_t<K2> const, M2>>,
                    std::remove_cv_t<K2>>,
                std::remove_cv_t<K2>>
            ...>
        >
{
    static_assert(!std::is_void_v<K2>, "Cannot bind key_type to void");
    static_assert(!std::is_void_v<M2>, "Cannot bind mapped_type to void");
};

/* Container is associative and new type is a non-pair T,
 * rebind key_type to K (original key type),
 * mapped_type to T,
 * allocator<std::pair<K const, M>>> (if any) to allocator<std::pair<K const, T>>,
 * comparator<K> (if any) to comparator<K> (i.e. no change),
 * hasher<K> (if any) to hasher<K> (i.e. no change) */
template <template <typename, typename, typename...> class Container,
          typename K, typename M, typename T,
          typename... Args>
struct rebind<Container<K, M, Args...>, T, true, false>
    : type_is<
        Container<std::remove_cv_t<K>, T,
            rebind_if_hash_t<
                rebind_if_comparator_t<
                    rebind_if_alloc_t<Args, std::pair<std::remove_cv_t<K> const, T>>,
                    std::remove_cv_t<K>>,
               std::remove_cv_t<K>>
            ...>
        >
{
    static_assert(!std::is_void_v<T>, "Cannot bind mapped_type to void");
};

/* Don't rebind char strings (doing so would cause issues with
 * string manipulation functions that don't return chars, e.g.
 * std::toupper) */
template <typename String, typename T, bool B>
struct rebind<String, T, B, true>
    : type_is<String> { };

template <typename C, typename T>
using rebind_t = typename rebind<C,T>::type;

/* Convenience meta function for invoking rebind. Calls rebind_t
 * with T and decayed return type of invoking an instance of F with
 * an instance of T */
template <typename T, typename F>
struct rebind_container
    : type_is<rebind_t<T,
          std::decay_t<std::invoke_result_t<F, value_type_of_t<T>>>>> { };

template <typename T, typename F>
using rebind_container_t = typename rebind_container<T,F>::type;


/* Check if T is a container */
template <typename T, typename = void>
struct is_container : std::false_type { };

/* True iff std::begin and std::end are overloaded for Container */
template <typename T>
struct is_container<T, std::void_t<decltype(std::begin(std::declval<T&>())),
                                   decltype(std::end(std::declval<T&>()))>>
    : std::true_type { };

template <typename T>
inline bool constexpr is_container_v = is_container<T>::value;

template <typename T>
struct enable_if_container : std::enable_if<is_container_v<T>> { };

template <typename T>
using enable_if_container_t = typename enable_if_container<T>::type;

template <typename, typename = void>
struct names_unary_push_back : std::false_type { };

template <typename T>
struct names_unary_push_back<T,
    std::void_t<decltype(std::declval<T>().push_back(std::declval<typename T::value_type>()))>>
    : std::true_type { };

template <typename T>
inline bool constexpr names_unary_push_back_v = names_unary_push_back<T>::value;

template <typename, typename = void>
struct names_unary_push_front : std::false_type { };

template <typename T>
struct names_unary_push_front<T,
    std::void_t<decltype(std::declval<T>().push_front(std::declval<typename T::value_type>()))>>
    : std::true_type { };

template <typename T>
inline bool constexpr names_unary_push_front_v = names_unary_push_front<T>::value;

template <typename, typename = void>
struct names_unary_insert : std::false_type { };

template <typename T>
struct names_unary_insert<T,
    std::void_t<decltype(std::declval<T>().insert(std::declval<typename T::value_type>()))>>
    : std::true_type { };

template <typename T>
inline bool constexpr names_unary_insert_v = names_unary_insert<T>::value;

template <typename, typename = void>
struct supports_preallocation : std::false_type { };

template <typename T>
struct supports_preallocation<T, std::void_t<decltype(std::declval<T>().reserve(std::declval<std::size_t>()))>>
    : std::true_type { };

template <typename T>
inline bool constexpr supports_preallocation_v = supports_preallocation<T>::value;

/* Output iterator for types whose only means
 * of insertion are through call to unary insert */
template <typename Container>
class ordered_insert_iterator {
    public:
        using iterator_category = std::output_iterator_tag;
        using value_type = void;
        using difference_type = void;
        using pointer = void;
        using reference = void;
        using container_type = Container;

        constexpr ordered_insert_iterator() noexcept = default;
        constexpr ordered_insert_iterator(Container& c)
            : ctr_{std::addressof(c)} { }

        constexpr ordered_insert_iterator&
            operator=(typename Container::value_type const& v) {
            ctr_->insert(v);
            return *this;
        }

        constexpr ordered_insert_iterator&
            operator=(typename Container::value_type && v) {
            ctr_->insert(std::move(v));
            return *this;
        }

        constexpr ordered_insert_iterator& operator*() {
            return *this;
        }

        constexpr ordered_insert_iterator& operator++() {
            return *this;
        }

        constexpr ordered_insert_iterator operator++(int) {
            auto cpy = *this;
            return cpy;
        }

    private:
        Container* ctr_;
};


/* Convenience class template whos operator() constructs
 * an ouput iterator appropriate for the container */
template <typename Container, typename = void>
struct universal_inserter;

/* Array, use begin */
template <typename T, std::size_t N>
struct universal_inserter<std::array<T,N>> {
    constexpr typename std::array<T,N>::iterator operator()(std::array<T,N>& c) {
        return std::begin(c);
    }
};

/* Container has push_back, use std::back_insert_iterator */
template <typename Container>
struct universal_inserter<Container, std::enable_if_t<names_unary_push_back_v<Container>>> {
    constexpr std::back_insert_iterator<Container> operator()(Container& c) {
        return std::back_insert_iterator<Container>(c);
    }
};

/* Container has no push_back but has push_front, use std::front_insert_iterator */
template <typename Container>
struct universal_inserter<Container, std::enable_if_t<!names_unary_push_back_v<Container> &&
                                                       names_unary_push_front_v<Container>>> {
    constexpr std::front_insert_iterator<Container> operator()(Container& c) {
        return std::front_insert_iterator<Container>(c);
    }
};

/* Container has neither push_back nor push_front but does have unary insert, use
 * ordered_insert_iterator */
template <typename Container>
struct universal_inserter<Container, std::enable_if_t<!names_unary_push_back_v<Container> &&
                                                       !names_unary_push_front_v<Container> &&
                                                        names_unary_insert_v<Container>>> {
    constexpr ordered_insert_iterator<Container> operator()(Container& c) {
        return ordered_insert_iterator<Container>(c);
    }
};

/* Convert instance of SrcContainer to DstContainer */
template <typename SrcContainer, typename DstContainer, typename F, typename FRet,
          bool = is_associative_v<SrcContainer>,
          bool = is_pair_v<FRet>>
struct convert {
    /* Use when std::is_same_v<SrcContainer, DstContainer> is false */
    constexpr DstContainer operator()(SrcContainer& src, F&& f) const {
        DstContainer dst;
        if constexpr(supports_preallocation_v<DstContainer>)
            dst.reserve(src.size());

        std::transform(std::begin(src), std::end(src),
                       universal_inserter<DstContainer>{}(dst),
                       std::forward<F>(f));
        return dst;
    };

    /* Use in rvalue overload when std::is_same_v<SrcContainer, DstContainer> is true */
    constexpr void operator()(in_place_t, SrcContainer& src, F&& f) const {
        std::transform(std::begin(src), std::end(src),
                       std::begin(src), std::forward<F>(f));
    };
};

/* Partial specialization for when the containers are associative but the return
 * type of callable F is not a pair. Map the return type to the mapped_type in the
 * associative container */
template <typename SrcContainer, typename DstContainer, typename F, typename FRet>
struct convert<SrcContainer, DstContainer, F, FRet, true, false> {
    /* Use when std::is_same_v<SrcContainer, DstContainer> == false */
    constexpr DstContainer operator()(SrcContainer& src, F&& f) const {
        DstContainer dst;
        if constexpr(supports_preallocation_v<DstContainer>)
            dst.reserve(src.size());

        std::transform(std::begin(src), std::end(src),
                       universal_inserter<DstContainer>{}(dst),
                       [&f](auto&& pair) {
            auto const key = pair.first;
            return std::make_pair(key, std::invoke(std::forward<F>(f), std::forward<decltype(pair)>(pair)));
        });

        return dst;
    };

    /* Use in rvalue overload when std::is_same_v<SrcContainer, DstContainer> is true */
    constexpr void operator()(in_place_t, SrcContainer& src, F&& f) const {
        std::transform(std::begin(src), std::end(src),
                       std::begin(src),
                       [&f](auto&& pair) {
            auto const key = pair.first;
            return std::make_pair(key, std::invoke(std::forward<F>(f), std::forward<decltype(pair)>(pair)));
        });
    };
};

/* Partial specialization for when SrcContainer and DstContainer are standard arrays.
 * Must be handled separately to support non-default constructible types */
template <typename T1, typename T2, std::size_t N, typename F, typename FRet, bool B>
struct convert<std::array<T1,N>, std::array<T2,N>, F, FRet, false, B> {
    /* Use when std::is_same_v<T1,T2> == false */
    constexpr std::array<T2,N> operator()(std::array<T1,N>& src, F&& f) const {
        return construct(src, std::forward<F>(f), std::make_index_sequence<N>{});
    }

    /* Use in rvalue overload when std::is_same_v<T1,T2> == true */
    constexpr void operator()(in_place_t, std::array<T1,N>& src, F&& f) const {
        std::transform(std::begin(src), std::end(src),
                       std::begin(src), std::forward<F>(f));
    }

    /* Create the dst array by invoking f. This avoids default constructing the elements */
    template <std::size_t... Is>
    static constexpr std::array<T2,N> construct(std::array<T1,N> const& src, F&& f, std::index_sequence<Is...>) {
        return {std::invoke(std::forward<F>(f), src[Is])...};
    }
};

#endif

struct no_init_t {
    explicit no_init_t() = default;
};

inline no_init_t constexpr no_init{};

/* expected hierarchy */

/* expected_base */
/* Provides constexpr default constructor, destructor
 * and internal storage */

template <typename T, typename E,
          bool = std::is_trivially_destructible_v<T>,
          bool = std::is_trivially_destructible_v<E>>
struct expected_base;

/* Both T and E are trivially destructible, expected should be
 * trivially destructible */
template <typename T, typename E>
struct expected_base<T, E, true, true> {

    constexpr expected_base() : has_val_(true), val_(T()) { }
    constexpr expected_base(no_init_t) : has_val_{false} { }

    ~expected_base() = default;

    bool has_val_;
    union {
        T val_;
        unexpected<E> unexpect_;
    };
};

/* T not trivially destructible and E trivially destructible,
 * must call T's destructor explicitly */
template <typename T, typename E>
struct expected_base<T, E, false, true> {

    constexpr expected_base() : has_val_(true), val_(T()) { }
    constexpr expected_base(no_init_t) : has_val_{false} { }

    ~expected_base() {
        if(has_val_)
            val_.~T();
    }

    bool has_val_;
    union {
        T val_;
        unexpected<E> unexpect_;
    };
};

/* T trivially destructible and E not trivially destructible,
 * must call E's destructor explicitly */
template <typename T, typename E>
struct expected_base<T, E, true, false> {

    constexpr expected_base() : has_val_(true), val_(T()) { }
    constexpr expected_base(no_init_t) : has_val_{false} { }

    ~expected_base() {
        if(!has_val_)
            unexpect_.~unexpected<E>();
    }

    bool has_val_;
    union {
        T val_;
        unexpected<E> unexpect_;
    };
};

/* Neither T nor E trivially destructible, must call
 * both T's and E's destructors explicitly */
template <typename T, typename E>
struct expected_base<T, E, false, false> {

    constexpr expected_base() : has_val_(true), val_(T()) { }
    constexpr expected_base(no_init_t) : has_val_{false} { }

    ~expected_base() {
        if(has_val_)
            val_.~T();
        else
            unexpect_.~unexpected<E>();
    }

    bool has_val_;
    union {
        T val_;
        unexpected<E> unexpect_;
    };
};


/* The below are partial specialilzations for where T is void, use
 * dummy type in union. */

/* T is void, E trivially destructible. expected should be
 * trivially destructible */
template <typename E>
struct expected_base<void, E, false, true> {

    constexpr expected_base() : has_val_(true) { }
    constexpr expected_base(no_init_t) : has_val_{false} { }

    ~expected_base() = default;

    bool has_val_;
    struct none{};
    union {
        none val_;
        unexpected<E> unexpect_;
    };
};

/* T is void and E not trivially destructible,
 * must call E's destructor explicitly */
template <typename E>
struct expected_base<void, E, false, false> {

    constexpr expected_base() : has_val_(true) { }
    constexpr expected_base(no_init_t) : has_val_{false} { }

    ~expected_base() {
        if(!has_val_)
            unexpect_.~unexpected<E>();
    }

    bool has_val_;
    struct none{};
    union {
        none val_;
        unexpected<E> unexpect_;
    };
};

/* expected_construction_base */
/* Provides utility functions for placement newing
 * T or unexpected<E> into *this */
template <typename T, typename E>
struct expected_construction_base : expected_base<T,E> {
    using expected_base<T,E>::expected_base;
    constexpr expected_construction_base() = default;

    template <typename... Args>
    constexpr void store_val(Args&&... args) {
        new(std::addressof(this->val_)) T(std::forward<Args>(args)...);
        this->has_val_ = true;
    }

    template <typename... Args>
    constexpr void store_unexpect(Args&&... args) {
        new (std::addressof(this->unexpect_)) unexpected<E>(std::forward<Args>(args)...);
        this->has_val_ = false;
    }

    constexpr T& internal_get_value() & {
        return this->val_;
    }

    constexpr T const& internal_get_value() const & {
        return this->val_;
    }

    constexpr T&& internal_get_value() && {
        return std::move(this->val_);
    }

    constexpr T const&& internal_get_value() const && {
        return std::move(this->val_);
    }

    constexpr unexpected<E>& internal_get_unexpect() & {
        return this->unexpect_;
    }

    constexpr unexpected<E> const& internal_get_unexpect() const & {
        return this->unexpect_;
    }

    constexpr unexpected<E>&& internal_get_unexpect() && {
        return std::move(this->unexpect_);
    }

    constexpr unexpected<E> const&& internal_get_unexpect() const && {
        return std::move(this->unexpect_);
    }

    constexpr bool has_value() const noexcept {
        return this->has_val_;
    }
};

/* T is void. Provides utility functions for placement newing
 * void or unexpected<E> into *this */
template <typename E>
struct expected_construction_base<void, E> : expected_base<void,E> {
    using expected_base<void,E>::expected_base;
    constexpr expected_construction_base() = default;

    template <typename... Args>
    constexpr void store_val(Args&&...) noexcept {
        this->has_val_ = true;
    }

    template <typename... Args>
    constexpr void store_unexpect(Args&&... args) {
        new (std::addressof(this->unexpect_)) unexpected<E>(std::forward<Args>(args)...);
        this->has_val_ = false;
    }

    constexpr unexpected<E>& internal_get_unexpect() & {
        return this->unexpect_;
    }

    constexpr unexpected<E> const& internal_get_unexpect() const & {
        return this->unexpect_;
    }

    constexpr unexpected<E>&& internal_get_unexpect() && {
        return std::move(this->unexpect_);
    }

    constexpr unexpected<E> const&& internal_get_unexpect() const && {
        return std::move(this->unexpect_);
    }

    constexpr bool has_value() const noexcept {
        return this->has_val_;
    }
};

/* expected_default_ctor_base */
/* expected should be default constructible if T is default constructible
 * or is void */
template <typename T, typename E,
    bool = is_default_constructible_or_void_v<T>>
struct expected_default_ctor_base : expected_construction_base<T,E> {
    using expected_construction_base<T,E>::expected_construction_base;
    expected_default_ctor_base() = default;
};

/* Delete default ctor iff T is neither default constructible nor void */
template <typename T, typename E>
struct expected_default_ctor_base<T,E,false> : expected_construction_base<T,E> {
    using expected_construction_base<T,E>::expected_construction_base;
    expected_default_ctor_base() = delete;
};

/* expected_copy_ctor_base */
/* Delete copy ctor if T is neither copy constructible nor void
 * or E is not copy constructible. Make the copy constructor
 * constexpr if T is trivially copy constructible or void, and
 * E is trivially copy constructible */
template <typename T, typename E,
          bool = is_copy_constructible_or_void_v<T> &&
                 std::is_copy_constructible_v<E>,
          bool = is_trivially_copy_constructible_or_void_v<T> &&
                 std::is_trivially_copy_constructible_v<E>>
struct expected_copy_ctor_base;

/* Partial specializationf for when:
 * T is not copy constructible or void, and/or E is not copy
 * constructible, delete copy ctor */
template <typename T, typename E>
struct expected_copy_ctor_base<T, E, false, false>
    : expected_default_ctor_base<T,E> {
    using expected_default_ctor_base<T,E>::expected_default_ctor_base;
    expected_copy_ctor_base() = default;

    expected_copy_ctor_base(expected_copy_ctor_base const&) = delete;
};

/* Partial specialization for when
 * T is copy constructible or void and E is copy constructible
 * T is not trivially copy constructible or void, and/or E is not trivially
 * copy constructible. expected should be non-constexpr copy constructible */
template <typename T, typename E>
struct expected_copy_ctor_base<T, E, true, false>
    : expected_default_ctor_base<T,E> {
    using expected_default_ctor_base<T,E>::expected_default_ctor_base;
    expected_copy_ctor_base() = default;

    expected_copy_ctor_base(expected_copy_ctor_base const& rhs) {
        if(rhs.has_val_)
            this->store_val(rhs.val_);
        else
            this->store_unexpect(unexpected(rhs.internal_get_unexpect().value()));
    }
};

/* Partial specialization for when
 * T is trivially copy constructible or void, and E is trivially
 * copy constructible. expected should be constexpr copy constructible */
template <typename T, typename E>
struct expected_copy_ctor_base<T, E, true, true>
    : expected_default_ctor_base<T,E> {
    using expected_default_ctor_base<T,E>::expected_default_ctor_base;
    expected_copy_ctor_base() = default;

    constexpr expected_copy_ctor_base(expected_copy_ctor_base const& rhs) {
        if(rhs.has_val_)
            this->store_val(rhs.val_);
        else
            this->store_unexpect(unexpected(rhs.internal_get_unexpect().value()));
    }
};

/* expected_move_ctor_base */
/* Delete move ctor if T is neither move constructible nor void
 * or E is not move constructible. Make the move constructor
 * constexpr if T is trivially move constructible or void, and
 * E is trivially move constructible */
template <typename T, typename E,
          bool = is_move_constructible_or_void_v<T> &&
                 std::is_move_constructible_v<E>,
          bool = is_trivially_move_constructible_or_void_v<T> &&
                 std::is_trivially_move_constructible_v<E>>
struct expected_move_ctor_base;

/* Partial specialization for when
 * T is neither move constructible nor void, and/or E is not move
 * constructible. Delete move ctor */
template <typename T, typename E>
struct expected_move_ctor_base<T, E, false, false>
    : expected_copy_ctor_base<T,E> {
    using expected_copy_ctor_base<T,E>::expected_copy_ctor_base;
    expected_move_ctor_base() = default;

    expected_move_ctor_base(expected_move_ctor_base const&) = default;
    expected_move_ctor_base(expected_move_ctor_base&&) = delete;
};

/* Partial specialization for when
 * T is move constructible or void, and E is move constructible,
 * T is not trivially move constructible or void, and/or E is not
 * trivially move constructible. expected should have non-constexpr
 * move ctor */
template <typename T, typename E>
struct expected_move_ctor_base<T, E, true, false>
    : expected_copy_ctor_base<T,E> {
    using expected_copy_ctor_base<T,E>::expected_copy_ctor_base;
    expected_move_ctor_base() = default;

    expected_move_ctor_base(expected_move_ctor_base const&) = default;
    expected_move_ctor_base(expected_move_ctor_base&& rhs)
        noexcept(is_nothrow_move_constructible_or_void_v<T> &&
                 std::is_nothrow_move_constructible_v<E>) {
        if(rhs.has_val_)
            this->store_val(std::move(rhs.val_));
        else
            this->store_unexpect(unexpected(std::move(rhs.internal_get_unexpect().value())));
    }
};

/* Partial specialization for when:
 * T is trivially move constructible or void and E is trivially move
 * constructible. expected should have constexpr move ctor */
template <typename T, typename E>
struct expected_move_ctor_base<T, E, true, true>
    : expected_copy_ctor_base<T,E> {
    using expected_copy_ctor_base<T,E>::expected_copy_ctor_base;
    expected_move_ctor_base() = default;

    expected_move_ctor_base(expected_move_ctor_base const&) = default;
    constexpr expected_move_ctor_base(expected_move_ctor_base&& rhs)
        noexcept(is_nothrow_move_constructible_or_void_v<T> &&
                 std::is_nothrow_move_constructible_v<E>) {
        if(rhs.has_val_)
            this->store_val(std::move(rhs.val_));
        else
            this->store_unexpect(unexpected(std::move(rhs.internal_get_unexpect().value())));
    }
};

/* expected_copy_assign_base */
/* Conditionally enable copy assignment */
template <typename T, typename E,
          bool = expected_enable_copy_assignment_v<T,E>>
struct expected_copy_assign_base;

/* Conditions for copy assignment not met, delete */
template <typename T, typename E>
struct expected_copy_assign_base<T, E, false>
    : expected_move_ctor_base<T,E> {

    using expected_move_ctor_base<T,E>::expected_move_ctor_base;
    expected_copy_assign_base() = default;
    expected_copy_assign_base(expected_copy_assign_base const&) = default;
    expected_copy_assign_base(expected_copy_assign_base&&) = default;

    expected_copy_assign_base& operator=(expected_copy_assign_base const&) = delete;
};

template <typename T, typename E>
struct expected_copy_assign_base<T, E, true>
    : expected_move_ctor_base<T,E> {
    using expected_move_ctor_base<T,E>::expected_move_ctor_base;
    expected_copy_assign_base() = default;
    expected_copy_assign_base(expected_copy_assign_base const&) = default;
    expected_copy_assign_base(expected_copy_assign_base&&) = default;

    expected_copy_assign_base& operator=(expected_copy_assign_base const& rhs) {

        if(this->has_value() && rhs.has_value()) {
            if constexpr(!std::is_void_v<T>)
                this->internal_get_value() = rhs.internal_get_value();
        }

        else if(!this->has_value() && !rhs.has_value())
            this->internal_get_unexpect() = unexpected(rhs.internal_get_unexpect().value());

        else if(this->has_value() && !rhs.has_value()) {
            if constexpr(std::is_void_v<T>) {
                /* If unexpected<E> ctor throws, this->has_val_ is not changed */
                this->store_unexpect(unexpected(rhs.internal_get_unexpect().value()));
            }
            else if constexpr(std::is_nothrow_copy_constructible_v<E>) {
                this->internal_get_value().~T();
                this->store_unexpect(unexpected(rhs.internal_get_unexpect().value()));
            }
            else if constexpr(std::is_nothrow_move_constructible_v<E>) {
                unexpected<E> tmp = unexpected(rhs.internal_get_unexpect().value());
                this->internal_get_value().~T();
                this->store_unexpect(std::move(tmp));
            }
            else {
                T tmp = this->internal_get_value();
                this->internal_get_value().~T();
                try {
                    this->store_unexpect(unexpected(rhs.internal_get_unexpect().value()));
                }
                catch(...) {
                    this->store_val(std::move(tmp));
                    throw;
                }
            }
        }

        else {
            if constexpr(std::is_void_v<T>) {
                this->internal_get_unexpect().~unexpected<E>();
                this->store_val();
            }
            else if constexpr(std::is_nothrow_copy_constructible_v<T>) {
                this->internal_get_unexpect().~unexpected<E>();
                this->store_val(rhs.internal_get_value());
            }
            else if constexpr(std::is_nothrow_move_constructible_v<T>) {
                T tmp = rhs.internal_get_value();
                this->internal_get_unexpect().~unexpected<E>();
                this->store_val(std::move(tmp));
            }
            else {
                unexpected<E> tmp = unexpected(this->internal_get_unexpect().value());
                this->internal_get_unexpect().~unexpected<E>();
                try {
                    this->store_val(rhs.internal_get_value());
                }
                catch(...) {
                    this->store_unexpect(std::move(tmp));
                    throw;
                }
            }
        }

        return *this;
    }
};



/* expected_move_assign_base */
/* Conditionally enable move assignment */
template <typename T, typename E,
          bool = expected_enable_move_assignment_v<T,E>>
struct expected_move_assign_base;

/* Conditions for move assignment not met, delete the operator */
template <typename T, typename E>
struct expected_move_assign_base<T,E,false>
    : expected_copy_assign_base<T,E> {
    using expected_copy_assign_base<T,E>::expected_copy_assign_base;
    expected_move_assign_base() = default;
    expected_move_assign_base(expected_move_assign_base const&) = default;
    expected_move_assign_base(expected_move_assign_base&&) = default;

    expected_move_assign_base& operator=(expected_move_assign_base const&) = default;
    expected_move_assign_base& operator=(expected_move_assign_base&&) = delete;
};

/* T is move constructible and move assignable, T is nothrow move constructible
 * and nothrow move assignable => enable move assignment */
template <typename T, typename E>
struct expected_move_assign_base<T, E, true>
    : expected_copy_assign_base<T,E> {

    using expected_copy_assign_base<T,E>::expected_copy_assign_base;
    expected_move_assign_base() = default;
    expected_move_assign_base(expected_move_assign_base const&) = default;
    expected_move_assign_base(expected_move_assign_base&&) = default;
    expected_move_assign_base& operator=(expected_move_assign_base const&) = default;

    expected_move_assign_base& operator=(expected_move_assign_base&& rhs)
                noexcept(std::is_nothrow_move_constructible_v<T> &&
                         std::is_nothrow_move_assignable_v<T>) {

        if(this->has_value() && rhs.has_value()) {
            if constexpr(!std::is_void_v<T>)
                this->internal_get_value() = rhs.internal_get_value();
        }

        else if(!this->has_value() && !rhs.has_value())
            this->internal_get_unexpect() = unexpected(rhs.internal_get_unexpect().value());

        else if(this->has_value() && !rhs.has_value()) {
            if constexpr(std::is_void_v<T>) {
                /* If unexpected<E> ctor throws, ths->has_val_ is not changed */
                this->store_unexpect(unexpected(std::move(rhs.internal_get_unexpect().value())));
            }
            else if constexpr(std::is_nothrow_move_constructible_v<E>) {
                this->internal_get_value().~T();
                this->store_unexpect(unexpected(std::move(rhs.internal_get_unexpect().value())));
            }
            else {
                T tmp = std::move(this->internal_get_value());
                this->internal_get_value().~T();
                try {
                    this->store_unexpect(unexpected(std::move(rhs.internal_get_unexpect().value())));
                }
                catch(...) {
                    this->store_val(std::move(tmp));
                    throw;
                }
            }
        }

        else {
            if constexpr(std::is_void_v<T>) {
                this->internal_get_unexpect().~unexpected<E>();
                this->store_val();
            }
            else if constexpr(std::is_nothrow_move_constructible_v<T>) {
                this->internal_get_unexpect().~unexpected<E>();
                this->store_val(std::move(rhs.internal_get_value()));
            }
            else {
                unexpected<E> tmp = unexpected(std::move(this->internal_get_unexpect().value()));
                this->internal_get_unexpect().~unexpected<E>();
                try {
                    this->store_val(std::move(rhs.internal_get_value()));
                }
                catch(...) {
                    this->store_unexpect(std::move(tmp));
                    throw;
                }
            }
        }

        return *this;
    }
};

/* expected_interface_base */
/* Provides functions availabe for all instantiations of expected.
 * Operations unique to when either T is void or T is non-void are
 * provided in derived classes */
template <typename T, typename E>
class expected_interface_base : expected_move_assign_base<T,E> {
    static_assert(!std::is_reference_v<T>, "T must not be reference");
    static_assert(!std::is_reference_v<E>, "E must not be reference");
    static_assert(!std::is_same_v<T, std::remove_cv_t<unexpected<E>>>,
                  "T must not be unexpected<E>");
    using base_t = expected_move_assign_base<T,E>;

    public:
        using base_t::base_t;
        expected_interface_base() = default;
        expected_interface_base(expected_interface_base const&) = default;
        expected_interface_base(expected_interface_base&&) = default;
        expected_interface_base& operator=(expected_interface_base const&) = default;
        expected_interface_base& operator=(expected_interface_base&&) = default;

        /* Conditionally explicit unexpected lvalue ctor */
        template <typename G = E,
                  expected_detail::enable_if_constructible_t<E, G const&>* = nullptr,
                  expected_detail::enable_if_convertible_t<G const&, E>* = nullptr>
        constexpr expected_interface_base(unexpected<G> const& e);

        template <typename G = E,
                  expected_detail::enable_if_constructible_but_not_convertible_t<E, G const&>* = nullptr>
        constexpr explicit expected_interface_base(unexpected<G> const& e);

        /* Conditionally explicit unexpected rvalue ctor */
        template <typename G = E,
                  expected_detail::enable_if_constructible_t<E, G&&>* = nullptr,
                  expected_detail::enable_if_convertible_t<G&&, E>* = nullptr>
        constexpr expected_interface_base(unexpected<G>&& e)
                        noexcept(std::is_nothrow_constructible_v<E, G&&>);

        template <typename G = E,
                  expected_detail::enable_if_constructible_but_not_convertible_t<E, G&&>* = nullptr>
        constexpr explicit expected_interface_base(unexpected<G>&& e)
                        noexcept(std::is_nothrow_constructible_v<E, G&&>);

        template <typename... Args,
                  expected_detail::enable_if_constructible_t<E, Args...>* = nullptr>
        constexpr explicit expected_interface_base(unexpect_t, Args&&... args);

        template <typename U, typename... Args,
                  expected_detail::enable_if_constructible_t<E, std::initializer_list<U>&, Args...>* = nullptr>
        constexpr explicit
            expected_interface_base(unexpect_t, std::initializer_list<U> il, Args&&... args);

        template <typename G = E, typename EE = E,
                  expected_detail::expected_enable_unexpected_copy_assignment_t<EE>* = nullptr>
        expected_interface_base& operator=(unexpected<G> const& e);

        template <typename G = E, typename EE = E,
                  expected_detail::expected_enable_unexpected_move_assignment_t<EE>* = nullptr>
        expected_interface_base& operator=(unexpected<G>&& e);

        constexpr explicit operator bool() const noexcept;
        constexpr bool has_value() const noexcept;

        constexpr E& error() &;
        constexpr E const& error() const &;
        constexpr E&& error() &&;
        constexpr E const&& error() const &&;

        template <typename T1, typename E1, typename E2>
        friend constexpr bool operator==(expected<T1, E1> const& x, unexpected<E2> const& e);
        template <typename T1, typename E1, typename E2>
        friend constexpr bool operator==(unexpected<E2> const& e, expected<T1, E1> const& x);
        template <typename T1, typename E1, typename E2>
        friend constexpr bool operator!=(expected<T1, E1> const& x, unexpected<E2> const& e);
        template <typename T1, typename E1, typename E2>
        friend constexpr bool operator!=(unexpected<E2> const& e, expected<T1, E1> const& x);

    protected:
        template <typename... Args>
        void store_val(Args&&... args);

        template <typename... Args>
        void store_unexpect(Args&&... args);

        template <typename U = T, typename = std::enable_if_t<!std::is_void_v<U>>>
        constexpr U& internal_get_value() &;

        template <typename U = T, typename = std::enable_if_t<!std::is_void_v<U>>>
        constexpr U const& internal_get_value() const &;

        template <typename U = T, typename = std::enable_if_t<!std::is_void_v<U>>>
        constexpr U&& internal_get_value() &&;

        template <typename U = T, typename = std::enable_if_t<!std::is_void_v<U>>>
        constexpr U const&& internal_get_value() const &&;

        constexpr unexpected<E>& internal_get_unexpect() &;
        constexpr unexpected<E> const& internal_get_unexpect() const &;
        constexpr unexpected<E>&& internal_get_unexpect() &&;
        constexpr unexpected<E> const&& internal_get_unexpect() const &&;
};

template <typename T, typename E>
template <typename G,
          expected_detail::enable_if_constructible_t<E, G const&>*,
          expected_detail::enable_if_convertible_t<G const&, E>*>
constexpr expected_interface_base<T,E>::expected_interface_base(unexpected<G> const& e)
    : base_t(no_init) {
    this->store_unexpect(e);
}

template <typename T, typename E>
template <typename G,
          expected_detail::enable_if_constructible_but_not_convertible_t<E, G const&>*>
constexpr expected_interface_base<T,E>::expected_interface_base(unexpected<G> const& e)
    : base_t(no_init) {
    this->store_unexpect(e);
}

template <typename T, typename E>
template <typename G,
          expected_detail::enable_if_constructible_t<E, G&&>*,
          expected_detail::enable_if_convertible_t<G&&, E>*>
constexpr expected_interface_base<T,E>::expected_interface_base(unexpected<G>&& e)
                noexcept(std::is_nothrow_constructible_v<E, G&&>) : base_t(no_init) {
    this->store_unexpect(std::move(e));
}

template <typename T, typename E>
template <typename G,
          expected_detail::enable_if_constructible_but_not_convertible_t<E, G&&>*>
constexpr expected_interface_base<T,E>::expected_interface_base(unexpected<G>&& e)
                noexcept(std::is_nothrow_constructible_v<E, G&&>) : base_t(no_init) {
    this->store_unexpect(std::move(e));
}

template <typename T, typename E>
template <typename... Args,
          expected_detail::enable_if_constructible_t<E, Args...>*>
constexpr expected_interface_base<T,E>::expected_interface_base(unexpect_t, Args&&... args)
    : base_t(no_init) {
    this->store_unexpect(std::forward<Args>(args)...);
}

template <typename T, typename E>
template <typename U, typename... Args,
          expected_detail::enable_if_constructible_t<E, std::initializer_list<U>&, Args...>*>
constexpr expected_interface_base<T,E>::expected_interface_base(unexpect_t,
                std::initializer_list<U> il, Args&&... args) : base_t(no_init) {

    this->store_unexpect(il, std::forward<Args>(args)...);
}

template <typename T, typename E>
template <typename G, typename EE,
          expected_detail::expected_enable_unexpected_copy_assignment_t<EE>*>
expected_interface_base<T,E>&
expected_interface_base<T,E>::operator=(unexpected<G> const& e) {
    if(!bool(*this))
        this->internal_get_unexpect() = unexpected(e.internal_get_unexpect().value());

    else {
        if constexpr(!std::is_void_v<T>)
            this->internal_get_value().~T();

        this->store_unexpect(unexpected(e.internal_get_unexpect().value()));
    }

    return *this;
}

template <typename T, typename E>
template <typename G, typename EE,
          expected_detail::expected_enable_unexpected_move_assignment_t<EE>*>
expected_interface_base<T,E>& expected_interface_base<T,E>::operator=(unexpected<G>&& e) {
    if(!bool(*this))
        this->internal_get_unexpect() = unexpected(std::move(e.internal_get_unexpect().value()));

    else {
        if constexpr(!std::is_void_v<T>)
            this->internal_get_value().~T();

        this->store_unexpect(unexpected(std::move(e.internal_get_unexpect().value())));
    }

    return *this;
}

template <typename T, typename E>
constexpr expected_interface_base<T,E>::operator bool() const noexcept {
    return base_t::has_value();
}

template <typename T, typename E>
constexpr bool expected_interface_base<T,E>::has_value() const noexcept {
    return base_t::has_value();
}

template <typename T, typename E>
constexpr E& expected_interface_base<T,E>::error() & {
    return this->unexpect_.value();
}

template <typename T, typename E>
constexpr E const& expected_interface_base<T,E>::error() const & {
    return this->unexpect_.value();
}

template <typename T, typename E>
constexpr E&& expected_interface_base<T,E>::error() && {
    return std::move(this->unexpect_.value());
}

template <typename T, typename E>
constexpr E const&& expected_interface_base<T,E>::error() const && {
    return std::move(this->unexpect_.value());
}

template <typename T1, typename E1, typename E2>
constexpr bool operator==(expected<T1, E1> const& x, unexpected<E2> const& e) {
    return bool(x) ? false : unexpected(x.error()) == e;
}

template <typename T1, typename E1, typename E2>
constexpr bool operator==(unexpected<E2> const& e, expected<T1, E1> const& x) {
    return bool(x) ? false : unexpected(x.error()) == e;
}

template <typename T1, typename E1, typename E2>
constexpr bool operator!=(expected<T1, E1> const& x, unexpected<E2> const& e) {
    return bool(x) ? true : unexpected(x.error()) != e;
}

template <typename T1, typename E1, typename E2>
constexpr bool operator!=(unexpected<E2> const& e, expected<T1, E1> const& x) {
    return bool(x) ? true : unexpected(x.error()) != e;
}

template <typename T, typename E>
template <typename... Args>
void expected_interface_base<T,E>::store_val(Args&&... args) {
    base_t::store_val(std::forward<Args>(args)...);
}

template <typename T, typename E>
template <typename... Args>
void expected_interface_base<T,E>::store_unexpect(Args&&... args) {
    base_t::store_unexpect(std::forward<Args>(args)...);
}

template <typename T, typename E>
template <typename U, typename>
constexpr U& expected_interface_base<T,E>::internal_get_value() & {
    return base_t::internal_get_value();
}

template <typename T, typename E>
template <typename U, typename>
constexpr U const& expected_interface_base<T,E>::internal_get_value() const & {
    return base_t::internal_get_value();
}

template <typename T, typename E>
template <typename U, typename>
constexpr U&& expected_interface_base<T,E>::internal_get_value() && {
    return std::move(base_t::internal_get_value());
}

template <typename T, typename E>
template <typename U, typename>
constexpr U const&& expected_interface_base<T,E>::internal_get_value() const && {
    return std::move(base_t::internal_get_value());
}

template <typename T, typename E>
constexpr unexpected<E>& expected_interface_base<T,E>::internal_get_unexpect() & {
    return base_t::internal_get_unexpect();
}

template <typename T, typename E>
constexpr unexpected<E> const& expected_interface_base<T,E>::internal_get_unexpect() const & {
    return base_t::internal_get_unexpect();
}

template <typename T, typename E>
constexpr unexpected<E>&& expected_interface_base<T,E>::internal_get_unexpect() && {
    return std::move(base_t::internal_get_unexpect());
}

template <typename T, typename E>
constexpr unexpected<E> const&& expected_interface_base<T,E>::internal_get_unexpect() const && {
    return std::move(base_t::internal_get_unexpect());
}

} /* namespace expected_detail */


/* Primary template (T is not void) */
template <typename T, typename E>
class expected : public expected_detail::expected_interface_base<T,E> {
    using base_t = expected_detail::expected_interface_base<T,E>;
    public:
        using value_type = T;
        using error_type = E;
        using unexpected_type = unexpected<E>;

        template <typename U>
        using rebind = expected<U, error_type>;

        using base_t::base_t;
        expected() = default;
        expected(expected const&) = default;
        expected(expected&&) = default;
        expected& operator=(expected const&) = default;
        expected& operator=(expected&&) = default;

        /* Conditionally explicit perfect forwarding conversion ctor */
        template <typename U = T, typename TT = T, typename EE = E,
                  expected_detail::expected_enable_forwarding_ref_ctor_t<TT, EE, U>* = nullptr,
                  expected_detail::expected_enable_implicit_forwarding_ref_ctor_t<TT, U>* = nullptr>
        constexpr expected(U&& v);

        template <typename U = T, typename TT = T, typename EE = E,
                  expected_detail::expected_enable_explicit_forwarding_ref_ctor_t<TT, EE, U>* = nullptr>
        constexpr explicit expected(U&& v);


        /* Conditionally explicit conversion constructors */
        template <typename U, typename G, typename TT = T, typename EE = E,
                  expected_detail::expected_enable_copy_conversion_t<TT,EE,U,G>* = nullptr,
                  expected_detail::expected_enable_implicit_copy_conversion_t<TT,EE,U,G>* = nullptr>
        constexpr expected(expected<U,G> const& rhs);

        template <typename U, typename G, typename TT = T, typename EE = E,
                  expected_detail::expected_enable_explicit_copy_conversion_t<TT,EE,U,G>* = nullptr>
        constexpr explicit expected(expected<U,G> const& rhs);

        template <typename U, typename G, typename TT = T, typename EE = E,
                  expected_detail::expected_enable_move_conversion_t<TT,EE,U,G>* = nullptr,
                  expected_detail::expected_enable_implicit_move_conversion_t<TT,EE,U,G>* = nullptr>
        constexpr expected(expected<U,G>&& rhs);

        template <typename U, typename G, typename TT = T, typename EE = E,
                  expected_detail::expected_enable_explicit_move_conversion_t<TT,EE,U,G>* = nullptr>
        constexpr explicit expected(expected<U,G>&& rhs);

        template <typename... Args,
                  expected_detail::enable_if_constructible_t<T, Args...>* = nullptr>
        constexpr explicit expected(in_place_t, Args&&... args);

        template <typename U, typename... Args,
                  expected_detail::enable_if_constructible_t<T, std::initializer_list<U>&, Args...>* = nullptr>
        constexpr explicit expected(in_place_t, std::initializer_list<U> il, Args&&... args);

        template <typename U = T, typename TT = T, typename EE = E,
                  expected_detail::expected_enable_unary_forwarding_assign_t<TT,EE,U>* = nullptr>
        expected& operator=(U&& v);

        template <typename... Args,
                  expected_detail::enable_if_nothrow_constructible_t<T, Args...>* = nullptr>
        T& emplace(Args&&... args);

        template <typename U, typename... Args,
                  expected_detail::enable_if_nothrow_constructible_t<T, std::initializer_list<U>&, Args...>* = nullptr>
        T& emplace(std::initializer_list<U>& il, Args&&... args);

        template <typename TT = T, typename EE = E,
                  expected_detail::expected_enable_swap_t<TT,EE>* = nullptr>
        void swap(expected& rhs) noexcept(std::is_nothrow_move_constructible_v<T> &&
                                          std::is_nothrow_swappable_v<T> &&
                                          std::is_nothrow_move_constructible_v<E> &&
                                          std::is_nothrow_swappable_v<E>);

        constexpr T* operator->();
        constexpr T const* operator->() const;

        constexpr T& operator*() &;
        constexpr T const& operator*() const &;
        constexpr T&& operator*() &&;
        constexpr T const&& operator*() const &&;

        constexpr T& value() &;
        constexpr T const& value() const &;
        constexpr T&& value() &&;
        constexpr T const&& value() const &&;

        template <typename U>
        constexpr T value_or(U&& v) const &;
        template <typename U>
        constexpr T value_or(U&& v) &&;

        template <typename T1, typename E1, typename T2, typename E2>
        friend constexpr bool operator==(expected<T1, E1> const& x,
                                         expected<T2, E2> const& y);

        template <typename T1, typename E1, typename T2, typename E2>
        friend constexpr bool operator!=(expected<T1, E1> const& x,
                                         expected<T2, E2> const& y);

        template <typename T1, typename E1, typename T2>
        friend constexpr bool operator==(expected<T1, E1> const& x, T2 const& v);
        template <typename T1, typename E1, typename T2>
        friend constexpr bool operator==(T2 const& v, expected<T1, E1> const& x);
        template <typename T1, typename E1, typename T2>
        friend constexpr bool operator!=(expected<T1, E1> const& x, T2 const& v);
        template <typename T1, typename E1, typename T2>
        friend constexpr bool operator!=(T2 const& v, expected<T1, E1> const& x);

        #ifdef VIEN_EXPECTED_EXTENDED
        template <typename F>
        constexpr expected<std::decay_t<std::invoke_result_t<F,T>>, E>
            map(F&& f) &;
        template <typename F>
        constexpr expected<std::decay_t<std::invoke_result_t<F,T>>, E>
            map(F&& f) const &;
        template <typename F>
        constexpr expected<std::decay_t<std::invoke_result_t<F,T>>, E>
            map(F&& f) &&;
        template <typename F>
        constexpr expected<std::decay_t<std::invoke_result_t<F,T>>, E>
             map(F&& f) const &&;

        template <typename F, typename TT = T,
                  expected_detail::enable_if_container_t<TT>* = nullptr>
        constexpr expected<expected_detail::rebind_container_t<T,F>, E>
            map_range(F&& f) &;

        template <typename F, typename TT = T,
                  expected_detail::enable_if_container_t<TT>* = nullptr>
        constexpr expected<expected_detail::rebind_container_t<T,F>, E>
            map_range(F&& f) const &;

        template <typename F, typename TT = T,
                  expected_detail::enable_if_container_t<TT>* = nullptr>
        constexpr expected<expected_detail::rebind_container_t<T,F>, E>
            map_range(F&& f) &&;

        template <typename F, typename TT = T,
                  expected_detail::enable_if_container_t<TT>* = nullptr>
        constexpr expected<expected_detail::rebind_container_t<T,F>, E>
            map_range(F&& f) const &&;

        template <typename F>
        constexpr expected<T, std::decay_t<std::invoke_result_t<F,E>>>
            map_error(F&& f) &;
        template <typename F>
        constexpr expected<T, std::decay_t<std::invoke_result_t<F,E>>>
            map_error(F&& f) const &;
        template <typename F>
        constexpr expected<T, std::decay_t<std::invoke_result_t<F,E>>>
            map_error(F&& f) &&;
        template <typename F>
        constexpr expected<T, std::decay_t<std::invoke_result_t<F,E>>>
            map_error(F&& f) const &&;

        template <typename M, typename F>
        constexpr T map_or_else(M&& map, F&& fallback) &;
        template <typename M, typename F>
        constexpr T map_or_else(M&& map, F&& fallback) const &;
        template <typename M, typename F>
        constexpr T map_or_else(M&& map, F&& fallback) &&;
        template <typename M, typename F>
        constexpr T map_or_else(M&& map, F&& fallback) const &&;

        template <typename F>
        constexpr expected and_then(F&& f) &;
        template <typename F>
        constexpr expected and_then(F&& f) const &;
        template <typename F>
        constexpr expected and_then(F&& f) &&;
        template <typename F>
        constexpr expected and_then(F&& f) const &&;

        template <typename F>
        constexpr expected or_else(F&& f) &;
        template <typename F>
        constexpr expected or_else(F&& f) const &;
        template <typename F>
        constexpr expected or_else(F&& f) &&;
        template <typename F>
        constexpr expected or_else(F&& f) const &&;
        #endif
};

template <typename T, typename E>
template <typename U, typename TT, typename EE,
          expected_detail::expected_enable_forwarding_ref_ctor_t<TT,EE,U>*,
          expected_detail::expected_enable_implicit_forwarding_ref_ctor_t<TT,U>*>
constexpr expected<T,E>::expected(U&& v) : base_t(expected_detail::no_init) {
    this->store_val(std::forward<U>(v));
}

template <typename T, typename E>
template <typename U, typename TT, typename EE,
          expected_detail::expected_enable_explicit_forwarding_ref_ctor_t<TT,EE,U>*>
constexpr expected<T,E>::expected(U&& v) : base_t(expected_detail::no_init) {
    this->store_val(std::forward<U>(v));
}

template <typename T, typename E>
template <typename U, typename G, typename TT, typename EE,
          expected_detail::expected_enable_copy_conversion_t<TT,EE,U,G>*,
          expected_detail::expected_enable_implicit_copy_conversion_t<TT,EE,U,G>*>
constexpr expected<T,E>::expected(expected<U,G> const& rhs)
    : base_t(expected_detail::no_init) {
    if(bool(rhs))
        this->store_val(rhs.value());
    else
        this->store_unexpect(unexpected(rhs.error()));
}

template <typename T, typename E>
template <typename U, typename G, typename TT, typename EE,
          expected_detail::expected_enable_explicit_copy_conversion_t<TT,EE,U,G>*>
constexpr expected<T,E>::expected(expected<U,G> const& rhs)
    : base_t(expected_detail::no_init) {
    if(bool(rhs))
        this->store_val(rhs.value());
    else
        this->store_unexpect(unexpected(rhs.error()));
}

template <typename T, typename E>
template <typename U, typename G, typename TT, typename EE,
          expected_detail::expected_enable_move_conversion_t<TT,EE,U,G>*,
          expected_detail::expected_enable_implicit_move_conversion_t<TT,EE,U,G>*>
constexpr expected<T,E>::expected(expected<U,G>&& rhs)
    : base_t(expected_detail::no_init) {
    if(bool(rhs))
        this->store_val(std::move(rhs.value()));
    else
        this->store_unexpect(unexpected(std::move(rhs.error())));
}

template <typename T, typename E>
template <typename U, typename G, typename TT, typename EE,
          expected_detail::expected_enable_explicit_move_conversion_t<TT,EE,U,G>*>
constexpr expected<T,E>::expected(expected<U,G>&& rhs)
    : base_t(expected_detail::no_init) {
    if(bool(rhs))
        this->store_val(std::move(rhs.value()));
    else
        this->store_unexpect(unexpected(std::move(rhs.error())));
}

template <typename T, typename E>
template <typename... Args,
          expected_detail::enable_if_constructible_t<T, Args...>*>
constexpr expected<T,E>::expected(in_place_t, Args&&... args)
    : base_t(expected_detail::no_init) {
    this->store_val(std::forward<Args>(args)...);
}

template <typename T, typename E>
template <typename U, typename... Args,
          expected_detail::enable_if_constructible_t<T, std::initializer_list<U>&, Args...>*>
constexpr expected<T,E>::expected(in_place_t, std::initializer_list<U> il, Args&&... args)
    : base_t(expected_detail::no_init) {
    this->store_val(il, std::forward<Args>(args)...);
}

template <typename T, typename E>
template <typename U, typename TT, typename EE,
          expected_detail::expected_enable_unary_forwarding_assign_t<TT,EE,U>*>
expected<T,E>& expected<T,E>::operator=(U&& v) {
    if(bool(*this))
        this->internal_get_value() = std::forward<U>(v);

    else {
        if constexpr(std::is_nothrow_constructible_v<T, U&&>) {
            this->internal_get_unexpect().~unexpected<E>();
            this->store_val(std::forward<U>(v));
        }
        else {
            unexpected<E> tmp = unexpected(std::move(this->internal_get_unexpect()));
            this->internal_get_unexpect().~unexpected<E>();
            try {
                this->store_val(std::forward<U>(v));
            }
            catch(...) {
                this->store_unexpect(std::move(tmp));
                throw;
            }
        }
    }

    return *this;
}

template <typename T, typename E>
template <typename... Args,
          expected_detail::enable_if_nothrow_constructible_t<T, Args...>*>
T& expected<T,E>::emplace(Args&&... args) {
    if(bool(*this))
        this->store_val(std::forward<Args>(args)...);
    else {
        if constexpr(std::is_nothrow_constructible_v<T, Args...>) {
            this->internal_get_unexpect().~unexpected<E>();
            this->store_val(std::forward<Args>(args)...);
        }
        else if constexpr(std::is_nothrow_constructible_v<T>) {
            T tmp(std::forward<Args>(args)...);
            this->internal_get_unexpect().~unexpected<E>();
            this->store_val(std::move(tmp));
        }
        else {
            unexpected<E> tmp(std::move(this->internal_get_unexpect().value()));
            this->internal_get_unexpect().~unexpected<E>();
            try {
                this->store_val(std::forward<Args>(args)...);
            }
            catch(...) {
                this->store_unexpect(std::move(tmp));
                throw;
            }
        }
    }

    return this->internal_get_value();
}

template <typename T, typename E>
template <typename U, typename... Args,
          expected_detail::enable_if_nothrow_constructible_t<T, std::initializer_list<U>&, Args...>*>
T& expected<T,E>::emplace(std::initializer_list<U>& il, Args&&... args) {
    if(bool(*this))
        this->store_val(il, std::forward<Args>(args)...);

    else {
        if constexpr(std::is_nothrow_constructible_v<T, std::initializer_list<U>&, Args...>) {
            this->internal_get_unexpect().~unexpected<E>();
            this->store_val(il, std::forward<Args>(args)...);
        }
        else if constexpr(std::is_nothrow_move_constructible_v<T>) {
            T tmp(il, std::forward<Args>(args)...);
            this->internal_get_unexpect().~unexpected<E>();
            this->store_val(std::move(tmp));
        }
        else {
            unexpected<E> tmp(std::move(this->internal_get_unexpect().value()));
            this->internal_get_unexpect().~unexpected<E>();
            try {
                this->store_val(il, std::forward<Args>(args)...);
            }
            catch(...) {
                this->store_unexpect(std::move(tmp));
                throw;
            }
        }
    }

    return this->internal_get_value();
}

template <typename T, typename E>
template <typename TT, typename EE,
          expected_detail::expected_enable_swap_t<TT,EE>*>
void expected<T,E>::swap(expected& rhs) noexcept(std::is_nothrow_move_constructible_v<T> &&
                                                 std::is_nothrow_swappable_v<T> &&
                                                 std::is_nothrow_move_constructible_v<E> &&
                                                 std::is_nothrow_swappable_v<E>) {
    using std::swap;

    if(bool(*this) && bool(rhs))
        swap(this->internal_get_value(), rhs.internal_get_value());

    else if(!bool(*this) && !bool(rhs))
        swap(this->internal_get_unexpect(), rhs.internal_get_unexpect());

    else if(!bool(*this) && bool(rhs))
        rhs.swap(*this);

    else {
        if constexpr(std::is_nothrow_move_constructible_v<E>) {
            unexpected<E> tmp = std::move(rhs.internal_get_unexpect());
            rhs.internal_get_unexpect().~unexpected<E>();

            /* GCC erroneousely warns about throw; always calling terminate
             * but swap is noexcept only if both T and E are nothrow move
             * constructible, meaning the catch(...) is never entered */
            #if defined __GNUC__ && !defined __clang__
            #pragma GCC diagnostic push
            #pragma GCC diagnostic ignored "-Wterminate"
            #endif
            try {
                rhs.store_val(std::move(this->internal_get_value()));
                this->internal_get_value().~T();
                this->store_unexpect(std::move(tmp));
            }
            catch(...) {
                rhs.store_unexpect(std::move(tmp));
                throw;
            }
            #if defined __GNUC__ && !defined __clang__
            #pragma GCC diagnostic pop
            #endif
        }
        else if constexpr(std::is_nothrow_move_constructible_v<T>) {
            T tmp = std::move(this->internal_get_value());
            this->internal_get_value().~T();
            try {
                this->store_unexpect(unexpected(std::move(rhs.internal_get_unexpect().value())));
                rhs.internal_get_unexpect().~unexpected<E>();
                rhs.store_val(std::move(tmp));
            }
            catch(...) {
                this->store_val(std::move(tmp));
                throw;
            }
        }

    }
}

template <typename T, typename E>
constexpr T* expected<T,E>::operator->() {
    return std::addressof(this->internal_get_value());
}

template <typename T, typename E>
constexpr T const* expected<T,E>::operator->() const {
    return std::addressof(this->internal_get_value());
}

template <typename T, typename E>
constexpr T& expected<T,E>::operator*() & {
    return this->internal_get_value();
}

template <typename T, typename E>
constexpr T const& expected<T,E>::operator*() const & {
    return this->internal_get_value();
}

template <typename T, typename E>
constexpr T&& expected<T,E>::operator*() && {
    return std::move(this->internal_get_value());
}

template <typename T, typename E>
constexpr T const&& expected<T,E>::operator*() const && {
    return std::move(this->internal_get_value());
}

template <typename T, typename E>
constexpr T& expected<T,E>::value() & {
    if(!bool(*this))
        throw bad_expected_access(this->error());
    return this->internal_get_value();
}

template <typename T, typename E>
constexpr T const& expected<T,E>::value() const & {
    if(!bool(*this))
        throw bad_expected_access(this->error());
    return this->internal_get_value();
}

template <typename T, typename E>
constexpr T&& expected<T,E>::value() && {
    if(!bool(*this))
        throw bad_expected_access(this->error());
    return std::move(this->internal_get_value());
}

template <typename T, typename E>
constexpr T const&& expected<T,E>::value() const && {
    if(!bool(*this))
        throw bad_expected_access(this->error());
    return std::move(this->internal_get_value());
}

template <typename T, typename E>
template <typename U>
constexpr T expected<T,E>::value_or(U&& v) const & {
    return bool(*this) ? **this : static_cast<T>(std::forward<U>(v));
}

template <typename T, typename E>
template <typename U>
constexpr T expected<T,E>::value_or(U&& v) && {
    return bool(*this) ? std::move(**this) : static_cast<T>(std::forward<U>(v));
}

template <typename T1, typename E1, typename T2, typename E2>
constexpr bool operator==(expected<T1, E1> const& x, expected<T2, E2> const& y) {
    if(bool(x) != bool(y))
        return false;

    else if(!bool(x))
        return x.error() == y.error();

    else
        return *x == *y;
}

template <typename T1, typename E1, typename T2, typename E2>
constexpr bool operator!=(expected<T1, E1> const& x, expected<T2, E2> const& y) {
    if(bool(x) != bool(y))
        return true;

    else if(!bool(x))
        return x.error() != y.error();

    else
        return *x != *y;
}

template <typename T1, typename E1, typename T2>
constexpr bool operator==(expected<T1, E1> const& x, T2 const& v) {
    return bool(x) ? *x == v : false;
}

template <typename T1, typename E1, typename T2>
constexpr bool operator==(T2 const& v, expected<T1, E1> const& x) {
    return bool(x) ? *x == v : false;
}

template <typename T1, typename E1, typename T2>
constexpr bool operator!=(expected<T1, E1> const& x, T2 const& v) {
    return bool(x) ? *x != v : false;
}

template <typename T1, typename E1, typename T2>
constexpr bool operator!=(T2 const& v, expected<T1, E1> const& x) {
    return bool(x) ? *x != v : false;
}

#ifdef VIEN_EXPECTED_EXTENDED

template <typename T, typename E>
template <typename F>
[[nodiscard]]
constexpr expected<std::decay_t<std::invoke_result_t<F,T>>, E>
expected<T,E>::map(F&& f) & {

    using result_t = expected_detail::expected_mapped_type_t<T,E,F>;
    using result_value_t = typename result_t::value_type;

    if constexpr(std::is_void_v<result_value_t>) {
        std::invoke(std::forward<F>(f), **this);
        return bool(*this) ?
            result_t{} :
            result_t(unexpect, this->error());
    }
    else {
        return bool(*this) ?
                result_t(std::invoke(std::forward<F>(f), **this)) :
                result_t(unexpect, this->error());
    }
}

template <typename T, typename E>
template <typename F>
[[nodiscard]]
constexpr expected<std::decay_t<std::invoke_result_t<F,T>>, E>
expected<T,E>::map(F&& f) const & {

    using result_t = expected_detail::expected_mapped_type_t<T,E,F>;
    using result_value_t = typename result_t::value_type;

    if constexpr(std::is_void_v<result_value_t>) {
        std::invoke(std::forward<F>(f), **this);
        return bool(*this) ?
            result_t{} :
            result_t(unexpect, this->error());
    }
    else {
        return bool(*this) ?
                result_t(std::invoke(std::forward<F>(f), **this)) :
                result_t(unexpect, this->error());
    }
}

template <typename T, typename E>
template <typename F>
[[nodiscard]]
constexpr expected<std::decay_t<std::invoke_result_t<F,T>>, E>
expected<T,E>::map(F&& f) && {

    using result_t = expected_detail::expected_mapped_type_t<T,E,F>;
    using result_value_t = typename result_t::value_type;

    if constexpr(std::is_void_v<result_value_t>) {
        std::invoke(std::forward<F>(f), std::move(**this));
        return bool(*this) ?
            result_t{} :
            result_t(unexpect, std::move(this->error()));
    }
    else {
        return bool(*this) ?
                result_t(std::invoke(std::forward<F>(f), std::move(**this))) :
                result_t(unexpect, std::move(this->error()));
    }
}

template <typename T, typename E>
template <typename F>
[[nodiscard]]
constexpr expected<std::decay_t<std::invoke_result_t<F,T>>, E>
expected<T,E>::map(F&& f) const && {

    using result_t = expected_detail::expected_mapped_type_t<T,E,F>;
    using result_value_t = typename result_t::value_type;

    if constexpr(std::is_void_v<result_value_t>) {
        std::invoke(std::forward<F>(f), std::move(**this));
        return bool(*this) ?
            result_t{} :
            result_t(unexpect, std::move(this->error()));
    }
    else {
        return bool(*this) ?
                result_t(std::invoke(std::forward<F>(f), std::move(**this))) :
                result_t(unexpect, std::move(this->error()));
    }
}

template <typename T, typename E>
template <typename F, typename TT,
          expected_detail::enable_if_container_t<TT>*>
[[nodiscard]]
constexpr expected<expected_detail::rebind_container_t<T,F>, E>
expected<T,E>::map_range(F&& f) & {
    using invoke_t =
        std::decay_t<std::invoke_result_t<F, expected_detail::value_type_of_t<T>>>;
    using container_t = expected_detail::rebind_t<T, invoke_t>;

    using result_t = expected<container_t, E>;

    if(!bool(*this))
        return result_t(unexpect, this->error());

    return result_t(expected_detail::convert<T, container_t, F, invoke_t>
                        {}(**this, std::forward<F>(f)));
}

template <typename T, typename E>
template <typename F, typename TT,
          expected_detail::enable_if_container_t<TT>*>
[[nodiscard]]
constexpr expected<expected_detail::rebind_container_t<T,F>, E>
expected<T,E>::map_range(F&& f) const & {
    using invoke_t =
        std::decay_t<std::invoke_result_t<F, expected_detail::value_type_of_t<T>>>;
    using container_t = expected_detail::rebind_t<T, invoke_t>;

    using result_t = expected<container_t, E>;

    if(!bool(*this))
        return result_t(unexpect, this->error());

    return result_t(expected_detail::convert<T, container_t, F, invoke_t>
                        {}(**this, std::forward<F>(f)));
}

template <typename T, typename E>
template <typename F, typename TT,
          expected_detail::enable_if_container_t<TT>*>
[[nodiscard]]
constexpr expected<expected_detail::rebind_container_t<T,F>, E>
expected<T,E>::map_range(F&& f) && {
    using invoke_t =
        std::decay_t<std::invoke_result_t<F, expected_detail::value_type_of_t<T>>>;
    using container_t = expected_detail::rebind_t<T, invoke_t>;

    using result_t = expected<container_t, E>;

    if(!bool(*this))
        return result_t(unexpect, std::move(this->error()));

    /* T and container_t are the same, transform **this and move
     * **this to new instance */
    if constexpr(std::is_same_v<T, container_t>) {
        return result_t(expected_detail::convert<T, container_t, F, invoke_t>
                            {}(expected_detail::in_place, **this, std::forward<F>(f)));
    }
    /* T and container_t are not the same, must create new container */
    else {
        return result_t(expected_detail::convert<T, container_t, F, invoke_t>
                            {}(**this, std::forward<F>(f)));
    }
}

template <typename T, typename E>
template <typename F, typename TT,
          expected_detail::enable_if_container_t<TT>*>
[[nodiscard]]
constexpr expected<expected_detail::rebind_container_t<T,F>, E>
expected<T,E>::map_range(F&& f) const && {
    using invoke_t =
        std::decay_t<std::invoke_result_t<F, expected_detail::value_type_of_t<T>>>;
    using container_t = expected_detail::rebind_t<T, invoke_t>;

    using result_t = expected<container_t, E>;

    if(!bool(*this))
        return result_t(unexpect, std::move(this->error()));

    if constexpr(std::is_same_v<T, container_t>) {
        return result_t(expected_detail::convert<T, container_t, F, invoke_t>
                            {}(expected_detail::in_place, **this, std::forward<F>(f)));
    }
    else {
        return result_t(expected_detail::convert<T, container_t, F, invoke_t>
                            {}(**this, std::forward<F>(f)));
    }
}

template <typename T, typename E>
template <typename F>
[[nodiscard]]
constexpr expected<T, std::decay_t<std::invoke_result_t<F,E>>>
expected<T,E>::map_error(F&& f) & {

    using result_t = expected_detail::expected_mapped_error_type_t<T,E,F>;

    return bool(*this) ?
            result_t(**this) :
            result_t(unexpect, std::invoke(std::forward<F>(f), this->error()));
}

template <typename T, typename E>
template <typename F>
[[nodiscard]]
constexpr expected<T, std::decay_t<std::invoke_result_t<F,E>>>
expected<T,E>::map_error(F&& f) const & {

    using result_t = expected_detail::expected_mapped_error_type_t<T,E,F>;

    return bool(*this) ?
            result_t(**this) :
            result_t(unexpect, std::invoke(std::forward<F>(f), this->error()));
}

template <typename T, typename E>
template <typename F>
[[nodiscard]]
constexpr expected<T, std::decay_t<std::invoke_result_t<F,E>>>
expected<T,E>::map_error(F&& f) && {

    using result_t = expected_detail::expected_mapped_error_type_t<T,E,F>;

    return bool(*this) ?
            result_t(std::move(**this)) :
            result_t(unexpect, std::invoke(std::forward<F>(f), std::move(this->error())));
}

template <typename T, typename E>
template <typename F>
[[nodiscard]]
constexpr expected<T, std::decay_t<std::invoke_result_t<F,E>>>
expected<T,E>::map_error(F&& f) const && {

    using result_t = expected_detail::expected_mapped_error_type_t<T,E,F>;

    return bool(*this) ?
            result_t(std::move(**this)) :
            result_t(unexpect, std::invoke(std::forward<F>(f), std::move(this->error())));
}

template <typename T, typename E>
template <typename M, typename F>
[[nodiscard]]
constexpr T expected<T,E>::map_or_else(M&& map, F&& fallback) & {
    return bool(*this) ?
        std::invoke(std::forward<M>(map), **this) :
        std::invoke(std::forward<F>(fallback), this->error());
}

template <typename T, typename E>
template <typename M, typename F>
[[nodiscard]]
constexpr T expected<T,E>::map_or_else(M&& map, F&& fallback) const & {
    return bool(*this) ?
        std::invoke(std::forward<M>(map), **this) :
        std::invoke(std::forward<F>(fallback), this->error());
}

template <typename T, typename E>
template <typename M, typename F>
[[nodiscard]]
constexpr T expected<T,E>::map_or_else(M&& map, F&& fallback) && {
    return bool(*this) ?
        std::invoke(std::forward<M>(map), std::move(**this)) :
        std::invoke(std::forward<F>(fallback), std::move(this->error()));
}

template <typename T, typename E>
template <typename M, typename F>
[[nodiscard]]
constexpr T expected<T,E>::map_or_else(M&& map, F&& fallback) const && {
    return bool(*this) ?
        std::invoke(std::forward<M>(map), std::move(**this)) :
        std::invoke(std::forward<F>(fallback), std::move(this->error()));
}

template <typename T, typename E>
template <typename F>
[[nodiscard]]
constexpr expected<T,E> expected<T,E>::and_then(F&& f) & {
    return bool(*this) ?
            expected(std::invoke(std::forward<F>(f), **this)) :
            expected(unexpect, this->error());
}

template <typename T, typename E>
template <typename F>
[[nodiscard]]
constexpr expected<T,E> expected<T,E>::and_then(F&& f) const & {
    return bool(*this) ?
            expected(std::invoke(std::forward<F>(f), **this)) :
            expected(unexpect, this->error());
}

template <typename T, typename E>
template <typename F>
[[nodiscard]]
constexpr expected<T,E> expected<T,E>::and_then(F&& f) && {
    return bool(*this) ?
            expected(std::invoke(std::forward<F>(f), std::move(**this))) :
            expected(unexpect, std::move(this->error()));
}

template <typename T, typename E>
template <typename F>
[[nodiscard]]
constexpr expected<T,E> expected<T,E>::and_then(F&& f) const && {
    return bool(*this) ?
            expected(std::invoke(std::forward<F>(f), std::move(**this))) :
            expected(unexpect, std::move(this->error()));
}

template <typename T, typename E>
template <typename F>
[[nodiscard]]
constexpr expected<T,E> expected<T,E>::or_else(F&& f) & {
    static_assert(std::is_same_v<E, std::invoke_result_t<F,E&>>,
                  "Callable F must return an instance of type E");

    if(bool(*this))
        return *this;

    return expected(unexpect, std::invoke(std::forward<F>(f), this->error()));
}

template <typename T, typename E>
template <typename F>
[[nodiscard]]
constexpr expected<T,E> expected<T,E>::or_else(F&& f) const & {
    static_assert(std::is_same_v<E, std::invoke_result_t<F,E const&>>,
                  "Callable F must return an instance of type E");

    if(bool(*this))
        return *this;

    return expected(unexpect, std::invoke(std::forward<F>(f), this->error()));
}

template <typename T, typename E>
template <typename F>
[[nodiscard]]
constexpr expected<T,E> expected<T,E>::or_else(F&& f) && {
    static_assert(std::is_same_v<E, std::invoke_result_t<F,E&&>>,
                  "Callable F must return an instance of type E");

    if(bool(*this))
        return std::move(*this);

    return expected(unexpect, std::invoke(std::forward<F>(f),
                                                         std::move(this->error())));
}

template <typename T, typename E>
template <typename F>
[[nodiscard]]
constexpr expected<T,E> expected<T,E>::or_else(F&& f) const && {
    static_assert(std::is_same_v<E, std::invoke_result_t<F,E const&&>>,
                  "Callable F must return an instance of type E");

    if(bool(*this))
        return std::move(*this);

    return expected(unexpect, std::invoke(std::forward<F>(f),
                                                         std::move(this->error())));
}

#endif

template <typename E>
class expected<void, E> : public expected_detail::expected_interface_base<void,E> {
    using base_t = expected_detail::expected_interface_base<void,E>;
    public:
        using value_type = void;
        using error_type = E;
        using unexpected_type = unexpected<E>;

        template <typename U>
        using rebind = expected<U, error_type>;

        using base_t::base_t;
        expected() = default;
        expected(expected const&) = default;
        expected(expected&&) = default;
        expected& operator=(expected const&) = default;
        expected& operator=(expected&&) = default;

        /* Conditionally explicit conversion constructors */
        template <typename U, typename G, typename TT = void, typename EE = E,
                  expected_detail::expected_enable_copy_conversion_t<TT,EE,U,G>* = nullptr,
                  expected_detail::expected_enable_implicit_copy_conversion_t<TT,EE,U,G>* = nullptr>
        constexpr expected(expected<U,G> const& rhs);

        template <typename U, typename G, typename TT = void, typename EE = E,
                  expected_detail::expected_enable_explicit_copy_conversion_t<TT,EE,U,G>* = nullptr>
        constexpr explicit expected(expected<U,G> const& rhs);

        template <typename U, typename G, typename TT = void, typename EE = E,
                  expected_detail::expected_enable_move_conversion_t<TT,EE,U,G>* = nullptr,
                  expected_detail::expected_enable_implicit_move_conversion_t<TT,EE,U,G>* = nullptr>
        constexpr expected(expected<U,G>&& rhs);

        template <typename U, typename G, typename TT = void, typename EE = E,
                  expected_detail::expected_enable_explicit_move_conversion_t<TT,EE,U,G>* = nullptr>
        constexpr explicit expected(expected<U,G>&& rhs);

        template <typename... Args,
                  expected_detail::enable_if_empty_pack_t<Args...>* = nullptr>
        constexpr explicit expected(in_place_t, Args&&...);

        void emplace();

        template <typename TT = void, typename EE = E,
                  expected_detail::expected_enable_swap_t<TT,EE>* = nullptr>
        void swap(expected& rhs) noexcept(std::is_nothrow_move_constructible_v<E> &&
                                          std::is_nothrow_swappable_v<E>);

        constexpr void value() const;

        template <typename E1,typename E2>
        friend constexpr bool operator==(expected<void, E1> const& x,
                                         expected<void, E2> const& y);

        template <typename E1, typename E2>
        friend constexpr bool operator!=(expected<void, E1> const& x,
                                         expected<void, E2> const& y);

        #ifdef VIEN_EXPECTED_EXTENDED
        template <typename F>
        constexpr expected<std::decay_t<std::invoke_result_t<F>>, E>
            map(F&& f) &;
        template <typename F>
        constexpr expected<std::decay_t<std::invoke_result_t<F>>, E>
            map(F&& f) const &;
        template <typename F>
        constexpr expected<std::decay_t<std::invoke_result_t<F>>, E>
            map(F&& f) &&;
        template <typename F>
        constexpr expected<std::decay_t<std::invoke_result_t<F>>, E>
            map(F&& f) const &&;

        template <typename F>
        constexpr expected<void, std::decay_t<std::invoke_result_t<F,E>>>
            map_error(F&& f) &;
        template <typename F>
        constexpr expected<void, std::decay_t<std::invoke_result_t<F,E>>>
            map_error(F&& f) const &;
        template <typename F>
        constexpr expected<void, std::decay_t<std::invoke_result_t<F,E>>>
            map_error(F&& f) &&;
        template <typename F>
        constexpr expected<void, std::decay_t<std::invoke_result_t<F,E>>>
            map_error(F&& f) const &&;

        template <typename F>
        constexpr expected or_else(F&& f) &;
        template <typename F>
        constexpr expected or_else(F&& f) const &;
        template <typename F>
        constexpr expected or_else(F&& f) &&;
        template <typename F>
        constexpr expected or_else(F&& f) const &&;
        #endif
};

template <typename E>
template <typename U, typename G, typename TT, typename EE,
          expected_detail::expected_enable_copy_conversion_t<TT,EE,U,G>*,
          expected_detail::expected_enable_implicit_copy_conversion_t<TT,EE,U,G>*>
constexpr expected<void,E>::expected(expected<U,G> const& rhs)
    : base_t(expected_detail::no_init) {
    if(bool(rhs))
        this->store_val();
    else
        this->store_unexpect(unexpected(rhs.error()));
}

template <typename E>
template <typename U, typename G, typename TT, typename EE,
          expected_detail::expected_enable_explicit_copy_conversion_t<TT,EE,U,G>*>
constexpr expected<void,E>::expected(expected<U,G> const& rhs)
    : base_t(expected_detail::no_init) {
    if(bool(rhs))
        this->store_val();
    else
        this->store_unexpect(unexpected(rhs.error()));
}

template <typename E>
template <typename U, typename G, typename TT, typename EE,
          expected_detail::expected_enable_move_conversion_t<TT,EE,U,G>*,
          expected_detail::expected_enable_implicit_move_conversion_t<TT,EE,U,G>*>
constexpr expected<void,E>::expected(expected<U,G>&& rhs)
    : base_t(expected_detail::no_init) {
    if(bool(rhs))
        this->store_val();
    else
        this->store_unexpect(unexpected(std::move(rhs.error())));
}

template <typename E>
template <typename U, typename G, typename TT, typename EE,
          expected_detail::expected_enable_explicit_move_conversion_t<TT,EE,U,G>*>
constexpr expected<void,E>::expected(expected<U,G>&& rhs)
    : base_t(expected_detail::no_init) {
    if(bool(rhs))
        this->store_val();
    else
        this->store_unexpect(unexpected(std::move(rhs.error())));
}

template <typename E>
template <typename... Args,
          expected_detail::enable_if_empty_pack_t<Args...>*>
constexpr expected<void, E>::expected(in_place_t, Args&&...)
    : base_t(expected_detail::no_init) {
    this->store_val();
}

template <typename E>
void expected<void,E>::emplace() {
    if(!bool(*this)) {
        this->internal_get_unexpect().~unexpected<E>();
        this->store_val();
    }
}

template <typename E>
template <typename TT, typename EE,
          expected_detail::expected_enable_swap_t<TT,EE>*>
void expected<void,E>::swap(expected& rhs) noexcept(std::is_nothrow_move_constructible_v<E> &&
                                                    std::is_nothrow_swappable_v<E>) {
    using std::swap;

    // if(bool(*this) && bool(rhs))
    /* nothing to do */

    if(!bool(*this) && !bool(rhs))
        swap(this->internal_get_unexpect(), rhs.internal_get_unexpect());

    else if(!bool(*this) && bool(rhs))
        rhs.swap(*this);

    else if(bool(*this) && !bool(rhs)) {
        /* No need to catch potential exception as the only thing
         * to do would have been to rethrow it */
        this->store_unexpect(unexpected(std::move(rhs).internal_get_unexpect().value()));
        rhs.internal_get_unexpect().~unexpected<E>();
        rhs.store_val();
    }
}

template <typename E>
constexpr void expected<void,E>::value() const {
    if(!bool(*this))
        throw bad_expected_access(this->error());
}

template <typename E1, typename E2>
constexpr bool operator==(expected<void, E1> const& x, expected<void, E2> const& y) {
    if(bool(x) != bool(y))
        return false;

    else if(!bool(x))
        return x.error() == y.error();

    else
        return true;
}

template <typename E1, typename E2>
constexpr bool operator!=(expected<void, E1> const& x, expected<void, E2> const& y) {
    if(bool(x) != bool(y))
        return true;

    else if(!bool(x))
        return x.error() != y.error();

    else
        return true;
}

#ifdef VIEN_EXPECTED_EXTENDED

template <typename E>
template <typename F>
[[nodiscard]]
constexpr expected<std::decay_t<std::invoke_result_t<F>>, E>
expected<void,E>::map(F&& f) & {

    using result_t = expected_detail::expected_mapped_type_t<void,E,F>;
    using result_value_t = typename result_t::value_type;

    if constexpr(std::is_void_v<result_value_t>) {
        std::invoke(std::forward<F>(f));
        return bool(*this) ?
            result_t{} :
            result_t(unexpect, this->error());
    }
    else {
        return bool(*this) ?
                result_t(std::invoke(std::forward<F>(f))) :
                result_t(unexpect, this->error());
    }
}

template <typename E>
template <typename F>
[[nodiscard]]
constexpr expected<std::decay_t<std::invoke_result_t<F>>, E>
expected<void,E>::map(F&& f) const & {

    using result_t = expected_detail::expected_mapped_type_t<void,E,F>;
    using result_value_t = typename result_t::value_type;

    if constexpr(std::is_void_v<result_value_t>) {
        std::invoke(std::forward<F>(f));
        return bool(*this) ?
            result_t{} :
            result_t(unexpect, this->error());
    }
    else {
        return bool(*this) ?
                result_t(std::invoke(std::forward<F>(f))) :
                result_t(unexpect, this->error());
    }
}

template <typename E>
template <typename F>
[[nodiscard]]
constexpr expected<std::decay_t<std::invoke_result_t<F>>, E>
expected<void,E>::map(F&& f) && {

    using result_t = expected_detail::expected_mapped_type_t<void,E,F>;
    using result_value_t = typename result_t::value_type;

    if constexpr(std::is_void_v<result_value_t>) {
        std::invoke(std::forward<F>(f));
        return bool(*this) ?
            result_t{} :
            result_t(unexpect, std::move(this->error()));
    }
    else {
        return bool(*this) ?
                result_t(std::invoke(std::forward<F>(f))) :
                result_t(unexpect, std::move(this->error()));
    }
}

template <typename E>
template <typename F>
[[nodiscard]]
constexpr expected<std::decay_t<std::invoke_result_t<F>>, E>
expected<void,E>::map(F&& f) const && {

    using result_t = expected_detail::expected_mapped_type_t<void,E,F>;
    using result_value_t = typename result_t::value_type;

    if constexpr(std::is_void_v<result_value_t>) {
        std::invoke(std::forward<F>(f));
        return bool(*this) ?
            result_t{} :
            result_t(unexpect, std::move(this->error()));
    }
    else {
        return bool(*this) ?
                result_t(std::invoke(std::forward<F>(f))) :
                result_t(unexpect, std::move(this->error()));
    }
}

template <typename E>
template <typename F>
[[nodiscard]]
constexpr expected<void, std::decay_t<std::invoke_result_t<F,E>>>
expected<void, E>::map_error(F&& f) & {

    using result_t = expected_detail::expected_mapped_error_type_t<void,E,F>;

    return bool(*this) ?
            result_t{} :
            result_t(unexpect, std::invoke(std::forward<F>(f), this->error()));
}

template <typename E>
template <typename F>
[[nodiscard]]
constexpr expected<void, std::decay_t<std::invoke_result_t<F,E>>>
expected<void, E>::map_error(F&& f) const & {

    using result_t = expected_detail::expected_mapped_error_type_t<void,E,F>;

    return bool(*this) ?
            result_t{} :
            result_t(unexpect, std::invoke(std::forward<F>(f), this->error()));
}

template <typename E>
template <typename F>
[[nodiscard]]
constexpr expected<void, std::decay_t<std::invoke_result_t<F,E>>>
expected<void, E>::map_error(F&& f) && {

    using result_t = expected_detail::expected_mapped_error_type_t<void,E,F>;

    return bool(*this) ?
            result_t{} :
            result_t(unexpect, std::invoke(std::forward<F>(f), std::move(this->error())));
}

template <typename E>
template <typename F>
[[nodiscard]]
constexpr expected<void, std::decay_t<std::invoke_result_t<F,E>>>
expected<void, E>::map_error(F&& f) const && {

    using result_t = expected_detail::expected_mapped_error_type_t<void,E,F>;

    return bool(*this) ?
            result_t{} :
            result_t(unexpect, std::invoke(std::forward<F>(f), std::move(this->error())));
}

template <typename E>
template <typename F>
[[nodiscard]]
constexpr expected<void,E> expected<void,E>::or_else(F&& f) & {
    static_assert(std::is_same_v<E, std::invoke_result_t<F,E&>>,
                  "Callable F must return an instance of type E");

    if(bool(*this))
        return *this;

    return expected(unexpect, std::invoke(std::forward<F>(f), this->error()));
}

template <typename E>
template <typename F>
[[nodiscard]]
constexpr expected<void,E> expected<void,E>::or_else(F&& f) const & {
    static_assert(std::is_same_v<E, std::invoke_result_t<F,E const&>>,
                  "Callable F must return an instance of type E");

    if(bool(*this))
        return *this;

    return expected(unexpect, std::invoke(std::forward<F>(f), this->error()));
}

template <typename E>
template <typename F>
[[nodiscard]]
constexpr expected<void,E> expected<void,E>::or_else(F&& f) && {
    static_assert(std::is_same_v<E, std::invoke_result_t<F,E&&>>,
                  "Callable F must return an instance of type E");

    if(bool(*this))
        return std::move(*this);

    return expected(unexpect, std::invoke(std::forward<F>(f),
                                                         std::move(this->error())));
}

template <typename E>
template <typename F>
[[nodiscard]]
constexpr expected<void,E> expected<void,E>::or_else(F&& f) const && {
    static_assert(std::is_same_v<E, std::invoke_result_t<F,E const&&>>,
                  "Callable F must return an instance of type E");

    if(bool(*this))
        return std::move(*this);

    return expected(unexpect, std::invoke(std::forward<F>(f),
                                                         std::move(this->error())));
}

#endif

template <typename E>
class expected<void const, E> : public expected<void, E> {
    public:
        using value_type = void const;
        using error_type = E;
        using unexpected_type = unexpected<E>;

        template <typename U>
        using rebind = expected<U, error_type>;

        using expected<void,E>::expected;
        expected() = default;
        expected(expected const&) = default;
        expected(expected&&) = default;
        expected& operator=(expected const&) = default;
        expected& operator=(expected&&) = default;
};

template <typename E>
class expected<void volatile, E> : public expected<void, E> {
    public:
        using value_type = void const;
        using error_type = E;
        using unexpected_type = unexpected<E>;

        template <typename U>
        using rebind = expected<U, error_type>;

        using expected<void,E>::expected;
        expected() = default;
        expected(expected const&) = default;
        expected(expected&&) = default;
        expected& operator=(expected const&) = default;
        expected& operator=(expected&&) = default;
};

template <typename E>
class expected<void const volatile, E> : public expected<void, E> {
    public:
        using value_type = void const;
        using error_type = E;
        using unexpected_type = unexpected<E>;

        template <typename U>
        using rebind = expected<U, error_type>;

        using expected<void,E>::expected;
        expected() = default;
        expected(expected const&) = default;
        expected(expected&&) = default;
        expected& operator=(expected const&) = default;
        expected& operator=(expected&&) = default;
};

template <typename E>
class unexpected {
    static_assert(!std::is_same_v<E, void>, "E cannot be void");
    public:
        template <typename Err = E,
                  expected_detail::enable_if_constructible_t<E, Err&&>* = nullptr>
        constexpr explicit unexpected(Err&& err);

        template <typename... Args,
                  expected_detail::enable_if_constructible_t<E, Args...>* = nullptr>
        constexpr explicit unexpected(in_place_t, Args&&... args);

        template <typename U, typename... Args,
                  expected_detail::enable_if_constructible_t<E, std::initializer_list<U>, Args...>* = nullptr>
        constexpr explicit unexpected(in_place_t, std::initializer_list<U> il, Args&&... args);

        constexpr unexpected(unexpected const&) = default;
        constexpr unexpected(unexpected&&) = default;

        /* Implicit conversion ctor templates available iff is_convertible_v<Err, E>
         * is true. If not, the explicit versions are used */
        template <typename Err, typename EE = E,
                  expected_detail::unexpected_enable_copy_conversion_t<EE, Err>* = nullptr,
                  expected_detail::unexpected_enable_implicit_copy_conversion_t<EE, Err>* = nullptr>
        constexpr unexpected(unexpected<Err> const& e);
        template <typename Err, typename EE = E,
                  expected_detail::unexpected_enable_explicit_copy_conversion_t<EE, Err>* = nullptr>
        constexpr explicit unexpected(unexpected<Err> const& e);

        template <typename Err, typename EE = E,
                  expected_detail::unexpected_enable_move_conversion_t<EE, Err>* = nullptr,
                  expected_detail::unexpected_enable_implicit_move_conversion_t<EE, Err>* = nullptr>
        constexpr unexpected(unexpected<Err>&& e);
        template <typename Err, typename EE = E,
                  expected_detail::unexpected_enable_explicit_move_conversion_t<EE, Err>* = nullptr>
        constexpr explicit unexpected(unexpected<Err>&& e);

        constexpr unexpected& operator=(unexpected const&) = default;
        constexpr unexpected& operator=(unexpected&&) = default;
        template <typename Err = E, typename EE = E,
                  expected_detail::unexpected_enable_copy_assignment_t<EE, Err>* = nullptr>
        constexpr unexpected& operator=(unexpected<Err> const& e);
        template <typename Err = E, typename EE = E,
                  expected_detail::unexpected_enable_move_assignment_t<EE, Err>* = nullptr>
        constexpr unexpected& operator=(unexpected<Err>&& e);

        constexpr E& value() & noexcept;
        constexpr E const& value() const & noexcept;
        constexpr E&& value() && noexcept;
        constexpr E const&& value() const && noexcept;

        template <typename EE = E,
                  expected_detail::enable_if_swappable_t<EE>* = nullptr>
        void swap(unexpected& other) noexcept(std::is_nothrow_swappable_v<E>);

        template <typename E1, typename E2>
        friend constexpr bool operator==(unexpected<E1> const& x, unexpected<E2> const& y);
        template <typename E1, typename E2>
        friend constexpr bool operator!=(unexpected<E1> const& x, unexpected<E2> const& y);

    private:
        E val_;
};

template <typename E>
unexpected(E) -> unexpected<E>;

template <typename E>
template <typename Err,
          expected_detail::enable_if_constructible_t<E, Err&&>*>
constexpr unexpected<E>::unexpected(Err&& err)
    : val_(std::forward<Err>(err)) { }

template <typename E>
template <typename... Args,
          expected_detail::enable_if_constructible_t<E, Args...>*>
constexpr unexpected<E>::unexpected(in_place_t, Args&&... args)
    : val_(std::forward<Args>(args)...) { }

template <typename E>
template <typename U, typename... Args,
          expected_detail::enable_if_constructible_t<E, std::initializer_list<U>, Args...>*>
constexpr unexpected<E>::unexpected(in_place_t, std::initializer_list<U> il, Args&&... args)
    : val_(il, std::forward<Args>(args)...) { }

template <typename E>
template <typename Err, typename EE,
          expected_detail::unexpected_enable_copy_conversion_t<EE, Err>*,
          expected_detail::unexpected_enable_implicit_copy_conversion_t<EE, Err>*>
constexpr unexpected<E>::unexpected(unexpected<Err> const& e)
    : val_(e.value()) { }

template <typename E>
template <typename Err, typename EE,
          expected_detail::unexpected_enable_explicit_copy_conversion_t<EE, Err>*>
constexpr unexpected<E>::unexpected(unexpected<Err> const& e)
    : val_(e.value()) { }

template <typename E>
template <typename Err, typename EE,
          expected_detail::unexpected_enable_move_conversion_t<EE, Err>*,
          expected_detail::unexpected_enable_implicit_move_conversion_t<EE, Err>*>
constexpr unexpected<E>::unexpected(unexpected<Err>&& e)
    : val_(std::move(e.value())) { }

template <typename E>
template <typename Err, typename EE,
          expected_detail::unexpected_enable_explicit_move_conversion_t<EE, Err>*>
constexpr unexpected<E>::unexpected(unexpected<Err>&& e)
    : val_(std::move(e.value())) { }

template <typename E>
template <typename Err, typename EE,
          expected_detail::unexpected_enable_copy_assignment_t<EE, Err>*>
constexpr unexpected<E>& unexpected<E>::operator=(unexpected<Err> const& e) {
    val_ = e.value();
    return *this;
}

template <typename E>
template <typename Err, typename EE,
          expected_detail::unexpected_enable_move_assignment_t<EE, Err>*>
constexpr unexpected<E>& unexpected<E>::operator=(unexpected<Err>&& e) {
    val_ = std::move(e.value());
    return *this;
}

template <typename E>
constexpr E& unexpected<E>::value() & noexcept {
    return val_;
}

template <typename E>
constexpr E const& unexpected<E>::value() const & noexcept {
    return val_;
}

template <typename E>
constexpr E&& unexpected<E>::value() && noexcept {
    return std::move(val_);
}

template <typename E>
constexpr E const&& unexpected<E>::value() const && noexcept {
    return std::move(val_);
}

template <typename E>
template <typename EE,
          expected_detail::enable_if_swappable_t<EE>*>
void unexpected<E>::swap(unexpected& other) noexcept(std::is_nothrow_swappable_v<E>) {
    using std::swap;
    swap(val_, other.val_);
}

template <typename E1, typename E2>
constexpr bool operator==(unexpected<E1> const& x, unexpected<E2> const& y) {
    return x.value() == y.value();
}

template <typename E1, typename E2>
constexpr bool operator!=(unexpected<E1> const& x, unexpected<E2> const& y) {
    return x.value() != y.value();
}

template <>
class bad_expected_access<void> : public std::exception {
    public:
        explicit bad_expected_access() = default;
};

template <typename E>
class bad_expected_access : public bad_expected_access<void> {
    public:
        explicit bad_expected_access(E e);
        E& error() &;
        E const& error() const &;
        E&& error() &&;
        E const&& error() const &&;
        virtual char const* what() const noexcept override;
    private:
        E val_;
};

template <typename E>
bad_expected_access<E>::bad_expected_access(E e)
    : val_(std::move(e)) { }

template <typename E>
E& bad_expected_access<E>::error() & {
    return val_;
}

template <typename E>
E const& bad_expected_access<E>::error() const & {
    return val_;
}

template <typename E>
E&& bad_expected_access<E>::error() && {
    return std::move(val_);
}

template <typename E>
E const&& bad_expected_access<E>::error() const && {
    return std::move(val_);
}

template <typename E>
char const* bad_expected_access<E>::what() const noexcept {
    return "Attempt to access expected without value\n";
}

} /* namespace v1 */
} /* namespace vien */

namespace std {

template <typename T1, typename E1,
          vien::expected_detail::enable_if_internally_swappable_t<vien::expected<T1,E1>>* = nullptr>
void swap(vien::expected<T1,E1>& x, vien::expected<T1,E1>& y) noexcept(noexcept(x.swap(y))) {
    x.swap(y);
}

template <typename E1,
          vien::expected_detail::enable_if_internally_swappable_t<vien::unexpected<E1>>* = nullptr>
void swap(vien::unexpected<E1>& x, vien::unexpected<E1>& y) noexcept(noexcept(x.swap(y))) {
    x.swap(y);
}
} /* namespace std */

#endif
