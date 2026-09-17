#include "starforge/audio/audio.hpp"

#include <algorithm>

namespace starforge::audio {

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
