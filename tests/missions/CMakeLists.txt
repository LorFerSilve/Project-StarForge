add_executable(starforge_tests_missions
    mission_tests.cpp
)

target_link_libraries(starforge_tests_missions
    PRIVATE
        StarForge::Missions
        Catch2::Catch2WithMain
)

sf_apply_project_options(starforge_tests_missions)

catch_discover_tests(starforge_tests_missions
    PROPERTIES
        LABELS "domain;headless;missions;procedural;determinism;persistence"
)
