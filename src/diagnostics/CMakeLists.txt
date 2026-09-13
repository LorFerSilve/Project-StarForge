add_library(sf_diagnostics STATIC
    assert.cpp
    log.cpp
)

add_library(StarForge::Diagnostics ALIAS sf_diagnostics)

target_include_directories(sf_diagnostics
    PUBLIC
        "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
)

target_link_libraries(sf_diagnostics PUBLIC StarForge::Core)

sf_apply_project_options(sf_diagnostics)
