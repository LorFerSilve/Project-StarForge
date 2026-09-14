add_executable(starforge_tests_vertical_slice
    horizon_test_cell_tests.cpp
)

target_link_libraries(starforge_tests_vertical_slice
    PRIVATE
        StarForge::VerticalSlice
        Catch2::Catch2WithMain
)

sf_apply_project_options(starforge_tests_vertical_slice)

catch_discover_tests(starforge_tests_vertical_slice
    PROPERTIES
        LABELS "domain;headless;vertical-slice;determinism;persistence"
)
