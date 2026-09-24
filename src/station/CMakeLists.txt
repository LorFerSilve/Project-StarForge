add_library(sf_station STATIC
    station.cpp
    station_processes.cpp
    logistics_transaction.cpp
)

add_library(StarForge::Station ALIAS sf_station)

target_include_directories(sf_station
    PUBLIC
        "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
)

target_link_libraries(sf_station
    PUBLIC
        StarForge::Core
        StarForge::Transactions
)

sf_apply_project_options(sf_station)
