/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** launch_game
*/

#include <SFML/Window/VideoMode.hpp>
#include <memory>
#include "UDPClient.hpp"
#include "core/SpriteManager.hpp"
#include "core/constants.hpp"
#include "core/registry.hpp"
#include "core/input_manager.hpp"
#include "core/tick_rate_manager.hpp"
#include "game_manager.hpp"
#include "rtype_client.hpp"

void rtc::GameManager::_launchGame()
{
    _window = std::make_shared<sf::RenderWindow>(
        sf::VideoMode(ecs::constants::SCREEN_WIDTH, ecs::constants::SCREEN_HEIGHT), "R-Type"
    );
    _window->setFramerateLimit(ecs::constants::FPS_LIMIT);

    runGui(_window, _roomManager, _inLobby);

    if (_inLobby) { // if lobby is true we are outsie of lobby the window has been closed
        return;
    }
    client::UDPClient udpClient(_ip, _gamePort);
    udpClient.run();

    ecs::Registry reg;
    float dt = 0.f;
    ecs::InputManager inputManager;
    ecs::TickRateManager tickRateManager;
    SpriteManager spriteManager;

    rtc::registerComponents(reg);
    rtc::registerSystems(reg, *_window, dt, udpClient, inputManager, tickRateManager, spriteManager);

    rtc::createPlayer(reg, udpClient, spriteManager);

    for (int i = 0; i < 20; ++i) {
        rtc::createStatic(reg, spriteManager, 48.f * i, 48.f * i);
    }

    run(reg, _window, dt, inputManager);
}
