add_executable(starforge_tests_persistence
    persistence_tests.cpp
)

target_link_libraries(starforge_tests_persistence
    PRIVATE
        StarForge::Persistence
        Catch2::Catch2WithMain
)

target_compile_definitions(starforge_tests_persistence
    PRIVATE
        STARFORGE_TEST_SOURCE_DIR="${PROJECT_SOURCE_DIR}/tests"
)

sf_apply_project_options(starforge_tests_persistence)

catch_discover_tests(starforge_tests_persistence
    PROPERTIES
        LABELS "persistence"
)
