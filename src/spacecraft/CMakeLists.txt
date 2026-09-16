add_library(sf_spacecraft STATIC
    spacecraft.cpp
)

add_library(StarForge::Spacecraft ALIAS sf_spacecraft)

target_include_directories(sf_spacecraft
    PUBLIC
        "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
)

target_link_libraries(sf_spacecraft
    PUBLIC
        StarForge::Core
)

sf_apply_project_options(sf_spacecraft)
