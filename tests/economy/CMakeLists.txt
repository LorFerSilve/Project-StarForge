add_executable(starforge_tests_economy
    economy_tests.cpp
    trade_transaction_tests.cpp
)

target_link_libraries(starforge_tests_economy
    PRIVATE
        StarForge::Economy
        StarForge::Station
        StarForge::Transactions
        Catch2::Catch2WithMain
)

sf_apply_project_options(starforge_tests_economy)

catch_discover_tests(starforge_tests_economy
    PROPERTIES
        LABELS "domain;headless;economy;transactions;conservation"
)
