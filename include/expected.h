#ifndef EXPECTED_H
#define EXPECTED_H

#include <initializer_list>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <utility>

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

/* Used for enabling copy conversion ctor */
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

/* Used for enabling non-explicit copy conversion */
template <typename T, typename E, typename U, typename G>
struct expected_enable_non_explicit_copy_conversion
    : std::bool_constant<std::is_convertible_v<U const&, T> &&
                         std::is_convertible_v<G const&, E>> { };

template <typename E, typename G>
struct expected_enable_non_explicit_copy_conversion<void, E, void, G> 
    : std::bool_constant<std::is_convertible_v<G const&, E>> { };

template <typename T, typename E, typename U, typename G>
inline bool constexpr expected_enable_non_explicit_copy_conversion_v =
    expected_enable_non_explicit_copy_conversion<T,E,U,G>::value;

/* Used for enabling move construction */
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
struct expected_enable_non_explicit_move_conversion
    : std::bool_constant<std::is_convertible_v<U&&, T> &&
                         std::is_convertible_v<G&&, E>> { };

template <typename E, typename G>
struct expected_enable_non_explicit_move_conversion<void, E, void, G> 
    : std::bool_constant<std::is_convertible_v<G&&, E>> { };

template <typename T, typename E, typename U, typename G>
inline bool constexpr expected_enable_non_explicit_move_conversion_v =
    expected_enable_non_explicit_move_conversion<T,E,U,G>::value;

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
struct expected_enable_non_explicit_forwarding_ref_ctor
    : std::bool_constant<std::is_convertible_v<U&&, T>> { };

template <typename T, typename U>
inline bool constexpr expected_enable_non_explicit_forwarding_ref_ctor_v =
    expected_enable_non_explicit_forwarding_ref_ctor<T,U>::value;


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
    constexpr void store(Args&&... args) noexcept {
        new(std::addressof(this->val_)) T(std::forward<Args>(args)...);
        this->has_val_ = true;
    }

    template <typename... Args>
    constexpr void store(unexpect_t, Args&&... args) noexcept {
        new (std::addressof(this->unexpect_)) unexpected<E>(std::forward<Args>(args)...);
        this->has_val_ = false;
    }

    constexpr unexpected<E>& get_unexpect() {
        return this->unexpect_;
    }

    constexpr unexpected<E> const& get_unexpect() const {
        return this->unexpect_;
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
    constexpr void store(unexpect_t, Args&&... args) noexcept {
        new (std::addressof(this->unexpect_)) unexpected<E>(std::forward<Args>(args)...);
        this->has_val_ = false;
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
            this->store(unexpect, unexpected(rhs.get_unexpect().value()));
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
            this->store(unexpect, unexpected(rhs.get_unexpect().value()));
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
            this->store(unexpect, std::move(rhs.get_unexpect().value()));
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
            this->store(unexpect, std::move(rhs.get_unexpect().value()));
    }
};

/* expeced_interface_base */
/* Provides functions availabe for all instantiations of expected.
 * Operations unique to when either T is void or T is non-void are 
 * provided in derived classes */
template <typename T, typename E>
class expected_interface_base : impl::expected_move_ctor_base<T,E> {
    static_assert(!std::is_reference_v<T>, "T must not be reference");
    static_assert(!std::is_reference_v<E>, "E must not be reference");
    static_assert(!std::is_same_v<T, std::remove_cv_t<unexpected<E>>>,
                  "T must not be unexpected<E>");
    using base_t = impl::expected_move_ctor_base<T,E>;

    public:
        using base_t::base_t;
        expected_interface_base() = default;
        expected_interface_base(expected_interface_base const&) = default;
        expected_interface_base(expected_interface_base&&) = default;

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

        constexpr explicit operator bool() const noexcept;

        constexpr E& error() &;
        constexpr E const& error() const &;
        constexpr E&& error() &&;
        constexpr E const&& error() const &&;

    protected:
        template <typename... Args>
        void store(Args&&... args) noexcept;

        template <typename U = T, typename = std::enable_if_t<!std::is_void_v<U>>>
        constexpr U& get_val();

        template <typename U = T, typename = std::enable_if_t<!std::is_void_v<U>>>
        constexpr U const& get_val() const;

        constexpr unexpected<E>& get_unexpect() const;
};

