add_executable(starforge_tests_ai
    ai_tests.cpp
)

target_link_libraries(starforge_tests_ai
    PRIVATE
        StarForge::AI
        Catch2::Catch2WithMain
)

sf_apply_project_options(starforge_tests_ai)

catch_discover_tests(starforge_tests_ai
    PROPERTIES
        LABELS "domain;headless;ai;navigation;determinism"
)
