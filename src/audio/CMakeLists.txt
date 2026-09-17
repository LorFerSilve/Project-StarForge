find_path(MINIAUDIO_INCLUDE_DIR miniaudio.h REQUIRED)

add_library(sf_audio STATIC
    audio.cpp
    miniaudio_backend.cpp
)

add_library(StarForge::Audio ALIAS sf_audio)

target_include_directories(sf_audio
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
    PRIVATE
        ${MINIAUDIO_INCLUDE_DIR}
)

sf_apply_project_options(sf_audio)
