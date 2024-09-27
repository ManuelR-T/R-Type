/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** game_runner
*/

#include "game_runner.hpp"
#include "GameProtocol.hpp"

rts::game_runner::game_runner(int port)
    : _port(port),
    _udp_server(port),
    _response_handler([](const rt::udp_packet &packet) { return packet.cmd; }),
    _window(sf::VideoMode(1000, 700), "R-Type") // ! for debug
{
    rts::register_udp_response(_reg, _response_handler);
    _udp_server.register_command([this](char *data, std::size_t size) {
        this->_response_handler.handle_response(data, size);
    });
    _udp_server.run();

    _window.setFramerateLimit(60); // ! for debug
    rts::register_components(_reg);
    rts::register_systems(_reg, _window, _dt);

    // * create static
    for (int i = 0; i < 10; ++i) {
        rts::create_static(_reg, 100.f * i, 100.f * i);
    }
}

void rts::game_runner::run_game()
{
    sf::Clock clock;

    while (_window.isOpen()) {
        _dt = clock.restart().asSeconds();

        // ! for debug
        sf::Event event;
        while (_window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                _window.close();
            }
        }
        // ! for debug
        _reg.run_systems();
    }
}
