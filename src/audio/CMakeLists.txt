add_library(sf_audio STATIC
    audio.cpp
)

add_library(StarForge::Audio ALIAS sf_audio)

target_include_directories(sf_audio
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

sf_apply_project_options(sf_audio)
