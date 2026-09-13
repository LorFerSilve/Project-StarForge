add_executable(starforge_tests_content
    content_model_tests.cpp
)

target_link_libraries(starforge_tests_content
    PRIVATE
        StarForge::ContentModel
        Catch2::Catch2WithMain
)

sf_apply_project_options(starforge_tests_content)

catch_discover_tests(starforge_tests_content
    PROPERTIES
        LABELS "content"
)
