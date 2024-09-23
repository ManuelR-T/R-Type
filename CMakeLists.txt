cmake_minimum_required(VERSION 3.10)

project(R-TYPE LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS On)

add_subdirectory(lib)
add_subdirectory(client)
add_subdirectory(server)

find_package( Doxygen )

if (DOXYGEN_FOUND)
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/doxy.conf)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/docs/doxygen)

    add_custom_target(doc_doxygen ALL
    COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_IN} > /dev/null 2>&1
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Generating API documentation with Doxygen"
    VERBATIM
    )
endif()