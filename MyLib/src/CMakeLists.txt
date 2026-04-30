add_library(mylib STATIC)

target_sources(mylib PRIVATE
    stb_image.cpp
    Shader.cpp
    ShaderProgram.cpp
    ComputeShader.cpp
    Camera.cpp
    Texture.cpp
    VertexArray.cpp
    Renderer.cpp
    Buffer.cpp
    Mesh.cpp
    Model.cpp
    Window.cpp
    InputManager.cpp
    Application.cpp
)

target_include_directories(mylib PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/extern/stb_image/include>
    $<INSTALL_INTERFACE:include>
)

find_package(OpenGL REQUIRED)

target_link_libraries(mylib
    PRIVATE OpenGL::GL
    PUBLIC glad
    PUBLIC glfw
    PUBLIC stb
    PUBLIC glm
    PUBLIC assimp
)

target_compile_features(mylib PUBLIC
    cxx_std_23
)

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(mylib PRIVATE
        -Wall -Wextra -Wpedantic
    )
endif()