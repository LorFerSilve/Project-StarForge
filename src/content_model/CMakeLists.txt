add_library(sf_content_model STATIC
    content_model.cpp
)

add_library(StarForge::ContentModel ALIAS sf_content_model)

target_include_directories(sf_content_model
    PUBLIC
        "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
)

target_link_libraries(sf_content_model
    PUBLIC
        StarForge::Core
)

sf_apply_project_options(sf_content_model)
