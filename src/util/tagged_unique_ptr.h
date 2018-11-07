#pragma once

#include <memory>
#include <type_traits>

namespace ext {

template<typename T, size_t alignment = std::alignment_of_v<T>>
class tagged_unique_ptr {
public:
    using pointer       = std::add_pointer_t<std::remove_reference_t<T>>;
    using element_type  = T;
private:

    uintptr_t ownedPointer;

    static constexpr size_t get_tag_bitmask() { return alignment - 1; }

    uintptr_t
    release_tagged() noexcept {
        auto p = ownedPointer; // with tag
        ownedPointer = reinterpret_cast<uintptr_t>(pointer());
        return p;
    }

public:
    constexpr tagged_unique_ptr() noexcept = default;

    explicit
    tagged_unique_ptr(pointer p) noexcept: ownedPointer(reinterpret_cast<uintptr_t>(p)) {}

    tagged_unique_ptr(nullptr_t) noexcept : tagged_unique_ptr() {}

    tagged_unique_ptr(tagged_unique_ptr &&other) noexcept : ownedPointer(other.release_tagged()) {}

    template<typename OtherType, size_t OtherAlignment,
            typename = std::enable_if<std::is_convertible_v<OtherType, pointer>>>
    tagged_unique_ptr(tagged_unique_ptr<OtherType, OtherAlignment> &&other) noexcept
            : ownedPointer(other.release_tagged()) {}

    ~tagged_unique_ptr() noexcept {
        if (get() != nullptr) {
            delete get();
        }
        ownedPointer = reinterpret_cast<uintptr_t>(pointer());
    }

    tagged_unique_ptr &
    operator=(tagged_unique_ptr &&other) noexcept {
        reset(other.release());
        return *this;
    }

    template<typename OtherType, std::enable_if_t<std::is_convertible_v<OtherType, pointer>>>
    tagged_unique_ptr &
    operator=(tagged_unique_ptr<OtherType> &&other) noexcept {
        reset(other.release());
        return *this;
    }

    tagged_unique_ptr &
    operator=(nullptr_t) noexcept {
        reset();
        return *this;
    }

    typename std::add_lvalue_reference<element_type>::type
    operator*() const {
        assert(get() != pointer());
        return *get();
    }

    pointer
    operator->() const noexcept {
        assert(get() != pointer());
        return get();
    }

    pointer
    get() const noexcept { return reinterpret_cast<pointer>(ownedPointer bitand (compl get_tag_bitmask())); }

    size_t
    get_tag() const noexcept { return ownedPointer bitand get_tag_bitmask(); }

    void
    set_tag(size_t tag) noexcept {
        assert(tag < alignment);
        ownedPointer |= tag;
    }

    tagged_unique_ptr &&
    with_tag(size_t tag) && noexcept {
        set_tag(tag);
        return std::move(*this);
    }

    explicit operator bool() const noexcept { return not(get() == pointer()); }

    pointer
    release() noexcept {
        pointer p = get(); // for external consumption -> without tag
        ownedPointer = pointer();
        return p;
    }

    void
    reset(pointer newValue = pointer()) noexcept {
        using std::swap;
        pointer oldValue = get();
        size_t oldTag = get_tag(); // keep old tag
        ownedPointer = newValue;
        set_tag(oldTag);
        if (oldValue != pointer()) {
            delete oldValue;
        }
    }

    void
    swap(tagged_unique_ptr &other) noexcept {
        using std::swap;
        swap(ownedPointer, other.ownedPointer);
    }

    tagged_unique_ptr(const tagged_unique_ptr &) = delete;

    tagged_unique_ptr &operator=(const tagged_unique_ptr &) = delete;
};


template<typename T, typename... Args>
inline
auto make_tagged_unique(Args &&... args) -> tagged_unique_ptr<T> {
    return tagged_unique_ptr<T>(new T(std::forward<Args>(args)...));
}

}

