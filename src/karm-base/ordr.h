#pragma once

#include <karm-meta/traits.h>

#include "_prelude.h"

#include "clamp.h"

namespace Karm::Base {

struct Ordr {
    enum Value {
        LESS = -1,
        EQUAL = 0,
        GREATER = 1,
    };

    Value _value;

    constexpr static Ordr from(int cmp) {
        return (Value)clamp(cmp, LESS, GREATER);
    }

    constexpr Ordr(Value value) : _value(value) {}

    constexpr bool operator==(const Ordr &other) const = default;

    constexpr bool isEq() const { return _value == EQUAL; }
    constexpr bool isNe() const { return _value != EQUAL; }
    constexpr bool isLt() const { return _value == LESS; }
    constexpr bool isGt() const { return _value == GREATER; }
    constexpr bool isLtEq() const { return _value != GREATER; }
    constexpr bool isGtEq() const { return _value != LESS; }
};

template <typename T>
concept Ordered = requires(T const &lhs, T const &rhs) {
    { lhs.cmp(rhs) } -> Meta::Same<Ordr>;
};

namespace Op {

constexpr bool eq(Ordered auto const &lhs, Ordered auto const &rhs) {
    return lhs.cmp(rhs).isEq();
}

constexpr bool eq(auto const &lhs, auto const &rhs) requires(!Ordered<decltype(lhs)> && !Ordered<decltype(rhs)>) {
    return lhs == rhs;
}

constexpr bool ne(Ordered auto const &lhs, Ordered auto const &rhs) {
    return lhs.cmp(rhs).isNe();
}

constexpr bool ne(auto const &lhs, auto const &rhs) requires(!Ordered<decltype(lhs)> && !Ordered<decltype(rhs)>) {
    return lhs != rhs;
}

constexpr bool lt(Ordered auto const &lhs, Ordered auto const &rhs) {
    return lhs.cmp(rhs).isLt();
}

constexpr bool lt(auto const &lhs, auto const &rhs) requires(!Ordered<decltype(lhs)> && !Ordered<decltype(rhs)>) {
    return lhs < rhs;
}

constexpr bool gt(Ordered auto const &lhs, Ordered auto const &rhs) {
    return lhs.cmp(rhs).isGt();
}

constexpr bool gt(auto const &lhs, auto const &rhs) requires(!Ordered<decltype(lhs)> && !Ordered<decltype(rhs)>) {
    return lhs > rhs;
}

constexpr bool gteq(Ordered auto const &lhs, Ordered auto const &rhs) {
    return lhs.cmp(rhs).isGtEq();
}

constexpr bool gteq(auto const &lhs, auto const &rhs) requires(!Ordered<decltype(lhs)> && !Ordered<decltype(rhs)>) {
    return lhs >= rhs;
}

constexpr bool lteq(Ordered auto const &lhs, Ordered auto const &rhs) {
    return lhs.cmp(rhs).isLtEq();
}

constexpr bool lteq(auto const &lhs, auto const &rhs) requires(!Ordered<decltype(lhs)> && !Ordered<decltype(rhs)>) {
    return lhs <= rhs;
}

} // namespace Op

} // namespace Karm::Base
