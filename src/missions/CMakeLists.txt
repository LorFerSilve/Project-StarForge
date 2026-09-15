add_library(sf_missions STATIC
    missions.cpp
)

add_library(StarForge::Missions ALIAS sf_missions)

target_include_directories(sf_missions
    PUBLIC
        "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
)

target_link_libraries(sf_missions
    PUBLIC
        StarForge::Core
)

sf_apply_project_options(sf_missions)
