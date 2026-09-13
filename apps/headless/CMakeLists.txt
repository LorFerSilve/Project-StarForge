add_executable(starforge-headless
    main.cpp
)

target_link_libraries(starforge-headless
    PRIVATE
        StarForge::Core
        StarForge::Diagnostics
        StarForge::Simulation
        StarForge::Transactions
        StarForge::ReadModels
)

sf_apply_project_options(starforge-headless)
