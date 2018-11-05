#pragma once

#include <type_traits>
#include <utility>
#include <variant>
#include <atomic>
#include <thread>
#include <functional>

/**
 * Lazy initialization wrapper around values.
 */
template<typename Result>
class Lazy {
    enum class State {
        initialized,
        executing_function,
        just_function,
    };

    union {
        mutable Result result;
        mutable std::function<Result()> function;
    };

    mutable
    std::atomic<State> state = State::just_function;

public:
    template <typename Fun>
    explicit constexpr
    Lazy(Fun &&function) noexcept : function(std::forward<Fun>(function)) {
    }

    ~Lazy() {
        if(state == State::initialized) {
            result.~Result();
        } else {
            function.~function();
        }
    }

    constexpr
    auto get() noexcept -> Result & {
        auto currentState = state.load(std::memory_order_acquire);
        // most likely case
        if (state == State::initialized) {
            return result;
        }
        if (state == State::executing_function) {
            // concurrent initialization -> restart and try if initialization finished when we access next time
            std::this_thread::yield();
            return get();
        }
        if (not state.compare_exchange_strong(currentState, State::executing_function)) {
            // race -> also restart
            std::this_thread::yield();
            return get();
        }
        // state == executing_function
        result = [&] {
            auto res = function();
            function.~function();
            return res;
        }();
        state.store(State::initialized, std::memory_order_release);
        return result;
    }

    constexpr
    auto get() const noexcept -> Result const & {
        auto currentState = state.load(std::memory_order_acquire);
        // most likely case
        if (state == State::initialized) {
            return result;
        }
        if (state == State::executing_function) {
            // concurrent initialization -> restart and try if initialization finished when we access next time
            std::this_thread::yield();
            return get();
        }
        if (not state.compare_exchange_strong(currentState, State::executing_function)) {
            // race -> also restart
            std::this_thread::yield();
            return get();
        }
        // state == executing_function
        result = [&] {
            auto res = function();
            function.~function();
            return res;
        }();
        state.store(State::initialized, std::memory_order_release);
        return result;
    }

    constexpr
    operator Result&() noexcept {
        return get();
    }

    constexpr
    operator Result const &() const noexcept {
        return get();
    }
};

template<typename Fun> Lazy(Fun &&arg) -> Lazy<std::result_of_t<Fun()>>;
