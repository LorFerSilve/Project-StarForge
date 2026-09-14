add_executable(starforge_tests_gameplay
    runtime_gameplay_tests.cpp
)

target_link_libraries(starforge_tests_gameplay
    PRIVATE
        StarForge::Gameplay
        Catch2::Catch2WithMain
)

sf_apply_project_options(starforge_tests_gameplay)

catch_discover_tests(starforge_tests_gameplay
    PROPERTIES
        LABELS "domain;headless;gameplay;determinism;combat"
)
