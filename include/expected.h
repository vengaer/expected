#ifndef EXPECTED_H
#define EXPECTED_H

#include <memory>
#include <stdexcept>
#include <type_traits>
#include <utility>

namespace std {
    template <typename>
    class initializer_list;
}


namespace vien {

struct in_place_t {
    explicit in_place_t() = default;
};

inline constexpr in_place_t in_place{};

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

template <typename T, typename E, 
          bool = std::is_trivially_destructible_v<T>, 
          bool = std::is_trivially_destructible_v<E>>
struct expected_storage_base;


/* T trivially destructible, E trivially destructible */
template <typename T, typename E>
struct expected_storage_base<T, E, true, true> {

    constexpr expected_storage_base() : has_val_(true), val_(T()) { }

    ~expected_storage_base() = default;

    bool has_val_;
    union {
        T val_;
        unexpected<E> unexpect_;
    };
};

/* T not trivially destructible, E trivially destructible */
template <typename T, typename E>
struct expected_storage_base<T, E, false, true> {
    
    constexpr expected_storage_base() : has_val_(true), val_(T()) { }

    ~expected_storage_base() {
        if(has_val_)
            val_.~T();
    }

    bool has_val_;
    union {
        T val_;
        unexpected<E> unexpect_;
    };
};

/* T trivially destructible, E not trivially destructible */
template <typename T, typename E>
struct expected_storage_base<T, E, true, false> {

    constexpr expected_storage_base() : has_val_(true), val_(T()) { }

    ~expected_storage_base() {
        if(!has_val_)
            unexpect_.~unexpected<E>();
    }

    bool has_val_;
    union {
        T val_;
        unexpected<E> unexpect_;
    };
};

/* Neither T nor E trivially destructible */
template <typename T, typename E>
struct expected_storage_base<T, E, false, false> {

    constexpr expected_storage_base() : has_val_(true), val_(T()) { }

