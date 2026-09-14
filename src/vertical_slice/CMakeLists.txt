add_library(sf_vertical_slice STATIC
    horizon_test_cell.cpp
)

add_library(StarForge::VerticalSlice ALIAS sf_vertical_slice)

target_include_directories(sf_vertical_slice
    PUBLIC
        "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
)

target_link_libraries(sf_vertical_slice
    PUBLIC
        StarForge::Core
        StarForge::Persistence
        StarForge::Transactions
        StarForge::World
)

sf_apply_project_options(sf_vertical_slice)
