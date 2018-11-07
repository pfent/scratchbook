#include <iostream>

#include <gsl/gsl>
#include "art/Art.h"
#include "art/Node4.h"
#include <fmt/core.h>
#include <gtest/gtest.h>
#include "util/lazy.h"
#include "util/lambda.h"
#include <type_safe/strong_typedef.hpp>
#include "util/tagged_unique_ptr.h"

struct MyInt : type_safe::strong_typedef<MyInt, int> {
    using strong_typedef::strong_typedef;
};

auto foobar(const int &a) -> void {
    fmt::print("foobar {}\n", a);
}

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

    auto tptr = ext::make_tagged_unique<int>(42).with_tag(1);
    assert_that(tptr.get_tag() == 1);
    tptr = ext::make_tagged_unique<char>('X').with_tag(2);
    assert_that(tptr.get_tag() == 2);

    auto other = std::move(tptr);
    assert_that(other.get_tag() == 2);



    return 0;
}
