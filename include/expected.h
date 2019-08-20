/* Based off of the 6th revision of the std::expected
 * proposal (p0323r6). Uses SFINAE to achieve the
 * conditionally explicit constructors to be
 * introduced in C++20.
 *
 * Define VIEN_EXPECTED_EXTENDED for functional
 * extensions not in the proposal. */

#ifndef VIEN_EXPECTED_H
#define VIEN_EXPECTED_H

/*
    --expected synopsis--

*** using syntax similar to conditionally noexcept for conditionally explicit

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
        cosntexpr expected(expected const&);
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
        cosntexpr E const& error() const &;
        constexpr E&& error() &&;
        constexpr E const&& error() cosnt &&;

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
        expected<std::decay_t<std::invoke_result_t<T>>, E> map(F&&) &;
        template <typename F>
        expected<std::decay_t<std::invoke_result_t<T>>, E> map(F&&) const &;
        template <typename F>
        expected<std::decay_t<std::invoke_result_t<T>>, E> map(F&&) &&;
        template <typename F>
        expected<std::decay_t<std::invoke_result_t<T>>, E> map(F&&) const &&;

        template <typename F>
        expected<T, std::decay_t<std::invoke_result_t<E>>> map_error(F&&) &;
        template <typename F>
        expected<T, std::decay_t<std::invoke_result_t<E>>> map_error(F&&) const &;
        template <typename F>
        expected<T, std::decay_t<std::invoke_result_t<E>>> map_error(F&&) &&;
        template <typename F>
        expected<T, std::decay_t<std::invoke_result_t<E>>> map_error(F&&) const &&;

        template <typename F>
        expected and_then(F&&) &;
        template <typename F>
        expected and_then(F&&) const &;
        template <typename F>
        expected and_then(F&&) &&;
        template <typename F>
        expected and_then(F&&) const &&;
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
/* Forward declare necessary std members */
namespace std {
    template <class>
    class Allocator;

    template <typename, std::size_t>
    struct array;

    template <class, class>
    class forward_list;

    template <class, class, class>
    class set;

    template <class, class, class>
    class map;

    template <class, class, class>
    class multiset;

    template <class, class, class>
    class multimap;

    template <class, class, class, class>
    class unordered_set;

    template <class, class, class, class, class>
    class unordered_map;

    template <class, class, class, class>
    class unordered_multiset;

    template <class, class, class, class, class>
    class unordered_multimap;
}
#endif

