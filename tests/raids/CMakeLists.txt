add_executable(starforge_tests_raids
    raids_tests.cpp
)

target_link_libraries(starforge_tests_raids
    PRIVATE
        StarForge::Raids
        Catch2::Catch2WithMain
)

sf_apply_project_options(starforge_tests_raids)

catch_discover_tests(starforge_tests_raids
    PROPERTIES
        LABELS "domain;headless;raids;determinism;persistence"
)
