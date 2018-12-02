#ifndef SCRATCHBOOK_NODE48_H
#define SCRATCHBOOK_NODE48_H

#include <array>
#include <algorithm>
#include <cstdint>

template<typename Child>
class Node48 {
    static constexpr auto invalidIndex = uint8_t(255);
    static constexpr auto maxChilds = 48;
    // header {
    uint8_t numChilds = 0;
    // TODO: Path compression in header?
    // } // header
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
        auto childIdx = childIndex[std::size_t(key)];
        if (childIdx == invalidIndex) {
            return end();
        }
        return &childs[childIdx];
    }

    auto find(std::byte key) -> iterator {
        return const_cast<Child *>(const_cast<const Node48 *>(this)->find(key));
    }

    auto size() const -> std::size_t {
        return numChilds;
    }

    auto max_size() const -> std::size_t {
        return maxChilds;
    }

    auto insert(std::byte key, Child child) -> void {
        if (size() >= max_size()) {
            throw std::runtime_error("Node48 is already full!");
        };
        if (find(key) != end()) {
            throw std::runtime_error("Duplicate key!");
        }
        insert_unsafe(key, std::move(child));
    }

    auto insert_unsafe(std::byte key, Child child) -> void {
        auto position = numChilds;
        childIndex[std::size_t(key)] = position;
        childs[position] = std::move(child);

        numChilds++;
    };

};

#endif //SCRATCHBOOK_NODE48_H
