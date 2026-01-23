add_library(mylib STATIC)

target_sources(mylib PRIVATE
    Shader.cpp
    ComputeShader.cpp
    Camera.cpp
)

target_include_directories(mylib PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(mylib
    PRIVATE opengl32
    PUBLIC glad
    PUBLIC glfw
    PUBLIC stbi
    PUBLIC glm
)

target_compile_features(mylib PUBLIC
    cxx_std_20
)

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(mylib PRIVATE
        -Wall -Wextra -Wpedantic
    )
endif()