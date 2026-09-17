#include "starforge/ui/presentation.hpp"

#include <algorithm>

namespace starforge::ui {
namespace {
constexpr float kMinimumUiScale = 0.8F;
constexpr float kMaximumUiScale = 1.5F;

[[nodiscard]] bool can_focus(const Element& element) noexcept {
    return element.visible && element.enabled && element.focusable && !element.reveals_hidden_knowledge;
}

[[nodiscard]] std::uint8_t precision_rank(const MarkerPrecision precision) noexcept {
    return static_cast<std::uint8_t>(precision);
}

[[nodiscard]] bool generation_field_matches(const std::uint64_t candidate,
                                            const std::uint64_t active) noexcept {
    return candidate == 0U || candidate == active;
}
}  // namespace

bool ui_scale_supported(const float scale) noexcept {
    return scale >= kMinimumUiScale && scale <= kMaximumUiScale;
}

float clamp_ui_scale(const float scale) noexcept {
    return std::clamp(scale, kMinimumUiScale, kMaximumUiScale);
}

void FocusScope::set_elements(std::vector<Element> elements) {
    std::optional<std::uint64_t> retained_id;
    if (const auto* current = focused(); current != nullptr) {
        retained_id = current->id;
    }
    elements_ = std::move(elements);
    focused_index_.reset();
    if (retained_id.has_value() && focus(*retained_id)) {
        return;
    }
    const auto first = std::find_if(elements_.begin(), elements_.end(), can_focus);
    if (first != elements_.end()) {
        focused_index_ = static_cast<std::size_t>(std::distance(elements_.begin(), first));
    }
}

const Element* FocusScope::focused() const noexcept {
    if (!focused_index_ || *focused_index_ >= elements_.size()) {
        return nullptr;
    }
    return &elements_[*focused_index_];
}

bool FocusScope::focus(const std::uint64_t id) noexcept {
    const auto it = std::find_if(elements_.begin(), elements_.end(), [id](const Element& element) {
        return element.id == id && can_focus(element);
    });
    if (it == elements_.end()) {
        return false;
    }
    focused_index_ = static_cast<std::size_t>(std::distance(elements_.begin(), it));
    return true;
}

bool FocusScope::next() noexcept {
    if (elements_.empty()) {
        return false;
    }
    if (!focused_index_) {
        const auto first = std::find_if(elements_.begin(), elements_.end(), can_focus);
        if (first == elements_.end()) {
            return false;
        }
        focused_index_ = static_cast<std::size_t>(std::distance(elements_.begin(), first));
        return true;
    }
    for (std::size_t index = *focused_index_ + 1U; index < elements_.size(); ++index) {
        if (can_focus(elements_[index])) {
            focused_index_ = index;
            return true;
        }
    }
    if (wrap_navigation_) {
        for (std::size_t index = 0; index < *focused_index_; ++index) {
            if (can_focus(elements_[index])) {
                focused_index_ = index;
                return true;
            }
        }
    }
    return false;
}

bool FocusScope::previous() noexcept {
    if (elements_.empty()) {
        return false;
    }
    if (!focused_index_) {
        for (std::size_t index = elements_.size(); index-- > 0;) {
            if (can_focus(elements_[index])) {
                focused_index_ = index;
                return true;
            }
        }
        return false;
    }
    for (std::size_t index = *focused_index_; index-- > 0;) {
        if (can_focus(elements_[index])) {
            focused_index_ = index;
            return true;
        }
    }
    if (wrap_navigation_) {
        for (std::size_t index = elements_.size(); index-- > *focused_index_ + 1U;) {
            if (can_focus(elements_[index])) {
                focused_index_ = index;
                return true;
            }
        }
    }
    return false;
}

void FocusScope::invalidate() noexcept {
    elements_.clear();
    focused_index_.reset();
}

bool marker_eligible(const MarkerDescriptor& marker) noexcept {
    if (!marker.known_to_player || marker.knowledge == KnowledgeState::Unknown ||
        marker.precision == MarkerPrecision::Unknown) {
        return false;
    }
    return !marker.through_wall || marker.through_wall_authorized;
}

bool marker_precision_allowed(const MarkerPrecision available, const MarkerPrecision requested) noexcept {
    if (available == MarkerPrecision::Unknown || requested == MarkerPrecision::Unknown) {
        return false;
    }
    return precision_rank(requested) <= precision_rank(available);
}

bool notifications_groupable(const NotificationEvent& lhs, const NotificationEvent& rhs) noexcept {
    return lhs.committed && rhs.committed && lhs.groupable && rhs.groupable && lhs.semantic_group != 0U &&
           lhs.semantic_group == rhs.semantic_group;
}

bool generation_compatible(const PresentationGenerationStamp& candidate,
                           const PresentationGenerationStamp& active) noexcept {
    return generation_field_matches(candidate.scene_generation, active.scene_generation) &&
           generation_field_matches(candidate.origin_epoch, active.origin_epoch) &&
           generation_field_matches(candidate.content_generation, active.content_generation) &&
           generation_field_matches(candidate.read_model_generation, active.read_model_generation);
}

bool PresentationEventCursor::consume(const PresentationEvent& event) noexcept {
    if (!event.committed) {
        ++diagnostics_.uncommitted;
        return false;
    }
    if (scene_generation_ != 0U && event.scene_generation != 0U &&
        event.scene_generation != scene_generation_) {
        ++diagnostics_.stale_scene;
        return false;
    }
    if (event.sequence == 0U || event.sequence <= last_sequence_) {
        ++diagnostics_.duplicate_or_out_of_order;
        return false;
    }
    last_sequence_ = event.sequence;
    ++diagnostics_.consumed;
    return true;
}

void PresentationEventCursor::reset_for_scene(const std::uint64_t scene_generation) noexcept {
    scene_generation_ = scene_generation;
    last_sequence_ = 0U;
    diagnostics_ = {};
}

std::vector<CueChannel> effective_alarm_channels(const AlarmPresentation& alarm,
                                                 const AccessibilitySettings& settings) {
    if (!alarm.known_to_player) {
        return {};
    }
    auto channels = alarm.channels;
    if (settings.master_muted) {
        std::erase(channels, CueChannel::Audio);
    }
    const bool critical = alarm.priority == AlarmPriority::P0 || alarm.priority == AlarmPriority::P1;
    if (critical) {
        if (std::find(channels.begin(), channels.end(), CueChannel::Visual) == channels.end()) {
            channels.push_back(CueChannel::Visual);
        }
        const bool has_non_visual = std::any_of(channels.begin(), channels.end(), [](CueChannel channel) {
            return channel != CueChannel::Visual;
        });
        if (!has_non_visual) {
            channels.push_back(CueChannel::Caption);
        }
    }
    return channels;
}

bool caption_eligible(const CaptionEvent& event, const AccessibilitySettings& settings) noexcept {
    return settings.closed_captions && event.legitimately_audible && event.known_to_player;
}

PresentationEffect apply_accessibility(PresentationEffect effect,
                                       const AccessibilitySettings& settings) noexcept {
    if (settings.photosensitivity_safe) {
        effect.rapid_flash = false;
    }
    if (settings.reduced_motion && effect.decorative) {
        effect.camera_motion = false;
    }
    if (settings.reduced_effects && effect.decorative) {
        effect.enabled = false;
    }
    return effect;
}

PresentationBudgetStatus evaluate_presentation_budget(const PresentationBudgetSnapshot& snapshot,
                                                       const PresentationBudgetLimits& limits) noexcept {
    PresentationBudgetStatus status{};
    status.ui_within_limits = snapshot.live_ui_nodes <= limits.live_ui_nodes &&
                              snapshot.visible_ui_nodes <= limits.visible_ui_nodes &&
                              snapshot.focusable_nodes <= limits.focusable_nodes;
    status.alarms_within_limits = snapshot.active_alarms <= limits.active_alarms;
    status.events_within_soft_limit =
        snapshot.queued_presentation_events <= limits.presentation_events_soft;
    status.events_within_hard_limit =
        snapshot.queued_presentation_events <= limits.presentation_events_hard;
    status.audio_within_limits = snapshot.logical_audio_voices <= limits.logical_audio_voices &&
                                 snapshot.mixed_audio_voices <= limits.mixed_audio_voices &&
                                 snapshot.spatial_audio_voices <= limits.spatial_audio_voices &&
                                 snapshot.streamed_audio_voices <= limits.streamed_audio_voices;
    status.audio_accounting_consistent =
        snapshot.mixed_audio_voices <= snapshot.logical_audio_voices &&
        snapshot.spatial_audio_voices <= snapshot.mixed_audio_voices &&
        snapshot.streamed_audio_voices <= snapshot.logical_audio_voices;
    status.subtitles_captions_within_limits =
        snapshot.subtitle_tracks <= limits.subtitle_tracks && snapshot.caption_cues <= limits.caption_cues;
    status.vfx_within_limits = snapshot.vfx_intents_this_frame <= limits.vfx_intents_per_frame;

    if (!status.events_within_hard_limit || !status.audio_accounting_consistent) {
        status.pressure = PresentationPressure::Hard;
    } else if (!status.within_limits()) {
        status.pressure = PresentationPressure::Soft;
    }
    return status;
}

}  // namespace starforge::ui
