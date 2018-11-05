#include <iostream>

#include <gsl/gsl>
#include "art/Art.h"
#include "art/Node4.h"
#include <fmt/core.h>
#include <gtest/gtest.h>
#include "util/lazy.h"
#include "util/lambda.h"

auto foobar(const int &a) -> void {
    fmt::print("foobar {}\n", a);
}

int main() {
    auto test = Node4<void *>();

    test.find(std::byte(128));
    fmt::print("Hello {}!", "world");

    Lazy asdf2 = Lazy([]() noexcept {
        fmt::print("initialized {}\n", "asdf");
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        return 42;
    });

    fmt::print("Before initializing {}\n", "lazy");
    fmt::print("The answer is {}\n", asdf2.get());

    foobar(asdf2);

    fmt::print("sizeof lazy: {}, sizeof raw: {}", sizeof(asdf2), sizeof(asdf2.get()));
    return 0;
}

class Asdfes {
    Lazy<int> asdf = Lazy([] {
        return 42;
    });
};