#include "starforge/audio/audio.hpp"

#include <algorithm>
#include <type_traits>

namespace starforge::audio {

bool player_audible(const PresentationAudioEvent& event) noexcept {
    if (!event.delivered || !event.player_facing_allowed) {
        return false;
    }

    switch (event.medium) {
        case AudioMedium::Atmosphere:
        case AudioMedium::ThinAtmosphere:
        case AudioMedium::StructureConduction:
        case AudioMedium::SuitInternal:
        case AudioMedium::Radio:
        case AudioMedium::Telemetry:
            return true;
        case AudioMedium::Vacuum:
            return false;
    }
    return false;
}

bool presentation_audio_eligible(const PresentationAudioEvent& event,
                                 const ActiveAudioContext& context) noexcept {
    if (!event.committed || !player_audible(event)) {
        return false;
    }
    if (event.scene_generation != 0U && event.scene_generation != context.scene_generation) {
        return false;
    }
    if (event.origin_epoch != 0U && event.origin_epoch != context.origin_epoch) {
        return false;
    }
    return true;
}

bool may_preempt(const VoicePriority incoming, const VoicePriority incumbent) noexcept {
    using Underlying = std::underlying_type_t<VoicePriority>;
    return static_cast<Underlying>(incoming) < static_cast<Underlying>(incumbent);
}

VoiceDecision evaluate_voice(const VoiceRequest& request, const MixSettings& settings) noexcept {
    VoiceDecision decision{};
    decision.affects_ai_hearing = false;
    if (settings.paused && request.bus != Bus::UI) {
        return decision;
    }
    decision.gain = std::clamp(settings.master_gain, 0.0F, 1.0F);
    decision.mix = decision.gain > 0.0F;
    return decision;
}

}  // namespace starforge::audio
