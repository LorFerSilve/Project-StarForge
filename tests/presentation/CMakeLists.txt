add_executable(starforge_tests_presentation
    presentation_tests.cpp
)

target_link_libraries(starforge_tests_presentation
    PRIVATE
        StarForge::Input
        StarForge::UI
        StarForge::Audio
        Catch2::Catch2WithMain
)

sf_apply_project_options(starforge_tests_presentation)

catch_discover_tests(starforge_tests_presentation
    PROPERTIES
        LABELS "unit;domain;headless;presentation;accessibility;input"
)