namespace vien {
using in_place_t = std::in_place_t;

struct unexpect_t {
    explicit unexpect_t() = default;
};

inline constexpr unexpect_t unexpect{};

inline namespace v1 {

template <typename, typename>
class expected;

template <typename>
class unexpected;

template <typename>
class bad_expected_access;


namespace impl {

template <typename T>
using remove_cvref = std::remove_cv<std::remove_reference_t<T>>;

template <typename T>
using remove_cvref_t = typename remove_cvref<T>::type;

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

template <typename T>
struct is_trivially_copy_constructible_or_void
    : std::bool_constant<std::is_trivially_copy_constructible_v<T> ||
                         std::is_void_v<T>> { };

template <typename T>
inline bool constexpr is_trivially_copy_constructible_or_void_v =
        is_trivially_copy_constructible_or_void<T>::value;

template <typename T>
struct is_trivially_move_constructible_or_void
    : std::bool_constant<std::is_trivially_move_constructible_v<T> ||
                         std::is_void_v<T>> { };

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

template <typename T, typename E, typename U, typename G>
struct expected_enable_copy_conversion
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
struct expected_enable_copy_conversion<void, E, void, G> : std::true_type { };

template <typename T, typename E, typename U, typename G>
inline bool constexpr expected_enable_copy_conversion_v =
    expected_enable_copy_conversion<T,E,U,G>::value;

template <typename T, typename E, typename U, typename G>
struct expected_enable_implicit_copy_conversion
    : std::bool_constant<std::is_convertible_v<U const&, T> &&
                         std::is_convertible_v<G const&, E>> { };

template <typename E, typename G>
struct expected_enable_implicit_copy_conversion<void, E, void, G>
    : std::bool_constant<std::is_convertible_v<G const&, E>> { };

template <typename T, typename E, typename U, typename G>
inline bool constexpr expected_enable_implicit_copy_conversion_v =
    expected_enable_implicit_copy_conversion<T,E,U,G>::value;

template <typename T, typename E, typename U, typename G>
struct expected_enable_move_conversion
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
struct expected_enable_move_conversion<void, E, void, G> : std::true_type { };

template <typename T, typename E, typename U, typename G>
inline bool constexpr expected_enable_move_conversion_v =
    expected_enable_move_conversion<T,E,U,G>::value;

/* Use for enabling non-explicit move constructor */
template <typename T, typename E, typename U, typename G>
struct expected_enable_implicit_move_conversion
    : std::bool_constant<std::is_convertible_v<U&&, T> &&
                         std::is_convertible_v<G&&, E>> { };

template <typename E, typename G>
struct expected_enable_implicit_move_conversion<void, E, void, G>
    : std::bool_constant<std::is_convertible_v<G&&, E>> { };

template <typename T, typename E, typename U, typename G>
inline bool constexpr expected_enable_implicit_move_conversion_v =
    expected_enable_implicit_move_conversion<T,E,U,G>::value;

template <typename T, typename E, typename U>
struct expected_enable_forwarding_ref_ctor
    : std::bool_constant<std::is_constructible_v<T, U&&> &&
                        !std::is_same_v<remove_cvref_t<U>, in_place_t> &&
                        !std::is_same_v<expected<T, E>, remove_cvref_t<U>> &&
                        !std::is_same_v<unexpected<E>, remove_cvref_t<U>>> { };

template <typename T, typename E, typename U>
inline bool constexpr expected_enable_forwarding_ref_ctor_v =
    expected_enable_forwarding_ref_ctor<T,E,U>::value;

template <typename T, typename U>
struct expected_enable_implicit_forwarding_ref_ctor
    : std::bool_constant<std::is_convertible_v<U&&, T>> { };

template <typename T, typename U>
inline bool constexpr expected_enable_implicit_forwarding_ref_ctor_v =
    expected_enable_implicit_forwarding_ref_ctor<T,U>::value;

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
struct expected_enable_unary_forwarding_assign
    : std::bool_constant<!std::is_same_v<expected<T,E>, remove_cvref_t<U>> &&
                         !std::conjunction_v<std::is_scalar<T>,
                                             std::is_same<T, std::decay_t<U>>> &&
                         std::is_constructible_v<T,U> &&
                         std::is_assignable_v<T&,U> &&
                         std::is_nothrow_move_constructible_v<E>> { };

template <typename T, typename E, typename U>
inline bool constexpr expected_enable_unary_forwarding_assign_v =
    expected_enable_unary_forwarding_assign<T,E,U>::value;

template <typename T, typename E>
struct expected_enable_swap
    : std::bool_constant<std::is_swappable_v<T> &&
                         std::is_swappable_v<E> &&
                         std::is_move_constructible_v<T> &&
                         std::is_move_constructible_v<E> &&
                        (std::is_nothrow_move_constructible_v<T> ||
                         std::is_nothrow_move_constructible_v<E>)> { };

template <typename E>
struct expected_enable_swap<void,E>
    : std::is_swappable<E> { };

template <typename T, typename E>
inline bool constexpr expected_enable_swap_v = expected_enable_swap<T,E>::value;

#ifdef VIEN_EXPECTED_EXTENDED

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

/* Type returned by expected<T,E>::map_error. T may be void */
template <typename T, typename E, typename F>
struct expected_mapped_error_type
    : type_is<expected<T, std::decay_t<std::invoke_result_t<F,E>>>>
{
    static_assert(!std::is_void_v<std::invoke_result_t<F,E>>,
                  "Cannot map error to void");
};

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

/* true iff has a binary operator() that returns bool */
template <template <typename> class C, typename T>
struct is_comparator<C<T>, std::enable_if_t<
    std::is_same_v<bool, decltype(std::declval<C<T>>()(std::declval<T&>(), std::declval<T&>()))>
>> : std::true_type { };

template <typename T>
inline bool constexpr is_comparator_v = is_comparator<T>::value;

/* Check if type is generated from std::hash */
template <typename>
struct is_hash : std::false_type { };

template <typename T>
struct is_hash<std::hash<T>> : std::true_type { };

template <typename T>
inline bool constexpr is_hash_v = is_hash<T>::value;

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

/* Rebind container value_type, allocator_type, compare and
 * hasher typedefs */
template <typename, typename>
struct rebind;

/* Container is a template, rebind value_type from F to T,
 * allocator<F> (if any) to allocator<T>,
 * compare<F> (if any) to compare<T>, and
 * hasher<F> (if any) to hasher<T>*/
template <template <typename, typename...> class Container,
          typename F, typename T,
          typename... Args>
struct rebind<Container<F, Args...>, T>
    : type_is<Container<T,
        rebind_if_hash_t<rebind_if_comparator_t<rebind_if_alloc_t<Args, T>, T>, T>...>
      > { };

/* Container is a standard array, rebind value_type from F to T
 * (size remains the same) */
template <typename F, std::size_t N, typename T>
struct rebind<std::array<F,N>, T> : type_is<std::array<T,N>> { };

template <typename C, typename T>
using rebind_t = typename rebind<C,T>::type;

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

/* Get value_type of T, if available. Otherwise, return T */
template <typename T, typename = void>
struct value_type_of : type_is<T> { };

template <typename T>
struct value_type_of<T, std::void_t<typename T::value_type>>
    : type_is<typename T::value_type> { };

template <typename T>
using value_type_of_t = typename value_type_of<T>::type;

/* Output iterator for std::array<T,N> */
template <typename>
class array_insert_iterator;

template <typename T, std::size_t N>
class array_insert_iterator<std::array<T, N>> {
    public:
        using iterator_category = std::output_iterator_tag;
        using value_type = void;
        using difference_type = void;
        using pointer = void;
        using reference = void;
        using container_type = std::array<T,N>;

        constexpr array_insert_iterator() noexcept = default;
        constexpr array_insert_iterator(std::array<T,N>& c)
            : ctr_{std::addressof(c)} { }

        constexpr array_insert_iterator& operator=(T const& v) {
            if(idx_ >= N)
                throw std::out_of_range("Array insertion out of bounds");
            (*ctr_)[idx_++] = v;
            return *this;
        }

        constexpr array_insert_iterator& operator=(T&& v) {
            if(idx_ >= N)
                throw std::out_of_range("Array insertion out of bounds");
            (*ctr_)[idx_++] = std::move(v);
            return *this;
        }

        constexpr array_insert_iterator& operator*() {
            return *this;
        }

        constexpr array_insert_iterator& operator++() {
            return *this;
        }

        constexpr array_insert_iterator operator++(int) {
            auto cpy = *this;
            return cpy;
        }

    private:
        std::array<T,N>* ctr_;
        std::size_t idx_{0};
};

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

/* Get suitable output iterator Container */
template <typename Container, typename = void>
struct insert_iterator;

/* If type names push_back, use std::back_insert_iterator */
template <typename Container>
struct insert_iterator<Container, std::void_t<
    decltype(std::declval<Container&>().push_back(
        std::declval<value_type_of_t<Container>>()
    ))>> : type_is<std::back_insert_iterator<Container>> { };

/* Use array_insert_iterator for std::array */
template <typename T, std::size_t N>
struct insert_iterator<std::array<T,N>>
    : type_is<array_insert_iterator<std::array<T,N>>> { };

/* Use std::front_insert_iterator for std::forward_list */
template <typename T, typename Alloc>
struct insert_iterator<std::forward_list<T, Alloc>>
    : type_is<std::front_insert_iterator<std::forward_list<T, Alloc>>> { };

/* Use ordered_insert_iterator for associative containers */
template <typename T, typename... Args>
struct insert_iterator<std::set<T, Args...>>
    : type_is<ordered_insert_iterator<std::set<T, Args...>>> { };

template <typename Key, typename T, typename... Args>
struct insert_iterator<std::map<Key, T, Args...>>
    : type_is<ordered_insert_iterator<std::map<Key, T, Args...>>> { };

template <typename T, typename... Args>
struct insert_iterator<std::multiset<T, Args...>>
    : type_is<ordered_insert_iterator<std::multiset<T, Args...>>> { };

template <typename Key, typename T, typename... Args>
struct insert_iterator<std::multimap<Key, T, Args...>>
    : type_is<ordered_insert_iterator<std::multimap<Key, T, Args...>>> { };

template <typename T, typename... Args>
struct insert_iterator<std::unordered_set<T, Args...>>
    : type_is<ordered_insert_iterator<std::unordered_set<T, Args...>>> { };

template <typename Key, typename T, typename... Args>
struct insert_iterator<std::unordered_map<Key, T, Args...>>
    : type_is<ordered_insert_iterator<std::unordered_map<Key, T, Args...>>> { };

template <typename T, typename... Args>
struct insert_iterator<std::unordered_multiset<T, Args...>>
    : type_is<ordered_insert_iterator<std::unordered_multiset<T, Args...>>> { };

template <typename Key, typename T, typename... Args>
struct insert_iterator<std::unordered_multimap<Key, T, Args...>>
    : type_is<ordered_insert_iterator<std::unordered_multimap<Key, T, Args...>>> { };

template <typename Container>
using insert_iterator_t = typename insert_iterator<Container>::type;

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
    constexpr void store(Args&&... args) {
        new(std::addressof(this->val_)) T(std::forward<Args>(args)...);
        this->has_val_ = true;
    }

    template <typename... Args>
    constexpr void store(unexpect_t, Args&&... args) {
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
    constexpr void store(Args&&...) noexcept {
        this->has_val_ = true;
    }

    template <typename... Args>
    constexpr void store(unexpect_t, Args&&... args) {
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
            this->store(rhs.val_);
        else
            this->store(unexpect, unexpected(rhs.internal_get_unexpect().value()));
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
            this->store(rhs.val_);
        else
            this->store(unexpect, unexpected(rhs.internal_get_unexpect().value()));
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
            this->store(std::move(rhs.val_));
        else
            this->store(unexpect, unexpected(std::move(rhs.internal_get_unexpect().value())));
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
            this->store(std::move(rhs.val_));
        else
            this->store(unexpect, unexpected(std::move(rhs.internal_get_unexpect().value())));
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
                this->store(unexpect, unexpected(rhs.internal_get_unexpect().value()));
            }
            else if constexpr(std::is_nothrow_copy_constructible_v<E>) {
                this->internal_get_value().~T();
                this->store(unexpect, unexpected(rhs.internal_get_unexpect().value()));
            }
            else if constexpr(std::is_nothrow_move_constructible_v<E>) {
                unexpected<E> tmp = unexpected(rhs.internal_get_unexpect().value());
                this->internal_get_value().~T();
                this->store(unexpect, std::move(tmp));
            }
            else {
                T tmp = this->internal_get_value();
                this->internal_get_value().~T();
                try {
                    this->store(unexpect, unexpected(rhs.internal_get_unexpect().value()));
                }
                catch(...) {
                    this->store(std::move(tmp));
                    throw;
                }
            }
        }

        else {
            if constexpr(std::is_void_v<T>) {
                this->internal_get_unexpect().~unexpected<E>();
                this->store();
            }
            else if constexpr(std::is_nothrow_copy_constructible_v<T>) {
                this->internal_get_unexpect().~unexpected<E>();
                this->store(rhs.internal_get_value());
            }
            else if constexpr(std::is_nothrow_move_constructible_v<T>) {
                T tmp = rhs.internal_get_value();
                this->internal_get_unexpect().~unexpected<E>();
                this->store(std::move(tmp));
            }
            else {
                unexpected<E> tmp = unexpected(this->internal_get_unexpect().value());
                this->internal_get_unexpect().~unexpected<E>();
                try {
                    this->store(rhs.internal_get_value());
                }
                catch(...) {
                    this->store(unexpect, std::move(tmp));
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
                this->store(unexpect, unexpected(std::move(rhs.internal_get_unexpect().value())));
            }
            else if constexpr(std::is_nothrow_move_constructible_v<E>) {
                this->internal_get_value().~T();
                this->store(unexpect, unexpected(std::move(rhs.internal_get_unexpect().value())));
            }
            else {
                T tmp = std::move(this->internal_get_value());
                this->internal_get_value().~T();
                try {
                    this->store(unexpect,
                        unexpected(std::move(rhs.internal_get_unexpect().value()))
                    );
                }
                catch(...) {
                    this->store(std::move(tmp));
                    throw;
                }
            }
        }

        else {
            if constexpr(std::is_void_v<T>) {
                this->internal_get_unexpect().~unexpected<E>();
                this->store();
            }
            else if constexpr(std::is_nothrow_move_constructible_v<T>) {
                this->internal_get_unexpect().~unexpected<E>();
                this->store(std::move(rhs.internal_get_value()));
            }
            else {
                unexpected<E> tmp = unexpected(std::move(this->internal_get_unexpect().value()));
                this->internal_get_unexpect().~unexpected<E>();
                try {
                    this->store(std::move(rhs.internal_get_value()));
                }
                catch(...) {
                    this->store(unexpect, std::move(tmp));
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
class expected_interface_base : impl::expected_move_assign_base<T,E> {
    static_assert(!std::is_reference_v<T>, "T must not be reference");
    static_assert(!std::is_reference_v<E>, "E must not be reference");
    static_assert(!std::is_same_v<T, std::remove_cv_t<unexpected<E>>>,
                  "T must not be unexpected<E>");
    static_assert(!std::is_same_v<unexpect_t, T>, "T must not be unexpect_t");
    static_assert(!std::is_same_v<unexpect_t, E>, "E must not be unexpect_t");
    using base_t = expected_move_assign_base<T,E>;

    public:
        using base_t::base_t;
        expected_interface_base() = default;
        expected_interface_base(expected_interface_base const&) = default;
        expected_interface_base(expected_interface_base&&) = default;
        expected_interface_base& operator=(expected_interface_base const&) = default;
        expected_interface_base& operator=(expected_interface_base&&) = default;

        /* Conditionally explicit unexpected lvalue ctor */
        template <typename G = E, typename EE = E,
                  typename = std::enable_if_t<std::is_constructible_v<E, G const&>>,
                  typename = std::enable_if_t<std::is_convertible_v<G const&, E>>>
        constexpr expected_interface_base(unexpected<G> const&);

        template <typename G = E, typename EE = E,
                  typename = std::enable_if_t<
                      std::is_constructible_v<E, G const&> &&
                     !std::is_convertible_v<G const&, E>
                  >>
        constexpr explicit expected_interface_base(unexpected<G> const&);

        /* Conditionally explicit unexpected rvalue ctor */
        template <typename G = E, typename EE = E,
                  typename = std::enable_if_t<std::is_constructible_v<E, G&&>>,
                  typename = std::enable_if_t<std::is_convertible_v<G&&, E>>>
        constexpr expected_interface_base(unexpected<G>&&)
                        noexcept(std::is_nothrow_constructible_v<E, G&&>);

        template <typename G = E, typename EE = E,
                  typename = std::enable_if_t<
                      std::is_constructible_v<E, G&&> &&
                     !std::is_convertible_v<G&&, E>
                  >>
        constexpr explicit expected_interface_base(unexpected<G>&&)
                        noexcept(std::is_nothrow_constructible_v<E, G&&>);

        template <typename... Args,
                  typename = std::enable_if_t<
                      std::is_constructible_v<E, Args&&...>
                  >>
        constexpr explicit expected_interface_base(unexpect_t, Args&&...);

        template <typename U, typename... Args,
                  typename = std::enable_if_t<
                      std::is_constructible_v<E, std::initializer_list<U>&, Args&&...>
                  >>
        constexpr explicit
            expected_interface_base(unexpect_t, std::initializer_list<U>, Args&&...);

        template <typename G = E, typename EE = E,
                  typename = std::enable_if_t<
                      std::is_nothrow_copy_constructible_v<EE> &&
                      std::is_move_assignable_v<EE>
                  >>
        expected_interface_base& operator=(unexpected<G> const&);

        template <typename G = E, typename EE = E,
                  typename = std::enable_if_t<
                      std::is_nothrow_move_constructible_v<EE> &&
                      std::is_move_assignable_v<EE>
                  >>
        expected_interface_base& operator=(unexpected<G>&&);

        constexpr explicit operator bool() const noexcept;
        constexpr bool has_value() const noexcept;

        constexpr E& error() &;
        constexpr E const& error() const &;
        constexpr E&& error() &&;
        constexpr E const&& error() const &&;

        template <typename T1, typename E1, typename E2>
        friend constexpr bool operator==(expected<T1, E1> const&, unexpected<E2> const&);
        template <typename T1, typename E1, typename E2>
        friend constexpr bool operator==(unexpected<E2> const&, expected<T1, E1> const&);
        template <typename T1, typename E1, typename E2>
        friend constexpr bool operator!=(expected<T1, E1> const&, unexpected<E2> const&);
        template <typename T1, typename E1, typename E2>
        friend constexpr bool operator!=(unexpected<E2> const&, expected<T1, E1> const&);

    protected:
        template <typename... Args>
        void store(Args&&... args);

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
template <typename G, typename, typename, typename>
constexpr expected_interface_base<T,E>::expected_interface_base(unexpected<G> const& e)
    : base_t(no_init) {
    this->store(unexpect, e);
}

template <typename T, typename E>
template <typename G, typename, typename>
constexpr expected_interface_base<T,E>::expected_interface_base(unexpected<G> const& e)
    : base_t(no_init) {
    this->store(unexpect, e);
}

template <typename T, typename E>
template <typename G, typename, typename, typename>
constexpr expected_interface_base<T,E>::expected_interface_base(unexpected<G>&& e)
                noexcept(std::is_nothrow_constructible_v<E, G&&>) : base_t(no_init) {
    this->store(unexpect, std::move(e));
}

template <typename T, typename E>
template <typename G, typename, typename>
constexpr expected_interface_base<T,E>::expected_interface_base(unexpected<G>&& e)
                noexcept(std::is_nothrow_constructible_v<E, G&&>) : base_t(no_init) {
    this->store(unexpect, std::move(e));
}

template <typename T, typename E>
template <typename... Args, typename>
constexpr expected_interface_base<T,E>::expected_interface_base(unexpect_t, Args&&... args)
    : base_t(no_init) {
    this->store(unexpect, std::forward<Args>(args)...);
}

template <typename T, typename E>
template <typename U, typename... Args, typename>
constexpr expected_interface_base<T,E>::expected_interface_base(unexpect_t,
                std::initializer_list<U> il, Args&&... args) : base_t(no_init) {

    this->store(unexpect, il, std::forward<Args>(args)...);
}

template <typename T, typename E>
template <typename G, typename, typename>
expected_interface_base<T,E>&
expected_interface_base<T,E>::operator=(unexpected<G> const& e) {
    if(!bool(*this))
        this->internal_get_unexpect() = unexpected(e.internal_get_unexpect().value());

    else {
        if constexpr(!std::is_void_v<T>)
            this->internal_get_value().~T();

        this->store(unexpect, unexpected(e.internal_get_unexpect().value()));
    }

    return *this;
}

template <typename T, typename E>
template <typename G, typename, typename>
expected_interface_base<T,E>& expected_interface_base<T,E>::operator=(unexpected<G>&& e) {
    if(!bool(*this))
        this->internal_get_unexpect() = unexpected(std::move(e.internal_get_unexpect().value()));

    else {
        if constexpr(!std::is_void_v<T>)
            this->internal_get_value().~T();

        this->store(unexpect, unexpected(std::move(e.internal_get_unexpect().value())));
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
void expected_interface_base<T,E>::store(Args&&... args) {
    base_t::store(std::forward<Args>(args)...);
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

} /* namespace impl */


/* Primary template (T is not void) */
template <typename T, typename E>
class expected : public impl::expected_interface_base<T,E> {
    using base_t = impl::expected_interface_base<T,E>;
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
                  typename = std::enable_if_t<
                      impl::expected_enable_forwarding_ref_ctor_v<TT, EE, U>
                  >,
                  typename = std::enable_if_t<
                      impl::expected_enable_implicit_forwarding_ref_ctor_v<TT, U>
                  >>
        constexpr expected(U&&);

        template <typename U = T, typename TT = T, typename EE = E,
                  typename = std::enable_if_t<
                      impl::expected_enable_forwarding_ref_ctor_v<TT, EE, U> &&
                     !impl::expected_enable_implicit_forwarding_ref_ctor_v<TT, U>
                  >>
        constexpr explicit expected(U&&);


        /* Conditionally explicit conversion constructors */
        template <typename U, typename G, typename TT = T, typename EE = E,
                  typename = std::enable_if_t<
                      impl::expected_enable_copy_conversion_v<TT, EE, U, G>
                  >,
                  typename = std::enable_if_t<
                      impl::expected_enable_implicit_copy_conversion_v<TT, EE, U, G>
                  >>
        constexpr expected(expected<U, G> const& rhs);

        template <typename U, typename G, typename TT = T, typename EE = E,
                  typename = std::enable_if_t<
                      impl::expected_enable_copy_conversion_v<TT, EE, U, G> &&
                     !impl::expected_enable_implicit_copy_conversion_v<TT, EE, U, G>
                  >>
        constexpr explicit expected(expected<U, G> const& rhs);

        template <typename U, typename G, typename TT = T, typename EE = E,
                  typename = std::enable_if_t<
                      impl::expected_enable_move_conversion_v<TT, EE, U, G>
                  >,
                  typename = std::enable_if_t<
                      impl::expected_enable_implicit_move_conversion_v<TT, EE, U, G>
                  >>
        constexpr expected(expected<U,G>&& rhs);

        template <typename U, typename G, typename TT = T, typename EE = E,
                  typename = std::enable_if_t<
                      impl::expected_enable_move_conversion_v<TT, EE, U, G> &&
                     !impl::expected_enable_implicit_move_conversion_v<TT, EE, U, G>
                  >>
        constexpr explicit expected(expected<U, G>&& rhs);

        template <typename... Args,
                  typename = std::enable_if_t<
                      std::is_constructible_v<T, Args...>
                  >>
        constexpr explicit expected(in_place_t, Args&&...);

        template <typename U, typename... Args,
                  typename = std::enable_if_t<
                      std::is_constructible_v<T, std::initializer_list<U>&, Args...>
                  >>
        constexpr explicit expected(in_place_t, std::initializer_list<U>, Args&&...);

        template <typename U = T, typename TT = T, typename EE = E,
                  typename = std::enable_if_t<
                      impl::expected_enable_unary_forwarding_assign_v<TT, EE, U>
                   >>
        expected& operator=(U&&);

        template <typename... Args,
                  typename = std::enable_if_t<
                      std::is_nothrow_constructible_v<T, Args&&...>
                  >>
        T& emplace(Args&&...);

        template <typename U, typename... Args,
                  typename = std::enable_if_t<
                      std::is_nothrow_constructible_v<T, std::initializer_list<U>&, Args&&...>
                  >>
        T& emplace(std::initializer_list<U>&, Args&&...);

        template <typename TT = T, typename EE = E,
                  typename = std::enable_if_t<
                      impl::expected_enable_swap_v<TT, EE>
                  >>
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
        constexpr T value_or(U&&) const &;
        template <typename U>
        constexpr T value_or(U&&) &&;

        template <typename T1, typename E1, typename T2, typename E2>
        friend constexpr bool operator==(expected<T1, E1> const&,
                                         expected<T2, E2> const&);

        template <typename T1, typename E1, typename T2, typename E2>
        friend constexpr bool operator!=(expected<T1, E1> const&,
                                         expected<T2, E2> const&);

        template <typename T1, typename E1, typename T2>
        friend constexpr bool operator==(expected<T1, E1> const&, T2 const&);
        template <typename T1, typename E1, typename T2>
        friend constexpr bool operator==(T2 const&, expected<T1, E1> const&);
        template <typename T1, typename E1, typename T2>
        friend constexpr bool operator!=(expected<T1, E1> const&, T2 const&);
        template <typename T1, typename E1, typename T2>
        friend constexpr bool operator!=(T2 const&, expected<T1, E1> const&);

        #ifdef VIEN_EXPECTED_EXTENDED
        template <typename F>
        expected<std::decay_t<std::invoke_result_t<F,T>>, E>
            map(F&&) &;
        template <typename F>
        expected<std::decay_t<std::invoke_result_t<F,T>>, E>
            map(F&&) const &;
        template <typename F>
        expected<std::decay_t<std::invoke_result_t<F,T>>, E>
            map(F&&) &&;
        template <typename F>
        expected<std::decay_t<std::invoke_result_t<F,T>>, E>
             map(F&&) const &&;

        template <typename F, typename TT = T,
                  typename = std::enable_if_t<
                      impl::is_container_v<TT>
                  >>
        expected<impl::rebind_t<T, std::decay_t<
                                        std::invoke_result_t<F, impl::value_type_of_t<T>>>>,
                 E>
            map_range(F&&) &;

        template <typename F, typename TT = T,
                  typename = std::enable_if_t<
                      impl::is_container_v<TT>
                  >>
        expected<impl::rebind_t<T, std::decay_t<
                                        std::invoke_result_t<F, impl::value_type_of_t<T>>>>,
                 E>
            map_range(F&&) const &;

        template <typename F, typename TT = T,
                  typename = std::enable_if_t<
                      impl::is_container_v<TT>
                  >>
        expected<impl::rebind_t<T, std::decay_t<
                                        std::invoke_result_t<F, impl::value_type_of_t<T>>>>,
                 E>
            map_range(F&&) &&;

        template <typename F, typename TT = T,
                  typename = std::enable_if_t<
                      impl::is_container_v<TT>
                  >>
        expected<impl::rebind_t<T, std::decay_t<
                                        std::invoke_result_t<F, impl::value_type_of_t<T>>>>,
                 E>
            map_range(F&&) const &&;

        template <typename F>
        expected<T, std::decay_t<std::invoke_result_t<F,E>>>
            map_error(F&&) &;
        template <typename F>
        expected<T, std::decay_t<std::invoke_result_t<F,E>>>
            map_error(F&&) const &;
        template <typename F>
        expected<T, std::decay_t<std::invoke_result_t<F,E>>>
            map_error(F&&) &&;
        template <typename F>
        expected<T, std::decay_t<std::invoke_result_t<F,E>>>
            map_error(F&&) const &&;

        template <typename F>
        expected and_then(F&&) &;
        template <typename F>
        expected and_then(F&&) const &;
        template <typename F>
        expected and_then(F&&) &&;
        template <typename F>
        expected and_then(F&&) const &&;
        #endif
};

template <typename T, typename E>
template <typename U, typename, typename, typename, typename>
constexpr expected<T,E>::expected(U&& v) : base_t(impl::no_init) {
    this->store(std::forward<U>(v));
}

template <typename T, typename E>
template <typename U, typename, typename, typename>
constexpr expected<T,E>::expected(U&& v) : base_t(impl::no_init) {
    this->store(std::forward<U>(v));
}

template <typename T, typename E>
template <typename U, typename G, typename, typename, typename, typename>
constexpr expected<T,E>::expected(expected<U, G> const& rhs) : base_t(impl::no_init) {
    if(bool(rhs))
        this->store(rhs.value());
    else
        this->store(unexpect, unexpected(rhs.error()));
}

template <typename T, typename E>
template <typename U, typename G, typename, typename, typename>
constexpr expected<T,E>::expected(expected<U, G> const& rhs) : base_t(impl::no_init) {
    if(bool(rhs))
        this->store(rhs.value());
    else
        this->store(unexpect, unexpected(rhs.error()));
}

template <typename T, typename E>
template <typename U, typename G, typename, typename, typename, typename>
constexpr expected<T,E>::expected(expected<U, G>&& rhs) : base_t(impl::no_init) {
    if(bool(rhs))
        this->store(std::move(rhs.value()));
    else
        this->store(unexpect, unexpected(std::move(rhs.error())));
}

template <typename T, typename E>
template <typename U, typename G, typename, typename, typename>
constexpr expected<T,E>::expected(expected<U, G>&& rhs) : base_t(impl::no_init) {
    if(bool(rhs))
        this->store(std::move(rhs.value()));
    else
        this->store(unexpect, unexpected(std::move(rhs.error())));
}

template <typename T, typename E>
template <typename... Args, typename>
constexpr expected<T,E>::expected(in_place_t, Args&&... args) : base_t(impl::no_init) {
    this->store(std::forward<Args>(args)...);
}

template <typename T, typename E>
template <typename U, typename... Args, typename>
constexpr expected<T,E>::expected(in_place_t, std::initializer_list<U> il, Args&&... args)
    : base_t(impl::no_init) {
    this->store(il, std::forward<Args>(args)...);
}

template <typename T, typename E>
template <typename U, typename, typename, typename>
expected<T,E>& expected<T,E>::operator=(U&& v) {
    if(bool(*this))
        this->internal_get_value() = std::forward<U>(v);

    else {
        if constexpr(std::is_nothrow_constructible_v<T, U&&>) {
            this->internal_get_unexpect().~unexpected<E>();
            this->store(std::forward<U>(v));
        }
        else {
            unexpected<E> tmp = unexpected(std::move(this->internal_get_unexpect()));
            this->internal_get_unexpect().~unexpected<E>();
            try {
                this->store(std::forward<U>(v));
            }
            catch(...) {
                this->store(unexpect, std::move(tmp));
                throw;
            }
        }
    }

    return *this;
}

template <typename T, typename E>
template <typename... Args, typename>
T& expected<T,E>::emplace(Args&&... args) {
    if(bool(*this))
        this->store(std::forward<Args>(args)...);
    else {
        if constexpr(std::is_nothrow_constructible_v<T, Args&&...>) {
            this->internal_get_unexpect().~unexpected<E>();
            this->store(std::forward<Args>(args)...);
        }
        else if constexpr(std::is_nothrow_constructible_v<T>) {
            T tmp(std::forward<Args>(args)...);
            this->internal_get_unexpect().~unexpected<E>();
            this->store(std::move(tmp));
        }
        else {
            unexpected<E> tmp(std::move(this->internal_get_unexpect().value()));
            this->internal_get_unexpect().~unexpected<E>();
            try {
                this->store(std::forward<Args>(args)...);
            }
            catch(...) {
                this->store(unexpect, std::move(tmp));
                throw;
            }
        }
    }

    return this->internal_get_value();
}

template <typename T, typename E>
template <typename U, typename... Args, typename>
T& expected<T,E>::emplace(std::initializer_list<U>& il, Args&&... args) {
    if(bool(*this))
        this->store(il, std::forward<Args>(args)...);

    else {
        if constexpr(std::is_nothrow_constructible_v<T, std::initializer_list<U>&, Args&&...>) {
            this->internal_get_unexpect().~unexpected<E>();
            this->store(il, std::forward<Args>(args)...);
        }
        else if constexpr(std::is_nothrow_move_constructible_v<T>) {
            T tmp(il, std::forward<Args>(args)...);
            this->internal_get_unexpect().~unexpected<E>();
            this->store(std::move(tmp));
        }
        else {
            unexpected<E> tmp(std::move(this->internal_get_unexpect().value()));
            this->internal_get_unexpect().~unexpected<E>();
            try {
                this->store(il, std::forward<Args>(args)...);
            }
            catch(...) {
                this->store(unexpect, std::move(tmp));
                throw;
            }
        }
    }

    return this->internal_get_value();
}

template <typename T, typename E>
template <typename, typename, typename>
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
                rhs.store(std::move(this->internal_get_value()));
                this->internal_get_value().~T();
                this->store(unexpect, std::move(tmp));
            }
            catch(...) {
                rhs.store(unexpect, std::move(tmp));
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
                this->store(unexpect, unexpected(std::move(rhs.internal_get_unexpect().value())));
                rhs.internal_get_unexpect().~unexpected<E>();
                rhs.store(std::move(tmp));
            }
            catch(...) {
                this->store(std::move(tmp));
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
expected<std::decay_t<std::invoke_result_t<F,T>>, E>
expected<T,E>::map(F&& f) & {

    using result_t = impl::expected_mapped_type_t<T,E,F>;
    using value_type = typename result_t::value_type;

    if constexpr(std::is_void_v<value_type>) {
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
expected<std::decay_t<std::invoke_result_t<F,T>>, E>
expected<T,E>::map(F&& f) const & {

    using result_t = impl::expected_mapped_type_t<T,E,F>;
    using value_type = typename result_t::value_type;

    if constexpr(std::is_void_v<value_type>) {
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
expected<std::decay_t<std::invoke_result_t<F,T>>, E>
expected<T,E>::map(F&& f) && {

    using result_t = impl::expected_mapped_type_t<T,E,F>;
    using value_type = typename result_t::value_type;

    if constexpr(std::is_void_v<value_type>) {
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
expected<std::decay_t<std::invoke_result_t<F,T>>, E>
expected<T,E>::map(F&& f) const && {

    using result_t = impl::expected_mapped_type_t<T,E,F>;
    using value_type = typename result_t::value_type;

    if constexpr(std::is_void_v<value_type>) {
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
template <typename F, typename, typename>
[[nodiscard]]
expected<
    impl::rebind_t<T, std::decay_t<std::invoke_result_t<F, impl::value_type_of_t<T>>>>, E
>
expected<T,E>::map_range(F&& f) & {
    using container_t =
        impl::rebind_t<T, std::decay_t<std::invoke_result_t<F, impl::value_type_of_t<T>>>>;

    using result_t = expected<container_t, E>;

    if(!bool(*this))
        return result_t(unexpect, this->error());

    container_t tmp;
    std::transform(std::begin(**this), std::end(**this),
                   impl::insert_iterator_t<container_t>(tmp),
                   std::forward<F>(f));

    return result_t(std::move(tmp));
}

template <typename T, typename E>
template <typename F, typename, typename>
[[nodiscard]]
expected<
    impl::rebind_t<T, std::decay_t<std::invoke_result_t<F, impl::value_type_of_t<T>>>>, E
>
expected<T,E>::map_range(F&& f) const & {
    using container_t =
        impl::rebind_t<T, std::decay_t<std::invoke_result_t<F, impl::value_type_of_t<T>>>>;

    using result_t = expected<container_t, E>;

    if(!bool(*this))
        return result_t(unexpect, this->error());

    container_t tmp;
    std::transform(std::begin(**this), std::end(**this),
                   impl::insert_iterator_t<container_t>(tmp),
                   std::forward<F>(f));

    return result_t(std::move(tmp));
}

template <typename T, typename E>
template <typename F, typename, typename>
[[nodiscard]]
expected<
    impl::rebind_t<T, std::decay_t<std::invoke_result_t<F, impl::value_type_of_t<T>>>>, E
>
expected<T,E>::map_range(F&& f) && {
    using container_t =
        impl::rebind_t<T, std::decay_t<std::invoke_result_t<F, impl::value_type_of_t<T>>>>;

    using result_t = expected<container_t, E>;

    if(!bool(*this))
        return result_t(unexpect, std::move(this->error()));

    /* T and container_t are the same, transform **this and move
     * **this to new instance */
    if constexpr(std::is_same_v<T, container_t>) {
        std::transform(std::begin(**this), std::end(**this),
                       std::begin(**this), std::forward<F>(f));

        return result_t(std::move(**this));
    }
    /* T and container_t are not the same, must create new container */
    else {
        container_t tmp;
        std::transform(std::begin(**this), std::end(**this),
                       impl::insert_iterator_t<container_t>(tmp),
                        std::forward<F>(f));

        return result_t(std::move(tmp));
    }
}

template <typename T, typename E>
template <typename F, typename, typename>
[[nodiscard]]
expected<
    impl::rebind_t<T, std::decay_t<std::invoke_result_t<F, impl::value_type_of_t<T>>>>, E
>
expected<T,E>::map_range(F&& f) const && {
    using container_t =
        impl::rebind_t<T, std::decay_t<std::invoke_result_t<F, impl::value_type_of_t<T>>>>;

    using result_t = expected<container_t, E>;

    if(!bool(*this))
        return result_t(unexpect, std::move(this->error()));

    if constexpr(std::is_same_v<T, container_t>) {
        std::transform(std::begin(**this), std::end(**this),
                       std::begin(**this), std::forward<F>(f));

        return result_t(std::move(**this));
    }
    else {
        container_t tmp;
        std::transform(std::begin(**this), std::end(**this),
                       impl::insert_iterator_t<container_t>(tmp),
                        std::forward<F>(f));

        return result_t(std::move(tmp));
    }
}

template <typename T, typename E>
template <typename F>
[[nodiscard]]
expected<T, std::decay_t<std::invoke_result_t<F,E>>>
expected<T,E>::map_error(F&& f) & {

    using result_t = impl::expected_mapped_error_type_t<T,E,F>;

    return bool(*this) ?
            result_t(**this) :
            result_t(unexpect, std::invoke(std::forward<F>(f), this->error()));
}

template <typename T, typename E>
template <typename F>
[[nodiscard]]
expected<T, std::decay_t<std::invoke_result_t<F,E>>>
expected<T,E>::map_error(F&& f) const & {

    using result_t = impl::expected_mapped_error_type_t<T,E,F>;

    return bool(*this) ?
            result_t(**this) :
            result_t(unexpect, std::invoke(std::forward<F>(f), this->error()));
}

template <typename T, typename E>
template <typename F>
[[nodiscard]]
expected<T, std::decay_t<std::invoke_result_t<F,E>>>
expected<T,E>::map_error(F&& f) && {

    using result_t = impl::expected_mapped_error_type_t<T,E,F>;

    return bool(*this) ?
            result_t(std::move(**this)) :
            result_t(unexpect, std::invoke(std::forward<F>(f), std::move(this->error())));
}

template <typename T, typename E>
template <typename F>
[[nodiscard]]
expected<T, std::decay_t<std::invoke_result_t<F,E>>>
expected<T,E>::map_error(F&& f) const && {

    using result_t = impl::expected_mapped_error_type_t<T,E,F>;

    return bool(*this) ?
            result_t(std::move(**this)) :
            result_t(unexpect, std::invoke(std::forward<F>(f), std::move(this->error())));
}

template <typename T, typename E>
template <typename F>
[[nodiscard]]
expected<T,E> expected<T,E>::and_then(F&& f) & {
    return bool(*this) ?
            expected(std::invoke(std::forward<F>(f), **this)) :
            expected(unexpect, this->error());
}

template <typename T, typename E>
template <typename F>
[[nodiscard]]
expected<T,E> expected<T,E>::and_then(F&& f) const & {
    return bool(*this) ?
            expected(std::invoke(std::forward<F>(f), **this)) :
            expected(unexpect, this->error());
}

template <typename T, typename E>
template <typename F>
[[nodiscard]]
expected<T,E> expected<T,E>::and_then(F&& f) && {
    return bool(*this) ?
            expected(std::invoke(std::forward<F>(f), std::move(**this))) :
            expected(unexpect, std::move(this->error()));
}

template <typename T, typename E>
template <typename F>
[[nodiscard]]
expected<T,E> expected<T,E>::and_then(F&& f) const && {
    return bool(*this) ?
            expected(std::invoke(std::forward<F>(f), std::move(**this))) :
            expected(unexpect, std::move(this->error()));
}

#endif

template <typename E>
class expected<void, E> : public impl::expected_interface_base<void,E> {
    using base_t = impl::expected_interface_base<void,E>;
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
                  typename = std::enable_if_t<
                      impl::expected_enable_copy_conversion_v<TT, EE, U, G>
                  >,
                  typename = std::enable_if_t<
                      impl::expected_enable_implicit_copy_conversion_v<TT, EE, U, G>
                  >>
        constexpr expected(expected<U, G> const& rhs);

        template <typename U, typename G, typename TT = void, typename EE = E,
                  typename = std::enable_if_t<
                      impl::expected_enable_copy_conversion_v<TT, EE, U, G> &&
                     !impl::expected_enable_implicit_copy_conversion_v<TT, EE, U, G>
                  >>
        constexpr explicit expected(expected<U, G> const& rhs);

        template <typename U, typename G, typename TT = void, typename EE = E,
                  typename = std::enable_if_t<
                      impl::expected_enable_move_conversion_v<TT, EE, U, G>
                  >,
                  typename = std::enable_if_t<
                      impl::expected_enable_implicit_move_conversion_v<TT, EE, U, G>
                  >>
        constexpr expected(expected<U,G>&& rhs);

        template <typename U, typename G, typename TT = void, typename EE = E,
                  typename = std::enable_if_t<
                      impl::expected_enable_move_conversion_v<TT, EE, U, G> &&
                     !impl::expected_enable_implicit_move_conversion_v<TT, EE, U, G>
                  >>
        constexpr explicit expected(expected<U, G>&& rhs);

        template <typename... Args,
                  typename = std::enable_if_t<sizeof...(Args) == 0>>
        constexpr explicit expected(in_place_t, Args&&...);

        void emplace();

        template <typename TT = void, typename EE = E,
                  typename = std::enable_if_t<
                      impl::expected_enable_swap_v<TT, EE>
                  >>
        void swap(expected& rhs) noexcept(std::is_nothrow_move_constructible_v<E> &&
                                          std::is_nothrow_swappable_v<E>);

        constexpr void value() const;

        template <typename E1,typename E2>
        friend constexpr bool operator==(expected<void, E1> const&,
                                         expected<void, E2> const&);

        template <typename E1, typename E2>
        friend constexpr bool operator!=(expected<void, E1> const&,
                                         expected<void, E2> const&);

        #ifdef VIEN_EXPECTED_EXTENDED
        template <typename F>
        expected<std::decay_t<std::invoke_result_t<F>>, E>
            map(F&&) &;
        template <typename F>
        expected<std::decay_t<std::invoke_result_t<F>>, E>
            map(F&&) const &;
        template <typename F>
        expected<std::decay_t<std::invoke_result_t<F>>, E>
            map(F&&) &&;
        template <typename F>
        expected<std::decay_t<std::invoke_result_t<F>>, E>
            map(F&&) const &&;

        template <typename F>
        expected<void, std::decay_t<std::invoke_result_t<F,E>>>
            map_error(F&&) &;
        template <typename F>
        expected<void, std::decay_t<std::invoke_result_t<F,E>>>
            map_error(F&&) const &;
        template <typename F>
        expected<void, std::decay_t<std::invoke_result_t<F,E>>>
            map_error(F&&) &&;
        template <typename F>
        expected<void, std::decay_t<std::invoke_result_t<F,E>>>
            map_error(F&&) const &&;
        #endif
};

template <typename E>
template <typename U, typename G, typename, typename, typename, typename>
constexpr expected<void,E>::expected(expected<U, G> const& rhs) : base_t(impl::no_init) {
    if(bool(rhs))
        this->store();
    else
        this->store(unexpect, unexpected(rhs.error()));
}

template <typename E>
template <typename U, typename G, typename, typename, typename>
constexpr expected<void,E>::expected(expected<U, G> const& rhs) : base_t(impl::no_init) {
    if(bool(rhs))
        this->store();
    else
        this->store(unexpect, unexpected(rhs.error()));
}

template <typename E>
template <typename U, typename G, typename, typename, typename, typename>
constexpr expected<void,E>::expected(expected<U, G>&& rhs) : base_t(impl::no_init) {
    if(bool(rhs))
        this->store();
    else
        this->store(unexpect, unexpected(std::move(rhs.error())));
}

template <typename E>
template <typename U, typename G, typename, typename, typename>
constexpr expected<void,E>::expected(expected<U, G>&& rhs) : base_t(impl::no_init) {
    if(bool(rhs))
        this->store();
    else
        this->store(unexpect, unexpected(std::move(rhs.error())));
}

template <typename E>
template <typename... Args, typename>
constexpr expected<void, E>::expected(in_place_t, Args&&...) : base_t(impl::no_init) {
    this->store();
}

template <typename E>
void expected<void,E>::emplace() {
    if(!bool(*this)) {
        this->internal_get_unexpect().~unexpected<E>();
        this->store();
    }
}

template <typename E>
template <typename, typename, typename>
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
        this->store(unexpect, unexpected(std::move(rhs).internal_get_unexpect().value()));
        rhs.internal_get_unexpect().~unexpected<E>();
        rhs.store();
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
expected<std::decay_t<std::invoke_result_t<F>>, E>
expected<void,E>::map(F&& f) & {

    using result_t = impl::expected_mapped_type_t<void,E,F>;
    using value_type = typename result_t::value_type;

    if constexpr(std::is_void_v<value_type>) {
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
expected<std::decay_t<std::invoke_result_t<F>>, E>
expected<void,E>::map(F&& f) const & {

    using result_t = impl::expected_mapped_type_t<void,E,F>;
    using value_type = typename result_t::value_type;

    if constexpr(std::is_void_v<value_type>) {
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
expected<std::decay_t<std::invoke_result_t<F>>, E>
expected<void,E>::map(F&& f) && {

    using result_t = impl::expected_mapped_type_t<void,E,F>;
    using value_type = typename result_t::value_type;

    if constexpr(std::is_void_v<value_type>) {
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
expected<std::decay_t<std::invoke_result_t<F>>, E>
expected<void,E>::map(F&& f) const && {

    using result_t = impl::expected_mapped_type_t<void,E,F>;
    using value_type = typename result_t::value_type;

    if constexpr(std::is_void_v<value_type>) {
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
expected<void, std::decay_t<std::invoke_result_t<F,E>>>
expected<void, E>::map_error(F&& f) & {

    using result_t = impl::expected_mapped_error_type_t<void,E,F>;

    return bool(*this) ?
            result_t{} :
            result_t(unexpect, std::invoke(std::forward<F>(f), this->error()));
}

template <typename E>
template <typename F>
[[nodiscard]]
expected<void, std::decay_t<std::invoke_result_t<F,E>>>
expected<void, E>::map_error(F&& f) const & {

    using result_t = impl::expected_mapped_error_type_t<void,E,F>;

    return bool(*this) ?
            result_t{} :
            result_t(unexpect, std::invoke(std::forward<F>(f), this->error()));
}

template <typename E>
template <typename F>
[[nodiscard]]
expected<void, std::decay_t<std::invoke_result_t<F,E>>>
expected<void, E>::map_error(F&& f) && {

    using result_t = impl::expected_mapped_error_type_t<void,E,F>;

    return bool(*this) ?
            result_t{} :
            result_t(unexpect, std::invoke(std::forward<F>(f), std::move(this->error())));
}

template <typename E>
template <typename F>
[[nodiscard]]
expected<void, std::decay_t<std::invoke_result_t<F,E>>>
expected<void, E>::map_error(F&& f) const && {

    using result_t = impl::expected_mapped_error_type_t<void,E,F>;

    return bool(*this) ?
            result_t{} :
            result_t(unexpect, std::invoke(std::forward<F>(f), std::move(this->error())));
}

#endif

template <typename E>
class unexpected {
    static_assert(!std::is_same_v<E, void>, "E cannot be void");
    public:
        template <typename Err = E, typename =
                std::enable_if_t<std::is_constructible_v<E, Err&&>>>
        constexpr explicit unexpected(Err&&);
        template <typename... Args, typename =
                std::enable_if_t<std::is_constructible_v<E, Args&&...>>>
        constexpr explicit unexpected(in_place_t, Args&&...);
        template <typename U, typename... Args, typename =
                std::enable_if_t<std::is_constructible_v<
                        E, std::initializer_list<U>, Args&&...>>>
        constexpr explicit unexpected(in_place_t, std::initializer_list<U>, Args&&...);

        constexpr unexpected(unexpected const&) = default;
        constexpr unexpected(unexpected&&) = default;

        /* Non-explicit conversion ctor templates available iff is_convertible_v<Err, E>
         * is true. If not, the explicit versions are used */
        template <typename Err, typename = std::enable_if_t<std::is_convertible_v<Err, E>>>
        constexpr unexpected(unexpected<Err> const&);
        template <typename Err>
        constexpr explicit unexpected(unexpected<Err> const&);
        template <typename Err, typename = std::enable_if_t<std::is_convertible_v<Err, E>>>
        constexpr unexpected(unexpected<Err>&&);
        template <typename Err>
        constexpr explicit unexpected(unexpected<Err>&&);

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

        template <typename Err = E,
                  typename = std::enable_if_t<
                      std::is_swappable_v<Err>
                  >>
        void swap(unexpected&) noexcept(std::is_nothrow_swappable_v<Err>);

        template <typename E1, typename E2>
        friend constexpr bool operator==(unexpected<E1> const&, unexpected<E2> const&);
        template <typename E1, typename E2>
        friend constexpr bool operator!=(unexpected<E1> const&, unexpected<E2> const&);

    private:
        E val_;
};

template <typename E>
unexpected(E) -> unexpected<E>;

template <typename E>
template <typename Err, typename>
constexpr unexpected<E>::unexpected(Err&& err)
    : val_(std::forward<Err>(err)) { }

template <typename E>
template <typename... Args, typename>
constexpr unexpected<E>::unexpected(in_place_t, Args&&... args)
    : val_(std::forward<Args>(args)...) { }

template <typename E>
template <typename U, typename... Args, typename>
constexpr unexpected<E>::unexpected(in_place_t, std::initializer_list<U> il, Args&&... args)
    : val_(il, std::forward<Args>(args)...) { }

template <typename E>
template <typename Err, typename>
constexpr unexpected<E>::unexpected(unexpected<Err> const& e)
    : val_(e.value()) { }

template <typename E>
template <typename Err>
constexpr unexpected<E>::unexpected(unexpected<Err> const& e)
    : val_(e.value()) { }

template <typename E>
template <typename Err, typename>
constexpr unexpected<E>::unexpected(unexpected<Err>&& e)
    : val_(std::move(e.value())) { }

template <typename E>
template <typename Err>
constexpr unexpected<E>::unexpected(unexpected<Err>&& e)
    : val_(std::move(e.value())) { }

template <typename E>
template <typename Err>
constexpr unexpected<E>& unexpected<E>::operator=(unexpected<Err> const& e) {
    val_ = e.value();
    return *this;
}

template <typename E>
template <typename Err>
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
template <typename Err, typename>
void unexpected<E>::swap(unexpected& other) noexcept(std::is_nothrow_swappable_v<Err>) {
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
          typename = std::enable_if_t<
              std::is_void_v<decltype(std::declval<vien::expected<T1,E1>&>()
                                        .swap(std::declval<vien::expected<T1,E1>&>()))
              >
          >>
void swap(vien::expected<T1,E1>& x, vien::expected<T1,E1>& y) noexcept(noexcept(x.swap(y))) {
    x.swap(y);
}

template <typename E1,
          typename = std::enable_if_t<
              std::is_void_v<decltype(std::declval<vien::unexpected<E1>>()
                                        .swap(std::declval<vien::unexpected<E1>>()))
              >
          >>
void swap(vien::unexpected<E1>& x, vien::unexpected<E1>& y) noexcept(noexcept(x.swap(y))) {
    x.swap(y);
}
}

#endif
