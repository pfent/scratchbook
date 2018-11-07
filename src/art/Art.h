#pragma once

#include <variant>
#include <any>
#include <memory>
#include "Node4.h"
#include "Node16.h"
#include "Node48.h"
#include "../util/assert_that.h"

/**
 * Adaptive Radix Tree
 * Maps keys to values, with lookup complexity proportional to the key length
 * Keys need to be prefix-free, i.e. no key can be a prefix of any other.
 * Please note, that C-style strings satisfy this constraint, since the terminating \0 byte prevents prefixes.
 * Equal length keys (e.g. int) are also fine
 *
 * @tparam T The value stored in this tree
 */
template<typename T>
class Art {
    using Node = std::variant<
            Node4<std::any>
            //Node16<std::any>,
            //Node48<std::any>
    >;

    // We might also use two tagged_unique_ptrs for the type?
    using Inner = Node;
    using Leaf = T;

    std::unique_ptr<Node> root = std::make_unique<Node>(); // Node4 by default
public:
    auto add(std::byte *keyBegin, std::byte *keyEnd, T value) -> void {
        auto *currentNode = root.get();
        auto lastKey = keyEnd - 1;
        for (auto currentKey = keyBegin; currentKey != keyEnd; ++currentKey) {
            std::visit([&](auto /* Node{4,16,48,256}<any> */ nodeWithAny) {
                std::any *found = nodeWithAny.find(*currentKey);
                if (found == nodeWithAny.end()) {
                    // not found -> either insert value or construct full path
                    // TODO: extract to function and grow
                    if (currentKey == lastKey) {
                        nodeWithAny.insert(*currentKey, std::any(std::move(value)));
                    } else {
                        nodeWithAny.insert(*currentKey, std::any(Node())); // Node4 by default
                    }
                    return /* @visit */;
                }
                if (found->type() == typeid(Leaf)) {
                    // key already in tree, nothing to do
                    // we do not allow prefixes of existing trees
                    assert_that(currentKey == lastKey);
                    currentKey = lastKey;
                    return /* @visit */;
                }
                if (found->type() == typeid(Inner)) {
                    currentNode = std::any_cast<Inner>(found);
                    return /* @visit */;
                }
                __builtin_unreachable();
            }, *currentNode);
        }
    }

    auto find(std::byte *keyBegin, std::byte* keyEnd) -> T* {
        auto *currentNode = root.get();
        auto lastKey = keyEnd - 1;
        T* result = nullptr;
        for (auto currentKey = keyBegin; currentKey != keyEnd; ++currentKey) {
            std::visit([&](auto /* Node{4,16,48,256}<any> */ nodeWithAny) {
                std::any *found = nodeWithAny.find(*currentKey);
                if (found == nodeWithAny.end()) {
                    // not found -> stop looking
                    currentKey = lastKey;
                    result = nullptr; // TODO: invalid iterator
                    return /* @visit */;
                }
                if (found->type() == typeid(Leaf)) {
                    assert_that(currentKey == lastKey);
                    currentKey = lastKey;
                    result = std::any_cast<Leaf>(found);
                    return /* @visit */;
                }
                if (found->type() == typeid(Inner)) {
                    currentNode = std::any_cast<Inner>(found);
                    return /* @visit */;
                }
                __builtin_unreachable();
            }, *currentNode);
        }
        return result;
    }
};
