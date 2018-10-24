#ifndef SCRATCHBOOK_NODE4_H
#define SCRATCHBOOK_NODE4_H

#include <array>
#include <algorithm>
#include <cstdint>

template<typename Child>
class Node4 {
    uint8_t numChilds = 0;
    // TODO: Path compression in header?
    std::array<std::byte, 4> keys = {};
    std::array<Child, 4> childs = {};

public:
    using iterator = Child *;
    using const_iterator = Child const *;

    auto begin() const noexcept -> Child const * {
        return childs.begin();
    }

    auto end() const noexcept -> Child const * {
        return &childs[numChilds];
    }

    auto find(std::byte key) const -> Child const * {
        const auto endPtr = &keys[numChilds];
        auto resPtr = std::find(keys.begin(), endPtr, key);
        if (resPtr == endPtr) {
            return end();
        }

        auto idx = std::distance(keys.begin(), resPtr);
        return &childs[idx];
    };

    auto size() const -> size_t {
        return numChilds;
    }

    auto max_size() const -> size_t {
        return 4;
    }

    auto insert(std::byte key, Child child) -> void {
        if (size() >= max_size()) {
            throw std::runtime_error("Node4 is already full!");
        };
        insert_unsafe(key, std::move(child));
    }

    // TODO: ensure sorted keys, so childs can be iterated in order
    auto insert_unsafe(std::byte key, Child child) -> void {
        childs[numChilds] = std::move(child);
        keys[numChilds] = key;
        ++numChilds;
    };
};

#endif //SCRATCHBOOK_NODE4_H
