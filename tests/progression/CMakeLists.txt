add_executable(starforge_tests_progression
    progression_tests.cpp
)

target_link_libraries(starforge_tests_progression
    PRIVATE
        StarForge::Progression
        Catch2::Catch2WithMain
)

sf_apply_project_options(starforge_tests_progression)

catch_discover_tests(starforge_tests_progression
    PROPERTIES
        LABELS "domain;headless;progression;economy;research;reputation;narrative"
)
