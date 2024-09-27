/*
** EPITECH PROJECT, 2024
** R-type
** File description:
** main
*/

#include "GameProtocol.hpp"
#include "core/registry.hpp"
#include "core/response_handler.hpp"
#include "game_runner.hpp"
#include "room_manager.hpp"
#include "rtype_server.hpp"

#include <SFML/Graphics.hpp>
#include <cstddef>
#include <cstring>
#include <thread>
#include "TCPServer.hpp"
#include "UDPServer.hpp"

#include <iostream>

static void run(ecs::registry &reg, sf::RenderWindow &window, float &dt)
{
    sf::Clock clock;

    while (window.isOpen()) {
        dt = clock.restart().asSeconds();

        // ! for debug
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        // ! for debug
        reg.run_systems();
    }
}

int main()
{
    server::TCPServer tcpServer(8080, sizeof(rt::tcp_packet));
    rts::room_manager room_manager;

    tcpServer.register_command([&room_manager, &tcpServer](tcp::socket &sock, char *data, std::size_t size) {
        rt::tcp_packet packet{};

        if (size != sizeof(packet)) {
            std::cerr << "\n\nINVALID PACK RECV SIZE: " << size << "\n\n";
            return;
        }

        std::memcpy(&packet, data, sizeof(packet));
        switch (packet.cmd) {
            case rt::tcp_command::CL_NEW_USER:
                tcpServer.add_user(sock, packet.body.cl_new_user.user_id);
                break;
            case rt::tcp_command::CL_DISCONNECT_USER:
                tcpServer.remove_user(packet.body.cl_new_user.user_id);
                break;
            case rt::tcp_command::CL_CREATE_ROOM:
                room_manager.create_room(
                    packet.body.cl_create_room.room_name, packet.body.cl_create_room.user_id, tcpServer
                );
                break;
            case rt::tcp_command::CL_DELETE_ROOM:
                room_manager.delete_room(
                    packet.body.cl_delete_room.room_name, packet.body.cl_delete_room.user_id, tcpServer
                );
                break;
            case rt::tcp_command::CL_JOIN_ROOM:
                room_manager.join_room(
                    packet.body.cl_join_room.room_name,
                    packet.body.cl_join_room.user_id,
                    packet.body.cl_join_room.user_name,
                    tcpServer
                );
                break;
            case rt::tcp_command::CL_LEAVE_ROOM:
                room_manager.leave_room(
                    packet.body.cl_leave_room.room_name, packet.body.cl_leave_room.user_id, tcpServer
                );
                break;
            case rt::tcp_command::CL_READY:
                room_manager.player_ready(packet.body.cl_ready.room_name, packet.body.cl_ready.user_id, tcpServer);
                break;
            case rt::tcp_command::CL_NOT_READY:
                room_manager.player_not_ready(
                    packet.body.cl_not_ready.room_name, packet.body.cl_not_ready.user_id, tcpServer
                );
                break;
            case rt::tcp_command::CL_ROOM_LIST:
                room_manager.send_list_room(packet.body.cl_room_list.user_id, tcpServer);
                break;
        }
    });

    tcpServer.run();

    std::string str;
    while (std::getline(std::cin, str)) {
        if (str == "quit" || str == "exit") {
            break;
        }
    }
    return 0;
}
