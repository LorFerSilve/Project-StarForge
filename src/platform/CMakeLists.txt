add_library(sf_platform STATIC
    fixed_step.cpp
)

add_library(StarForge::Platform ALIAS sf_platform)

target_include_directories(sf_platform
    PUBLIC
        "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
)

target_link_libraries(sf_platform
    PUBLIC
        StarForge::Core
)

sf_apply_project_options(sf_platform)
