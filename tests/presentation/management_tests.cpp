#include "starforge/ui/presentation.hpp"

#include <Catch2/catch_test_macros.hpp>

TEST_CASE("management preview is valid only for the exact current selection revision and scene") {
    const starforge::ui::ManagementPreviewToken preview{
        .selection_id = 42U,
        .source_revision = 17U,
        .scene_generation = 3U,
    };

    CHECK(preview.current(42U, 17U, 3U));
    CHECK_FALSE(preview.current(41U, 17U, 3U));
    CHECK_FALSE(preview.current(42U, 18U, 3U));
    CHECK_FALSE(preview.current(42U, 17U, 4U));
}

TEST_CASE("empty management selection never authorizes a preview") {
    const starforge::ui::ManagementPreviewToken preview{
        .selection_id = 0U,
        .source_revision = 17U,
        .scene_generation = 3U,
    };

    CHECK_FALSE(preview.current(0U, 17U, 3U));
}
