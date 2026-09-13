add_executable(starforge_tests_backend
    backend_contract_tests.cpp
)

target_link_libraries(starforge_tests_backend
    PRIVATE
        StarForge::Platform
        StarForge::Render
        StarForge::PlatformGlfw
        StarForge::RenderOpenGL
        Catch2::Catch2WithMain
)

sf_apply_project_options(starforge_tests_backend)

catch_discover_tests(starforge_tests_backend
    PROPERTIES
        LABELS "backends"
)