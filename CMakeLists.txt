cmake_minimum_required(VERSION 3.20)
project(mytest CXX)

add_subdirectory(MyLib)

add_executable(mytest)

target_sources(mytest PRIVATE
    Main.cpp
    Boid.cpp
)

target_link_libraries(mytest PRIVATE
    mylib
)

target_compile_features(mytest PUBLIC
    cxx_std_20
)