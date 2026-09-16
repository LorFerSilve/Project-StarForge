add_executable(starforge_tests_spacecraft
    spacecraft_tests.cpp
)

target_link_libraries(starforge_tests_spacecraft
    PRIVATE
        StarForge::Spacecraft
        Catch2::Catch2WithMain
)

sf_apply_project_options(starforge_tests_spacecraft)

catch_discover_tests(starforge_tests_spacecraft
    PROPERTIES
        LABELS "domain;headless;spacecraft;determinism;persistence;backend"
)
