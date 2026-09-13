add_library(sf_persistence_primitives STATIC
    primitives.cpp
)

add_library(StarForge::PersistencePrimitives ALIAS sf_persistence_primitives)

target_include_directories(sf_persistence_primitives
    PUBLIC
        "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
)

target_link_libraries(sf_persistence_primitives
    PUBLIC
        StarForge::Core
)

sf_apply_project_options(sf_persistence_primitives)
