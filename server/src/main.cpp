/*
** EPITECH PROJECT, 2024
** R-type
** File description:
** main
*/

#include "core/registry.hpp"
#include "core/response_handler.hpp"
#include "rtype_server.hpp"

#include <SFML/Graphics.hpp>
#include <cstddef>
#include <cstring>
#include <thread>
#include "UDPServer.hpp"

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
    int port = 8080;
    server::UDPServer udp_server(port);

    std::thread receiveThread([&udp_server]() { udp_server.run(); });

    ecs::registry reg;
    ecs::response_handler response_handler;

    register_response(reg, response_handler);
    udp_server.register_command([&response_handler](char *data, std::size_t size) {
        response_handler.handle_response(data, size);
    });

    float dt = 0.f;
    sf::RenderWindow window(sf::VideoMode(1000, 700), "R-Type"); // ! for deebug

    window.setFramerateLimit(60); // ! for debug
    register_components(reg);
    register_systems(reg, window, dt);

    for (int i = 0; i < 10; ++i) {
        create_static(reg, 100.f * i, 100.f * i);
    }

    run(reg, window, dt);
    return 0;
}
