#ifndef EXPECTED_H
#define EXPECTED_H

#include <type_traits>
#include <utility>

namespace vien {

struct in_place_t {
    explicit in_place_t() = default;
};

inline constexpr in_place_t in_place{};

inline namespace v1 {

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
        
        /* Mimcking C++20's conditionally explicit */
        template <typename Err, typename = std::enable_if_t<std::is_convertible_v<Err, E>>>
        constexpr unexpected(unexpected<Err> const&);
        template <typename Err>
        constexpr explicit unexpected(unexpected<Err> const&);
        template <typename Err, typename = std::enable_if_t<std::is_convertible_v<Err, E>>>
        constexpr unexpected(unexpected<Err>&&);
        template <typename Err>
        constexpr explicit unexpected(unexpected<Err>&&);

        constexpr unexpected(unexpected const&) = default;
        constexpr unexpected(unexpected&&) = default;

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

/* unexpected */
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

} /* namespace v1 */
} /* namespace vien */

#endif
