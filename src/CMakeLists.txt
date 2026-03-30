add_executable(mytest)

target_sources(mytest PRIVATE
    Main.cpp
)

set_target_properties(mytest PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

target_include_directories(mytest PRIVATE
    ${CMAKE_SOURCE_DIR}/src/include
)

target_link_libraries(mytest PRIVATE
    mylib
)

target_compile_features(mytest PUBLIC
    cxx_std_20
)

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(mylib PRIVATE
        -Wall -Wextra -Wpedantic
    )
endif()