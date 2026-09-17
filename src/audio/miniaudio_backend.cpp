#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio.h>

#include "starforge/audio/miniaudio_backend.hpp"

#include <algorithm>
#include <string>
#include <utility>

namespace starforge::audio {

struct MiniaudioBackend::Impl {
    ma_engine engine{};
    bool initialized{false};
};

MiniaudioBackend::MiniaudioBackend() : impl_(std::make_unique<Impl>()) {}
MiniaudioBackend::~MiniaudioBackend() { shutdown(); }
MiniaudioBackend::MiniaudioBackend(MiniaudioBackend&&) noexcept = default;
MiniaudioBackend& MiniaudioBackend::operator=(MiniaudioBackend&&) noexcept = default;

bool MiniaudioBackend::initialize() noexcept {
    if (impl_->initialized) {
        return true;
    }
    if (ma_engine_init(nullptr, &impl_->engine) != MA_SUCCESS) {
        return false;
    }
    impl_->initialized = true;
    return true;
}

void MiniaudioBackend::shutdown() noexcept {
    if (!impl_ || !impl_->initialized) {
        return;
    }
    ma_engine_uninit(&impl_->engine);
    impl_->engine = {};
    impl_->initialized = false;
}

bool MiniaudioBackend::recover_device() noexcept {
    shutdown();
    return initialize();
}

bool MiniaudioBackend::play_file(const std::string_view path) noexcept {
    if (!impl_->initialized || path.empty()) {
        return false;
    }
    const std::string null_terminated_path{path};
    return ma_engine_play_sound(&impl_->engine, null_terminated_path.c_str(), nullptr) == MA_SUCCESS;
}

void MiniaudioBackend::set_master_gain(const float gain) noexcept {
    if (impl_->initialized) {
        ma_engine_set_volume(&impl_->engine, std::clamp(gain, 0.0F, 1.0F));
    }
}

bool MiniaudioBackend::initialized() const noexcept {
    return impl_ && impl_->initialized;
}

}  // namespace starforge::audio
