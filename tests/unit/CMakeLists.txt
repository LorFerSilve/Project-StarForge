add_executable(starforge_tests_unit
    core_build_contract_tests.cpp
    core_identity_rng_tests.cpp
    diagnostics_contract_tests.cpp
)

target_link_libraries(starforge_tests_unit
    PRIVATE
        StarForge::Core
        StarForge::Diagnostics
        Catch2::Catch2WithMain
)

sf_apply_project_options(starforge_tests_unit)

catch_discover_tests(starforge_tests_unit
    PROPERTIES
        LABELS "unit"
)
