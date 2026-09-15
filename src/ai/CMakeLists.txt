add_library(sf_ai STATIC
    ai.cpp
)

add_library(StarForge::AI ALIAS sf_ai)

target_include_directories(sf_ai
    PUBLIC
        "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
)

target_link_libraries(sf_ai
    PUBLIC
        StarForge::Core
)

sf_apply_project_options(sf_ai)