template <typename T, typename E>
template <typename G, typename, typename, typename>
constexpr expected_interface_base<T,E>::expected_interface_base(unexpected<G> const& e) {
    this->store(unexpect, e);
}

template <typename T, typename E>
template <typename G, typename, typename>
constexpr expected_interface_base<T,E>::expected_interface_base(unexpected<G> const& e) {
    this->store(unexpect, e);
}

template <typename T, typename E>
template <typename G, typename, typename, typename>
constexpr expected_interface_base<T,E>::expected_interface_base(unexpected<G>&& e) 
                noexcept(std::is_nothrow_constructible_v<E, G&&>) {
    this->store(unexpect, std::move(e));
}

template <typename T, typename E>
template <typename G, typename, typename>
constexpr expected_interface_base<T,E>::expected_interface_base(unexpected<G>&& e)
                noexcept(std::is_nothrow_constructible_v<E, G&&>) {
    this->store(unexpect, std::move(e));
}


template <typename T, typename E>
constexpr expected_interface_base<T,E>::operator bool() const noexcept {
    return this->has_val_;
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

template <typename T, typename E>
template <typename... Args>
void expected_interface_base<T,E>::store(Args&&... args) noexcept {
    base_t::store(std::forward<Args>(args)...);
}

template <typename T, typename E>
template <typename U, typename>
constexpr U& expected_interface_base<T,E>::get_val() {
    return this->val_;
}

template <typename T, typename E>
template <typename U, typename>
constexpr U const& expected_interface_base<T,E>::get_val() const {
    return this->val_;
}
template <typename T, typename E>
constexpr unexpected<E>& expected_interface_base<T,E>::get_unexpect() const {
    return this->unexpect_;
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

        /* Conditionally explicit perfect forwarding conversion ctor */
        template <typename U = T, typename TT = T, typename EE = E,
                  typename = std::enable_if_t<
                      impl::expected_enable_forwarding_ref_ctor_v<TT, EE, U>
                  >,
                  typename = std::enable_if_t<
                      impl::expected_enable_non_explicit_forwarding_ref_ctor_v<TT, U>
                  >>
        constexpr expected(U&&);

        template <typename U = T, typename TT = T, typename EE = E,
                  typename = std::enable_if_t<
                      impl::expected_enable_forwarding_ref_ctor_v<TT, EE, U> &&
                     !impl::expected_enable_non_explicit_forwarding_ref_ctor_v<TT, U>
                  >>
        constexpr explicit expected(U&&);
                  

        /* Conditionally explicit conversion constructors */
        template <typename U, typename G, typename TT = T, typename EE = E,
                  typename = std::enable_if_t<
                      impl::expected_enable_copy_conversion_v<TT, EE, U, G>
                  >,
                  typename = std::enable_if_t<
                      impl::expected_enable_non_explicit_copy_conversion_v<TT, EE, U, G>
                  >>
        constexpr expected(expected<U, G> const& rhs);

        template <typename U, typename G, typename TT = T, typename EE = E,
                  typename = std::enable_if_t<
                      impl::expected_enable_copy_conversion_v<TT, EE, U, G> &&
                     !impl::expected_enable_non_explicit_copy_conversion_v<TT, EE, U, G>
                  >>
        constexpr explicit expected(expected<U, G> const& rhs);

        template <typename U, typename G, typename TT = T, typename EE = E,
                  typename = std::enable_if_t<
                      impl::expected_enable_move_conversion_v<TT, EE, U, G>
                  >,
                  typename = std::enable_if_t<
                      impl::expected_enable_non_explicit_move_conversion_v<TT, EE, U, G>
                  >>
        constexpr expected(expected<U,G>&& rhs);

        template <typename U, typename G, typename TT = T, typename EE = E,
                  typename = std::enable_if_t<
                      impl::expected_enable_move_conversion_v<TT, EE, U, G> &&
                     !impl::expected_enable_non_explicit_move_conversion_v<TT, EE, U, G>
                  >>
        constexpr explicit expected(expected<U, G>&& rhs);

        template <typename... Args, 
                  typename = std::enable_if_t<
                      std::is_constructible_v<T, Args...>
                  >>
        constexpr explicit expected(in_place_t, Args&&...);

        constexpr T& value() &;
        constexpr T const& value() const &;
        constexpr T&& value() &&;
        constexpr T const&& value() const &&;
};

template <typename T, typename E>
template <typename U, typename, typename, typename, typename>
constexpr expected<T,E>::expected(U&& v) {
    this->store(std::forward<U>(v));
}

template <typename T, typename E>
template <typename U, typename, typename, typename>
constexpr expected<T,E>::expected(U&& v) {
    this->store(std::forward<U>(v));
}

template <typename T, typename E>
template <typename U, typename G, typename, typename, typename, typename>
constexpr expected<T,E>::expected(expected<U, G> const& rhs) {
    if(bool(rhs))
        this->store(rhs.value());
    else
        this->store(unexpect, rhs.error());
}

template <typename T, typename E>
template <typename U, typename G, typename, typename, typename>
constexpr expected<T,E>::expected(expected<U, G> const& rhs) {
    if(bool(rhs))
        this->store(rhs.value());
    else
        this->store(unexpect, rhs.error());
}

template <typename T, typename E>
template <typename U, typename G, typename, typename, typename, typename>
constexpr expected<T,E>::expected(expected<U, G>&& rhs) {
    if(bool(rhs))
        this->store(std::move(rhs.value()));
    else
        this->store(unexpect, std::move(rhs.error()));
}

template <typename T, typename E>
template <typename U, typename G, typename, typename, typename>
constexpr expected<T,E>::expected(expected<U, G>&& rhs) {
    if(bool(rhs))
        this->store(std::move(rhs.value()));
    else
        this->store(unexpect, std::move(rhs.error()));
}

template <typename T, typename E>
template <typename... Args, typename>
constexpr expected<T,E>::expected(in_place_t, Args&&... args) {
    this->store(std::forward<Args>(args)...);
}

template <typename T, typename E>
constexpr T& expected<T,E>::value() & {
    if(!bool(*this))
        throw bad_expected_access(this->error());
    return this->get_val();
}

template <typename T, typename E>
constexpr T const& expected<T,E>::value() const & {
    if(!bool(*this))
        throw bad_expected_access(this->error());
    return this->get_val();
}

template <typename T, typename E>
constexpr T&& expected<T,E>::value() && {
    if(!bool(*this))
        throw bad_expected_access(this->error());
    return std::move(this->get_val());
}

template <typename T, typename E>
constexpr T const&& expected<T,E>::value() const && {
    if(!bool(*this))
        throw bad_expected_access(this->error());
    return std::move(this->get_val());
}

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

        /* Conditionally explicit conversion constructors */
        template <typename U, typename G, typename TT = void, typename EE = E,
                  typename = std::enable_if_t<
                      impl::expected_enable_copy_conversion_v<TT, EE, U, G>
                  >,
                  typename = std::enable_if_t<
                      impl::expected_enable_non_explicit_copy_conversion_v<TT, EE, U, G>
                  >>
        constexpr expected(expected<U, G> const& rhs);

        template <typename U, typename G, typename TT = void, typename EE = E,
                  typename = std::enable_if_t<
                      impl::expected_enable_copy_conversion_v<TT, EE, U, G> &&
                     !impl::expected_enable_non_explicit_copy_conversion_v<TT, EE, U, G>
                  >>
        constexpr explicit expected(expected<U, G> const& rhs);

        template <typename U, typename G, typename TT = void, typename EE = E,
                  typename = std::enable_if_t<
                      impl::expected_enable_move_conversion_v<TT, EE, U, G>
                  >,
                  typename = std::enable_if_t<
                      impl::expected_enable_non_explicit_move_conversion_v<TT, EE, U, G>
                  >>
        constexpr expected(expected<U,G>&& rhs);

        template <typename U, typename G, typename TT = void, typename EE = E,
                  typename = std::enable_if_t<
                      impl::expected_enable_move_conversion_v<TT, EE, U, G> &&
                     !impl::expected_enable_non_explicit_move_conversion_v<TT, EE, U, G>
                  >>
        constexpr explicit expected(expected<U, G>&& rhs);

        template <typename... Args, 
                  typename = std::enable_if_t<sizeof...(Args) == 0>>
        constexpr explicit expected(in_place_t, Args&&...);

        constexpr void value() const;
};

