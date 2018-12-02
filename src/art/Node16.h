#ifndef SCRATCHBOOK_NODE16_H
#define SCRATCHBOOK_NODE16_H

#include <array>
#include <algorithm>
#include <cstdint>

template<typename Child>
class Node16 {
    static constexpr auto maxChilds = 16;
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
        return const_cast<Child *>(const_cast<const Node16 *>(this)->find(key));
    };

    auto size() const -> std::size_t {
        return numChilds;
    }

    auto max_size() const -> std::size_t {
        return maxChilds;
    }

    auto insert(std::byte key, Child child) -> void {
        if (size() >= max_size()) {
            throw std::runtime_error("Node16 is already full!");
        };
        if (find(key) != end()) {
            throw std::runtime_error("Duplicate key!");
        }
        insert_unsafe(key, std::move(child));
    }

    auto insert_unsafe(std::byte key, Child child) -> void {
        // find space
        auto position = std::distance(keys.begin(), std::lower_bound(keys.begin(), keys.end(), key));

        // make space
        std::move(&keys[position], &keys[numChilds], &keys[position + 1]);
        std::move(&childs[position], &childs[numChilds], &childs[position + 1]);

        // insert into space
        childs[position] = std::move(child);
        keys[position] = key;
        ++numChilds;
    };
};

#endif //SCRATCHBOOK_NODE16_H
