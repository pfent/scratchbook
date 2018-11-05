#pragma once

/**
 * Combine multiple lambdas to a single one with overloaded operator()
 */
template<class... Ts>
struct overloaded : Ts ... {
    using Ts::operator()...;
};
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;
