#include "starforge/audio/audio.hpp"
#include "starforge/ui/presentation.hpp"

#include <Catch2/catch_test_macros.hpp>

#include <algorithm>

TEST_CASE("shipping focus skips unavailable and hidden-knowledge elements") {
    starforge::ui::FocusScope scope;
    scope.set_elements({
        {1, starforge::ui::SemanticRole::Button, "Visible", true, true, true, false},
        {2, starforge::ui::SemanticRole::Button, "Disabled", false, true, true, false},
        {3, starforge::ui::SemanticRole::Button, "Secret", true, true, true, true},
        {4, starforge::ui::SemanticRole::Button, "Next", true, true, true, false},
    });
    REQUIRE(scope.focused() != nullptr);
    CHECK(scope.focused()->id == 1);
    REQUIRE(scope.next());
    CHECK(scope.focused()->id == 4);
    CHECK_FALSE(scope.next());
    REQUIRE(scope.previous());
    CHECK(scope.focused()->id == 1);
    CHECK_FALSE(scope.previous());
    CHECK_FALSE(scope.focus(3));
    scope.invalidate();
    CHECK(scope.focused() == nullptr);
}

TEST_CASE("retained focus survives refresh and wrapping is opt-in") {
    starforge::ui::FocusScope scope;
    scope.set_elements({
        {1, starforge::ui::SemanticRole::Button, "First", true, true, true, false},
        {2, starforge::ui::SemanticRole::Button, "Second", true, true, true, false},
    });
    REQUIRE(scope.focus(2));
    scope.set_elements({
        {1, starforge::ui::SemanticRole::Button, "First refreshed", true, true, true, false},
        {2, starforge::ui::SemanticRole::Button, "Second refreshed", true, true, true, false},
    });
    REQUIRE(scope.focused() != nullptr);
    CHECK(scope.focused()->id == 2);
    CHECK_FALSE(scope.next());
    scope.set_wrap_navigation(true);
    REQUIRE(scope.next());
    CHECK(scope.focused()->id == 1);
    REQUIRE(scope.previous());
    CHECK(scope.focused()->id == 2);
}

TEST_CASE("muting presentation never changes AI hearing semantics") {
    const starforge::audio::VoiceRequest request{42, starforge::audio::Bus::Warnings,
                                                starforge::audio::VoicePriority::Critical, true};
    const auto audible = starforge::audio::evaluate_voice(request, {1.0F, false});
    const auto muted = starforge::audio::evaluate_voice(request, {0.0F, false});
    CHECK(audible.mix);
    CHECK_FALSE(muted.mix);
    CHECK_FALSE(audible.affects_ai_hearing);
    CHECK_FALSE(muted.affects_ai_hearing);
}

TEST_CASE("critical alarms retain redundant non-audio presentation when muted") {
    starforge::ui::AlarmPresentation alarm{starforge::ui::AlarmPriority::P0, "Hull breach", true,
                                           {starforge::ui::CueChannel::Audio}};
    starforge::ui::AccessibilitySettings settings{};
    settings.master_muted = true;
    const auto channels = starforge::ui::effective_alarm_channels(alarm, settings);
    CHECK(std::find(channels.begin(), channels.end(), starforge::ui::CueChannel::Audio) == channels.end());
    CHECK(std::find(channels.begin(), channels.end(), starforge::ui::CueChannel::Visual) != channels.end());
    CHECK(std::find(channels.begin(), channels.end(), starforge::ui::CueChannel::Caption) != channels.end());
}

TEST_CASE("captions respect audibility and knowledge boundaries") {
    starforge::ui::AccessibilitySettings settings{};
    settings.closed_captions = true;
    CHECK(starforge::ui::caption_eligible({"Footsteps", true, true}, settings));
    CHECK_FALSE(starforge::ui::caption_eligible({"Hidden enemy", false, true}, settings));
    CHECK_FALSE(starforge::ui::caption_eligible({"Unknown event", true, false}, settings));
}

TEST_CASE("accessibility transforms preserve semantic effect while removing unsafe presentation") {
    starforge::ui::AccessibilitySettings settings{};
    settings.photosensitivity_safe = true;
    settings.reduced_motion = true;
    const auto effect = starforge::ui::apply_accessibility({true, true, true}, settings);
    CHECK_FALSE(effect.rapid_flash);
    CHECK_FALSE(effect.camera_motion);
}

TEST_CASE("locked TA-13 presentation capacity envelope passes at exact limits") {
    const auto& limits = starforge::ui::kPresentationBudgetLimits;
    const starforge::ui::PresentationBudgetSnapshot snapshot{
        limits.live_ui_nodes,
        limits.visible_ui_nodes,
        limits.focusable_nodes,
        limits.active_alarms,
        limits.presentation_events_soft,
        limits.logical_audio_voices,
        limits.mixed_audio_voices,
        limits.spatial_audio_voices,
        limits.streamed_audio_voices,
        limits.subtitle_tracks,
        limits.caption_cues,
        limits.vfx_intents_per_frame,
    };
    const auto status = starforge::ui::evaluate_presentation_budget(snapshot);
    CHECK(status.within_limits());
    CHECK(status.pressure == starforge::ui::PresentationPressure::None);
    CHECK_FALSE(status.should_degrade_decorative());
}

TEST_CASE("presentation overload degrades decoration but preserves critical accessibility routes") {
    const auto& limits = starforge::ui::kPresentationBudgetLimits;
    starforge::ui::PresentationBudgetSnapshot snapshot{};
    snapshot.live_ui_nodes = limits.live_ui_nodes + 1U;
    snapshot.queued_presentation_events = limits.presentation_events_soft + 1U;
    snapshot.logical_audio_voices = limits.logical_audio_voices;
    snapshot.mixed_audio_voices = limits.mixed_audio_voices;
    snapshot.spatial_audio_voices = limits.spatial_audio_voices;
    snapshot.streamed_audio_voices = limits.streamed_audio_voices;

    auto status = starforge::ui::evaluate_presentation_budget(snapshot);
    CHECK_FALSE(status.within_limits());
    CHECK(status.pressure == starforge::ui::PresentationPressure::Soft);
    CHECK(status.should_degrade_decorative());
    CHECK(status.preserve_critical_cues());
    CHECK(status.preserve_accessibility());

    snapshot.live_ui_nodes = 0U;
    snapshot.queued_presentation_events = limits.presentation_events_hard + 1U;
    status = starforge::ui::evaluate_presentation_budget(snapshot);
    CHECK(status.pressure == starforge::ui::PresentationPressure::Hard);
    CHECK(status.preserve_critical_cues());
    CHECK(status.preserve_accessibility());
}

TEST_CASE("presentation budget rejects inconsistent audio accounting") {
    starforge::ui::PresentationBudgetSnapshot snapshot{};
    snapshot.logical_audio_voices = 4U;
    snapshot.mixed_audio_voices = 5U;
    const auto status = starforge::ui::evaluate_presentation_budget(snapshot);
    CHECK_FALSE(status.audio_accounting_consistent);
    CHECK(status.pressure == starforge::ui::PresentationPressure::Hard);
}
