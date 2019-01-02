#include <iostream>

#include "art/Art.h"
#include "art/Node4.h"
#include <gtest/gtest.h>
#include "util/lazy.h"
#include "util/lambda.h"
#include "util/tagged_unique_ptr.h"

int main() {
    auto artInsert = 42;
    auto artInsertPtr = reinterpret_cast<std::byte *>(&artInsert);
    auto art = Art<int>();
    art.add(artInsertPtr, artInsertPtr + sizeof(artInsert), artInsert);

    return 0;
}
