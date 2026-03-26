cmake_minimum_required(VERSION 3.20)
project(mytest CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(MyLib)

add_subdirectory(src)