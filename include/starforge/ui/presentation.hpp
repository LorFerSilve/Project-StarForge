#pragma once

#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace starforge::ui {

enum class SemanticRole : std::uint8_t { Text, Button, Toggle, Slider, ListItem, Alert };
enum class AlarmPriority : std::uint8_t { P0, P1, P2, P3 };
enum class CueChannel : std::uint8_t { Visual, Audio, Caption, Haptic };
enum class KnowledgeState : std::uint8_t { Unknown, Suspected, Confirmed, Stale };
enum class MarkerPrecision : std::uint8_t { Unknown, SignalApproximate, DirectionOnly, SearchArea, Exact };
enum class NotificationPriority : std::uint8_t { Critical, High, Normal, Informational };
enum class CommandPresentationState : std::uint8_t { Preview, Pending, Accepted, Rejected, Committed };

struct AccessibilitySettings {
    float ui_scale{1.0F};
    bool subtitles{true};
    bool closed_captions{false};
    bool high_contrast{false};
    bool reduced_motion{false};
    bool photosensitivity_safe{false};
    bool reduced_effects{false};
    bool master_muted{false};
};

[[nodiscard]] bool ui_scale_supported(float scale) noexcept;
[[nodiscard]] float clamp_ui_scale(float scale) noexcept;

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
    explicit FocusScope(bool wrap_navigation = false) noexcept : wrap_navigation_(wrap_navigation) {}

    void set_elements(std::vector<Element> elements);
    [[nodiscard]] const Element* focused() const noexcept;
    [[nodiscard]] bool focus(std::uint64_t id) noexcept;
    [[nodiscard]] bool next() noexcept;
    [[nodiscard]] bool previous() noexcept;
    void set_wrap_navigation(bool enabled) noexcept { wrap_navigation_ = enabled; }
    [[nodiscard]] bool wrap_navigation() const noexcept { return wrap_navigation_; }
    void invalidate() noexcept;

  private:
    std::vector<Element> elements_{};
    std::optional<std::size_t> focused_index_{};
    bool wrap_navigation_{false};
};

struct MarkerDescriptor {
    std::uint64_t source_id{};
    KnowledgeState knowledge{KnowledgeState::Unknown};
    MarkerPrecision precision{MarkerPrecision::Unknown};
    bool known_to_player{false};
    bool through_wall{false};
    bool through_wall_authorized{false};
};

[[nodiscard]] bool marker_eligible(const MarkerDescriptor& marker) noexcept;
[[nodiscard]] bool marker_precision_allowed(MarkerPrecision available,
                                            MarkerPrecision requested) noexcept;

struct NotificationEvent {
    std::uint64_t correlation{};
    std::uint64_t semantic_group{};
    std::uint64_t source_incident{};
    NotificationPriority priority{NotificationPriority::Normal};
    bool groupable{false};
    bool committed{false};
};

[[nodiscard]] bool notifications_groupable(const NotificationEvent& lhs,
                                           const NotificationEvent& rhs) noexcept;

struct CommandFeedback {
    std::uint64_t correlation{};
    std::uint64_t source_revision{};
    CommandPresentationState state{CommandPresentationState::Preview};

    [[nodiscard]] bool may_present_success() const noexcept {
        return state == CommandPresentationState::Committed;
    }
};

struct ManagementPreviewToken {
    std::uint64_t selection_id{};
    std::uint64_t source_revision{};
    std::uint64_t scene_generation{};

    [[nodiscard]] bool current(const std::uint64_t active_selection_id,
                               const std::uint64_t active_source_revision,
                               const std::uint64_t active_scene_generation) const noexcept {
        return selection_id != 0U && selection_id == active_selection_id &&
               source_revision == active_source_revision &&
               scene_generation == active_scene_generation;
    }
};

struct PresentationGenerationStamp {
    std::uint64_t scene_generation{};
    std::uint64_t origin_epoch{};
    std::uint64_t content_generation{};
    std::uint64_t read_model_generation{};
};

[[nodiscard]] bool generation_compatible(const PresentationGenerationStamp& candidate,
                                         const PresentationGenerationStamp& active) noexcept;

struct PresentationEvent {
    std::uint64_t sequence{};
    std::uint64_t correlation{};
    std::uint64_t scene_generation{};
    std::uint64_t source_read_model_generation{};
    bool committed{false};
};

