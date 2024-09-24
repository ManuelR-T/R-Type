/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** game_runner
*/

#pragma once

#include <SFML/Graphics.hpp>
#include <thread>
#include "UDPServer.hpp"
#include "core/registry.hpp"
#include "core/response_handler.hpp"
#include "rtype_server.hpp"

namespace rts {

class game_runner {
private:
    int _port = 0;
    server::UDPServer _udp_server;
    std::thread _receive_thread;
    ecs::registry _reg;
    ecs::response_handler _response_handler;
    float _dt = 0;
    sf::RenderWindow _window;

public:
    game_runner(int port);

    void run_game();
};
} // namespace rts
