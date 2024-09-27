/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** launch_game
*/

#include <memory>
#include "game_manager.hpp"

void rtc::game_manager::launch_game()
{
    _window = std::make_shared<sf::RenderWindow>(
        sf::VideoMode(ecs::constants::screen_width, ecs::constants::screen_height), "R-Type"
    );
    _window->setFramerateLimit(ecs::constants::fps_limit);

    runGui(_window, _room_manager, _in_lobby);

    if (_in_lobby) { // if lobby is true we are outsie of lobby the window has been closed
        return;
    }
    client::UDPClient udpClient(_ip, _game_port);
    udpClient.run();

    ecs::registry reg;
    float dt = 0.f;
    ecs::input_manager input_manager;
    ecs::tick_rate_manager tick_rate_manager;
    SpriteManager sprite_manager;

    rtc::register_components(reg);
    rtc::register_systems(reg, *_window, dt, udpClient, input_manager, tick_rate_manager, sprite_manager);

    rtc::create_player(reg, udpClient, sprite_manager);

    for (int i = 0; i < 20; ++i) {
        rtc::create_static(reg, sprite_manager, 48.f * i, 48.f * i);
    }

    run(reg, _window, dt, input_manager);
}