struct PresentationEventDiagnostics {
    std::size_t consumed{};
    std::size_t duplicate_or_out_of_order{};
    std::size_t stale_scene{};
    std::size_t uncommitted{};
};

class PresentationEventCursor {
  public:
    explicit PresentationEventCursor(std::uint64_t scene_generation = 0U) noexcept
        : scene_generation_(scene_generation) {}

    [[nodiscard]] bool consume(const PresentationEvent& event) noexcept;
    void reset_for_scene(std::uint64_t scene_generation) noexcept;
    [[nodiscard]] std::uint64_t last_sequence() const noexcept { return last_sequence_; }
    [[nodiscard]] std::uint64_t scene_generation() const noexcept { return scene_generation_; }
    [[nodiscard]] const PresentationEventDiagnostics& diagnostics() const noexcept { return diagnostics_; }

  private:
    std::uint64_t scene_generation_{};
    std::uint64_t last_sequence_{};
    PresentationEventDiagnostics diagnostics_{};
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
    bool enabled{true};
};

[[nodiscard]] PresentationEffect apply_accessibility(
    PresentationEffect effect, const AccessibilitySettings& settings) noexcept;

// TA-13 locked capacity envelope. Time-based p95 budgets are measured by the
// performance harness; these limits guard bounded presentation state at runtime.
struct PresentationBudgetLimits {
    std::size_t live_ui_nodes{20'000U};
    std::size_t visible_ui_nodes{8'000U};
    std::size_t focusable_nodes{1'024U};
    std::size_t active_alarms{128U};
    std::size_t presentation_events_soft{4'096U};
    std::size_t presentation_events_hard{16'384U};
    std::size_t logical_audio_voices{128U};
    std::size_t mixed_audio_voices{96U};
    std::size_t spatial_audio_voices{64U};
    std::size_t streamed_audio_voices{16U};
    std::size_t subtitle_tracks{8U};
    std::size_t caption_cues{16U};
    std::size_t vfx_intents_per_frame{2'048U};
};

inline constexpr PresentationBudgetLimits kPresentationBudgetLimits{};

struct PresentationBudgetSnapshot {
    std::size_t live_ui_nodes{};
    std::size_t visible_ui_nodes{};
    std::size_t focusable_nodes{};
    std::size_t active_alarms{};
    std::size_t queued_presentation_events{};
    std::size_t logical_audio_voices{};
    std::size_t mixed_audio_voices{};
    std::size_t spatial_audio_voices{};
    std::size_t streamed_audio_voices{};
    std::size_t subtitle_tracks{};
    std::size_t caption_cues{};
    std::size_t vfx_intents_this_frame{};
};

enum class PresentationPressure : std::uint8_t { None, Soft, Hard };

struct PresentationBudgetStatus {
    PresentationPressure pressure{PresentationPressure::None};
    bool ui_within_limits{true};
    bool alarms_within_limits{true};
    bool events_within_soft_limit{true};
    bool events_within_hard_limit{true};
    bool audio_within_limits{true};
    bool audio_accounting_consistent{true};
    bool subtitles_captions_within_limits{true};
    bool vfx_within_limits{true};

    [[nodiscard]] bool within_limits() const noexcept {
        return ui_within_limits && alarms_within_limits && events_within_soft_limit &&
               events_within_hard_limit && audio_within_limits && audio_accounting_consistent &&
               subtitles_captions_within_limits && vfx_within_limits;
    }

    [[nodiscard]] bool should_degrade_decorative() const noexcept {
        return pressure != PresentationPressure::None;
    }

    // TA-13 accessibility invariants: overload may coalesce/virtualize decoration,
    // never remove critical or user-selected accessibility presentation.
    [[nodiscard]] static constexpr bool preserve_critical_cues() noexcept { return true; }
    [[nodiscard]] static constexpr bool preserve_accessibility() noexcept { return true; }
};

[[nodiscard]] PresentationBudgetStatus evaluate_presentation_budget(
    const PresentationBudgetSnapshot& snapshot,
    const PresentationBudgetLimits& limits = kPresentationBudgetLimits) noexcept;

}  // namespace starforge::ui
