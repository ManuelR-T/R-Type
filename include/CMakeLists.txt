add_library(INCLUDE INTERFACE)

target_include_directories(INCLUDE PUBLIC
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/lib
    ${CMAKE_SOURCE_DIR}/lib/ecs
)