    ~expected_storage_base() {
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

/* T is void, E trivially destructible */
template <typename E>
struct expected_storage_base<void, E, false, true> {

    constexpr expected_storage_base() : has_val_(true) { }

    ~expected_storage_base() = default;

    bool has_val_;
    struct none{};
    union {
        none val_;
        unexpected<E> unexpect_;
    };
};

/* T is void, E not trivially destructible */
template <typename E>
struct expected_storage_base<void, E, false, false> {

    constexpr expected_storage_base() : has_val_(true) { }

    ~expected_storage_base() {
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

template <typename T, typename E>
struct expected_member_base : expected_storage_base<T,E> {
    template <typename... Args>
    void store(Args&&... args) noexcept {
        new(std::addressof(this->val_)) T(std::forward<Args>(args)...);
        this->has_val_ = true;
    }

    template <typename... Args>
    void store_unexpected(Args&&... args) noexcept {
        new (std::addressof(this->unexpect_)) unexpected<E>(std::forward<Args>(args)...);
        this->has_val_ = false;
    }
};

template <typename E>
struct expected_member_base<void, E> : expected_storage_base<void,E> {
    template <typename... Args>
    void store(Args&&...) noexcept {
        this->has_val_ = true;
    }

    template <typename... Args>
    void store_unexpected(Args&&... args) noexcept {
        new (std::addressof(this->unexpect_)) unexpected<E>(std::forward<Args>(args)...);
        this->has_val_ = false;
    }
};

template <typename T, typename E, 
    bool = is_default_constructible_or_void_v<T>>
struct expected_default_ctor_base : expected_member_base<T,E> { };

/* Delete default ctor if T is not default constructible or void */
template <typename T, typename E>
struct expected_default_ctor_base<T,E,false> : expected_member_base<T,E> {
    expected_default_ctor_base() = delete;
};

template <typename T, typename E,
          bool = is_copy_constructible_or_void_v<T> &&
                 std::is_copy_constructible_v<E>,
          bool = is_trivially_copy_constructible_or_void_v<T> &&
                 std::is_trivially_copy_constructible_v<E>>
struct expected_copy_ctor_base;

/* Partial specializationf for when:
 * T is not copy constructible or void, and/or E is not copy constructible */
template <typename T, typename E>
struct expected_copy_ctor_base<T, E, false, false> 
    : expected_default_ctor_base<T,E> {
    using expected_default_ctor_base<T,E>::expected_default_ctor_base;

    expected_copy_ctor_base(expected_copy_ctor_base const&) = delete;
};

/* Partial specialization for when:
 * T is copy constructible or void and E is copy constructible
 * T is not trivially copy constructible or void, and/or E is not trivially copy constructible */
template <typename T, typename E>
struct expected_copy_ctor_base<T, E, true, false>
    : expected_default_ctor_base<T,E> {
    using expected_default_ctor_base<T,E>::expected_default_ctor_base;

    expected_copy_ctor_base(expected_copy_ctor_base const& rhs) {
        if(rhs.has_val_)
            store(rhs.val_);
        else
            store_unexpected(unexpected(rhs.error()));
    }
};

/* Partial specialization for when:
 * T is trivially copy constructible or void, and E is trivially copy constructible */
template <typename T, typename E>
struct expected_copy_ctor_base<T, E, true, true>
    : expected_default_ctor_base<T,E> {
    using expected_default_ctor_base<T,E>::expected_default_ctor_base;

    constexpr expected_copy_ctor_base(expected_copy_ctor_base const& rhs) {
        if(rhs.has_val_)
            store(rhs.val_);
        else
            store_unexpected(unexpected(rhs.error()));
    }
};

template <typename T, typename E,
          bool = is_move_constructible_or_void_v<T> && 
                 std::is_move_constructible_v<E>,
          bool = is_trivially_move_constructible_or_void_v<T> &&
                 std::is_trivially_move_constructible_v<E>>
struct expected_move_ctor_base;

/* Partial specialization for when:
 * T is not move constructible or void, and/or E is not move constructible */
template <typename T, typename E>
struct expected_move_ctor_base<T, E, false, false> 
    : expected_copy_ctor_base<T,E> {
    using expected_copy_ctor_base<T,E>::expected_copy_ctor_base;

    expected_move_ctor_base(expected_move_ctor_base const&) = default;
    expected_move_ctor_base(expected_move_ctor_base&&) = delete;
};

/* Partial specialization for when:
 * T is move constructible or void, and E is move constructible,
 * T is not trivially move constructible or void, and/or E is not trivially move constructible */
template <typename T, typename E>
struct expected_move_ctor_base<T, E, true, false>
    : expected_copy_ctor_base<T,E> {
    using expected_copy_ctor_base<T,E>::expected_copy_ctor_base;

    expected_move_ctor_base(expected_move_ctor_base const&) = default;
    expected_move_ctor_base(expected_move_ctor_base&& rhs) 
        noexcept(is_nothrow_move_constructible_or_void_v<T> &&
                 std::is_nothrow_move_constructible_v<E>) {
        if(rhs.has_val_)
            store(std::move(rhs.val_));
        else
            store_unexpected(std::move(rhs.error()));
    }
};

/* Partial specialization for when:
 * T is trivially move constructible or void and E is trivially move constructible */
template <typename T, typename E>
struct expected_move_ctor_base<T, E, true, true>
    : expected_copy_ctor_base<T,E> {
    using expected_copy_ctor_base<T,E>::expected_copy_ctor_base;

    expected_move_ctor_base(expected_move_ctor_base const&) = default;
    constexpr expected_move_ctor_base(expected_move_ctor_base&& rhs) 
        noexcept(is_nothrow_move_constructible_or_void_v<T> &&
                 std::is_nothrow_move_constructible_v<E>) {
        if(rhs.has_val_)
            store(std::move(rhs.val_));
        else
            store_unexpected(std::move(rhs.error()));
    }
};

} /* namespace impl */


template <typename T, typename E>
class expected : impl::expected_move_ctor_base<T,E> {
    static_assert(!std::is_reference_v<T>, "T must not be a reference");
    static_assert(!std::is_reference_v<E>, "E must not be a reference");
    static_assert(!std::is_same_v<T, std::remove_cv_t<unexpected<E>>>,
                  "T must not be unexpected<E>");
    public:
        using value_type = T;
        using error_type = E;
        using unexpected_type = unexpected<E>;

        template <typename U>
        using rebind = expected<U, error_type>;

        using impl::expected_move_ctor_base<T,E>::expected_move_ctor_base;

    private:
};


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

        template <typename = std::enable_if_t<std::is_swappable_v<E>>>
        void swap(unexpected&) noexcept(std::is_nothrow_swappable_v<E>);

        template <typename E1, typename E2>
        friend constexpr bool operator==(unexpected<E1> const&, unexpected<E2> const&);
        template <typename E1, typename E2>
        friend constexpr bool operator!=(unexpected<E1> const&, unexpected<E2> const&);

        template <typename E1, typename>
        friend void swap(unexpected<E1>& x, unexpected<E1>& y) noexcept(noexcept(x.swap(y)));
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
template <typename>
void unexpected<E>::swap(unexpected& other) noexcept(std::is_nothrow_swappable_v<E>) {
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

template <typename E1, typename>
void swap(unexpected<E1>& x, unexpected<E1>& y) noexcept(noexcept(x.swap(y))) {
    x.swap(y);
}

template <>
class bad_expected_access<void> : public std::exception {
    public:
        explicit bad_expected_access();
};

template <typename E>
class bad_expected_access : public bad_expected_access<void> {
    public:
        explicit bad_expected_access(E);
        virtual char const* what() const noexcept override;
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
char const* bad_expected_access<E>::what() const noexcept {
    return "Bad expected access\n";
}

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

#endif
