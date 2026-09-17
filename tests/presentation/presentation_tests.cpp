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
    REQUIRE(scope.previous());
    CHECK(scope.focused()->id == 1);
    CHECK_FALSE(scope.focus(3));
    scope.invalidate();
    CHECK(scope.focused() == nullptr);
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
