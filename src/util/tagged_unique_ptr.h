#pragma once

#include <memory>
#include <type_traits>
#include <fmt/core.h>
#include "util/assert_that.h"

namespace ext {

template<typename Needle, typename T, typename... Ts>
struct is_contained {
    static constexpr bool value = std::is_same_v<Needle, T> ? true : is_contained<Needle, Ts...>::value;
};

template<typename Needle, typename T>
struct is_contained<Needle, T> {
    static constexpr bool value = std::is_same_v<Needle, T>;
};

template<typename Needle, typename... Ts>
inline constexpr bool is_contained_v = is_contained<Needle, Ts...>::value;

template<class Needle, class Tuple>
struct index_of;

template<class Needle, class... Ts>
struct index_of<Needle, std::tuple<Needle, Ts...>> {
    static const std::size_t value = 0;
};

template<class Needle, class T, class... Ts>
struct index_of<Needle, std::tuple<T, Ts...>> {
    static const std::size_t value = 1 + index_of<Needle, std::tuple<Ts...>>::value;
};

template<typename Needle, typename... Ts>
inline constexpr size_t index_of_v = index_of<Needle, std::tuple<Ts...>>::value;

template<typename T>
void delete_funtion(void *pointer) {
    fmt::print("deleted an {}, through pointer {}\n", std::string(typeid(T).name()), pointer);
    delete (static_cast<T *>(pointer));
}

template<size_t num, typename TypeTuple>
inline constexpr auto deleter_of_v = &delete_funtion<std::tuple_element_t<num, TypeTuple>>;

template<size_t numTypes, typename TypeTuple, size_t index>
void conditionally_call_deleter_of(size_t num, void *pointer) {
    std::ignore = pointer;
    if constexpr (numTypes > index) {
        if (num == index) {
            deleter_of_v<index, TypeTuple>(pointer);
            return;
        }
    }
}

template<size_t numTypes, typename TypeTuple, std::size_t... Is>
void call_deleter_of_impl(size_t num, void *pointer, std::index_sequence<Is...>) {
    (conditionally_call_deleter_of<numTypes, TypeTuple, Is>(num, pointer), ...);
}

template<typename... Ts>
void call_deleter_of(size_t num, void *pointer) {
    call_deleter_of_impl<sizeof...(Ts), std::tuple<Ts...>>(num, pointer, std::index_sequence_for<Ts...>{});
}

template<typename ... Ts>
class tagged_unique_ptr {
private:
    static constexpr size_t alignment = std::alignment_of_v<void *>;
    static_assert(sizeof...(Ts) <= alignment,
                  "Can't discriminate between more types than there are spare types due to alignment");
    uintptr_t ownedPointer = reinterpret_cast<uintptr_t>(nullptr);

    static constexpr size_t get_tag_bitmask() { return alignment - 1; }

    uintptr_t
    release_tagged() noexcept {
        auto p = ownedPointer; // with tag
        ownedPointer = reinterpret_cast<uintptr_t>(nullptr);
        return p;
    }

public:
    tagged_unique_ptr() noexcept = default;

    template<typename T,
            typename = std::enable_if<is_contained_v<T, Ts...>>>
    explicit
    tagged_unique_ptr(T *p) noexcept : ownedPointer(reinterpret_cast<uintptr_t>(p)) {
        set_tag(index_of_v<T, Ts...>);
    }

    tagged_unique_ptr(nullptr_t) noexcept : tagged_unique_ptr() {}

    tagged_unique_ptr(tagged_unique_ptr &&other) noexcept : ownedPointer(other.release_tagged()) {}

    ~tagged_unique_ptr() noexcept {
        if (get() != nullptr) {
            call_deleter_of<Ts...>(get_tag(), get());
        }
        ownedPointer = reinterpret_cast<uintptr_t>(nullptr);
    }

    tagged_unique_ptr &
    operator=(tagged_unique_ptr &&other) noexcept {
        reset(other.ownedPointer);
        other.release();
        return *this;
    }

    tagged_unique_ptr &
    operator=(nullptr_t) noexcept {
        reset();
        return *this;
    }

    template<typename T>
    bool is_pointer_to() {
        return index_of_v<T, Ts...> == get_tag();
    }

    void *
    get() const noexcept { return reinterpret_cast<void *>(ownedPointer bitand (compl get_tag_bitmask())); }

    template<typename T>
    std::add_pointer_t<T>
    get() const noexcept {
        assert_that(index_of_v<T, Ts...> == get_tag());
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

    explicit operator bool() const noexcept { return get() != nullptr; }

    template<typename T>
    std::add_pointer_t<T>
    release() noexcept {
        assert_that(index_of_v<T, Ts...> == get_tag());
        void *p = get(); // for external consumption -> without tag
        ownedPointer = reinterpret_cast<uintptr_t>(nullptr);
        return reinterpret_cast<T *>(p);
    }

    void *
    release() noexcept {
        void *p = get(); // for external consumption -> without tag
        ownedPointer = reinterpret_cast<uintptr_t>(nullptr);
        return p;
    }

    void
    reset(nullptr_t newValue = nullptr) noexcept {
        void *oldValue = get(); // might need to delete oldValue
        size_t oldTag = get_tag(); // keep old tag
        ownedPointer = reinterpret_cast<uintptr_t>(newValue);
        if (oldValue != nullptr) {
            call_deleter_of<Ts...>(oldTag, oldValue);
        }
    }

    template<typename T,
            typename = std::enable_if<is_contained_v<T, Ts...>>>
    void
    reset(T *newValue = nullptr) noexcept {
        void *oldValue = get(); // might need to delete oldValue
        size_t oldTag = get_tag(); // keep old tag
        ownedPointer = reinterpret_cast<uintptr_t>(newValue);
        set_tag(index_of_v<T, Ts...>);
        if (oldValue != nullptr) {
            call_deleter_of<Ts...>(oldTag, oldValue);
        }
    }

    void
    reset(uintptr_t newValue) noexcept {
        void *oldValue = get(); // might need to delete oldValue
        size_t oldTag = get_tag();
        ownedPointer = newValue;
        if (oldValue != nullptr) {
            call_deleter_of<Ts...>(oldTag, oldValue);
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

template<typename PointerType, typename ValueType, typename... Args>
inline
auto make_tagged_unique(Args &&... args) -> PointerType {
    return PointerType(new ValueType(std::forward<Args>(args)...));
}

template<typename PointerType, typename ValueType>
inline
auto make_tagged_unique_inline(ValueType &&arg) -> PointerType {
    void* ptr = new std::byte[sizeof(ValueType)];
    ValueType* valuePointer = new(ptr) ValueType(std::forward<ValueType>(arg));
    return PointerType(valuePointer);
}

}

