add_executable(r-type_client
    src/main.cpp
    src/register_ecs.cpp
    src/setup_tcp_connection.cpp
    src/setup_udp_connection.cpp
    src/launch_game.cpp
    src/RoomManager.cpp
    src/register_tcp_response.cpp
    src/register_udp_response.cpp
    src/run_gui.cpp
    src/menu/room_selection.cpp
)

target_compile_options(r-type_client PRIVATE -Wall -Wextra)

target_link_libraries(r-type_client PRIVATE FACTORY NETWORK UTILS ImGui-SFML::ImGui-SFML)

target_include_directories(r-type_client PRIVATE ${CMAKE_SOURCE_DIR}/include)

set_target_properties(r-type_client PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/)
