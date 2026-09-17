#pragma once

#include <memory>
#include <string_view>

namespace starforge::audio {

class MiniaudioBackend final {
  public:
    MiniaudioBackend();
    ~MiniaudioBackend();

    MiniaudioBackend(const MiniaudioBackend&) = delete;
    MiniaudioBackend& operator=(const MiniaudioBackend&) = delete;
    MiniaudioBackend(MiniaudioBackend&&) noexcept;
    MiniaudioBackend& operator=(MiniaudioBackend&&) noexcept;

    [[nodiscard]] bool initialize() noexcept;
    void shutdown() noexcept;
    [[nodiscard]] bool recover_device() noexcept;
    [[nodiscard]] bool play_file(std::string_view path) noexcept;
    void set_master_gain(float gain) noexcept;
    [[nodiscard]] bool initialized() const noexcept;

  private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

}  // namespace starforge::audio
