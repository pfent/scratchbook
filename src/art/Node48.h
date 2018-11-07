#ifndef SCRATCHBOOK_NODE48_H
#define SCRATCHBOOK_NODE48_H

#include <array>
#include <algorithm>
#include <cstdint>

template<typename Child>
class Node48 {
    static constexpr auto invalidIndex = uint8_t(255);
    static constexpr auto maxChilds = 48;
    uint8_t numChilds = 0;
    // TODO: Path compression in header?
    std::array<uint8_t, 256> childIndex = {};
    std::array<Child, maxChilds> childs = {};

public:
    using value_type = Child;
    using iterator = Child *;
    using const_iterator = Child const *;

    Node48() {
        childIndex.fill(invalidIndex);
    }

    auto begin() const -> const_iterator {
        return childs.begin();
    }

    auto begin() -> iterator {
        return childs.begin();
    }

    auto end() const -> const_iterator {
        return &childs[numChilds];
    }

    auto end() -> iterator {
        return &childs[numChilds];
    }

    auto find(std::byte key) const -> const_iterator {
        auto childIdx = childIndex[size_t(key)];
        if (childIdx == invalidIndex) {
            return end();
        }
        return &childs[childIdx];
    }

    auto find(std::byte key) -> iterator {
        return const_cast<Child *>(
                const_cast<std::add_const_t<decltype(this)>>(this)->find(key)
        );
    }

    auto size() const -> size_t {
        return numChilds;
    }

    auto max_size() const -> size_t {
        return maxChilds;
    }

};

#endif //SCRATCHBOOK_NODE48_H
