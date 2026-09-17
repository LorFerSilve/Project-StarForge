#include "starforge/audio/audio.hpp"
#include "starforge/ui/presentation.hpp"

#include <Catch2/catch_test_macros.hpp>

#include <algorithm>
#include <type_traits>

static_assert(!std::is_same_v<starforge::ui::AlarmPriority, starforge::ui::NotificationPriority>);

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

TEST_CASE("supported UI scale contract preserves the locked accessibility range") {
    CHECK(starforge::ui::ui_scale_supported(0.8F));
    CHECK(starforge::ui::ui_scale_supported(1.5F));
    CHECK_FALSE(starforge::ui::ui_scale_supported(0.79F));
    CHECK_FALSE(starforge::ui::ui_scale_supported(1.51F));
    CHECK(starforge::ui::clamp_ui_scale(0.1F) == 0.8F);
    CHECK(starforge::ui::clamp_ui_scale(2.0F) == 1.5F);
}

TEST_CASE("knowledge filtered markers cannot gain precision or wallhack eligibility") {
    using starforge::ui::KnowledgeState;
    using starforge::ui::MarkerPrecision;

    CHECK_FALSE(starforge::ui::marker_eligible(
        {1U, KnowledgeState::Unknown, MarkerPrecision::Exact, true, false, false}));
    CHECK_FALSE(starforge::ui::marker_eligible(
        {2U, KnowledgeState::Confirmed, MarkerPrecision::Exact, false, false, false}));
    CHECK_FALSE(starforge::ui::marker_eligible(
        {3U, KnowledgeState::Confirmed, MarkerPrecision::Exact, true, true, false}));
    CHECK(starforge::ui::marker_eligible(
        {3U, KnowledgeState::Confirmed, MarkerPrecision::Exact, true, true, true}));
    CHECK_FALSE(starforge::ui::marker_precision_allowed(MarkerPrecision::SearchArea,
                                                        MarkerPrecision::Exact));
    CHECK(starforge::ui::marker_precision_allowed(MarkerPrecision::Exact,
                                                   MarkerPrecision::SearchArea));
    CHECK_FALSE(starforge::ui::marker_precision_allowed(MarkerPrecision::Exact,
                                                        MarkerPrecision::Unknown));
}

TEST_CASE("notifications aggregate only by explicit committed semantic grouping") {
    const starforge::ui::NotificationEvent first{1U, 77U, 100U,
                                                  starforge::ui::NotificationPriority::Normal, true, true};
    const starforge::ui::NotificationEvent same_group{2U, 77U, 101U,
                                                       starforge::ui::NotificationPriority::Normal, true, true};
    const starforge::ui::NotificationEvent unrelated{3U, 78U, 100U,
                                                      starforge::ui::NotificationPriority::Normal, true, true};
    auto uncommitted = same_group;
    uncommitted.committed = false;

    CHECK(starforge::ui::notifications_groupable(first, same_group));
    CHECK_FALSE(starforge::ui::notifications_groupable(first, unrelated));
    CHECK_FALSE(starforge::ui::notifications_groupable(first, uncommitted));
}

TEST_CASE("success presentation requires authoritative commit") {
    starforge::ui::CommandFeedback feedback{42U, 9U, starforge::ui::CommandPresentationState::Preview};
    CHECK_FALSE(feedback.may_present_success());
    feedback.state = starforge::ui::CommandPresentationState::Accepted;
    CHECK_FALSE(feedback.may_present_success());
    feedback.state = starforge::ui::CommandPresentationState::Rejected;
    CHECK_FALSE(feedback.may_present_success());
    feedback.state = starforge::ui::CommandPresentationState::Committed;
    CHECK(feedback.may_present_success());
}

TEST_CASE("presentation generation checks reject stale scene origin content and read models") {
    const starforge::ui::PresentationGenerationStamp active{4U, 8U, 12U, 16U};
    CHECK(starforge::ui::generation_compatible({4U, 8U, 12U, 16U}, active));
    CHECK(starforge::ui::generation_compatible({}, active));
    CHECK_FALSE(starforge::ui::generation_compatible({3U, 8U, 12U, 16U}, active));
    CHECK_FALSE(starforge::ui::generation_compatible({4U, 7U, 12U, 16U}, active));
    CHECK_FALSE(starforge::ui::generation_compatible({4U, 8U, 11U, 16U}, active));
    CHECK_FALSE(starforge::ui::generation_compatible({4U, 8U, 12U, 15U}, active));
}

