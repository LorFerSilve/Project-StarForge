#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace starforge::ui {

enum class SemanticRole : std::uint8_t { Text, Button, Toggle, Slider, ListItem, Alert };
enum class AlarmPriority : std::uint8_t { P0, P1, P2, P3 };
enum class CueChannel : std::uint8_t { Visual, Audio, Caption, Haptic };

struct AccessibilitySettings {
    float ui_scale{1.0F};
    bool subtitles{true};
    bool closed_captions{false};
    bool high_contrast{false};
    bool reduced_motion{false};
    bool photosensitivity_safe{false};
    bool master_muted{false};
};

struct Element {
    std::uint64_t id{};
    SemanticRole role{SemanticRole::Text};
    std::string label{};
    bool enabled{true};
    bool visible{true};
    bool focusable{false};
    bool reveals_hidden_knowledge{false};
};

class FocusScope {
  public:
    void set_elements(std::vector<Element> elements);
    [[nodiscard]] const Element* focused() const noexcept;
    [[nodiscard]] bool focus(std::uint64_t id) noexcept;
    [[nodiscard]] bool next() noexcept;
    [[nodiscard]] bool previous() noexcept;
    void invalidate() noexcept;

  private:
    std::vector<Element> elements_{};
    std::optional<std::size_t> focused_index_{};
};

struct AlarmPresentation {
    AlarmPriority priority{AlarmPriority::P3};
    std::string text{};
    bool known_to_player{true};
    std::vector<CueChannel> channels{};
};

[[nodiscard]] std::vector<CueChannel> effective_alarm_channels(
    const AlarmPresentation& alarm, const AccessibilitySettings& settings);

struct CaptionEvent {
    std::string text{};
    bool legitimately_audible{false};
    bool known_to_player{false};
};

[[nodiscard]] bool caption_eligible(const CaptionEvent& event,
                                    const AccessibilitySettings& settings) noexcept;

struct PresentationEffect {
    bool rapid_flash{false};
    bool camera_motion{false};
    bool decorative{true};
};

[[nodiscard]] PresentationEffect apply_accessibility(
    PresentationEffect effect, const AccessibilitySettings& settings) noexcept;

}  // namespace starforge::ui
