add_library(sf_read_models STATIC
    read_model_utils.cpp
)

add_library(StarForge::ReadModels ALIAS sf_read_models)

target_include_directories(sf_read_models
    PUBLIC
        "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
)

target_link_libraries(sf_read_models PUBLIC StarForge::Core)

sf_apply_project_options(sf_read_models)
