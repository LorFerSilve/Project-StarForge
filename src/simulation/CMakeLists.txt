add_library(sf_simulation STATIC
    clock.cpp
    tick_pipeline.cpp
)

add_library(StarForge::Simulation ALIAS sf_simulation)

target_include_directories(sf_simulation
    PUBLIC
        "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
)

target_link_libraries(sf_simulation PUBLIC StarForge::Core)

sf_apply_project_options(sf_simulation)
