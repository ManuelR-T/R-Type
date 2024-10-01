add_executable(r-type_server
    src/main.cpp
    src/register_ecs.cpp
    src/create_entity.cpp
    src/game_runner.cpp
    src/register_tcp_response.cpp
    src/register_udp_response.cpp
    src/room_manager.cpp
)

target_compile_options(r-type_server PRIVATE -Wall -Wextra)
target_link_libraries(r-type_server PRIVATE ECS PRIVATE NETWORK PRIVATE UTILS)
include_directories(
    ${CMAKE_SOURCE_DIR}/lib/include
    ${CMAKE_SOURCE_DIR}/lib/network/udp
    ${CMAKE_SOURCE_DIR}/lib/network/tcp
    ${CMAKE_SOURCE_DIR}/lib/ecs
)
set_target_properties(r-type_server PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/)
