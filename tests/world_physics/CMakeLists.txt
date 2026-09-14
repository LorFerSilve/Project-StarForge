add_executable(starforge_tests_world_physics
    world_physics_tests.cpp
    physics_handle_tests.cpp
)

target_link_libraries(starforge_tests_world_physics
    PRIVATE
        StarForge::Physics
        StarForge::PhysicsJolt
        StarForge::World
        StarForge::Player
        Catch2::Catch2WithMain
)

sf_apply_project_options(starforge_tests_world_physics)

catch_discover_tests(starforge_tests_world_physics
    PROPERTIES
        LABELS "domain;headless;physics;determinism"
)
