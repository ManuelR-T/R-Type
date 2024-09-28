cmake_minimum_required(VERSION 3.10)

project(R-TYPE LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS On)

include(cmake/CPM.cmake)

include(cmake/dependencies.cmake)
include(cmake/options.cmake)
include(cmake/asio.cmake)

add_subdirectory(lib)
add_subdirectory(client)
add_subdirectory(server)

if (DOC AND DOC STREQUAL "ON")
    add_subdirectory(docs)
endif()
