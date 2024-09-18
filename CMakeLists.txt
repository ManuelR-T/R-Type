cmake_minimum_required(VERSION 3.10)

project(ECSProject LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS On)

add_subdirectory(lib)
add_subdirectory(client)
add_subdirectory(server)
