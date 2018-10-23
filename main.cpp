#include <iostream>

#include <gsl/gsl>
#include "art/Art.h"
#include "art/Node4.h"
#include <fmt/core.h>

int main() {
    auto test = Node4<void *>();

    test.find(std::byte(128));
    fmt::print("Hello {}!", "world");

    return 0;
}