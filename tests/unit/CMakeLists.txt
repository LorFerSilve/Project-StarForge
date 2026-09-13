add_executable(starforge_tests_unit
    core_build_contract_tests.cpp
    imp1_core_tests.cpp
    ../domain/imp1_transaction_tests.cpp
)

target_link_libraries(starforge_tests_unit
    PRIVATE
        StarForge::Core
        StarForge::Diagnostics
        StarForge::Simulation
        StarForge::Transactions
        StarForge::ReadModels
        Catch2::Catch2WithMain
)

sf_apply_project_options(starforge_tests_unit)

catch_discover_tests(starforge_tests_unit
    PROPERTIES
        LABELS "unit"
)
