/*
** EPITECH PROJECT, 2024
** R-type
** File description:
** main
*/

#include "core/registry.hpp"
#include "core/response_handler.hpp"
#include "rtype_server.hpp"
#include "game_runner.hpp"

#include "UDPServer.hpp"
#include <SFML/Graphics.hpp>
#include <thread>
#include <cstring>

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
    constexpr std::size_t nb_game = 3;
    std::vector<std::thread> game_list;

    for (std::size_t i = 0; i < nb_game; ++i) {
        game_list.push_back(std::thread([i](){ rts::game_runner game_runner(8080 + i); game_runner.run_game(); }));
    }

    for (std::size_t i = 0; i < nb_game; ++i) {
        game_list[i].join();
    }

    return 0;
}
