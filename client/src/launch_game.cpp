/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** launch_game
*/

#include <SFML/Window/VideoMode.hpp>
#include <cstddef>
#include <memory>
#include "ClientEntityFactory.hpp"
#include "ClientTickRate.hpp"
#include "GameManager.hpp"
#include "InputManager.hpp"
#include "RTypeClient.hpp"
#include "RTypeConst.hpp"
#include "RTypeUDPProtol.hpp"
#include "Registry.hpp"
#include "RoomManager.hpp"
#include "SoundManager.hpp"
#include "SpriteManager.hpp"
#include "TickRateManager.hpp"
#include "udp/UDPClient.hpp"

static void spawnPlayer(ntw::UDPClient &udp, std::size_t userId, const rtc::RoomManager &roomManager)
{
    std::size_t playerIndex = 1;
    for (const auto &[id, _] : roomManager.getCurrentRoomPlayer()) {
        if (id == userId) {
            break;
        }
        ++playerIndex;
    }
    rt::UDPClientPacket msg = {
        .header = {.cmd = rt::UDPCommand::NEW_ENTITY},
        .body =
            {.sharedEntityId = ecs::generateSharedEntityId(),
             .b =
                 {.newEntityData =
                      {.playerId = userId,
                       .playerIndex = playerIndex,
                       .type = rt::EntityType::PLAYER,
                       .moveData = {.pos = {.x = 100, .y = 150 + (50 * float(playerIndex))}}}}}
    };
    udp.send(reinterpret_cast<const char *>(&msg), sizeof(msg));
}

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
    ntw::TickRateManager<rtc::TickRate> tickRateManager;
    ecs::SpriteManager spriteManager;
    ecs::SoundManager soundManager;

    soundManager.loadMusic("battle", "assets/battle.ogg");
    soundManager.playMusic("battle", 5.f, true);

    rtc::registerComponents(reg);
    rtc::registerSystems(reg, *_window, dt, udpClient, inputManager, tickRateManager, spriteManager, _networkCallbacks);

    _setupUdpConnection(reg, spriteManager, udpClient);

    spawnPlayer(udpClient, _userId, this->_roomManager);
    ecs::ClientEntityFactory::createClientEntityFromJSON(reg, spriteManager, udpClient, "assets/bg.json");
    ecs::ClientEntityFactory::createClientEntityFromJSON(reg, spriteManager, udpClient, "assets/earth.json", 500, 123);
    ecs::ClientEntityFactory::createClientEntityFromJSON(
        reg, spriteManager, udpClient, "assets/planetShade75.json", 500, 123
    );
    ecs::ClientEntityFactory::createClientEntityFromJSON(
        reg, spriteManager, udpClient, "assets/planet50.json", 1500, 303
    );
    ecs::ClientEntityFactory::createClientEntityFromJSON(
        reg, spriteManager, udpClient, "assets/planetShade50.json", 1500, 303
    );
    ecs::ClientEntityFactory::createClientEntityFromJSON(
        reg, spriteManager, udpClient, "assets/planetShade25.json", 1000, 288
    );
    ecs::ClientEntityFactory::createClientEntityFromJSON(
        reg, spriteManager, udpClient, "assets/planetShade25.json", 1000, 288
    );
    ecs::ClientEntityFactory::createClientEntityFromJSON(reg, spriteManager, udpClient, "assets/sun.json");
    ecs::ClientEntityFactory::createClientEntityFromJSON(
        reg, spriteManager, udpClient, "assets/explosion.json", 300, 200
    );

    run(reg, _window, dt, inputManager);
}
