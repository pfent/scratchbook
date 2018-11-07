#include <iostream>

#include <gsl/gsl>
#include "art/Art.h"
#include "art/Node4.h"
#include <fmt/format.h>
#include <gtest/gtest.h>
#include "util/lazy.h"
#include "util/lambda.h"
#include <type_safe/strong_typedef.hpp>
#include "util/tagged_unique_ptr.h"

struct MyInt : type_safe::strong_typedef<MyInt, int> {
    using strong_typedef::strong_typedef;
};

auto takesInt(int a) {
    fmt::print("took int: {}\n", a);
}

auto takesInt(MyInt a) {
    fmt::print("took MyInt: {}\n", int(a));
}

int main() {
    auto artInsert = 42;
    auto artInsertPtr = reinterpret_cast<std::byte *>(&artInsert);
    auto art = Art<int>();
    art.add(artInsertPtr, artInsertPtr + sizeof(artInsert), artInsert);

    return 0;
}
