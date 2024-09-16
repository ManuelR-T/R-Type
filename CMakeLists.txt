cmake_minimum_required(VERSION 3.10)

project(ECSProject LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(lib)
add_subdirectory(client)
add_subdirectory(server)
