add_library(stbi STATIC)

target_sources(stbi
    PRIVATE
        src/stb_image.cpp
)

target_include_directories(stbi
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)