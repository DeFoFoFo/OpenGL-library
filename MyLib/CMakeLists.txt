cmake_minimum_required(VERSION 3.20)
project(mylib
    VERSION 0.0.1
    DESCRIPTION "My OpenGL helper functions and classes"
    LANGUAGES CXX C
)

add_subdirectory(extern)
add_subdirectory(src)