# expected

C++17 implementation of `std::expected` conforming to the interface proposed in [P0323R9](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2019/p0323r9.html).  

[![Build Status](https://gitlab.com/vilhelmengstrom/expected/badges/master/build.svg)](https://gitlab.com/vilhelmengstrom/expected/commits/master)

## Rationale 

The current standard is somewhat limiting when it comes to error handling. We are forced to either juggle error codes (efficient but messy) or rely on exceptions (expressive but inefficient). Languages such as [Haskell](https://www.haskell.org) and [Rust](https://www.rust-lang.org) have instead chosen to use monadic error handling. This way, functions that may fail can return an instance of one of two types, one being of the 'correct' return type and one being of an error type. `std::expected` is this approach brought to C++. This would allow for the expressiveness of exceptions but with a significantly smaller performance hit (a single boolean check is enough), while also making the code more concise.

Using `expected`, one might replace the following
```cpp

extern std::vector<int> function_that_may_fail();

void foo() {
    std::vector<int> vec;
    try {
        vec = function_that_may_fail();
    }
    catch(std::exception& e) {
        std::cerr << e.what() << "\n";
        /* Additional error handling */
        return;
    }
    /* Do something with vec */
}

```
with
```cpp
extern vien::expected<std::vector<int>, std::string> function_that_may_fail();

void foo() {
    auto e = function_that_may_fail();

    if(e.has_value()) {
        /* Do something with e.value() */
    }
    else {
        std::cerr << e.error() << "\n";
        /* Additional error handling */
    }
}
```

### Opt-in extensions
In addition to what is proposed in P0323R9, the implementation provides a few functional extensions, inspired in part by Rust's [Result enum](https://doc.rust-lang.org/std/result/enum.Result.html). In order to adhere to the interface proposed for the standard, these are opt-in and require that `VIEN_EXPECTED_EXTENDED` is defined before the expected header is included. The following extensions are available:

- `map` invokes a callable on the contained value, leaving a potential unexpected unchanged.
    ```cpp
        vien::expected<int, double> e1(10); // bool(e1) == true
        vien::expected<int, double> e1 = e1.map([](int i) { return 2 * i; });

        ASSERT(*e1 == 20);
    ```
- `map_range` like `map` but applies the callable to each element in a contained container.
    ```cpp
        std::string str = "expected"
        vien::expected<std::string, int> e1(std::move(str)); // bool(e1) == true
        vien::expected<std::string, int> e2 = e1.map_range([](unsigned char c) { return std::toupper(c); });

        ASSERT(*e2 == "EXPECTED");
    ```
- `map_error` invokes a callable on the contained unexpected, leaving a potential value unchanged.
    ```cpp
        vien::expected<int, std::string> e1(unexpect, "error"); // bool(e1) == false
        vien::expected<int, std::string> e2 = e1.map_error([](auto const& str) { return "fatal " + str; });

        ASSERT(e2.error() == "fatal error");
    ```
- `map_or_else` invokes a callable on the contained value, if any. Otherwise, invokes the fallback callable on the contained unexpected.
    ```cpp
        auto multiply_by_two = [](int i) { return 2 * i; };
        auto flip_sign = [](int i) { return -i; };
        vien::expected<int, int> e1(5);             // bool(e1) == true
        vien::expected<int, int> e2(unexpect, 20);  // bool(e2) == false

        vien::expected<int, int> e3 = e1.map_or_else(multiply_by_two, flip_sign);
        vien::expected<int, int> e4 = e2.map_or_else(multiply_by_two, flip_sign);

        ASSERT(e3.value() == 10);
        ASSERT(e4.error() == -20);
    ```

- `and_then` invokes a callable if the expected has a value. If the expected holds an unexpected, nothing is done.
    ```cpp
        auto square = [](int i) { return i * i; };
        vien::expected<int, std::string> e1(2); // bool(e1) == true
        vien::expected<int, std::string> e2 = e1.and_then(square).and_then(square);

        ASSERT(*e2 == 8);
    ```
- `or_else` invokes a callable if the expected has no value. If the expected holds a value, nothing is done.
    ```cpp
        auto square = [](int i) { return i * i; };
        vien::expected<std::string, int> e1(unexpect, 2); // bool(e1) == false
        vien::expected<std::string, int> e2 = e1.or_else(square).or_else(square);

        ASSERT(e2.error() == 8);
    ```

### Dependencies
[Catch2](https://github.com/catchorg/Catch2) is used for testing. The single-header version is included in the tests directory.

### Acknowledgements
Simon Brand has written a [very well received implementation](https://github.com/TartanLlama/expected) with support ranging back to C++11.
