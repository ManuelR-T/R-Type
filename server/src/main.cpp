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
    ecs::response_handler<rt::tcp_command, rt::tcp_packet> response_handler([](const rt::tcp_packet &packet) {
        return packet.cmd;
    });

    rts::register_tcp_response(room_manager, tcpServer, response_handler);
    tcpServer.register_command([&response_handler, &tcpServer](tcp::socket &sock, char *data, std::size_t size) {
        rt::tcp_command new_user_cmd = rt::tcp_command::CL_NEW_USER;

        if (std::memcmp(data, &new_user_cmd, sizeof(rt::tcp_command)) == 0) {
            rt::tcp_packet packet{};
            std::memcpy(&packet, data, sizeof(packet));
            tcpServer.add_user(sock, packet.body.cl_new_user.user_id);
        } else {
            response_handler.handle_response(data, size);
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
