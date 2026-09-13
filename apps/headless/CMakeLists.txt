add_executable(starforge-headless
    main.cpp
)

target_link_libraries(starforge-headless
    PRIVATE
        StarForge::Core
        StarForge::Simulation
)

sf_apply_project_options(starforge-headless)
