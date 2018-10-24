#ifndef SCRATCHBOOK_NODE48_H
#define SCRATCHBOOK_NODE48_H

#include <array>
#include <algorithm>
#include <cstdint>

template<typename Child>
class Node48 {
    static constexpr auto invalidIndex = uint8_t(255);
    uint8_t numChilds = 0;
    // TODO: Path compression in header?
    std::array<uint8_t, 256> childIndex = {};
    std::array<Child, 48> childs = {};

public:
    Node48() {
        childIndex.fill(invalidIndex);
    }

    using iterator = Child *;
    using const_iterator = Child const *;

    auto begin() const -> Child const * {
        return childs.begin();
    }

    auto end() const -> Child const * {
        return &childs[numChilds];
    }

    auto find(std::byte key) const -> Child const * {
        auto childIdx = childIndex[key];
        if (childIdx == invalidIndex) {
            return end();
        }
        return &childs[childIdx];
    }

};

#endif //SCRATCHBOOK_NODE48_H
