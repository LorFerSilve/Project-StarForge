add_library(glad_generated STATIC
    "${PROJECT_SOURCE_DIR}/third_party/generated/glad/src/gl.c"
)

target_include_directories(glad_generated
    PUBLIC
        "${PROJECT_SOURCE_DIR}/third_party/generated/glad/include"
)

set_target_properties(glad_generated PROPERTIES
    C_STANDARD 11
    C_STANDARD_REQUIRED ON
    C_EXTENSIONS OFF
)

add_library(sf_render_opengl STATIC
    opengl_renderer.cpp
)

add_library(StarForge::RenderOpenGL ALIAS sf_render_opengl)

target_link_libraries(sf_render_opengl
    PUBLIC
        StarForge::Render
    PRIVATE
        StarForge::Platform
        glad_generated
)

sf_apply_project_options(sf_render_opengl)
