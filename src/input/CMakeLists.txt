add_library(sf_input STATIC
    input.cpp
)

add_library(StarForge::Input ALIAS sf_input)

target_include_directories(sf_input
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

sf_apply_project_options(sf_input)