TEST_CASE("committed presentation events are consumed exactly once in the active scene") {
    starforge::ui::PresentationEventCursor cursor{7U};
    const starforge::ui::PresentationEvent first{1U, 100U, 7U, 10U, true};
    CHECK(cursor.consume(first));
    CHECK_FALSE(cursor.consume(first));
    CHECK_FALSE(cursor.consume({2U, 101U, 8U, 10U, true}));
    CHECK_FALSE(cursor.consume({2U, 101U, 7U, 10U, false}));
    CHECK(cursor.consume({2U, 101U, 7U, 10U, true}));
    CHECK(cursor.last_sequence() == 2U);
    CHECK(cursor.diagnostics().consumed == 2U);
    CHECK(cursor.diagnostics().duplicate_or_out_of_order == 1U);
    CHECK(cursor.diagnostics().stale_scene == 1U);
    CHECK(cursor.diagnostics().uncommitted == 1U);

    cursor.reset_for_scene(8U);
    CHECK(cursor.last_sequence() == 0U);
    CHECK(cursor.diagnostics().consumed == 0U);
    CHECK(cursor.consume({1U, 200U, 8U, 11U, true}));
}

TEST_CASE("muting presentation never changes AI hearing semantics") {
    const starforge::audio::GameplaySoundEvent gameplay_sound{42U, true};
    const starforge::audio::VoiceRequest request{42, starforge::audio::Bus::Warnings,
                                                starforge::audio::VoicePriority::Critical, true};
    const auto audible = starforge::audio::evaluate_voice(request, {1.0F, false});
    const auto muted = starforge::audio::evaluate_voice(request, {0.0F, false});
    CHECK(audible.mix);
    CHECK_FALSE(muted.mix);
    CHECK_FALSE(audible.affects_ai_hearing);
    CHECK_FALSE(muted.affects_ai_hearing);
    CHECK(gameplay_sound.audible_to_ai);
}

TEST_CASE("vacuum blocks ordinary audio while mediated paths remain eligible") {
    const starforge::audio::ActiveAudioContext active{7U, 11U};
    const starforge::audio::PresentationAudioEvent vacuum{
        1U, starforge::audio::AudioMedium::Vacuum, true, true, true, 7U, 11U};
    const starforge::audio::PresentationAudioEvent radio{
        2U, starforge::audio::AudioMedium::Radio, true, true, true, 7U, 11U};
    const starforge::audio::PresentationAudioEvent conduction{
        3U, starforge::audio::AudioMedium::StructureConduction, true, true, true, 7U, 11U};

    CHECK_FALSE(starforge::audio::player_audible(vacuum));
    CHECK_FALSE(starforge::audio::presentation_audio_eligible(vacuum, active));
    CHECK(starforge::audio::presentation_audio_eligible(radio, active));
    CHECK(starforge::audio::presentation_audio_eligible(conduction, active));
}

TEST_CASE("audio presentation rejects uncommitted or stale generation input") {
    const starforge::audio::ActiveAudioContext active{7U, 11U};
    starforge::audio::PresentationAudioEvent event{
        9U, starforge::audio::AudioMedium::Atmosphere, true, true, true, 7U, 11U};
    CHECK(starforge::audio::presentation_audio_eligible(event, active));

    event.committed = false;
    CHECK_FALSE(starforge::audio::presentation_audio_eligible(event, active));
    event.committed = true;
    event.scene_generation = 8U;
    CHECK_FALSE(starforge::audio::presentation_audio_eligible(event, active));
    event.scene_generation = 7U;
    event.origin_epoch = 12U;
    CHECK_FALSE(starforge::audio::presentation_audio_eligible(event, active));
}

TEST_CASE("critical audio priority can preempt decoration but never the reverse") {
    CHECK(starforge::audio::may_preempt(starforge::audio::VoicePriority::Critical,
                                        starforge::audio::VoicePriority::Decorative));
    CHECK(starforge::audio::may_preempt(starforge::audio::VoicePriority::Threat,
                                        starforge::audio::VoicePriority::Ambience));
    CHECK_FALSE(starforge::audio::may_preempt(starforge::audio::VoicePriority::Decorative,
                                              starforge::audio::VoicePriority::Critical));
    CHECK_FALSE(starforge::audio::may_preempt(starforge::audio::VoicePriority::Critical,
                                              starforge::audio::VoicePriority::Critical));
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
    CHECK(effect.enabled);

    settings.reduced_effects = true;
    const auto reduced = starforge::ui::apply_accessibility({false, false, true}, settings);
    CHECK_FALSE(reduced.enabled);
    const auto critical = starforge::ui::apply_accessibility({true, true, false}, settings);
    CHECK(critical.enabled);
    CHECK_FALSE(critical.rapid_flash);
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
