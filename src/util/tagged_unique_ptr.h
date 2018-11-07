#pragma once

#include <memory>
#include <type_traits>

namespace ext {

template<typename T>
void deleteVoidPtr(void *target) {
    delete static_cast<T *>(target);
}

template <size_t alignment = std::alignment_of_v<void *>>
class tagged_unique_ptr {
public:
    using deleterType = decltype(&deleteVoidPtr<nullptr_t>);
private:
    deleterType deleter;
    uintptr_t ownedPointer;

    static constexpr size_t get_tag_bitmask() { return alignment - 1; }

    uintptr_t
    release_tagged() noexcept {
        auto p = ownedPointer; // with tag
        ownedPointer = reinterpret_cast<uintptr_t>(nullptr);
        deleter = nullptr;
        return p;
    }

public:
    tagged_unique_ptr() noexcept = default;

    template<typename T>
    explicit
    tagged_unique_ptr(T *p, deleterType deleter = &deleteVoidPtr<T>) noexcept :
            deleter(deleter),
            ownedPointer(reinterpret_cast<uintptr_t>(p)) {}

    tagged_unique_ptr(nullptr_t) noexcept : tagged_unique_ptr() {}

    tagged_unique_ptr(tagged_unique_ptr &&other) noexcept :
            deleter(other.deleter),
            ownedPointer(other.release_tagged()) {}

    ~tagged_unique_ptr() noexcept {
        if (get() != nullptr) {
            deleter(get());
        }
        ownedPointer = reinterpret_cast<uintptr_t>(nullptr);
        deleter = nullptr;
    }

    tagged_unique_ptr &
    operator=(tagged_unique_ptr &&other) noexcept {
        reset(other.ownedPointer, other.deleter);
        other.release();
        return *this;
    }

    tagged_unique_ptr &
    operator=(nullptr_t) noexcept {
        reset();
        return *this;
    }

    void *
    get() const noexcept { return reinterpret_cast<void *>(ownedPointer bitand (compl get_tag_bitmask())); }

    template<typename T>
    std::add_pointer_t<T>
    get() const noexcept {
        return reinterpret_cast<std::add_pointer_t<T>>(ownedPointer bitand (compl get_tag_bitmask()));
    }

    size_t
    get_tag() const noexcept { return ownedPointer bitand get_tag_bitmask(); }

    void
    set_tag(size_t tag) noexcept {
        assert_that(tag < alignment);
        ownedPointer |= tag;
    }

    tagged_unique_ptr &&
    with_tag(size_t tag) && noexcept {
        set_tag(tag);
        return std::move(*this);
    }

    explicit operator bool() const noexcept { return not(get() == nullptr); }

    template<typename T>
    std::add_pointer_t<T>
    release() noexcept {
        void *p = get(); // for external consumption -> without tag
        ownedPointer = reinterpret_cast<uintptr_t>(nullptr);
        deleter = nullptr;
        return p;
    }

    void *
    release() noexcept {
        void *p = get(); // for external consumption -> without tag
        ownedPointer = reinterpret_cast<uintptr_t>(nullptr);
        deleter = nullptr;
        return p;
    }

    template<typename T = void>
    void
    reset(T* newValue = nullptr, deleterType newDeleter = &deleteVoidPtr<T>) noexcept {
        void *oldValue = get(); // might need to delete oldValue
        size_t oldTag = get_tag(); // keep old tag
        ownedPointer = reinterpret_cast<uintptr_t>(newValue);
        set_tag(oldTag);
        if (oldValue != nullptr) {
            deleter(oldValue);
        }
        deleter = newDeleter;
    }

    void
    reset(uintptr_t newValue, deleterType newDeleter) noexcept {
        void *oldValue = get(); // might need to delete oldValue
        ownedPointer = newValue;
        if (oldValue != nullptr) {
            deleter(oldValue);
        }
        deleter = newDeleter;
    }

    void
    swap(tagged_unique_ptr &other) noexcept {
        using std::swap;
        swap(ownedPointer, other.ownedPointer);
        swap(deleter, other.deleter);
    }

    tagged_unique_ptr(const tagged_unique_ptr &) = delete;

    tagged_unique_ptr &operator=(const tagged_unique_ptr &) = delete;
};


template<typename T, typename... Args>
inline
auto make_tagged_unique(Args &&... args) -> tagged_unique_ptr {
    return tagged_unique_ptr(new T(std::forward<Args>(args)...));
}

}

