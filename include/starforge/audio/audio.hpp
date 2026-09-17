#pragma once

#include <cstdint>

namespace starforge::audio {

enum class Bus : std::uint8_t { Master, Music, Dialogue, Sfx, UI, Ambience, VoiceRadio, Warnings };
enum class VoicePriority : std::uint8_t { Critical, Threat, Interaction, Local, Ambience, Decorative };
enum class AudioMedium : std::uint8_t {
    Atmosphere,
    ThinAtmosphere,
    Vacuum,
    StructureConduction,
    SuitInternal,
    Radio,
    Telemetry
};

struct GameplaySoundEvent {
    std::uint64_t correlation{};
    bool audible_to_ai{false};
};

struct PresentationAudioEvent {
    std::uint64_t correlation{};
    AudioMedium medium{AudioMedium::Atmosphere};
    bool delivered{false};
    bool player_facing_allowed{false};
    bool committed{false};
    std::uint64_t scene_generation{};
    std::uint64_t origin_epoch{};
};

struct ActiveAudioContext {
    std::uint64_t scene_generation{};
    std::uint64_t origin_epoch{};
};

[[nodiscard]] bool player_audible(const PresentationAudioEvent& event) noexcept;
[[nodiscard]] bool presentation_audio_eligible(const PresentationAudioEvent& event,
                                               const ActiveAudioContext& context) noexcept;
[[nodiscard]] bool may_preempt(VoicePriority incoming, VoicePriority incumbent) noexcept;

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
