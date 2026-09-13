add_executable(starforge
    main.cpp
)

target_link_libraries(starforge
    PRIVATE
        StarForge::Core
        StarForge::PlatformGlfw
        StarForge::RenderOpenGL
        StarForge::ContentModel
        StarForge::Persistence
        StarForge::Simulation
        StarForge::Transactions
        StarForge::ReadModels
)

sf_apply_project_options(starforge)