add_library(glad STATIC)

target_sources(glad
    PRIVATE
        src/gl.c
)

target_include_directories(glad
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)