add_executable( r-type_client
    src/main.cpp
    src/create_entity.cpp
    src/register_ecs.cpp
    src/setup_tcp_connection.cpp
    src/launch_game.cpp
    src/room_manager.cpp
    src/register_tcp_response.cpp
    src/run_gui.cpp
)

target_compile_options( r-type_client PRIVATE -Wall -Wextra)
target_link_libraries( r-type_client PRIVATE ECS PRIVATE NETWORK PRIVATE UTILS PRIVATE ARGPARSER PRIVATE ImGui-SFML::ImGui-SFML)
include_directories(
    ${CMAKE_SOURCE_DIR}/lib/utils
    ${CMAKE_SOURCE_DIR}/lib/network/udp
    ${CMAKE_SOURCE_DIR}/lib/network/tcp
    ${CMAKE_SOURCE_DIR}/lib/network
    ${CMAKE_SOURCE_DIR}/include
)

set_target_properties( r-type_client PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/)
