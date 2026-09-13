add_library(sf_core STATIC
    build_contract.cpp
)

add_library(StarForge::Core ALIAS sf_core)

target_include_directories(sf_core
    PUBLIC
        "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
)

sf_apply_project_options(sf_core)
