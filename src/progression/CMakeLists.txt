add_library(sf_progression STATIC
    progression.cpp
    composite_transaction.cpp
    campaign_progression.cpp
    finale_readiness.cpp
)

add_library(StarForge::Progression ALIAS sf_progression)

target_include_directories(sf_progression
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(sf_progression
    PUBLIC
        StarForge::Core
        StarForge::Station
        StarForge::Transactions
)

sf_apply_project_options(sf_progression)
