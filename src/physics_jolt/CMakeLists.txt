find_package(Jolt CONFIG REQUIRED)

add_library(sf_physics_jolt STATIC
    jolt_world.cpp
)

add_library(StarForge::PhysicsJolt ALIAS sf_physics_jolt)

target_include_directories(sf_physics_jolt
    PUBLIC
        "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
)

target_link_libraries(sf_physics_jolt
    PUBLIC
        StarForge::Physics
    PRIVATE
        Jolt::Jolt
)

sf_apply_project_options(sf_physics_jolt)
