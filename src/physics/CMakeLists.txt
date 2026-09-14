add_library(sf_physics STATIC
    physics.cpp
)

add_library(StarForge::Physics ALIAS sf_physics)

target_include_directories(sf_physics
    PUBLIC
        "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
)

target_link_libraries(sf_physics
    PUBLIC
        StarForge::Core
)

sf_apply_project_options(sf_physics)
