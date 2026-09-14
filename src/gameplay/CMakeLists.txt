add_library(sf_gameplay STATIC
    runtime_gameplay.cpp
)

add_library(StarForge::Gameplay ALIAS sf_gameplay)

target_include_directories(sf_gameplay
    PUBLIC
        "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
)

target_link_libraries(sf_gameplay
    PUBLIC
        StarForge::Core
        StarForge::World
)

sf_apply_project_options(sf_gameplay)
