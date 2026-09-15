add_executable(starforge_tests_station
    station_systems_tests.cpp
    station_processes_tests.cpp
)

target_link_libraries(starforge_tests_station
    PRIVATE
        StarForge::Station
        Catch2::Catch2WithMain
)

sf_apply_project_options(starforge_tests_station)

catch_discover_tests(starforge_tests_station
    PROPERTIES
        LABELS "domain;headless;station;determinism;conservation"
)

add_test(NAME starforge_station_benchmark_smoke COMMAND starforge-station-benchmark)
set_tests_properties(starforge_station_benchmark_smoke PROPERTIES
    LABELS "headless;station;benchmark;determinism"
)
