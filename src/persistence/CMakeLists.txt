add_library(sf_persistence STATIC
    persistence.cpp
)

add_library(StarForge::Persistence ALIAS sf_persistence)

target_include_directories(sf_persistence
    PUBLIC
        "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
)

target_link_libraries(sf_persistence
    PUBLIC
        StarForge::Core
        StarForge::ContentModel
        StarForge::PersistencePrimitives
)

sf_apply_project_options(sf_persistence)
