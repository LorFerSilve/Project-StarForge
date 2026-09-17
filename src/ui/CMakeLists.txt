add_library(sf_ui STATIC
    presentation.cpp
)

add_library(StarForge::UI ALIAS sf_ui)

target_include_directories(sf_ui
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

sf_apply_project_options(sf_ui)
