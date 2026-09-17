#pragma once

#include <cstdint>

namespace starforge::audio {

enum class Bus : std::uint8_t { Master, Music, Dialogue, Sfx, UI, Ambience, VoiceRadio, Warnings };
enum class VoicePriority : std::uint8_t { Critical, Threat, Interaction, Local, Ambience, Decorative };

struct VoiceRequest {
    std::uint64_t correlation{};
    Bus bus{Bus::Sfx};
    VoicePriority priority{VoicePriority::Local};
    bool gameplay_audible{true};
};

struct MixSettings {
    float master_gain{1.0F};
    bool paused{false};
};

struct VoiceDecision {
    bool mix{false};
    float gain{0.0F};
    bool affects_ai_hearing{false};
};

[[nodiscard]] VoiceDecision evaluate_voice(const VoiceRequest& request,
                                           const MixSettings& settings) noexcept;

}  // namespace starforge::audio
