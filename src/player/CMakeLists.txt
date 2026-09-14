add_library(sf_player STATIC
    character_motor.cpp
)

add_library(StarForge::Player ALIAS sf_player)

target_include_directories(sf_player
    PUBLIC
        "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
)

target_link_libraries(sf_player
    PUBLIC
        StarForge::Physics
)

sf_apply_project_options(sf_player)