template <typename E>
template <typename U, typename G, typename, typename, typename, typename>
constexpr expected<void,E>::expected(expected<U, G> const& rhs) {
    if(bool(rhs))
        this->store();
    else
        this->store(unexpect, rhs.error());
}

template <typename E>
template <typename U, typename G, typename, typename, typename>
constexpr expected<void,E>::expected(expected<U, G> const& rhs) {
    if(bool(rhs))
        this->store();
    else
        this->store(unexpect, rhs.error());
}

template <typename E>
template <typename U, typename G, typename, typename, typename, typename>
constexpr expected<void,E>::expected(expected<U, G>&& rhs) {
    if(bool(rhs))
        this->store();
    else
        this->store(unexpect, std::move(rhs.error()));
}

template <typename E>
template <typename U, typename G, typename, typename, typename>
constexpr expected<void,E>::expected(expected<U, G>&& rhs) {
    if(bool(rhs))
        this->store();
    else
        this->store(unexpect, std::move(rhs.error()));
}

template <typename E>
template <typename... Args, typename>
constexpr expected<void, E>::expected(in_place_t, Args&&...) {
    this->store();
}

template <typename E>
constexpr void expected<void,E>::value() const {
    if(!bool(*this))
        throw bad_expected_access(this->error());
}

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

        constexpr E const& value() const & noexcept;
        constexpr E& value() & noexcept;
        constexpr E const&& value() const && noexcept;
        constexpr E&& value() && noexcept;

        template <typename Err = E, typename = std::enable_if_t<std::is_swappable_v<Err>>>
        void swap(unexpected&) noexcept(std::is_nothrow_swappable_v<Err>);

        template <typename E1, typename E2>
        friend constexpr bool operator==(unexpected<E1> const&, unexpected<E2> const&);
        template <typename E1, typename E2>
        friend constexpr bool operator!=(unexpected<E1> const&, unexpected<E2> const&);

        template <typename E1>
        friend std::enable_if_t<std::is_swappable_v<E1>>
        std::swap(unexpected<E1>& x, unexpected<E1>& y) noexcept(noexcept(x.swap(y)));
    private:
        E val_;

        /* Required for accessing val_ from instances with 
         * other type params */
        template <typename>
        friend class unexpected;
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
    : val_(e.val_) { }

