#pragma once

#include "starforge/content/content_model.hpp"

#include <cstddef>
#include <span>

namespace starforge::content {

[[nodiscard]] ContentFingerprint sha256(std::span<const std::byte> bytes);

}  // namespace starforge::content
