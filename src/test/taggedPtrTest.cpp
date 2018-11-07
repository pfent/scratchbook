#include "util/tagged_unique_ptr.h"
#include <fmt/format.h>

int main() {
    //auto artInsert = 42;
    //auto artInsertPtr = reinterpret_cast<std::byte *>(&artInsert);
    //auto art = Art<int>();
    //art.add(artInsertPtr, artInsertPtr + sizeof(artInsert), artInsert);

    auto tptr = ext::tagged_unique_ptr<int, char, float>(new int(42));
    fmt::print("int constructed in tagged pointer {:x}\n", reinterpret_cast<uintptr_t>(tptr.get<int>()));
    assert_that(tptr.get_tag() == 0);
    assert_that(tptr.is_pointer_to<int>());
    assert_that(not tptr.is_pointer_to<char>());
    tptr = ext::tagged_unique_ptr<int, char, float>(new char('X'));
    fmt::print("char constructed in tagged pointer {:x}\n", reinterpret_cast<uintptr_t>(tptr.get<char>()));
    assert_that(tptr.get_tag() == 1);
    assert_that(tptr.is_pointer_to<char>());
    tptr = ext::tagged_unique_ptr<int, char, float>(new float(0.3f));
    fmt::print("float constructed in tagged pointer {:x}\n", reinterpret_cast<uintptr_t>(tptr.get<float>()));
    assert_that(tptr.get_tag() == 2);
    assert_that(tptr.is_pointer_to<float>());

    auto other = std::move(tptr);
    fmt::print("float moved to other pointer {:x}\n", reinterpret_cast<uintptr_t>(other.get<float>()));
    assert_that(other.get_tag() == 2);
    assert_that(other.is_pointer_to<float>());
    other.reset();
    other.reset(new char(' '));
    assert_that(other.get_tag() == 1);
    assert_that(other.is_pointer_to<char>());

    fmt::print("all tagged pointers ok\n");

    return 0;
}