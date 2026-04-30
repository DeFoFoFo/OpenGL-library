add_executable(${CMAKE_PROJECT_NAME})

target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    Main.cpp
)

set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/src/include
)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
    mylib
)

target_compile_features(${CMAKE_PROJECT_NAME} PUBLIC
    cxx_std_23
)

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(mylib PRIVATE
        -Wall -Wextra -Wpedantic
    )
endif()