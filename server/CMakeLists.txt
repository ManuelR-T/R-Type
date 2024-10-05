add_executable(r-type_server
    src/main.cpp
    src/register_ecs.cpp
    src/create_entity.cpp
    src/GameRunner.cpp
    src/register_tcp_response.cpp
    src/register_udp_response.cpp
    src/RoomManager.cpp
)

target_compile_options(r-type_server PRIVATE -Wall -Wextra)
target_link_libraries(r-type_server PRIVATE FACTORY NETWORK UTILS)
include_directories(
    ${CMAKE_SOURCE_DIR}/include
)
set_target_properties(r-type_server PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/)
