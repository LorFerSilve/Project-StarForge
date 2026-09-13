#pragma once

#include <expected>
#include <type_traits>
#include <utility>

namespace starforge::core {

template <typename T, typename E>
using Result = std::expected<T, E>;

template <typename E>
using Unexpected = std::unexpected<E>;

template <typename E>
[[nodiscard]] constexpr auto unexpected(E&& error) {
    return Unexpected<std::decay_t<E>>(std::forward<E>(error));
}

} // namespace starforge::core
