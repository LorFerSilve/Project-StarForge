add_library(sf_world STATIC
    world.cpp
)

add_library(StarForge::World ALIAS sf_world)

target_include_directories(sf_world
    PUBLIC
        "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
)

target_link_libraries(sf_world
    PUBLIC
        StarForge::Core
        StarForge::Physics
)

sf_apply_project_options(sf_world)
