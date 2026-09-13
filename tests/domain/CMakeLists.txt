find_package(Threads REQUIRED)

add_executable(starforge_tests_domain
    simulation_determinism_tests.cpp
    transaction_coordinator_tests.cpp
    read_model_tests.cpp
)

target_link_libraries(starforge_tests_domain
    PRIVATE
        StarForge::Core
        StarForge::Simulation
        StarForge::Transactions
        StarForge::ReadModels
        Catch2::Catch2WithMain
        Threads::Threads
)

sf_apply_project_options(starforge_tests_domain)

catch_discover_tests(starforge_tests_domain
    PROPERTIES
        LABELS "domain;headless;determinism"
)
