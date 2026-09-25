add_library(sf_economy STATIC
    economy.cpp
    market.cpp
)

add_library(StarForge::Economy ALIAS sf_economy)

target_include_directories(sf_economy
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(sf_economy
    PUBLIC
        StarForge::Core
        StarForge::Transactions
)

sf_apply_project_options(sf_economy)
