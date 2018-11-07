#pragma once

#ifndef NDEBUG

#include <exception>

#endif

namespace {
#ifdef NDEBUG
constexpr bool real_assert = false;
#else
constexpr bool real_assert = true;
#endif
}

template<typename Expr>
auto assert_that(Expr e) {
    if constexpr (real_assert) {
        if (not static_cast<bool>(e)) {
            throw std::runtime_error("assertion failed");
        }
    }
}
