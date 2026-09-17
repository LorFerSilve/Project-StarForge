add_library(sf_raids STATIC
    raids.cpp
)

add_library(StarForge::Raids ALIAS sf_raids)

target_include_directories(sf_raids
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(sf_raids
    PUBLIC
        StarForge::Core
        StarForge::Missions
)

sf_apply_project_options(sf_raids)
