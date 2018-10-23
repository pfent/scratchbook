#include <iostream>

#include "art/Art.h"
#include "art/Node4.h"

int main() {
    std::cout << "Hello, World!" << std::endl;

    auto test = Node4<void *>();

    test.find(std::byte(128));
    return 0;
}