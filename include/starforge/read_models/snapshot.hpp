#pragma once

#include "starforge/core/strong_id.hpp"

#include <memory>
#include <utility>

namespace starforge::read_models {

template <typename T>
class SnapshotPublisher final {
public:
    explicit SnapshotPublisher(T initial)
        : snapshot_(std::make_shared<const T>(std::move(initial))) {}

    [[nodiscard]] std::shared_ptr<const T> snapshot() const noexcept { return snapshot_; }
    [[nodiscard]] core::ReadModelGeneration generation() const noexcept { return generation_; }

    void publish(T value) {
        snapshot_ = std::make_shared<const T>(std::move(value));
        generation_ = core::ReadModelGeneration{generation_.value() + 1};
    }

private:
    std::shared_ptr<const T> snapshot_;
    core::ReadModelGeneration generation_{1};
};

[[nodiscard]] const char* contract_name() noexcept;

} // namespace starforge::read_models
