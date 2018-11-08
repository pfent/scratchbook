#ifndef SCRATCHBOOK_NODE4_H
#define SCRATCHBOOK_NODE4_H

#include <array>
#include <algorithm>
#include <cstdint>

template<typename Child>
class Node4 {
    static constexpr auto maxChilds = 4;
    // header {
    uint8_t numChilds = 0;
    // TODO: Path compression in header?
    // } // header
    std::array<std::byte, maxChilds> keys = {};
    std::array<Child, maxChilds> childs = {};

public:
    using value_type = Child;
    using iterator = Child *;
    using const_iterator = Child const *;

    auto begin() const noexcept -> const_iterator {
        return childs.begin();
    }

    auto begin() noexcept -> iterator {
        return childs.begin();
    }

    auto end() const noexcept -> const_iterator {
        return &childs[numChilds];
    }

    auto end() noexcept -> iterator {
        return &childs[numChilds];
    }

    auto find(std::byte key) const -> const_iterator {
        const auto endPtr = &keys[numChilds];
        auto resPtr = std::find(keys.begin(), endPtr, key);
        if (resPtr == endPtr) {
            return end();
        }

        auto idx = std::distance(keys.begin(), resPtr);
        return &childs[idx];
    };

    auto find(std::byte key) -> iterator {
        return const_cast<Child *>(const_cast<const Node4 *>(this)->find(key));
    };

    auto size() const -> size_t {
        return numChilds;
    }

    auto max_size() const -> size_t {
        return maxChilds;
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
