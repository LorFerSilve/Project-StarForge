#include "starforge/ui/presentation.hpp"

#include <algorithm>

namespace starforge::ui {
namespace {
[[nodiscard]] bool can_focus(const Element& element) noexcept {
    return element.visible && element.enabled && element.focusable && !element.reveals_hidden_knowledge;
}
}  // namespace

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
    return effect;
}

}  // namespace starforge::ui
