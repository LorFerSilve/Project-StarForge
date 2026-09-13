function(sf_apply_project_options target_name)
    target_compile_features(${target_name} PUBLIC cxx_std_23)
    set_target_properties(${target_name} PROPERTIES
        CXX_STANDARD 23
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
    )

    sf_apply_project_warnings(${target_name})

    if(STARFORGE_ENABLE_ASAN)
        if(MSVC)
            target_compile_options(${target_name} PRIVATE /fsanitize=address)
            target_link_options(${target_name} PRIVATE /fsanitize=address)
        elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
            target_compile_options(${target_name} PRIVATE -fsanitize=address -fno-omit-frame-pointer)
            target_link_options(${target_name} PRIVATE -fsanitize=address)
        else()
            message(FATAL_ERROR "STARFORGE_ENABLE_ASAN requested on unsupported compiler")
        endif()
    endif()

    if(STARFORGE_ENABLE_UBSAN)
        if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
            target_compile_options(${target_name} PRIVATE -fsanitize=undefined -fno-omit-frame-pointer)
            target_link_options(${target_name} PRIVATE -fsanitize=undefined)
        else()
            message(FATAL_ERROR "STARFORGE_ENABLE_UBSAN currently requires Clang or GCC")
        endif()
    endif()

    if(STARFORGE_ENABLE_TSAN)
        if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
            target_compile_options(${target_name} PRIVATE -fsanitize=thread -fno-omit-frame-pointer)
            target_link_options(${target_name} PRIVATE -fsanitize=thread)
        else()
            message(FATAL_ERROR "STARFORGE_ENABLE_TSAN currently requires Clang or GCC")
        endif()
    endif()
endfunction()
