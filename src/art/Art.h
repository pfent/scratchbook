#pragma once

#include <variant>
#include <any>
#include <memory>
#include "Node4.h"
#include "Node16.h"
#include "Node48.h"
#include "../util/assert_that.h"
#include "../util/tagged_unique_ptr.h"

/**
 * Fancy type alias, so a same type containing next pointer works
 */
template<typename T>
struct NodeImpl {
    using Inner = NodeImpl;
    using Leaf = T;
    using NextPtr = ext::tagged_unique_ptr<Inner, Leaf>;

    std::variant<
            Node4<NextPtr>,
            Node16<NextPtr>,
            Node48<NextPtr>
    > data;
};

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
    using Node = NodeImpl<T>;
    using NextPtr = typename Node::NextPtr;
    using LeafNode = typename Node::Leaf;
    using InnerNode = typename Node::Inner;
    std::unique_ptr<Node> root = std::make_unique<Node>(); // Node4 by default
public:
    auto add(std::byte *keyBegin, std::byte *keyEnd, T value) -> void {
        auto *currentNode = root.get();
        auto lastKey = keyEnd - 1;
        for (auto currentKey = keyBegin; currentKey != keyEnd; ++currentKey) {
            std::visit([&](auto /* NodeImpl */ &nodeWithAny) {
                NextPtr *found = nodeWithAny.find(*currentKey);
                if (found == nodeWithAny.end()) {
                    // not found -> either insert value or construct full path
                    // TODO: extract to function and grow
                    if (currentKey == lastKey) {
                        nodeWithAny.insert(*currentKey, ext::make_tagged_unique_inline<NextPtr>(std::move(value)));
                    } else {
                        nodeWithAny.insert(*currentKey, ext::make_tagged_unique<NextPtr, Node>()); // Node4 by default
                    }
                    return /* @visit */;
                }
                if (found->template points_to<LeafNode>()) {
                    // key already in tree, nothing to do
                    // we do not allow prefixes of existing trees
                    assert_that(currentKey == lastKey);
                    currentKey = lastKey;
                    return /* @visit */;
                }
                if (found->template points_to<InnerNode>()) {
                    currentNode = found->template get<InnerNode>();
                    return /* @visit */;
                }
                __builtin_unreachable();
            }, currentNode->data);
        }
    }

    auto find(std::byte *keyBegin, std::byte *keyEnd) -> T * {
        auto *currentNode = root.get();
        auto lastKey = keyEnd - 1;
        T *result = nullptr;
        for (auto currentKey = keyBegin; currentKey != keyEnd; ++currentKey) {
            std::visit([&](auto /* Node{4,16,48,256}<any> */ &nodeWithAny) {
                NextPtr *found = nodeWithAny.find(*currentKey);
                if (found == nodeWithAny.end()) {
                    // not found -> stop looking
                    currentKey = lastKey;
                    result = nullptr; // TODO: invalid iterator
                    return /* @visit */;
                }
                if (found->template points_to<LeafNode>()) {
                    assert_that(currentKey == lastKey);
                    currentKey = lastKey;
                    result = found->template get<LeafNode>();
                    return /* @visit */;
                }
                if (found->template points_to<InnerNode>()) {
                    currentNode = found->template get<InnerNode>();
                    return /* @visit */;
                }
                __builtin_unreachable();
            }, *currentNode);
        }
        return result;
    }
};
