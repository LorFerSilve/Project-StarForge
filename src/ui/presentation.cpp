#include "starforge/ui/presentation.hpp"

#include <algorithm>

namespace starforge::ui {
namespace {
[[nodiscard]] bool can_focus(const Element& element) noexcept {
    return element.visible && element.enabled && element.focusable && !element.reveals_hidden_knowledge;
}
}  // namespace

void FocusScope::set_elements(std::vector<Element> elements) {
    elements_ = std::move(elements);
    focused_index_.reset();
    (void)next();
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
    const auto start = focused_index_.value_or(elements_.size() - 1U);
    for (std::size_t step = 1; step <= elements_.size(); ++step) {
        const auto index = (start + step) % elements_.size();
        if (can_focus(elements_[index])) {
            focused_index_ = index;
            return true;
        }
    }
    focused_index_.reset();
    return false;
}

bool FocusScope::previous() noexcept {
    if (elements_.empty()) {
        return false;
    }
    const auto start = focused_index_.value_or(0U);
    for (std::size_t step = 1; step <= elements_.size(); ++step) {
        const auto index = (start + elements_.size() - (step % elements_.size())) % elements_.size();
        if (can_focus(elements_[index])) {
            focused_index_ = index;
            return true;
        }
    }
    focused_index_.reset();
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