template <typename E>
template <typename Err>
constexpr unexpected<E>::unexpected(unexpected<Err> const& e)
    : val_(e.val_) { }

template <typename E>
template <typename Err, typename>
constexpr unexpected<E>::unexpected(unexpected<Err>&& e)
    : val_(std::move(e.val_)) { }

template <typename E>
template <typename Err>
constexpr unexpected<E>::unexpected(unexpected<Err>&& e)
    : val_(std::move(e.val_)) { }

template <typename E>
template <typename Err>
constexpr unexpected<E>& unexpected<E>::operator=(unexpected<Err> const& e) {
    val_ = e.val_;
    return *this;
}

template <typename E>
template <typename Err>
constexpr unexpected<E>& unexpected<E>::operator=(unexpected<Err>&& e) {
    val_ = std::move(e.val_);
    return *this;
}

template <typename E>
constexpr E const& unexpected<E>::value() const & noexcept {
    return val_;
}

template <typename E>
constexpr E& unexpected<E>::value() & noexcept {
    return val_;
}

template <typename E>
constexpr E const&& unexpected<E>::value() const && noexcept {
    return std::move(val_);
}

template <typename E>
constexpr E&& unexpected<E>::value() && noexcept {
    return std::move(val_);
}

template <typename E>
template <typename Err, typename>
void unexpected<E>::swap(unexpected& other) noexcept(std::is_nothrow_swappable_v<Err>) {
    std::swap(val_, other.val_);
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

        virtual char const* what() const noexcept override {
            return "Attempt to access expected without value\n";
        }
};

template <typename E>
class bad_expected_access : public bad_expected_access<void> {
    public:
        explicit bad_expected_access(E);
        E& error() &;
        E const& error() const &;
        E&& error() &&;
        E const&& error() const &&;
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

} /* namespace v1 */
} /* namespace vien */

namespace std {
template <typename E1>
void swap(vien::unexpected<E1>& x, vien::unexpected<E1>& y) noexcept(noexcept(x.swap(y))) {
    x.swap(y);
}
}

#endif
