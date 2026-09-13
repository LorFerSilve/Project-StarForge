#pragma once

#include <optional>
#include <utility>
#include <variant>

namespace starforge::core {

template <typename T, typename E>
class [[nodiscard]] Result {
public:
    [[nodiscard]] static Result success(T value) {
        return Result(std::in_place_index<0>, std::move(value));
    }

    [[nodiscard]] static Result failure(E error) {
        return Result(std::in_place_index<1>, std::move(error));
    }

    [[nodiscard]] bool has_value() const noexcept { return storage_.index() == 0U; }
    [[nodiscard]] explicit operator bool() const noexcept { return has_value(); }

    [[nodiscard]] T& value() & { return std::get<0>(storage_); }
    [[nodiscard]] const T& value() const& { return std::get<0>(storage_); }
    [[nodiscard]] T&& value() && { return std::get<0>(std::move(storage_)); }

    [[nodiscard]] E& error() & { return std::get<1>(storage_); }
    [[nodiscard]] const E& error() const& { return std::get<1>(storage_); }

private:
    template <std::size_t Index, typename U>
    explicit Result(std::in_place_index_t<Index> index, U&& value)
        : storage_(index, std::forward<U>(value)) {}

    std::variant<T, E> storage_;
};

template <typename E>
class [[nodiscard]] Result<void, E> {
public:
    [[nodiscard]] static Result success() { return Result(std::nullopt); }
    [[nodiscard]] static Result failure(E error) { return Result(std::move(error)); }

    [[nodiscard]] bool has_value() const noexcept { return !error_.has_value(); }
    [[nodiscard]] explicit operator bool() const noexcept { return has_value(); }

    [[nodiscard]] E& error() & { return error_.value(); }
    [[nodiscard]] const E& error() const& { return error_.value(); }

private:
    explicit Result(std::optional<E> error) : error_(std::move(error)) {}
    explicit Result(E error) : error_(std::move(error)) {}

    std::optional<E> error_;
};

}  // namespace starforge::core
