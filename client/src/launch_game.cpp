/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** launch_game
*/

#include <SFML/Window/VideoMode.hpp>
#include <memory>
#include "GameManager.hpp"
#include "RTypeClient.hpp"
#include "RTypeConst.hpp"
#include "TickRateManager.hpp"
#include "core/InputManager.hpp"
#include "core/Registry.hpp"
#include "core/SpriteManager.hpp"
#include "factory/EntityFactory.hpp"
#include "udp/UDPClient.hpp"

void rtc::GameManager::_launchGame()
{
    _window = std::make_shared<sf::RenderWindow>(sf::VideoMode(rt::SCREEN_WIDTH, rt::SCREEN_HEIGHT), "R-Type");
    _window->setFramerateLimit(rt::FPS_LIMIT);

    runGui(_window, _roomManager, _inLobby);

    if (_inLobby) {
        return;
    }

    ntw::UDPClient udpClient(_ip, _gamePort);

    ecs::Registry reg;
    float dt = 0.f;
    ecs::InputManager inputManager;
    ntw::TickRateManager tickRateManager;
    ecs::SpriteManager spriteManager;

    rtc::registerComponents(reg);
    rtc::registerSystems(reg, *_window, dt, udpClient, inputManager, tickRateManager, spriteManager, _networkCallbacks);

    _setupUdpConnection(reg, spriteManager, udpClient);
    ecs::EntityFactory entityFactory(reg,spriteManager,udpClient);

    entityFactory.createEntityFromJSON("assets/player.json");

    run(reg, _window, dt, inputManager);
}
