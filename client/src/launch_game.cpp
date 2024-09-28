/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** launch_game
*/

#include <SFML/Window/VideoMode.hpp>
#include <memory>
#include "GameManager.hpp"
#include "TickRateManager.hpp"
#include "UDPClient.hpp"
#include "core/InputManager.hpp"
#include "core/Registry.hpp"
#include "core/SpriteManager.hpp"
#include "rtype_client.hpp"
#include "rtype_const.hpp"

void rtc::GameManager::_launchGame()
{
    _window = std::make_shared<sf::RenderWindow>(sf::VideoMode(rt::SCREEN_WIDTH, rt::SCREEN_HEIGHT), "R-Type");
    _window->setFramerateLimit(rt::FPS_LIMIT);

    runGui(_window, _roomManager, _inLobby);

    if (_inLobby) { // if lobby is true we are outsie of lobby the window has been closed
        return;
    }
    client::UDPClient udpClient(_ip, _gamePort);
    udpClient.run();

    ecs::Registry reg;
    float dt = 0.f;
    ecs::InputManager inputManager;
    TickRateManager tickRateManager;
    SpriteManager spriteManager;

    rtc::registerComponents(reg);
    rtc::registerSystems(reg, *_window, dt, udpClient, inputManager, tickRateManager, spriteManager);

    rtc::createPlayer(reg, udpClient, spriteManager);

    for (int i = 0; i < 20; ++i) {
        rtc::createStatic(reg, spriteManager, 48.f * i, 48.f * i);
    }

    run(reg, _window, dt, inputManager);
}
