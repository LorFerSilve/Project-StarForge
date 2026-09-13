add_library(sf_transactions STATIC
    coordinator.cpp
)

add_library(StarForge::Transactions ALIAS sf_transactions)

target_include_directories(sf_transactions
    PUBLIC
        "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
)

target_link_libraries(sf_transactions PUBLIC StarForge::Core)

sf_apply_project_options(sf_transactions)
