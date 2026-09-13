add_executable(starforge_tests_domain
    imp1_transaction_tests.cpp
)

target_link_libraries(starforge_tests_domain
    PRIVATE
        StarForge::Core
        StarForge::Simulation
        StarForge::Transactions
        Catch2::Catch2WithMain
)

sf_apply_project_options(starforge_tests_domain)

catch_discover_tests(starforge_tests_domain
    PROPERTIES
        LABELS "domain;determinism"
)
