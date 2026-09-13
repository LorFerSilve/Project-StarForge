add_library(sf_render STATIC
    render.cpp
)

add_library(StarForge::Render ALIAS sf_render)

target_include_directories(sf_render
    PUBLIC
        "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
)

target_link_libraries(sf_render
    PUBLIC
        StarForge::Core
)

sf_apply_project_options(sf_render)
