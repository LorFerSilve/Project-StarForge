find_package(glfw3 CONFIG REQUIRED)

add_library(sf_platform_glfw STATIC
    glfw_window.cpp
)

add_library(StarForge::PlatformGlfw ALIAS sf_platform_glfw)

target_link_libraries(sf_platform_glfw
    PUBLIC
        StarForge::Platform
    PRIVATE
        glfw
)

sf_apply_project_options(sf_platform_glfw)
